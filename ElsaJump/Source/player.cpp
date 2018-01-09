//
//  player.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#include "player.h"
#include "graphics.h"
#include "globals.h"
#include "game.h"
#include "world.h"
#include <math.h>

namespace player_constants {
    const float MULTI = 1;
    const float GRAVITY = .0008f * MULTI; //.0008
    const float JUMP_SPEED = 0.55  * MULTI; //.55
    const float SPRING_SPEED = 1  * MULTI;
    const float MOVE_SPEED = .26f  * MULTI;
}


 
Player::Player(Graphics &graphics, float x, float y) :
AnimatedSprite(graphics, "Elsa", 0, 0, 16, 16, x, y, 200),
_dy(-player_constants::JUMP_SPEED),
_dx(0),
_hasShield(false),
_isJumping(true),
_maxJumpHeightReached(false),
_facing(RIGHT),
_isDead(false)
{
    setupAnimations();
    playAnimation("JumpRight");
}

void Player::setupAnimations() {
    addAnimation(3, 0, 48, "JumpLeft", 16, 16);
    addAnimation(3, 0, 32, "JumpRight", 16, 16);
    addAnimation(3, 0, 0, "ChillLeft", 16, 16);
    addAnimation(3, 0, 16, "ChillRight", 16, 16);
  
}

void Player::animationDone(std::string currentAnimation) {}

float Player::getY() const {
    return _y;
}

float Player::getX() const {
    return _x;
}

void Player::moveLeft() {
    _dx = -player_constants::MOVE_SPEED;
    _facing = LEFT;
   
}

void Player::moveRight() {
    _dx = player_constants::MOVE_SPEED;
    _facing = RIGHT;
}


void Player::stopMoving() {
    _dx = 0;
}

void Player::jump(float y) {
    _isJumping = true;
    if(y < 0){
        y = -y;
        _dy = -player_constants::SPRING_SPEED;
    } else {
        _dy = -player_constants::JUMP_SPEED;
    }
     _y = y - 16 * globals::SPRITE_SCALE;
    
    if(_facing == RIGHT)
        playAnimation("JumpRight");
    else
        playAnimation("JumpLeft");
    

}


void Player::update(float elapsedTime) {
    
    if(!_isDead){
        
        if(_dy > 0){
            _isJumping = false;
            
        }
        
    }
    
    
    //screen edges
    if(_x + 20 > globals::SCREEN_WIDTH && _dx > 0)
        _x = -40;
    else if(_x + 16 * globals::SPRITE_SCALE - 20 <= 0 && _dx < 0)
        _x = globals::SCREEN_WIDTH - 20;
    
    
    AnimatedSprite::update(elapsedTime);
}

void Player::fixedUpdate(float fixedTime){
    
    if(!_isDead){
        
        
        //Apply gravity
        _dy += player_constants::GRAVITY * fixedTime;
        
        
        
        //Move by dy
        _y += _dy * fixedTime;
        
        if(_dy > 0){
            _isJumping = false;
        }
        
        if(_dy > -.25){
            if(_facing == LEFT)
                playAnimation("ChillLeft");
            else
                 playAnimation("ChillRight");
        }
            
        
        //Move by dx
        _x += _dx * fixedTime;
        
    }
    
}

void Player::draw(Graphics &graphics) {

    AnimatedSprite::draw(graphics, _x, _y);
}

int Player::checkPlatformCollisions(Platform** platforms, int nPlatforms){
    for(int i = 0; i < nPlatforms; i++){
        std::pair<bool, bool> collision = platforms[i]->checkPlatformCollision(_x, _y);
        //return neg number if hit a spring
        if(collision.first){
            return !collision.second ? platforms[i]->getY() : -platforms[i]->getY();
        }
    }
    return -10000;
}
   
void Player::killed(){
    //_isDead = true;
    _y = globals::SCREEN_HEIGHT + 40;
    _x = globals::SCREEN_WIDTH / 2;
    _dy = -player_constants::JUMP_SPEED;
}

void Player::shift(float amt){
    _y += amt;
}

float Player::getDY() const{
    return _dy;
}


