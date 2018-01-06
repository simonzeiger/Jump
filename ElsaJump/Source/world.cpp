//
//  background.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "world.h"
#include "globals.h"
#include <iostream>
#include <SDL2/SDL.h>

World::World(){}


World::World(Player* player, Graphics* graphics) :
_player(player),
_graphics(graphics)
{
    _nPlatforms = 0;
    initPlatforms();

    _shifting = false;
    _shiftCount = 0;
    _shiftDistance = 100;
    _prevPlayerY = globals::SCREEN_HEIGHT;
}

void World::update(float elapsedTime){
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
        
        if(_shiftCount > _prevPlayerY / 3.5){
            _shiftDistance = -_player->getDY() * elapsedTime;
            for(int i = 0; i < _nPlatforms; i++){
                _platforms[i]->shift(_shiftDistance);
                if(_platforms[i]->getY() > globals::SCREEN_HEIGHT  + 50){
                    resetPlatform(_platforms[i]);
                }
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
        if(y >= -globals::SCREEN_HEIGHT){
            printf("%d ", y);
            _player->jump(y);
            if(y < 0)
                y = -y;
            if(static_cast<int>(_prevPlayerY) > y) {
                shift(y);
            }
            _prevPlayerY = y;
            
        }
    }
    
    if(_player->getY() > globals::SCREEN_HEIGHT + 100){
        _player->killed();
        resetAll();
    }
    
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->update(elapsedTime);
    }

    
   
}

void World::draw(Graphics &graphics){
    _player->draw(graphics);

    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->draw(graphics);
    }
    
}

Platform** World::platforms() {
    return _platforms;
}

int World::nPlatforms() const {
    return _nPlatforms;
}

void World::addPlatform(int x, int y){
    if(_nPlatforms < MAX_PLATFORMS){
        _platforms[_nPlatforms] = new Platform(x, y);
        _platforms[_nPlatforms]->addSpring(true, *_graphics);
        _nPlatforms++;
    } else {
        printf("Exceeds max platforms");
    }
}

void World::initPlatforms(){
    for(int i = 0; i < MAX_PLATFORMS; i++){
        Vector2<int> nextPos = getNextPlatformPos();
        addPlatform(nextPos.X, nextPos.Y);
    }
}

Vector2<int> World::getNextPlatformPos(){
    int prevY = _nPlatforms == 0 ? globals::SCREEN_HEIGHT + 82 : _platforms[_nPlatforms - 1]->getY();
    int randY = randInt(_nPlatforms == 0 ? 120 : MIN_DISTANCE, MAX_DISTANCE);
    int randX = _nPlatforms == 0 ? randInt(globals::SCREEN_WIDTH / 2 - 40, globals::SCREEN_WIDTH / 2 + 40) : randInt(0, globals::SCREEN_WIDTH - 80);
    return Vector2<int>{randX, prevY - randY};
}

void World::resetPlatform(Platform* platform) {
    Vector2<int> nextPos = getNextPlatformPos();
    platform->setX(nextPos.X);
    platform->setY(nextPos.Y);
    Platform temp = *_platforms[_nPlatforms - 1];
    *_platforms[_nPlatforms - 1] = *platform;
    _platforms[_nPlatforms - 1]->deleteSpring();
    *platform = temp;
}

void World::resetAll(){
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->deleteSpring();
        delete _platforms[i];
    }
    _nPlatforms = 0;
    initPlatforms();
}

void World::shift(float y){
    _shifting = true;
    _shiftCount = 0;
    
    //_shiftDistance = (globals::SCREEN_HEIGHT - y);
    //_shiftDistance = y;
    
}



