//
//  sprite.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#include "sprite.h"
#include "graphics.h"
#include "globals.h"

std::map<std::string, SDL_Texture*> Sprite::_spriteSheets;


Sprite::Sprite(Graphics &graphics, const std::string name, int sourceX, int sourceY, int width, int height,
               float posX, float posY) :
_x(posX),
_width(width),
_height(height),
_y(posY)
{
   
    _spriteSheet = _spriteSheets[name];
    
    _sourceRect.x = sourceX;
    _sourceRect.y = sourceY;
    _sourceRect.w = width;
    _sourceRect.h = height;
    
    if (_spriteSheet == NULL) {
        printf("Error: Unable to load image\n");
        
    }
    
}

Sprite::~Sprite() {}

void Sprite::addTexture(std::string name, SDL_Texture* texture){
    _spriteSheets[name] = texture;
}

void Sprite::draw(Graphics &graphics, int x, int y, int scale) {
    if(_y + _height * scale > -20) {
        SDL_Rect destinationRectangle = { x, y, _sourceRect.w * scale,
            _sourceRect.h * scale };
        graphics.blitSurface(_spriteSheet, &_sourceRect, &destinationRectangle);

    }
}

int Sprite::width() {
    return _width;
}

int Sprite::getX() const{
    return _x;
}

int Sprite::getY() const{
    return _y;
}

void Sprite::setX(float x) {
    _x = x;
}

void Sprite::setY(float y) {
    _y = y;
}

