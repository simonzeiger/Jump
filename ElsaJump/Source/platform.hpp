//
//  platform.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef platform_hpp
#define platform_hpp

#include "graphics.hpp"
#include "collidable.hpp"
#include "globals.h"

class Platform: public Collidable {
public:
    Platform(float x, float y);
    void draw(Graphics &graphics);
    void update();
    bool checkCollision(float playerX, float playerY);
    const static int PLATFORM_WIDTH = 80;
    int getY() const;

private:
    Vector2<float> _prevPlayerPos;
};
#endif /* platform_hpp */
