//
//  enemy.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/13/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef enemy_h
#define enemy_h

#include "animatedsprite.h"
#include "collidables.h"

class Enemy: public Collidable, public AnimatedSprite {
public:
    Enemy(float x, float y, Graphics &graphics);
    void draw(Graphics &graphics);
    void fixedUpdate(float fixedTime);
    int checkCollision(float playerX, float playerY);
    void animationDone(std::string currentAnimation);
    void setupAnimations();
    void shift(float y);
    void setX(float x);
    void setY(float y);
private:
    
};

#endif /* enemy_hpp */
