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
    if(_spriteSheets["Ball"] == _spriteSheet || (_y + _height * scale > -20 && _y < globals::SCREEN_HEIGHT + 20)) {
        
        SDL_Rect destinationRectangle = { x, y, _sourceRect.w * scale,
            _sourceRect.h * scale };
        graphics.blitSurface(_spriteSheet, &_sourceRect, &destinationRectangle);

    }
}

void Sprite::flush() {
    std::map<std::string, SDL_Texture*>::iterator i = _spriteSheets.begin();
    for(; i != _spriteSheets.end();++i)
        SDL_DestroyTexture(i->second);
}

int Sprite::width() const{
    return _width;
}

int Sprite::height() const{
    return _height;
}

void Sprite::setSpriteSheet(SDL_Texture* texture, int width, int height){
    _width = width;
    _height = height;
    _sourceRect.w = width;
    _sourceRect.h = height;
    
    for (auto &i : _spriteSheets) {
        if (i.second == _spriteSheet) {
            i.second = texture;
            break; 
        }
    }

    SDL_DestroyTexture(_spriteSheet);
    _spriteSheet = texture;
    
   
    
}



float Sprite::getX() const{
    return _x;
}

float Sprite::getY() const{
    return _y;
}

void Sprite::setX(float x) {
    _x = x;
}

void Sprite::setY(float y) {
    _y = y;
}

