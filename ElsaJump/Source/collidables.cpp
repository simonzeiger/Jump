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

bool Collidable::checkCollision(float playerX, float playerY){
    if(_prevPlayerY + 16 * globals::SPRITE_SCALE < _y){
        _prevPlayerY = playerY;

        if(playerY + 16 * globals::SPRITE_SCALE > _y){
            // + and - 15 to account for hitbox
            if(playerX + 30 > _x && playerX + 15 < _x + _width){
                return true;
            }
        }
    }
    _prevPlayerY = playerY;
    
    return false;
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

using std::cout;
using std::endl;

Platform::Platform(float x, float y, Graphics &graphics) :
Collidable(x, y, 64, 0),
AnimatedSprite(graphics, "Platform", 0, 0, 64, 64, x, y, 100)
{
    _spring = nullptr;
    _isMoving = false;
    _onlyOnce = false;
    _fake = false;
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
}

void Platform::animationDone(std::string currentAnimation){
    if(currentAnimation == "Break")
        playAnimation("FullyBroken");
        
}

void Platform::draw(Graphics &graphics){
    if(!_destroyOnce && Collidable::_y + Collidable::_height > -Sprite::_height){
        AnimatedSprite::draw(graphics, Collidable::_x, Collidable::_y, globals::PLATFORM_SCALE);
        if(_spring != nullptr)
            _spring->draw(graphics);
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
        } else if  (Collidable::_x <= 0) {
            _dx *= -1;
            Collidable::_x = 1;
            Sprite::_x = 1;
            
        }
        Collidable::_x += _dx * fixedTime;
        Sprite::_x += _dx * fixedTime;
         if(_spring != nullptr)
             _spring->setX( _spring->getX() + _dx * fixedTime);
    }
    
    if(_spring != nullptr)
        _spring->fixedUpdate(fixedTime);
}

void Platform::enableMoving(float speed){
    _isMoving = true;
    _dx = globals::randInt(0, 1) ? -speed : speed;
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
    disableMoving();
    disableOnlyOnce();
    makeReal();
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

std::pair<bool, bool> Platform::checkPlatformCollision(float playerX, float playerY) {
    if(!_destroyOnce && !_fake){
        
        bool spring = _spring != nullptr;
        bool platform = Collidable::checkCollision(playerX, playerY);
        if(spring)
            spring = _spring->checkCollision(playerX, playerY);
        if(_onlyOnce && platform) _destroyOnce = true;
        return std::pair<bool, bool> (platform, spring);
    } else {
        return std::pair<bool, bool> (false, false);
    }
}

std::string Platform::getType(){
    return "Platform";
}

void Platform::shift(float y){
    Collidable::shift(y);
    Sprite::_y += y;
    if(_spring != nullptr) _spring->shift(y);
    
}

bool Platform::hasSpring(){
    return _spring != nullptr;
}

void Platform::setX(float x){
    Collidable::setX(x);
    Sprite::_x = x;
    if(_spring != nullptr)
        _spring->setX(x);
}

void Platform::setY(float y){
    Collidable::setY(y);
    Sprite::_y = y;
    if(_spring != nullptr)
        _spring->setY(y - 32);
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

std::string Spring::getType(){
    return "Spring";
}



