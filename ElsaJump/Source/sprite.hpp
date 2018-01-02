//
//  sprite.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

//TODO: Figure out if a defualt constructor is necessary

#ifndef sprite_hpp
#define sprite_hpp

#include <SDL2/SDL.h>

#include <string>

class Graphics;

/* Sprite class
 * Holds all information for individual sprites
 */

class Sprite {
public:
    Sprite();
    Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
           float posX, float posY);
    virtual ~Sprite();
    virtual void update();
    void draw(Graphics &graphics, int x, int y, int scale);
protected:
    SDL_Rect _sourceRect;
    SDL_Texture* _spriteSheet;
    float _x, _y;
    int _width,  _height;
};

#endif /* sprite_hpp */