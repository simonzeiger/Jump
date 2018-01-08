//
//  game.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/30/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#ifndef game_hpp
#define game_hpp

#include "player.h"
#include "world.h"

class Graphics;

class Game {
public:
    Game();
    ~Game();
private:
    void gameLoop();
    void draw(Graphics &graphics);
    void update(float elsapedTime);
    void fixedUpdate(float fixedTime);
    Player _player;
    World _world;
};

#endif /* game_hpp */
