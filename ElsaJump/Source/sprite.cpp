//
//  sprite.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#include "sprite.hpp"
#include "graphics.hpp"
#include "globals.h"

Sprite::Sprite(){}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
               float posX, float posY) :
_x(posX),
_width(width),
_height(height),
_y(posY)
{
    _sourceRect.x = sourceX;
    _sourceRect.y = sourceY;
    _sourceRect.w = width;
    _sourceRect.h = height;
    
    _spriteSheet = SDL_CreateTextureFromSurface(graphics.renderer(), graphics.loadImage(filePath));
    if (_spriteSheet == NULL) {
        printf("Error: Unable to load image\n");
        
    }
    
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics &graphics, int x, int y, int scale) {
    SDL_Rect destinationRectangle = { x, y, _sourceRect.w * scale,
        _sourceRect.h * scale };
    graphics.blitSurface(_spriteSheet, &_sourceRect, &destinationRectangle);
}

void Sprite::update() {}

