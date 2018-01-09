//
//  cloud.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/8/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "cloud.h"
#include "globals.h"

Cloud::Cloud (Graphics &graphics, float x, float y) :
Sprite(graphics, "Cloud", 0, 0, 16, 16, x, y)
{
    _dx = static_cast<float>(globals::randInt(0, 10)) / 100;
}

void Cloud::fixedUpdate(float fixedTime){
    if(_y > -_height - 20)
        _x += _dx * fixedTime;
    
}

void Cloud::draw(Graphics &graphics){
    Sprite::draw(graphics, _x, _y, globals::SPRITE_SCALE);
}

void Cloud::changeDir() {
    _dx *= -1;
}

void Cloud::shift(float y){
    _y += y;
}
