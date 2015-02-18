#ifndef _AVATAR_H_
#define _AVATAR_H_

#include "SDL/SDL.h"
#include "event.h"
class CAvatar : public CEvent {
public:
    CAvatar();
    ~CAvatar();
    int OnExecute();

    bool OnInit();
    void OnCleanup();
    void  OnLoop();
    void OnRender();
    void OnEvent(SDL_Event&);
    void InitSceneConstants();
    void InitProjectionMatrix();

    void OnEvent(SDL_Event* Event);
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnResize(int w, int h);
    void OnExpose();
    void OnExit();


private:
    bool should_be_running;
    int window_width;
    int window_height;
    const char* window_title;

    float world_rx;
    float world_ry;

    float world_origin_x;
    float world_origin_y;
    float world_origin_z;
    float camera_min_tz;

    float camera_tx;
    float camera_ty;
    float camera_tz;

    float camera_aspect_ratio;
    float camera_min_z;
    float camera_max_z;
    float camera_fovy;

    bool needs_rendering;

    SDL_Surface* sdl_pimage;
};

#endif
