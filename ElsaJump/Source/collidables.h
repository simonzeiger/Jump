//
//  collidable.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/2/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef collidable_h
#define collidable_h

#include "graphics.h"
class Collidable {
public:
    Collidable(float x, float y, int width, int height);
    virtual ~Collidable();
    virtual void draw(Graphics &graphics) = 0;
    virtual void fixedUpdate(float fixedUpdate) = 0;
    virtual bool checkCollision(float playerX, float playerY);
    virtual void setY(float y);
    virtual void setX(float x);
    float getX();
    virtual void shift(float y);
    
    int getCollidableWidth();
protected:
    float _x, _y;
    int _width, _height;
    float _prevPlayerY;
    

};

#include "animatedsprite.h"
#include "globals.h"

class Platform: public Collidable, public AnimatedSprite {
public:
    Platform(float x, float y, Graphics &graphics);
    ~Platform();
    
    void draw(Graphics &graphics);
    virtual void fixedUpdate(float fixedTime);
    void addSpring(bool lOrR, Graphics &graphics);
    void deleteSpring();
    void shift(float y);
    void setY(float y);
    void setX(float x);
    bool hasSpring();
    bool isMoving();
    void setupAnimations();
    void animationDone(std::string currentAnimation);
    bool isReal();
    bool isOnlyOnce();

    void enableOnlyOnce();
    void enableMoving(float speed);
    void makeFake();
    void reset();
    
    std::pair<bool, bool> checkPlatformCollision(float playerX, float playerY);
private:
    void disableMoving();
    void disableOnlyOnce();
    void makeReal();
    
    Collidable* _spring;
    bool _isMoving;
    float _dx;
    bool _onlyOnce;
    bool _destroyOnce;
    bool _fake;
    float _dy;
   
};



#include "animatedsprite.h"

class Spring: public Collidable, public AnimatedSprite {
public:
    Spring(float x, float y, Graphics &graphics);
    void draw(Graphics &graphics);
    void fixedUpdate(float fixedTime);
    bool checkCollision(float playerX, float playerY);
    void animationDone(std::string currentAnimation);
    void setupAnimations();
    void shift(float y);
    void setX(float x);
    void setY(float y);
    void getX();
private:
    
};
#endif /* collidable_hpp */

