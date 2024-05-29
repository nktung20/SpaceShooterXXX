#ifndef mainFunc_H_INCLUDED
#define mainFunc_H_INCLUDED

#pragma once

#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
extern SDL_Window *g_window;
extern SDL_Renderer *g_screen;
extern SDL_Event g_event;

const int SCREEN_WIDTH = 1264;
const int SCREEN_HEIGHT = 720;
const SDL_Rect EXPLOSION_CLIPS[5] = {
    {0, 0, 60, 60},
    {60, 0, 60, 60},
    {120, 0, 60, 60},
    {180, 0, 60, 60},
    {240, 0, 60, 60}};
const SDL_Rect B_EXPLOSION_CLIPS[10] = {
    {0, 0, 100, 100},
    {100, 0, 100, 100},
    {200, 0, 100, 100},
    {300, 0, 100, 100},
    {400, 0, 100, 100},
    {500, 0, 100, 100},
    {600, 0, 100, 100},
    {700, 0, 100, 100},
    {800, 0, 100, 100},
    {900, 0, 100, 100}};
bool init();

void close();

bool checkCollision(const SDL_FRect &a, const SDL_FRect &b);

#endif // mainFunc_H_
