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
#include <vector>

using std::vector;

class Background {
public:
    Background();
    void draw(Graphics &graphics);
    void update(float elapsedTime);
    vector<Platform> platforms() const;

private:
    vector<Platform> _platforms;
    int _nPlatforms;
};

#endif /* background_hpp */
