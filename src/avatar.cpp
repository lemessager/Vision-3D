#define SDL_VIDEO_MODE_OPTIONS (SDL_RESIZABLE | SDL_OPENGL)
#define SDL_DEPTH 32

#define SCENE_ROTATION_STEP 5
#define CAMERA_Z_OFFSET 2
#define CAMERA_TRANSLATION_STEP 0.1
#define RDR_FRAME_LENGTH 1
#define RDR_CUBE_HALF_SIDE 0.5

#include "avatar.h"
#include "gl_objects.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../include/event.h"

CAvatar::CAvatar(){
    should_be_running=false;
    needs_rendering=true;
    window_height=800;
    window_width=800;
    window_title="Avatar";

    world_origin_x=0.5;
    world_origin_y=0.5;
    world_origin_z=10;
    camera_min_tz=2;
    InitSceneConstants();
}

CAvatar::~CAvatar(){}

void CAvatar::InitSceneConstants(){
    world_rx=0;
    world_ry=0;

    camera_min_tz=world_origin_z+ CAMERA_Z_OFFSET;
    camera_tx=world_origin_x;
    camera_ty=world_origin_y;
    camera_tz=camera_min_tz;
}

int CAvatar::OnExecute(){
    if(OnInit()==false){
        return -1;
    }

    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    should_be_running=true;
    needs_rendering=true;
    while(should_be_running){
        while(SDL_PollEvent(&event)){
            OnEvent(&event);
        }
        OnLoop();
        OnRender();
    }
    OnCleanup();

    return 0;
}

bool CAvatar::OnInit(){
    char sdl_wdw_pos[]="SDL_VIDEO_WINDOW_POS";
    char sdl_wdw_ctr[]="SDL_VIDEO_CENTERED=1";
    putenv(sdl_wdw_pos);
    putenv(sdl_wdw_ctr);

    if(SDL_Init(SDL_INIT_EVERYTHING)<0){
        return false;
    }

    window_width=1000;
    window_height=768;
    window_title="Avatar Main Window l";

    SDL_WM_SetCaption(window_title,0);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
   // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    /*
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);
*/
    sdl_pimage=SDL_SetVideoMode(window_width,window_height,SDL_DEPTH,SDL_VIDEO_MODE_OPTIONS);
    if(sdl_pimage==NULL)
        return false;

    glClearColor(0,0,0,0);
    glViewport(0,0,window_width,window_height);

    camera_aspect_ratio=((float)window_width)/((float)window_height);
    camera_min_z=0.1;
    camera_max_z=10;
    camera_fovy=60;

    InitProjectionMatrix();

    return true;
}

void CAvatar::InitProjectionMatrix(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera_fovy, camera_aspect_ratio, camera_min_z, camera_max_z);
}

void CAvatar::OnCleanup(){
    if(sdl_pimage)
        SDL_FreeSurface(sdl_pimage);
    SDL_Quit();
}

void CAvatar::OnLoop(){

}

void CAvatar::OnRender(){
    if(!needs_rendering)
        return;
    needs_rendering=false;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    GLfloat scaling[]={1,0,0,0,
                       0,1.5,0,0,
                       0,0,1,0,
                       0,0,0,1};

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-camera_tx,-camera_ty,-camera_tz);
    glRotatef(world_rx,1,0,0);
    glRotatef(world_ry,0,1,0);
    glMultMatrixf(scaling);

    gl_objects obj;
    obj.DrawFrame(world_origin_x, world_origin_y, world_origin_z, RDR_FRAME_LENGTH);
    obj.DrawCube(world_origin_x, world_origin_y, world_origin_z, RDR_CUBE_HALF_SIDE);

    SDL_GL_SwapBuffers();
}

void CAvatar::OnEvent(SDL_Event* Event){
needs_rendering=true;
    CEvent::OnEvent(Event);
}

void CAvatar::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
    switch(sym){
    case SDLK_ESCAPE:
        OnExit();
        break;

     case SDLK_SPACE:
        InitSceneConstants();
        needs_rendering=true;
        break;

     case SDLK_LEFT:
        if(mod & KMOD_SHIFT){
            world_ry-=SCENE_ROTATION_STEP;
            if(world_ry<0)
                world_ry+=360;
        }
        else
            camera_tx-=CAMERA_TRANSLATION_STEP;
        needs_rendering=true;
        break;

       case SDLK_RIGHT:
            //a completer
            break;
    case SDLK_DOWN:
        // a completer
        break;
       case SDLK_UP:
        // a completer
        break;
    case SDLK_q:
        camera_tz -= CAMERA_TRANSLATION_STEP;
        if (camera_tz< camera_min_tz)
            camera_tz=camera_min_z;
        needs_rendering = true;
        break;
    case SDLK_a:
        camera_tz += CAMERA_TRANSLATION_STEP;
        needs_rendering = true;
        break;
     }

}

void CAvatar::OnResize(int w,int h){
    window_width= w;
    window_height=h;
    SDL_FreeSurface (sdl_pimage);
    sdl_pimage= SDL_SetVideoMode(window_width, window_height, SDL_DEPTH,SDL_VIDEO_MODE_OPTIONS);
    glViewport(0,0,window_width,window_height);

    camera_aspect_ratio = ((float)window_width/(float)window_height);
    InitProjectionMatrix();
    needs_rendering = true;

}


void CAvatar::OnExpose(){
    // a completer
}
void CAvatar::OnExit(){
    should_be_running = false;
}
