//
//  player.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#include "player.hpp"
#include "graphics.hpp"
#include "globals.h"
#include "game.hpp"
#include "background.hpp"
#include <math.h>

namespace player_constants {
    const float GRAVITY = .0008f; //.0008
    const float JUMP_SPEED = 0.55; //.55
    const float MOVE_SPEED = .3f;
}


Player::Player(){}

Player::Player(Graphics &graphics, float x, float y) :
AnimatedSprite(graphics,"Content/Sprites/ElsaChar.png", 0, 0, 16, 16, x, y, 100),
_dy(-player_constants::JUMP_SPEED),
_dx(0),
_facing(RIGHT),
_hasShield(false),
_isJumping(true),
_maxJumpHeightReached(false),
_isDead(false)
{
    setupAnimations();
    playAnimation("IdleRight");
}

void Player::setupAnimations() {
    //addAnimation(1, 0, 0, "IdleLeft", 16, 16);
    addAnimation(1, 0, 16, "IdleRight", 16, 16);
    addAnimation(1, 0, 32, "IdleRightShield", 16, 16);
    addAnimation(3, 0, 32, "RunRightShield", 16, 16);
    //addAnimation(3, 0, 0, "RunLeft", 16, 16);
    addAnimation(3, 0, 16, "RunRight", 16, 16);
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
    //if(!_hasShield) playAnimation("RunLeft");
    //else  playAnimation("RunLeftShield");
}

void Player::moveRight() {
    _dx = player_constants::MOVE_SPEED;
    _facing = RIGHT;
   // if(!_hasShield)playAnimation("RunRight");
    //else  playAnimation("RunRightShield");
}


void Player::stopMoving() {
   // playAnimation("IdleRight");
    _dx = 0;
}

void Player::jump(float y) {
    _isJumping = true;
    _y = y - 16 * globals::SPRITE_SCALE;
    
    _dy = -player_constants::JUMP_SPEED;
    playAnimation("IdleRight");

}


void Player::update(float elapsedTime) {
    
    if(!_isDead){
        
        
        //Apply gravity
        _dy += player_constants::GRAVITY * elapsedTime;
        
        
        
        //Move by dy
        _y += _dy * elapsedTime;
        
        if(_dy > 0){
            _isJumping = false;
            
        }
        
    }
    
    
    //Move by dx
    _x += _dx * elapsedTime;
    
    //screen edges
    if(_x + 20 > globals::SCREEN_WIDTH && _dx > 0)
        _x = -40;
    else if(_x + 16 * globals::SPRITE_SCALE - 20 <= 0 && _dx < 0)
        _x = globals::SCREEN_WIDTH - 20;
    
    
    AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
  /*  SDL_SetRenderDrawColor(graphics.renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(graphics.renderer(), _x, _y, _x, _y + 16 * globals::SPRITE_SCALE);
    SDL_RenderDrawLine(graphics.renderer(), _x, _y, _x + 16 * globals::SPRITE_SCALE, _y);
    SDL_RenderDrawLine(graphics.renderer(), _x + 16 * globals::SPRITE_SCALE, _y , _x + 16 * globals::SPRITE_SCALE, _y + 16 * globals::SPRITE_SCALE);
    SDL_RenderDrawLine(graphics.renderer(), _x, _y  + 16 * globals::SPRITE_SCALE, _x + 16 * globals::SPRITE_SCALE, _y + 16 * globals::SPRITE_SCALE);*/


    AnimatedSprite::draw(graphics, _x, _y);
}

int Player::checkPlatformCollisions(Platform** platforms, int nPlatforms){
    for(int i = 0; i < nPlatforms; i++){
        if(platforms[i]->checkCollision(_x, _y))
            return platforms[i]->getY();
    }
    return -1;
}
   
void Player::killed(){
    //_isDead = true;
    _y = globals::SCREEN_HEIGHT + 40;
    _dy = -player_constants::JUMP_SPEED;
}

void Player::shift(float amt){
    _y += amt;
}

float Player::getDY() const{
    return _dy;
}


