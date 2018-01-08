//
//  spring.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/5/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef spring_hpp
#define spring_hpp

#include "animatedsprite.h"
#include "graphics.h"
#include "collidable.h"
#include "globals.h"

class Spring: public Collidable, public AnimatedSprite {
public:
    Spring(float x, float y, Graphics &graphics);
    void draw(Graphics &graphics);
    void update(float elapsedTime);
    bool checkCollision(float playerX, float playerY);
    void animationDone(std::string currentAnimation);
    void setupAnimations();
    void shift(float y);
    void setX(float x);
    void setY(float y);
    std::string getType();
    
};

#endif /* spring_hpp */
