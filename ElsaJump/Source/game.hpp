//
//  game.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/30/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#ifndef game_hpp
#define game_hpp

#include "player.hpp"
#include "background.hpp"

class Graphics;

class Game {
public:
    Game();
    ~Game();
private:
    void gameLoop();
    void draw(Graphics &graphics);
    void update(float elsapedTime);
    Player _player;
    Background _background;
};

#endif /* game_hpp */
