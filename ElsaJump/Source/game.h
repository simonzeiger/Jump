//
//  game.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/30/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#ifndef game_h
#define game_h

#include "player.h"
#include "numsprite.h"
#include "world.h"
#include "graphics.h"

class Graphics;

class Game {
public:
    Game();
    ~Game();
private:
    void gameLoop();
    void draw();
    void update( );
    void fixedUpdate(float fixedTime);
    Player* _player;
    World* _world;
    Graphics* _graphics;
    static const int _nNumSprites = 10;
    NumSprite* _numSprites[_nNumSprites];
};

#endif /* game_hpp */
