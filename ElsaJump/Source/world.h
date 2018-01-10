//
//  background.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef background_h
#define background_h

#include "graphics.h"
#include "collidables.h"
#include "player.h"
#include "cloud.h"
#include "globals.h"


class World {
public:
    World(Player *player, Graphics* graphics);
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
    
   
    
   
};
#endif /* background_hpp */
