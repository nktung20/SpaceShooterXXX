#include "Player.h"
Player::Player() {
    x_pos_=600;
    y_pos_=650;
    rect_.x=600;
    rect_.y=650;
    x_val_=0;
    y_val_=0;
}
Player::~Player() {

}

bool Player::loadImg(std::string path, SDL_Renderer* screen) {
    bool ret=Objects::loadImg(path,screen);
    return ret;
}
void Player::Update()
{
    rect_.x = x_pos_;
    rect_.y = y_pos_;
}


void Player::handleInput(SDL_Event events,SDL_Renderer* screen)
{
    value=0;
    if (events.type==SDL_KEYDOWN && events.key.repeat == 0) {
        switch(events.key.keysym.sym) {
            case SDLK_RIGHT :
                x_val_+=p_speed;
                break;

            case SDLK_LEFT:
                x_val_-=p_speed;
                break;

            case SDLK_UP:
            y_val_-=p_speed;
            break;

            case SDLK_DOWN:
            y_val_+=p_speed;

            break;
            case SDLK_e:
                Bullet* p_bullet=new Bullet();
            p_bullet->loadImg("assets/playerBullet.png",screen);
            p_bullet->setRect(this->rect_.x+(60/2)-2,rect_.y);
            p_bullet->set_y(6);
            p_bullet->setmoving(true);
            p_bullets.push_back(p_bullet);
            value=1;
            break;

        }
    } else if (events.type==SDL_KEYUP) {
        switch(events.key.keysym.sym) {
            case SDLK_RIGHT : {
                x_val_ = 0;
                break;
            }
            case SDLK_LEFT: {
                x_val_ = 0;
                break;
            }
            case SDLK_UP:
                y_val_ = 0;
                break;
            case SDLK_DOWN:
                y_val_ = 0;
                break;
             default:
                break;
        }
    }

}

void Player::handleBullet(SDL_Renderer* des) {
            for (int i=0;i<int(p_bullets.size());i++) {
        Bullet* p_bullet=p_bullets.at(i);
        if(p_bullet!=NULL) {
            if(p_bullet->getmoving()==true) {
                p_bullet->handleMove(SCREEN_WIDTH,SCREEN_HEIGHT);
                p_bullet->renderTexture(des);
            }
            else {
                p_bullets.erase(p_bullets.begin()+i);
                if (p_bullet!=NULL) {

                p_bullet=NULL;
                }
            }
        }
    }
}
void Player::movePlayer() {
    x_pos_+=x_val_;
    if (x_pos_<0 || x_pos_ + 60 > SCREEN_WIDTH) x_pos_-=x_val_;
    y_pos_+=y_val_;
    if (y_pos_<0 || y_pos_ + 60 > SCREEN_HEIGHT) y_pos_-=y_val_;

}

bool Player::checkBulletToEnemy(SDL_FRect e){
        for (int i=0;i<int(p_bullets.size());i++) {
        if(checkCollision(p_bullets[i]->getRect(),e)==true) {
                    return true;
        }
    }
    return false;
}
