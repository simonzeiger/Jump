//
//  powerup.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/16/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "powerup.h"
#include "globals.h"

Powerup::Powerup(float x, float y, Graphics &graphics) :
Collidable(x + 8, y, 20, 20),
AnimatedSprite(graphics, "PowerUp", 0, 0, 32, 32,
               x, y, 100)
{
    setupAnimations();
    
    if(globals::randInt(1, 10) <= 1){
        playAnimation("JetPack");
        _myType = JetPack;
    } else {
        playAnimation("GoldSpring");
        _myType = GoldSpring;
    }
    
}


void Powerup::draw(Graphics &graphics){
    AnimatedSprite::draw(graphics, AnimatedSprite::_x, AnimatedSprite::_y, 2);
}

void Powerup::fixedUpdate(float fixedTime){
    AnimatedSprite::fixedUpdate(fixedTime);
}

int Powerup::checkCollision(float playerX, float playerY){
    if(playerX + 25 > Collidable::_x && playerX + 15 < Collidable::_x + Collidable::_width){
        if((playerY > Collidable::_y && playerY < Collidable::_y + Collidable::_height) || (playerY + 37 > Collidable::_y && playerY + 28 < Collidable::_y + Collidable::_height)){
            
            return true;
        }
    }
    return false;
}

void Powerup::setX(float x){
    Collidable::setX(x + 8);
    AnimatedSprite::setX(x);
}
void Powerup::setY(float y){
    Collidable::setY(y);
    AnimatedSprite::setY(y);
}

void Powerup::shift(float y){
    Collidable::shift(y);
    AnimatedSprite::_y += y;
}

void Powerup::setupAnimations(){
    addAnimation(1, 0, 0, "JetPack", 32, 32);
    addAnimation(1, 0, 32, "GoldSpring", 32, 32);
    addAnimation(4, 32, 0, "JetPackAnim", 32, 32);
    addAnimation(2, 32, 32, "GoldSpringyAnim", 32, 32);
}

void Powerup::animationDone(std::string currentAnimation){
    
}
