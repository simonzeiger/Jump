//
//  platform.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef platform_hpp
#define platform_hpp

#include "graphics.h"
#include "collidable.h"
#include "globals.h"
#include "spring.h"

class Platform: public Collidable {
public:
    Platform(float x, float y);
    ~Platform();
    void draw(Graphics &graphics);
    void update(float elapsedTime);
    void addSpring(bool lOrR, Graphics &graphics);
    void deleteSpring();
    void shift(float y);
    std::pair<bool, bool> checkPlatformCollision(float playerX, float playerY);
private:
    Spring* _spring;
};
#endif /* platform_hpp */
