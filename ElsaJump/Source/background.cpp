//
//  background.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "background.hpp"
#include <SDL2/SDL.h>

Background::Background(){
    _platforms.push_back(Platform(50, 600));
    _nPlatforms = 1;
}

void Background::update(float elapsedTime){
    
}

void Background::draw(Graphics &graphics){
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i].draw(graphics);
    }
}

vector<Platform> Background::platforms() const {
    return _platforms;
}



