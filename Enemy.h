#ifndef ENEMY_H_
#define ENEMY_H_

// #include "mainFunc.h"
#include "Player.h"
#include "Bullet.h"

const int Enemy_width = 60;
const int Enemy_height = 60;
const int Boss_width = 100;
const int Boss_height = 100;

class Enemy : public Objects
{
public:
    Enemy();
    Enemy(int _x, double _vel);
    ~Enemy();

    void handleMove(const int &x_border, const int &y_border);
    void set_x(const int &x_Val_)
    {
        x_val_ = x_Val_;
    }
    void set_y(const int &y_Val_)
    {
        y_val_ = y_Val_;
    }
    int get_x()
    {
        return x_val_;
    }
    int get_pos_y()
    {
        return rect_.y;
    }
    int get_y()
    {
        return y_val_;
    }
    void set_v(const int &v)
    {
        velocity = v;
    }
    void createBullet(const int &x_border, const int &y_border, SDL_Renderer *screen, const float &player_pos_x, const float &player_pos_y, std::vector<Bullet *> &e_bullets, double &v);
    void B_createBullet(const int &x_border, const int &y_border, SDL_Renderer *screen, const float &player_pos_x, const float &player_pos_y, std::vector<Bullet *> &b_bullets, double &v);

private:
    Uint64 startTime;
    float x_val_;
    float y_val_;
    float velocity;
};

#endif // ENEMY_H_
