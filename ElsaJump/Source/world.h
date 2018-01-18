//
//  background.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef background_h
#define background_h

#include "sprite.h"
#include "globals.h"
#include <vector>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_ttf.h>
#else
#include <SDL2_ttf/SDL_ttf.h>
#endif

class Enemy;
class Player;
class Platform;
class Cloud;
class Powerup;

class World {
public:
    World(Player *player, Graphics* graphics);
    ~World();
    void draw();
    void update();
    void fixedUpdate(float fixedTime);
    Platform** platforms();
    int nPlatforms() const;
    int score();
    void resetScore();
private:
    void shift();
    void addPlatform(int x, int y);
    void initPlatforms();
    void resetPlatform(Platform* platform);
    void resetAll();
    Vector2<int> getNextPlatformPos();
    void initClouds();
    void resetCloud(Cloud* cloud);
    Vector2<int> getNextCloudPos();
    
    static const int MAX_PLATFORMS = 25;
    Platform* _platforms[MAX_PLATFORMS];
    static const int MAX_CLOUDS = 10;
    Cloud* _clouds[MAX_CLOUDS];
    std::vector<Sprite> _scoreSprites;
    Platform* _enemyPlatform;
    Powerup* _powerUp;

    
    float _score;
    int _nPlatforms;
    bool _shifting;
    float _shiftCount;
    float _shiftDistance;
    float _prevPlayerY;
    Platform* _topPlatform;
    Cloud* _topCloud;
    Player* _player;
    Graphics* _graphics;
    
    
    int _highScoreCounter;
    std::vector<std::pair<std::string, int> > _highScores;
    int _highScoreY;

    

   
    
   
};

extern TTF_Font* font;

#endif /* background_hpp */
