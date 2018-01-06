//
//  spring.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/5/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "spring.h"

Spring::Spring(float x, float y, Graphics &graphics) :
Collidable(x, y, 32, 32),
AnimatedSprite(graphics, "Content/Sprites/Spring.png", 0, 0, 32, 32, x, y, 100)
{
    Collidable::_x = x;
    Collidable::_y = y;
    
    setupAnimations();
    playAnimation("Smoll");
}


void Spring::draw(Graphics &graphics){
    AnimatedSprite::draw(graphics, Collidable::_x, Collidable::_y, 1);

}

void Spring::update(float elapsedTime){
    AnimatedSprite::update(elapsedTime);

}

bool Spring::checkCollision(float playerX, float playerY){
    if(Collidable::checkCollision(playerX, playerY - 32)){
        playAnimation("Springy", false);
        return true;
    }
    return false;
    
}

void Spring::setupAnimations() {
    addAnimation(2, 0, 0, "Springy", 32, 32);
    addAnimation(1, 0, 0, "Smoll", 32, 32);
    addAnimation(1, 32, 0, "Yuge", 32, 32);
}

void Spring::shift(float y){
    Collidable::shift(y);
    AnimatedSprite::_y += y;
}


void Spring::animationDone(std::string currentAnimation) {
    if(currentAnimation == "Springy") playAnimation("Yuge");
}




