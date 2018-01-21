//
//  powerup.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/16/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef powerup_h
#define powerup_h

#include "animatedsprite.h"
#include "globals.h"
#include "collidables.h"

class Enemy;

class Powerup: public Collidable, public AnimatedSprite {
public:
    Powerup(float x, float y, Graphics &graphics);
    
    void draw(Graphics &graphics);
    void fixedUpdate(float fixedTime);
    
    int checkCollision(float playerX, float playerY);
    void setX(float x);
    void setY(float y);
    void shift(float y);
    PowerupType type();
    void extend();
    void shrink();
    
    void setupAnimations();
    void animationDone(std::string currentAnimation);    
    
private:
    
    PowerupType _myType = GoldSpring;
   
};



#endif /* powerup_h*/
