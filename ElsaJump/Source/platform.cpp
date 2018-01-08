//
//  platform.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "platform.h"
#include <iostream>

using std::cout;
using std::endl;

Platform::Platform(float x, float y, Graphics &graphics) :
Collidable(x, y, 65, 0),
Sprite(graphics, "Elsa", 0, 0, 65, 0, x, y)
{
    _spring = nullptr;
}



Platform::~Platform(){
   
}

void Platform::draw(Graphics &graphics){
    SDL_SetRenderDrawColor(graphics.renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(graphics.renderer(), Sprite::_x, Sprite::_y, Sprite::_x + Sprite::_width, Sprite::_y);
    if(_spring != nullptr)
        _spring->draw(graphics);

}

void Platform::update(float elapsedTime){
    if(_spring != nullptr)
        _spring->update(elapsedTime);
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
