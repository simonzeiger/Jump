//
//  enemy.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/13/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "enemy.h"


Enemy::Enemy(float x, float y, Graphics &graphics) :
Collidable(x, y, 64, 48),
AnimatedSprite(graphics, "Enemy", 0, 0, 64, 48, x, y, 50)
{
    Collidable::_x = x;
    Collidable::_y = y;
    
    setupAnimations();
    playAnimation("EnemyNorm");
}


void Enemy::draw(Graphics &graphics){
    AnimatedSprite::draw(graphics, Collidable::_x, Collidable::_y, 1);
    
}


void Enemy::fixedUpdate(float fixedTime){
    AnimatedSprite::fixedUpdate(fixedTime);
    
}

int Enemy::checkCollision( float playerX, float playerY){
    
    

    SDL_Rect rect1 = {(int)playerX + 15, (int) playerY, 28, 48};
        SDL_Rect rect2 = {(int)Sprite::_x, (int) Sprite::_y, 64, 48};
        if (rect1.x < rect2.x + rect2.w &&
            rect1.x + rect1.w > rect2.x &&
            rect1.y < rect2.y + rect2.h &&
            rect1.h + rect1.y > rect2.y)
                 return 1;

        return 0;
    
}

void Enemy::setupAnimations() {
    addAnimation(1, 0, 0, "EnemyNorm", 64, 48);
}

void Enemy::shift(float y){
    Collidable::shift(y);
    AnimatedSprite::_y += y;
}

void Enemy::setX(float x){
    Collidable::setX(x);
    AnimatedSprite::_x = x;
}

void Enemy::setY(float y){
    Collidable::setY(y);
    AnimatedSprite::_y = y;
}
void Enemy::animationDone(std::string currentAnimation) {
}
