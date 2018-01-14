//
//  collidable.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/2/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "collidables.h"
#include "globals.h"

//Base collidable class

Collidable::Collidable(float x, float y, int width, int height) :
_width(width),
_height(height)
{
    _x = x;
    _y = y;
    _prevPlayerY = globals::SCREEN_HEIGHT;
}

Collidable::~Collidable(){
    
}

int Collidable::checkCollision(float playerX, float playerY){
    if(_prevPlayerY + 16 * globals::SPRITE_SCALE < _y){
        _prevPlayerY = playerY;

        if(playerY + 16 * globals::SPRITE_SCALE > _y){
            // + and - 15 to account for hitbox
            if(playerX + 30 > _x && playerX + 15 < _x + _width){
                return 1;
            }
        }
    }
    _prevPlayerY = playerY;
    
    return 0;
}


void Collidable::setY(float y){
    _y = y;
}
void Collidable::setX(float x){
    _x = x;
}

void Collidable::shift(float y){
    _y += y;
}

int Collidable::getCollidableWidth() {
    return _width;
}

float Collidable::getX() {
    return _x;
}


//Platform
#include <iostream>
#include "enemy.h"

using std::cout;
using std::endl;

Platform::Platform(float x, float y, Graphics &graphics) :
Collidable(x, y, 64, 0),
AnimatedSprite(graphics, "Platform", 0, 0, 64, 64, x, y, 70)
{
    _spring = nullptr;
    _isMoving = false;
    _onlyOnce = false;
    _destroyOnce = false;
    _fake = false;
    _dy = .1;
    _enemy = nullptr;
    setupAnimations();
    playAnimation("Normal");
}



Platform::~Platform(){
    
}

void Platform::setupAnimations() {
    addAnimation(1, 0, 0, "Normal", 64, 64);
    addAnimation(1, 0, 64, "Cracked", 64, 64);
    addAnimation(2, 64, 64, "Break", 64, 64);
    addAnimation(1, 128, 64, "FullyBroken", 64, 64);
    addAnimation(1, 0 , 128, "OnlyOnce", 64, 64);
    addAnimation(1, 0 , 192, "Moving", 64, 64);
}

void Platform::animationDone(std::string currentAnimation){
    if(currentAnimation == "Break")
        playAnimation("FullyBroken");
        
}

void Platform::draw(Graphics &graphics){
    if((!_destroyOnce || _fake) && Collidable::_y + Collidable::_height > -AnimatedSprite::_height){
        AnimatedSprite::draw(graphics, Collidable::_x, Collidable::_y, globals::PLATFORM_SCALE);
        if(_spring != nullptr)
            _spring->draw(graphics);
        if(_enemy != nullptr)
            _enemy->draw(graphics);
    }
    
}

void Platform::fixedUpdate(float fixedTime){
    AnimatedSprite::fixedUpdate(fixedTime);
    if(_isMoving){
        float end = globals::SCREEN_WIDTH - getCollidableWidth();
        if(Collidable::_x >= end) {
            _dx *= -1;
            Collidable::_x = end - 1;
            Sprite::_x = end - 1;
             if(_spring != nullptr)
                 _spring->setX( _spring->getX() - 1);
             if(_enemy != nullptr)
                _enemy->setX(end - 1);
        } else if  (Collidable::_x <= 0) {
            _dx *= -1;
            Collidable::_x = 1;
            Sprite::_x = 1;
             if(_spring != nullptr)
                 _spring->setX( _spring->getX() + 1);
            if(_enemy != nullptr)
                _enemy->setX(1);
            
        }
        Collidable::_x += _dx * fixedTime;
        Sprite::_x += _dx * fixedTime;
        if(_spring != nullptr)
             _spring->setX( _spring->getX() + _dx * fixedTime);
        if(_enemy != nullptr)
            _enemy->setX( _enemy->Sprite::getX() + _dx * fixedTime);
    }
    
    if(_destroyOnce && _fake){
        setY(AnimatedSprite::_y + (_dy += .01) * fixedTime);
    }
    
    if(_spring != nullptr)
        _spring->fixedUpdate(fixedTime);
    if(_enemy != nullptr)
        _enemy->fixedUpdate(fixedTime);
}

void Platform::enableMoving(float speed){
    _isMoving = true;
    _dx = globals::randInt(0, 1) ? -speed : speed;
    playAnimation("Moving");
}

bool Platform::isReal() {
    return !_fake;
}

bool Platform::isOnlyOnce() {
    return _onlyOnce;
}


void Platform::disableMoving(){
    _isMoving = false;
    
}

bool Platform::isMoving() {
    return _isMoving;
}

void Platform::enableOnlyOnce() {
    _onlyOnce = true;
    _destroyOnce = false;
    playAnimation("OnlyOnce");
    
}

