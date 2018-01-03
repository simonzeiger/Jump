//
//  platform.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "platform.hpp"
#include <iostream>

using std::cout;
using std::endl;

Platform::Platform(float x, float y) :
Collidable(x, y)
{
    _x = x;
    _y = y;
    _prevPlayerY = 0;
}

void Platform::update(){
    
}

void Platform::draw(Graphics &graphics){
    SDL_SetRenderDrawColor(graphics.renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(graphics.renderer(), _x, _y, _x + PLATFORM_WIDTH, _y);
}

bool Platform::checkCollision(float playerX, float playerY) {
    
    if(_prevPlayerY + 16 * globals::SPRITE_SCALE < _y){
        _prevPlayerY = playerY;

        if(playerY + 16 * globals::SPRITE_SCALE > _y){
            // + and - 15 to account for hitbox
            if(playerX + 25 > _x && playerX + 16 * globals::SPRITE_SCALE - 25 < _x + PLATFORM_WIDTH)
                return true;
        }
    }
    _prevPlayerY = playerY;

    return false;
}


