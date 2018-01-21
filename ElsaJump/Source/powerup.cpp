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
Collidable(x + 16, y, 26, 26),
AnimatedSprite(graphics, "PowerUp", 0, 0, 32, 32,
               x, y, 100)
{
    setupAnimations();
    
    if(globals::randInt(1, 10) <= 4){
        playAnimation("JetPack");
        _myType = JetPack;
    } else {
        playAnimation("GoldSpring");
        _myType = GoldSpring;
    }
    
}

PowerupType Powerup::type(){
    return _myType;
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
            if(_myType == JetPack)
                playAnimation("JetPackAnim");
            else if(_myType == GoldSpring)
                playAnimation("GoldSpringLongBoye");
            return true;
        }
    }
    return false;
}

void Powerup::extend(){
    playAnimation("GoldSpringLongBoye");
}
   

void Powerup::shrink(){
    playAnimation("GoldSpringShortBoye");

}

void Powerup::setX(float x){
    Collidable::setX(x + 16);
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
    addAnimation(1, 32, 32, "GoldSpringShortBoye", 32, 32);
    addAnimation(1, 64, 32, "GoldSpringLongBoye", 32, 32);
}

void Powerup::animationDone(std::string currentAnimation){
  
}
