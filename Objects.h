#ifndef OBJECTS_H_
#define OBJECTS_H_

#include "mainFunc.h"

class Objects {
public:
    Objects() ;
    ~Objects();


bool loadImg(std::string path,SDL_Renderer* screen ) ;
void renderTexture(SDL_Renderer* dest, SDL_Rect* clip = NULL) ;
void free() ;
SDL_FRect getRect ();
void setRect (const float & x, const float& y);
protected:
    SDL_FRect rect_;
    SDL_Texture* p_object_;
};

#endif
