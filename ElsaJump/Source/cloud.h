//
//  cloud.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/8/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef cloud_h
#define cloud_h

#include "sprite.h"

class Cloud : public Sprite {

public:
    Cloud(Graphics &graphics, float x, float y);
    void fixedUpdate(float fixedTime);
    void draw(Graphics &graphics);
    void shift(float y);
    void changeDir();

private:
    float _dx;
    
};


#endif /* cloud_hpp */
