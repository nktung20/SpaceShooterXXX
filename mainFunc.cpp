#include "mainFunc.h"

 SDL_Window* g_window=NULL;
 SDL_Renderer* g_screen=NULL;
 SDL_Event g_event;

bool init() {
    bool success=true;
    if(SDL_Init(SDL_INIT_EVERYTHING)<0 ) {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
    g_window=SDL_CreateWindow("SpaceShooterXXX",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (g_window==NULL) success=false;
    else {
            g_screen=SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
            if (g_screen==NULL) success=false;
            else {
                SDL_SetRenderDrawColor(g_screen,255,255,255,255);
                int imgFlags=IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags)&& imgFlags))
                    success=false;
                if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 )<0) return false;
                if (TTF_Init() == -1) return false;
            }
    }
    return success;

}
void close () {

    SDL_DestroyRenderer(g_screen);
    g_screen=NULL;
    SDL_DestroyWindow(g_window);
    g_window=NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
bool checkCollision(const SDL_FRect &a, const SDL_FRect &s) {
    return a.x < s.x + s.w &&
		a.x + a.w > s.x &&
		a.y < s.y + s.h &&
		a.y + a.h > s.y;
}

