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
#ifndef __EMSCRIPTEN__
    std::string _path;
#endif
    bool gameLoop(SDL_Event &event, int &lastUpdateTime, bool &ballLoaded, std::string &inputText);
    static std::vector<std::pair<std::string, int> > highScores();
private:
    void draw();
    void update( );
    void fixedUpdate(float fixedTime);
    void displayEndGame();
    bool checkSpriteClick(const Sprite &sprite, int mouseX, int mouseY);
    Player* _player;
    World* _world;
    Graphics* _graphics;
   static const int _nNumSprites = 10;
    NumSprite* _numSprites[_nNumSprites];
#ifndef __EMSCRIPTEN__
    static std::vector<std::pair<std::string, int> > _highScores;
    std::string _playerName;
#endif
    std::vector<Sprite> _endGameSprites;
    int _editTextTimer;
    bool _startEditing;
    int _textTimer;
   
    
};

#endif /* game_hpp */
