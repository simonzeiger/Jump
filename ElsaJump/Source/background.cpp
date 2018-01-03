//
//  background.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "background.hpp"
#include <iostream>
#include <SDL2/SDL.h>

Background::Background(){
    _nPlatforms = 0;
    addPlatform(50, 600);
    addPlatform(150, 450);
    addPlatform(250, 300);
    addPlatform(350, 150);
    addPlatform(250, -10);
    addPlatform(50, -100);

    _shifting = false;
    _shiftCount = 0;
}

void Background::update(float elapsedTime){
    //std::cout << _shiftCount << std::endl;
    if(_shifting){
        for(int i = 0; i < _nPlatforms; i++){
            _platforms[i]->shift(.6);
            
        }
        _shiftCount++;
        if(_shiftCount > 140){
            _shifting = false;
            _shiftCount = 0;
        }
    }
}

void Background::draw(Graphics &graphics){
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->draw(graphics);
    }
}

Platform** Background::platforms() {
    return _platforms;
}

int Background::nPlatforms() const {
    return _nPlatforms;
}

void Background::addPlatform(float x, float y){
    _platforms[_nPlatforms] = new Platform(x, y);
    _nPlatforms++;
}

void Background::shift(){
    _shifting = true;
}




