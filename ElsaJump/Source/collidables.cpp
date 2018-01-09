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


//Platform
#include <iostream>

using std::cout;
using std::endl;

Platform::Platform(float x, float y, Graphics &graphics) :
Collidable(x, y, 64, 0),
Sprite(graphics, "Elsa", 0, 0, 65, 0, x, y)
{
    _spring = nullptr;
}



Platform::~Platform(){
    
}

void Platform::draw(Graphics &graphics){
    if(Collidable::_y + Collidable::_height > -20){
        SDL_SetRenderDrawColor(graphics.renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(graphics.renderer(), Sprite::_x, Sprite::_y, Sprite::_x + Sprite::_width, Sprite::_y);
        if(_spring != nullptr)
            _spring->draw(graphics);
    }
    
    
}

void Platform::fixedUpdate(float elapsedTime){
    if(_spring != nullptr)
        _spring->fixedUpdate(elapsedTime);
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
    bool spring = _spring != nullptr;
    bool platform = Collidable::checkCollision(playerX, playerY);
    if(spring)
        spring = _spring->checkCollision(playerX, playerY);
    //if(platform && spring) printf("%d ", spring);
    
    return std::pair<bool, bool> (platform, spring);
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
AnimatedSprite(graphics, "Spring", 0, 0, 32, 32, x, y, 100)
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
    playAnimation("Smoll");
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