void Platform::disableOnlyOnce() {
    _onlyOnce = false;
    _destroyOnce = false;
}

void Platform::makeFake() {
    _fake = true;
    playAnimation("Cracked");
}

void Platform::reset() {
    deleteSpring();
    deleteEnemy();
    disableMoving();
    disableOnlyOnce();
    makeReal();
    _dy = 0;
    playAnimation("Normal");
}

void Platform::makeReal(){
    _fake = false;
    playAnimation("Normal");
}

void Platform::addSpring(bool lOrR, Graphics &graphics) {
    if(_spring == nullptr){
        float x = lOrR? Collidable::_x : Collidable::_x + Collidable::_width - 32;
        _spring = new Spring(x, Collidable::_y - 32, graphics);
    }
}

void Platform::deleteSpring() {
    delete _spring;
    _spring = nullptr;
}

Enemy*  Platform::addEnemy(Graphics &graphics){
    _enemy = new Enemy(Sprite::_x, Sprite::_y - 48, graphics);
    return _enemy;
}
void  Platform::deleteEnemy(){
    delete _enemy;
    _enemy = nullptr;
}

bool Platform::hasEnemy(){
    return _enemy != nullptr;
}

int Platform::checkEnemyCollision(float playerX, float playerY){
    if(_enemy == nullptr)
        return -10000;
    return _enemy->checkCollision( playerX, playerY);
    
}

// int: -1 for enemy, 0 for nada, 1 for platform
std::pair<int, bool> Platform::checkPlatformCollision(float playerX, float playerY) {
    
    if(!_destroyOnce){
        bool spring = _spring != nullptr;
        int platform = 0;
        
        int y = Sprite::_y;
        if(hasEnemy())
            y -= 48;
        
        if(_prevPlayerY + 16 * globals::SPRITE_SCALE < y){
            _prevPlayerY = playerY;
            
            if(playerY + 16 * globals::SPRITE_SCALE > y){
                // + and - 15 to account for hitbox
                if(playerX + 30 > Collidable::_x && playerX + 15 < Collidable::_x + Collidable::_width){
                    if(hasEnemy()) {
                        platform = -1;
                        deleteEnemy();
                    } else
                        platform = 1;
                }
            }
        }
        
        _prevPlayerY = playerY;
        
        if(spring)
            spring = _spring->checkCollision(playerX, playerY);
        if(platform){
            if (_fake){
                playAnimation("Break");
                _destroyOnce = true;
                _dx = 0;
                return std::pair<int, bool> (false, false);
            } else if(_onlyOnce)
                _destroyOnce = true;
        }
      
        return std::pair<int, bool> (platform, spring);
    }
        
    return std::pair<int, bool> (false, false);
    
}

void Platform::shift(float y){
    Collidable::shift(y);
    Sprite::_y += y;
    if(_spring != nullptr) _spring->shift(y);
    if(_enemy != nullptr) _enemy->shift(y);

    
}

Enemy& Platform::enemy() {
    return *_enemy;
}

bool Platform::hasSpring(){
    return _spring != nullptr;
}

void Platform::setX(float x){
    Collidable::setX(x);
    Sprite::_x = x;
    if(_spring != nullptr)
        _spring->setX(x);
    if(_enemy != nullptr)
        _enemy->setX(x);
}

void Platform::setY(float y){
    Collidable::setY(y);
    Sprite::_y = y;
    if(_spring != nullptr)
        _spring->setY(y - 32);
    if(_enemy != nullptr)
        _enemy->setY(y - 48);
}

//Spring

Spring::Spring(float x, float y, Graphics &graphics) :
Collidable(x, y, 32, 32),
AnimatedSprite(graphics, "Spring", 0, 0, 32, 32, x, y, 50)
{
    Collidable::_x = x;
    Collidable::_y = y;
    
    setupAnimations();
    playAnimation("Smoll");
}


void Spring::draw(Graphics &graphics){
    AnimatedSprite::draw(graphics, Collidable::_x, Collidable::_y, 1);
    
}


void Spring::fixedUpdate(float fixedTime){
    AnimatedSprite::fixedUpdate(fixedTime);
    
}

int Spring::checkCollision(float playerX, float playerY){
    if(Collidable::checkCollision(playerX, playerY - 32)){
        playAnimation("Springy", false);
        return 1;
    }
    return 0;
    
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

void Spring::setX(float x){
    Collidable::setX(x);
    AnimatedSprite::_x = x;
}

void Spring::setY(float y){
    playAnimation("Smoll");
    Collidable::setY(y);
    AnimatedSprite::_y = y;
}

void Spring::animationDone(std::string currentAnimation) {
    if(currentAnimation == "Springy") playAnimation("Yuge");
}





