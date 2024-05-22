#include "Explosion.h"

Explosion::Explosion() {}
Explosion::~Explosion() {}

void Explosion::updateFrame()
{
    frame++;
    b_frame++;
    if (frame >= 25)
        valid = false;
    if (b_frame >= 50)
        b_valid = false;
}
void Explosion::createExplosion(float x, float y, std::vector<Explosion *> &e_explosions)
{
    Explosion *e_explosion = new Explosion();
    e_explosion->loadImg("assets/explosion.png", g_screen);
    e_explosion->setRect(x, y);
    e_explosions.push_back(e_explosion);
}
void Explosion::createBossExplosion(float x, float y, std::vector<Explosion *> &b_explosions)
{
    Explosion *b_explosion = new Explosion();
    b_explosion->loadImg("assets/b_explosion.png", g_screen);
    b_explosion->setRect(x, y);
    b_explosions.push_back(b_explosion);
}
