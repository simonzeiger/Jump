//
//  background.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef background_hpp
#define background_hpp

#include "graphics.h"
#include "platform.h"
#include "player.h"
#include "globals.h"


class World {
public:
    World(Player *player, Graphics* graphics);
    void draw(Graphics &graphics);
    void update(float elapsedTime);
    void fixedUpdate(float fixedTime);
    Platform** platforms();
    int nPlatforms() const;
    void shift(float y);
private:
    static const int MAX_PLATFORMS = 15;

    Platform* _platforms[MAX_PLATFORMS];
    int _nPlatforms;
    bool _shifting;
    float _shiftCount;
    float _shiftDistance;
    float _prevPlayerY;
    Platform* _topPlatform;
    Player* _player;
    Graphics* _graphics;
    
    void addPlatform(int x, int y);
    void initPlatforms();
    void resetPlatform(Platform* platform);
    void resetAll();
    Vector2<int> getNextPlatformPos();
    
   
};
#endif /* background_hpp */
