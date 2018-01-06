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

Platform::Platform(float x, float y) :
Collidable(x, y, 80, 0)
{
    _spring = nullptr;
}



Platform::~Platform(){
   
}

void Platform::draw(Graphics &graphics){
    SDL_SetRenderDrawColor(graphics.renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(graphics.renderer(), _x, _y, _x + _width, _y);
    if(_spring != nullptr)
        _spring->draw(graphics);

}

void Platform::update(float elapsedTime){
    if(_spring != nullptr)
        _spring->update(elapsedTime);
}

void Platform::addSpring(bool lOrR, Graphics &graphics) {
    if(_spring == nullptr){
        float x = lOrR? _x : _x + _width - 32;
        _spring = new Spring(x, _y - 32, graphics);
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

void Platform::shift(float y){
    Collidable::shift(y);
    if(_spring != nullptr) _spring->shift(y);
    
}
