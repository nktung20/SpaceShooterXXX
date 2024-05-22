#include "Objects.h"
#include "mainFunc.h"

Objects::Objects()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}
Objects::~Objects()
{
    free();
}

bool Objects::loadImg(std::string path, SDL_Renderer *screen)
{
    SDL_Texture *new_texture = NULL;
    SDL_Surface *load_surface = IMG_Load(path.c_str());
    if (load_surface != NULL)
    {
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if (new_texture != NULL)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    }

    p_object_ = new_texture;
    return p_object_ != NULL;
}

void Objects::renderTexture(SDL_Renderer *dest, SDL_Rect *clip)
{
    SDL_FRect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
    if (clip != NULL)
    {
        renderquad.w = clip->w;
        renderquad.h = clip->h;
    }
    SDL_RenderCopyF(dest, p_object_, clip, &renderquad);
}
void Objects::free()
{
    if (p_object_ != NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}
SDL_FRect Objects::getRect()
{
    return rect_;
}
void Objects::setRect(const float &x, const float &y)
{
    rect_.x = x;
    rect_.y = y;
}
