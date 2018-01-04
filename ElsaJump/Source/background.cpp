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

Background::Background(){}


Background::Background(Player* player) :
_player(player)
{
    _nPlatforms = 0;
    addPlatform(50, 600);
    addPlatform(150, 500);
    addPlatform(250, 300);
    addPlatform(350, 150);
    addPlatform(250, -10);
    addPlatform(50, -100);

    _shifting = false;
    _shiftCount = 0;
    _shiftDistance = 100;
    _prevPlayerY = globals::SCREEN_HEIGHT;
}

void Background::update(float elapsedTime){
    
    //linear deacceleration
   /*if(_shifting){
    
        float shiftAmt = SHIFT_RATE -  _shiftCount / _shiftDivider;

        for(int i = 0; i < _nPlatforms; i++){
            _platforms[i]->shift(shiftAmt);
            
        }
       _player->shift(shiftAmt);
       _prevPlayerY += shiftAmt;
        _shiftCount++;
        if(_shiftCount > SHIFT_RATE * _shiftDivider){
            _shifting = false;
            _shiftCount = 0;
        }
    }*/
    
    //quadratic ease
   /* if(_shifting){
        
        
        float shiftEase = (1 - ((_shiftCount / shiftDistance) * (_shiftCount / shiftDistance)));

        for(int i = 0; i < _nPlatforms; i++){
            _platforms[i]->shift(shiftEase);
            
        }
        _player->shift(shiftEase);
        _prevPlayerY += shiftEase;
        _shiftCount += 1;
        if(_shiftCount  >= shiftDistance){
            _shifting = false;
            _shiftCount = 0;
        }
    }
    
    _player->update(elapsedTime);
    
    if(!_player->_isJumping){
        int y  = _player->checkPlatformCollisions(_platforms, _nPlatforms);
        if(y >= 0){
            _player->jump(y);
            printf("Prev: %d Cuurr: %d\n", _prevPlayerY, y);
            if(_prevPlayerY > y) shift(y);
            _prevPlayerY = y;
            
        }
    } */
    
    /*lerp
    if(_shifting){
        
        _shiftDistance = _prevPlayerY - _player->getY();
        
        float shiftLerp = .006 * elapsedTime * _shiftDistance;
        printf("SD: %f SL: %f\n",  _shiftDistance, shiftLerp);
        for(int i = 0; i < _nPlatforms; i++){
            _platforms[i]->shift(shiftLerp);
            
        }
        //_player->shift(shiftLerp);
        //_prevPlayerY += shiftLerp;
        _shiftCount += shiftLerp;
       
        _shiftDistance -= shiftLerp;
    } */

    //lerpy but actually simple
    if(_shifting){
        
        _shiftCount += elapsedTime;
        
        if(_shiftCount > _prevPlayerY / 3){
            _shiftDistance = -_player->getDY() * elapsedTime;
            for(int i = 0; i < _nPlatforms; i++){
                _platforms[i]->shift(_shiftDistance);
                
            }
            
            _player->shift(_shiftDistance);
            
            _prevPlayerY += _shiftDistance;
        }
        
        if(!_player->_isJumping){
            _shifting = false;
            _shiftCount = 0;
        }
    }
    
    _player->update(elapsedTime);
    
    if(!_player->_isJumping){
        int y  = _player->checkPlatformCollisions(_platforms, _nPlatforms);
        if(y >= 0){
            _player->jump(y);
            //printf("Prev: %d Cuurr: %d\n", _prevPlayerY, y);
            if(_prevPlayerY > y) shift(y);
            _prevPlayerY = y;
            
        }
    }
    
   
}

void Background::draw(Graphics &graphics){
    _player->draw(graphics);

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

void Background::shift(float y){
    _shifting = true;
    _shiftCount = 0;
    
    //_shiftDistance = (globals::SCREEN_HEIGHT - y);
    //_shiftDistance = y;
    
}



