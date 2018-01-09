//
//  NumSprite.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/9/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "numsprite.h"
#include "graphics.h"

NumSprite::NumSprite (Graphics &graphics, int x, int y) :
Sprite(graphics, "Nums", 0, 0 , 32, 32, x, y){
    _x = x;
    _y = y;
    num = 0;
}

void NumSprite::draw(Graphics &graphics){
    
    if(num >= 0){
        SDL_Rect destinationRectangle = { _x, _y, _sourceRect.w,
            _sourceRect.h };
        
        SDL_Rect sourceRect = {32 * num, 0, 32, 32};
        
        graphics.blitSurface(_spriteSheet, &sourceRect, &destinationRectangle);
    }

}
