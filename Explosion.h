#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "Objects.h"

class Explosion : public Objects
{
public:
    Explosion();
    ~Explosion();
    int frame = 0;
    int b_frame = 0;
    bool valid = true;
    bool b_valid = true;

    void updateFrame();
    static void createExplosion(float x, float y, std::vector<Explosion *> &e_explosions);
    static void createBossExplosion(float x, float y, std::vector<Explosion *> &b_explosions);
};
#endif // EXPLOSION_H_
