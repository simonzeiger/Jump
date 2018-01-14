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
#include "enemy.h"
#include "world.h"
#include "projectile.h"
#include <math.h>

namespace player_constants {
    const float MULTI = 1;
    const float GRAVITY = .0008f * MULTI; //.0008
    const float JUMP_SPEED = 0.55  * MULTI; //.55
    const float SPRING_SPEED = 1  * MULTI;
    const float MOVE_SPEED = .28f  * MULTI;
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
    for(int i = 0; i < MAX_BALLS; i++){
        _balls[i] = new Projectile(graphics, this);
    }

}

Player::~Player(){
    for(int i = 0; i < MAX_BALLS; i++)
        delete _balls[i];
}

void Player::setupAnimations() {
    addAnimation(3, 0, 48, "JumpLeft", 16, 16);
    addAnimation(3, 0, 32, "JumpRight", 16, 16);
    addAnimation(3, 0, 0, "ChillLeft", 16, 16);
    addAnimation(3, 0, 16, "ChillRight", 16, 16);
    addAnimation(1, 48, 0, "NoHairAnimRight", 16, 16);
    addAnimation(1, 48, 16, "NoHairAnimLeft", 16, 16);

  
}

void Player::animationDone(std::string currentAnimation) {}

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

void Player::animationStateMachine(){
    
    if(_currentAnimation == "JumpLeft" || _currentAnimation == "JumpRight"){
        if(_facing == LEFT){
            if(_dy > -.25)
                playAnimation("NoHairAnimLeft");
            else
                playAnimation("JumpLeft");
            
        } else {
            if(_dy > -.25)
                playAnimation("NoHairAnimRight");
            else
                playAnimation("JumpRight");
        }
        
    } else if(_currentAnimation == "ChillLeft" || _currentAnimation == "ChillRight") {
        if(_facing == RIGHT)
            playAnimation("ChillRight");
        else
            playAnimation("ChillLeft");
        
    } else if (_currentAnimation == "NoHairAnimRight" || _currentAnimation == "NoHairAnimLeft") {
       
        if(_facing == RIGHT)
            playAnimation("NoHairAnimRight");
        else {
            
            playAnimation("NoHairAnimLeft");
        }

            
    }
    
}


void Player::update( ) {
    
    
    if(_dy > 0){
        _isJumping = false;
        
    }
    
    animationStateMachine();
    
    
    //screen edges
    if(_x + 20 > globals::SCREEN_WIDTH && _dx > 0)
        _x = -40;
    else if(_x + 16 * globals::SPRITE_SCALE - 20 <= 0 && _dx < 0)
        _x = globals::SCREEN_WIDTH - 20;
    
  
}


void Player::fixedUpdate(float fixedTime){
    

    
    
        //Apply gravity
        _dy += player_constants::GRAVITY * fixedTime;
        
        
        
        //Move by dy
        _y += _dy * fixedTime;
            
        
        //Move by dx
        if(!_isDead) _x += _dx * fixedTime;
    
        AnimatedSprite::fixedUpdate(fixedTime);
    
        for(int i = 0; i < MAX_BALLS; i++)
            if(_balls[i]->isLoaded())
                _balls[i]->fixedUpdate(fixedTime);

    
    
}

Direction Player::facing(){
    return _facing;
}

bool Player::isJumping() {
    return _isJumping;
}

void Player::draw(Graphics &graphics) {
    
    AnimatedSprite::draw(graphics, _x, _y);
    
    for(int i = 0; i < MAX_BALLS; i++)
        if(_balls[i]->isLoaded())
            _balls[i]->draw(graphics);
}

bool Player::isDead() {
    return _isDead;
}

int Player::checkPlatformCollisions(Platform** platforms, int nPlatforms){
    if(!_isDead){
        for(int i = 0; i < nPlatforms; i++){
            
            if(platforms[i]->getY() > 0){
                std::pair<int, bool> collision = platforms[i]->checkPlatformCollision(_x, _y);
                
                //return neg number if hit a spring
                if(collision.first){
                    return !collision.second ? (collision.first > 0 ? platforms[i]->getY() : platforms[i]->getY() - 48): -platforms[i]->getY();
                }
                
            }
            
            int en = platforms[i]->checkEnemyCollision(_x, _y);
            if(en == 1)
                kill();
        }
    }
    return -10000;
}

bool Player::checkEnemyCollision(Enemy* enemy){
    return enemy->checkCollision(_x, _y);

}

   
void Player::kill(){
    _isDead = true;
    _dy = 0;
   
}



void Player::shift(float amt){
    _y += amt;
}

float Player::getDY() const{
    return _dy;
}

void Player::revive(){
    _y = globals::SCREEN_HEIGHT + 40;
    _x = globals::SCREEN_WIDTH / 2;
    _dy = -player_constants::JUMP_SPEED;
    
    _isDead = false;
}

void Player::loadBall(){
    _balls[MAX_BALLS - 1]->load();
    
}

void Player::throwBall(int mouseX, int mouseY){
        _balls[MAX_BALLS - 1]->addTarget(mouseX, mouseY);
        Projectile* temp = _balls[0];
        
        for(int i = 0; i < MAX_BALLS - 1; i++){
            _balls[i] = _balls[i + 1];
        }
        _balls[MAX_BALLS - 1] = temp;
    
}


