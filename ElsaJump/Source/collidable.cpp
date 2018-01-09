//
//  collidable.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/2/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "collidable.h"
#include "globals.h"

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


