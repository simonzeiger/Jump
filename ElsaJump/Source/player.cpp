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
    const float ENEMY_JUMP_SPEED = .7 * MULTI;
    const float MOVE_SPEED = .28f  * MULTI;
    const float JETPACK_SPEED = 1.35;
    const float JETPACK_HORIZ_SPEED = .35f;
    const int SPRING_TIMES = 3;
    const int JETPACK_TIME = 4000;
}


 
Player::Player(Graphics &graphics, float x, float y) :
AnimatedSprite(graphics, "Elsa", 0, 0, 16, 16, x, y, 200),
_dy(-player_constants::JUMP_SPEED),
_dx(0),
_hasShield(false),
_isJumping(true),
_maxJumpHeightReached(false),
_facing(RIGHT),
_isDead(false),
_hasSpring(false),
_hasJetPack(false),
_jetPackTimer(0),
_springCounter(0)
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
    if(_hasJetPack)
        _dx = -player_constants::JETPACK_HORIZ_SPEED;
    else
        _dx = -player_constants::MOVE_SPEED;

    _facing = LEFT;
   
}

void Player::moveRight() {
    if(_hasJetPack)
        _dx = player_constants::JETPACK_HORIZ_SPEED;
    else
        _dx = player_constants::MOVE_SPEED;
    _facing = RIGHT;
}


void Player::stopMoving() {
    _dx = 0;
}

void Player::jetPack(){
    if(_jetPackTimer < player_constants::JETPACK_TIME){
        _dy = -player_constants::JETPACK_SPEED;
        _isJumping = true;
        _hasJetPack = true;
    }
}


void Player::jump(float y, std::string jumpMode) {
    if(_hasSpring){
        _springCounter++;
        if(_springCounter == player_constants::SPRING_TIMES){
            _springCounter = 0;
            _hasSpring = false;
        }
    }
    _isJumping = true;
    if(jumpMode == "Spring"){
        _dy = -player_constants::SPRING_SPEED;
    } else if(jumpMode == "Normal"){
        
        
        _dy = -player_constants::JUMP_SPEED;
        
    } else if (jumpMode == "Enemy"){
        _dy = -player_constants::ENEMY_JUMP_SPEED;
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
        if(_jetPackTimer != 0){
            _jetPackTimer = 0;
            _hasJetPack = false;
        }
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
    
    if(_hasJetPack){
        jetPack();
        _jetPackTimer += fixedTime;
    }
    
    
    //Apply gravity
    _dy += player_constants::GRAVITY * fixedTime;
    
    
    
    //Move by dy
    if(!_isDead || _y < globals::SCREEN_HEIGHT)
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

Projectile** Player::balls(){
    return _balls;
}

int Player::checkPlatformCollisions(Platform** platforms, int nPlatforms){
    if(!_isDead){
        for(int i = 0; i < nPlatforms; i++){
            
            if(platforms[i]->getY() > 0 && !_isJumping){
                std::pair<int, bool> collision = platforms[i]->checkPlatformCollision(_x, _y);
                
                //return neg number if hit a spring
                if(collision.first){
                    //if collision.first is neg. than u actually jumped on a monster
                    float y = !collision.second && !_hasSpring ? (collision.first > 0 ? platforms[i]->getY() : platforms[i]->getY() - 48) : -platforms[i]->getY();
                    if(y < 0)
                        jump(-y, "Spring");
                    else if(collision.first < 0 )
                        jump(y, "Enemy");
                    else
                        jump(y);
                    return y;
                }
                
            }
            
            if( platforms[i]->hasEnemy()){
                int en = platforms[i]->checkEnemyCollision(_x, _y);
                if(en == 1 && !_hasJetPack)
                    kill();
            }
        }
    }
    return -10000;
}

bool Player::checkEnemyCollision(Enemy* enemy){
    return enemy->checkCollision( _x, _y);

}

   
void Player::kill(){
    _isDead = true;
    _dy = 0;
    _hasJetPack = false;
    _hasSpring = false;
   
}

void Player::goldSpring() {
    _hasSpring = true;
}

bool Player::hasJetPack(){

    return _jetPackTimer < player_constants::JETPACK_TIME && _jetPackTimer != 0;
}



void Player::shift(float amt){
    _y += amt;
}

float Player::getDY() const{
    return _dy;
}

bool Player::hasSpring(){
    return _hasSpring;
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


