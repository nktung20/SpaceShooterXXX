#ifndef BULLET_H_
#define BULLET_H_

#include "Objects.h"
#include "mainFunc.h"

class Bullet:public Objects {
public:
    Bullet();
    ~Bullet();
    void setTarget(float x ,float y);
    void set_x(const float& x_Val_) {
    x_val_=x_Val_;
    }
    void set_y(const float& y_Val_) {
    y_val_=y_Val_;
    }
    float get_x() {
   return x_val_;
    }
    float get_y() {
   return y_val_;
    }
    void setVel(float _vel);
    void setmoving(const bool &move) {moving=move;}
    bool getmoving() const {return moving;}
    void handleMove(const int& x_border,const int& y_border);
    void handleEnemy(const int &x_border,const int& y_border);

private:
    float x_val_;
    float y_val_;
    float vel_;
    float dx;
    float dy;
    float target_x_;
    float target_y_;
    bool moving;
};

#endif // BULLET_H_
