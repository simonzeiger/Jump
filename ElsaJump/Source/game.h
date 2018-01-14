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
#include <vector>


class Graphics;

class Game {
public:
    Game();
    ~Game();
    std::string _path;
    static std::vector<std::pair<std::string, int> > highScores();
private:
    void gameLoop();
    void draw();
    void update( );
    void fixedUpdate(float fixedTime);
    Player* _player;
    World* _world;
    Graphics* _graphics;
    std::string _playerName;
    static const int _nNumSprites = 10;
    NumSprite* _numSprites[_nNumSprites];
    static std::vector<std::pair<std::string, int> > _highScores;

    // Driver function to sort the vector elements
    // by second element of pairs
   
    
};

#endif /* game_hpp */
