//
//  background.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef background_hpp
#define background_hpp

#include "graphics.hpp"
#include "platform.hpp"
#include "player.hpp"
#include "globals.h"


class Background {
public:
    Background();
    Background(Player *player);
    void draw(Graphics &graphics);
    void update(float elapsedTime);
    Platform** platforms();
    int nPlatforms() const;
    void shift(float y);
    constexpr static float SHIFT_RATE = 1; //1
private:
    static const int MAX_PLATFORMS = 20;
    Platform* _platforms[MAX_PLATFORMS];
    static const int MAX_DISTANCE = 178;
    static const int MIN_DISTANCE = 15;
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
};
#endif /* background_hpp */
