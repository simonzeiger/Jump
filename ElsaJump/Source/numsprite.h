//
//  NumSprite.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/9/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef numsprite_h
#define numsprite_h

#include "sprite.h"

class NumSprite : public Sprite {
    
public:
    NumSprite(Graphics &graphics, int x, int y);
    void draw(Graphics &graphics);
    int num;

private:
    int _x, _y;
};


#endif /* NumSprite_hpp */
