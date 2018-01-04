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
Collidable(x, y, 80, 0)
{
    _x = x;
    _y = y;
    _prevPlayerY = globals::SCREEN_HEIGHT;
}


void Platform::draw(Graphics &graphics){
    SDL_SetRenderDrawColor(graphics.renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(graphics.renderer(), _x, _y, _x + _width, _y);
}




