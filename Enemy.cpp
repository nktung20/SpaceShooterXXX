#include "Enemy.h"

Enemy::Enemy(){
    rect_.x=SCREEN_WIDTH/2;
    rect_.y=0;
    rect_.w=Enemy_width;
    rect_.h=Enemy_height;
    x_val_=0;
    y_val_=0;
    startTime=0;

}
Enemy::Enemy(int _x,double _vel){
   rect_.x=_x;
    rect_.y=0;
    rect_.w=Enemy_width;
    rect_.h=Enemy_height;
    x_val_=0;
    y_val_=_vel;
    startTime=0;
}
Enemy::~Enemy(){}


void Enemy::handleMove(const int& x_border,const int& y_border){
    rect_.y+=y_val_;

}
void Enemy::createBullet(const int& x_border,const int& y_border,SDL_Renderer* screen,const float &player_pos_x,const float&player_pos_y,std::vector <Bullet*> &e_bullets,double &v) {
    if((SDL_GetTicks64()-startTime)>=2000){
    Bullet* e_bullet=new Bullet();
            e_bullet->loadImg("assets/enemyBullet.png",screen);
            e_bullet->setRect(this->rect_.x+18,rect_.y);
            e_bullet->setTarget(player_pos_x+30,player_pos_y+25);
            e_bullet->setVel(v);
            e_bullet->setmoving(true);
            e_bullets.push_back(e_bullet);
            startTime=SDL_GetTicks64();

    }
}
void Enemy::B_createBullet(const int& x_border,const int& y_border,SDL_Renderer* screen,const float &player_pos_x,const float&player_pos_y,std::vector <Bullet*> &b_bullets,double &v) {
    if((SDL_GetTicks64()-startTime)>=1500){
    Bullet* b_bullet=new Bullet();
            b_bullet->loadImg("assets/b_bullet.png",screen);
            b_bullet->setRect(this->rect_.x+18,rect_.y);
            b_bullet->setTarget(player_pos_x+30,player_pos_y+25);
            b_bullet->setVel(v);
            b_bullet->setmoving(true);
            b_bullets.push_back(b_bullet);
            startTime=SDL_GetTicks64();
    }
}

