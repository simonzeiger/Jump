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
#include "sprite.h"
#include "globals.h"
#include "spring.h"

class Platform: public Collidable, public Sprite {
public:
    Platform(float x, float y, Graphics &graphics);
    ~Platform();
    
    void draw(Graphics &graphics);
    void fixedUpdate(float fixedTime);
    void addSpring(bool lOrR, Graphics &graphics);
    void deleteSpring();
    void shift(float y);
    void setY(float y);
    void setX(float x);
    bool hasSpring();
    
    std::pair<bool, bool> checkPlatformCollision(float playerX, float playerY);
    std::string getType();
private:
    Collidable* _spring;
};
#endif /* platform_hpp */
