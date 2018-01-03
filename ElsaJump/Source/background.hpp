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
    void draw(Graphics &graphics);
    void update(float elapsedTime);
    Platform** platforms();
    int nPlatforms() const;
    void shift();

private:
    Platform* _platforms[10];
    void addPlatform(float x, float y);
    int _nPlatforms;
    bool _shifting;
    float _shiftCount;
};

#endif /* background_hpp */
