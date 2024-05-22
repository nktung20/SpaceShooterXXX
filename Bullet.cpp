#include "Bullet.h"

Bullet::Bullet()
{
    x_val_ = 0;
    y_val_ = 0;
    moving = false;
    vel_ = 0;
    dx = 0;
    dy = 0;
    target_x_ = 0;
    target_y_ = 0;
}
Bullet::~Bullet()
{
}
void Bullet::setTarget(float x, float y)
{
    target_x_ = x;
    target_y_ = y;
    dx = target_x_ - rect_.x;
    dy = target_y_ - rect_.y;
    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length;
    dy /= length;
}
void Bullet::setVel(float _vel)
{
    vel_ = _vel;
}
void Bullet::handleMove(const int &x_border, const int &y_border)
{
    rect_.y -= y_val_;
    if (rect_.y > y_border)
        moving = false;
}
void Bullet::handleEnemy(const int &x_border, const int &y_border)
{
    rect_.x += dx * vel_;
    rect_.y += dy * vel_;
    if (rect_.x > x_border || rect_.y > y_border || rect_.x < 0 || rect_.y < 0)
        moving = false;
}
