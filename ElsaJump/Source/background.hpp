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
    Platform* _platforms[10];
    void addPlatform(float x, float y);
    int _nPlatforms;
    bool _shifting;
    float _shiftCount;
    float _shiftDistance;
    int _prevPlayerY;
    Player* _player;
};
#endif /* background_hpp */
