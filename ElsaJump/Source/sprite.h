//
//  sprite.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//


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
    int width() const;
     int height() const;
    float getX() const;
    float getY() const;
    void setX(float x);
    void setY(float y);
    static void flush();
    void setSpriteSheet(SDL_Texture* texture, int width, int height);
protected:
    SDL_Rect _sourceRect;
    SDL_Texture* _spriteSheet;
    float _x, _y;
    int _width,  _height;
    
private:
    static std::map<std::string, SDL_Texture*> _spriteSheets;

};

#endif /* sprite_hpp */
