//
//  sprite.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

//TODO: Figure out if a defualt constructor is necessary

#ifndef sprite_h
#define sprite_h

#include <SDL2/SDL.h>

#include <string>
#include <map>

class Graphics;

/* Sprite class
 * Holds all information for individual sprites
 */

class Sprite {
public:
    Sprite(Graphics &graphics, const std::string name, int sourceX, int sourceY, int width, int height,
           float posX, float posY);
    virtual ~Sprite();
    void draw(Graphics &graphics, int x, int y, int scale);
    static void addTexture(std::string name, SDL_Texture* texture);
    int width();
    int getX() const;
    int getY() const;
    void setX(float x);
    void setY(float y);
    
protected:

    SDL_Rect _sourceRect;
    SDL_Texture* _spriteSheet;
    float _x, _y;
    int _width,  _height;
private:
     static std::map<std::string, SDL_Texture*> _spriteSheets;
};

#endif /* sprite_hpp */
