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

    _shifting = false;
    _shiftCount = 0;
    _shiftDistance = 100;
    _prevPlayerY = globals::SCREEN_HEIGHT;
    _topPlatform = nullptr;
    
    initPlatforms();
}

void World::update(float elapsedTime){
    //lerpy but actually simple
    
    _player->update(elapsedTime);
    
    if(!_player->_isJumping){
        int y  = _player->checkPlatformCollisions(_platforms, _nPlatforms);
        if(y >= -globals::SCREEN_HEIGHT){
            _player->jump(y);
            if(y < 0){
                y*= -1;
                shift(y);
            } else if(static_cast<int>(_prevPlayerY) > y) {
                shift(y);
            }
            _prevPlayerY = y;
            
        }
    }
    
    if(_player->getY() > globals::SCREEN_HEIGHT + 100){
        _player->killed();
        _prevPlayerY = globals::SCREEN_HEIGHT;
        resetAll();
    }
    
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->update(elapsedTime);
    }

}

void World::fixedUpdate(float fixedTime){
    
    if(_shifting){
        
        
        _shiftCount += fixedTime;
        
        if(_shiftCount > _prevPlayerY / 3.5){
            _shiftDistance = -_player->getDY() * fixedTime;
            
            
            for(int i = 0; i < _nPlatforms; i++){
                _platforms[i]->shift(_shiftDistance);
                if(_platforms[i]->getY() > globals::SCREEN_HEIGHT + 50){
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
    
    _player->fixedUpdate(fixedTime);
}

void World::draw(Graphics &graphics){

    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->draw(graphics);
    }
    
    _player->draw(graphics);

    
}

Platform** World::platforms() {
    return _platforms;
}

int World::nPlatforms() const {
    return _nPlatforms;
}

void World::addPlatform(int x, int y){
    if(_nPlatforms < MAX_PLATFORMS){
        _platforms[_nPlatforms] = new Platform(x, y, *_graphics);
        if(randInt(1, SPRING_PROBABILITY) == 1)
            _platforms[_nPlatforms]->addSpring(randInt(0, 1), *_graphics);
        _nPlatforms++;
    } else {
        printf("Exceeds max platforms\n");
    }
}

void World::initPlatforms(){
    for(int i = 0; i < MAX_PLATFORMS; i++){
        Vector2<int> nextPos = getNextPlatformPos();
        addPlatform(nextPos.X, nextPos.Y);
        _topPlatform = _platforms[_nPlatforms - 1];

    }
}

Vector2<int> World::getNextPlatformPos(){
    int prevY = _nPlatforms == 0 ? globals::SCREEN_HEIGHT + 82 : _topPlatform->getY();
    
    int randY;
    if (_nPlatforms == 0)
        randY = randInt(120, MAX_DISTANCE);
    else if(_topPlatform->hasSpring())
        randY = randInt(MIN_DISTANCE, randInt(0,1) ? MAX_SPRING_DISTANCE : MAX_DISTANCE);
    else
        randY = randInt(MIN_DISTANCE, MAX_DISTANCE);
    
    int randX = _nPlatforms == 0 ? randInt(globals::SCREEN_WIDTH / 2 - 40, globals::SCREEN_WIDTH / 2 + 40) : randInt(0, globals::SCREEN_WIDTH - 80);
    
    return Vector2<int>{randX, prevY - randY};
}

void World::resetPlatform(Platform* platform) {
    platform->deleteSpring();
    
    Vector2<int> nextPos = getNextPlatformPos();
    platform->setX(nextPos.X);
    platform->setY(nextPos.Y);
    if(randInt(1, SPRING_PROBABILITY) == 1){
        platform->addSpring(randInt(0, 1), *_graphics);
    }
    _topPlatform = platform;
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
    
    
}



