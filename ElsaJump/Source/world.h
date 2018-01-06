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
    World();
    World(Player *player, Graphics* graphics);
    void draw(Graphics &graphics);
    void update(float elapsedTime);
    Platform** platforms();
    int nPlatforms() const;
    void shift(float y);
private:
    static const int MAX_PLATFORMS = 15;
    Platform* _platforms[MAX_PLATFORMS];
    static const int MAX_DISTANCE = 178;
    static const int MIN_DISTANCE = 25;
    void addPlatform(int x, int y);
    void initPlatforms();
    void resetPlatform(Platform* platform);
    void resetAll();
    Vector2<int> getNextPlatformPos();
    int _nPlatforms;
    bool _shifting;
    float _shiftCount;
    float _shiftDistance;
    float _prevPlayerY;
    Player* _player;
    Graphics* _graphics;
};
#endif /* background_hpp */
