#ifndef PLAYER_H_
#define PLAYER_H_

#include "mainFunc.h"
#include "Objects.h"
#include "Bullet.h"

#include <vector>

#define p_speed 10
class Player : public Objects
{
public:
    float value;
    Player();
    ~Player();

    bool loadImg(std::string path, SDL_Renderer *screen);
    void handleInput(SDL_Event events, SDL_Renderer *screen);
    void Update();
    void movePlayer();
    bool checkBulletToEnemy(SDL_FRect e);
    void handleBullet(SDL_Renderer *des);

private:
    std::vector<Bullet *> p_bullets;

    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;
};

#endif // PLAYER_H_
