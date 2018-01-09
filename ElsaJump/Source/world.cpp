//
//  background.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "world.h"
#include "globals.h"
#include "cloud.h"
#include <iostream>
#include <SDL2/SDL.h>

const int SHIFT_DELAY = 3.5; // 3.5
const int MAX_DISTANCE = 178;
const int MAX_SPRING_DISTANCE = 600;
const int MIN_DISTANCE = 25;
const int SPRING_PROBABILITY = 10;


//TODO:Implement platforms who move left to right faster and faster as score goes up

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
    _topCloud = nullptr;
    _score = 0;
    
    initPlatforms();
    initClouds();
}

void World::update( ){
    
    
    _player->update();
    
    if(!_player->_isJumping){
        int y  = _player->checkPlatformCollisions(_platforms, _nPlatforms);
        if(y >= -globals::SCREEN_HEIGHT){
            _player->jump(y);
            if(y < 0){
                y*= -1;
                shift();
                _prevPlayerY = y;
            } else if(static_cast<int>(_prevPlayerY) > y) {
                shift();
                _prevPlayerY = y;
            }
            
            
        }
    }
    
    if(_player->getY() > globals::SCREEN_HEIGHT + 100){
        _player->killed();
        _prevPlayerY = globals::SCREEN_HEIGHT;
        resetAll();
    }
  

}

void World::fixedUpdate(float fixedTime){
    
    
  
    for(int i = 0; i < MAX_CLOUDS; i++){
        _clouds[i]->fixedUpdate(fixedTime);
        if(_clouds[i]->getX() <= 0 || _clouds[i]->getX() + _clouds[i]->width() * globals::SPRITE_SCALE >= globals::SCREEN_WIDTH)
            getNextCloudPos();

    }
    
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->fixedUpdate(fixedTime);
    }
    
    //lerpy but actually simple
    if(_shifting){
        
        
        _shiftCount += fixedTime;
        _shiftDistance = -_player->getDY() * fixedTime;
        _score += _shiftDistance / 10;

        if(_shiftCount > _prevPlayerY / SHIFT_DELAY){
            
            
            for(int i = 0; i < _nPlatforms; i++){
                _platforms[i]->shift(_shiftDistance);
                if(_platforms[i]->getY() > globals::SCREEN_HEIGHT + 50){
                    resetPlatform(_platforms[i]);
                }
            }
            
            for(int i = 0; i < MAX_CLOUDS; i++){
                _clouds[i]->shift(_shiftDistance/3);
                if(_clouds[i]->getY() > globals::SCREEN_HEIGHT + 50){
                    resetCloud(_clouds[i]);
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

void World::draw(){
    for(int i = 0; i < MAX_CLOUDS; i++){
        _clouds[i]->draw(*_graphics);
    }
    
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->draw(*_graphics);
    }
    
    _player->draw(*_graphics);

    
}

int World::score() {
    return static_cast<int>(_score);
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
        if(globals::randInt(1, SPRING_PROBABILITY) == 1)
            _platforms[_nPlatforms]->addSpring(globals::randInt(0, 1), *_graphics);
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

void World::initClouds() {
    for(int i = 0; i < MAX_CLOUDS; i++){
        Vector2<int> nextPos = getNextCloudPos();
        
        _clouds[i] = new Cloud(*_graphics, nextPos.X, nextPos.Y);
        if(globals::randInt(0, 1) == 1)
            _clouds[i]->changeDir();
        
        _topCloud = _clouds[i];
        
    }
}

Vector2<int> World::getNextPlatformPos(){
    int prevY = _nPlatforms == 0 ? globals::SCREEN_HEIGHT + 82 : _topPlatform->getY();
    
    int randY;
    if (_nPlatforms == 0)
        randY = globals::randInt(120, MAX_DISTANCE);
    else if(_topPlatform->hasSpring())
        randY = globals::randInt(MIN_DISTANCE, globals::randInt(1,6) == 1 ? MAX_SPRING_DISTANCE * .7 : MAX_DISTANCE * 1.4);
    else
        randY = globals::randInt(MIN_DISTANCE, globals::randInt(1,4) == 1 ? MAX_DISTANCE: MAX_DISTANCE / 2); //TODO: farther away as score goes up
    
    int randX = _nPlatforms == 0 ? globals::randInt(globals::SCREEN_WIDTH / 2 - 40, globals::SCREEN_WIDTH / 2 + 40) : globals::randInt(0, globals::SCREEN_WIDTH -  64);
    
    return Vector2<int>{randX, prevY - randY};
}

Vector2<int> World::getNextCloudPos() {
    int prevY = _topCloud == nullptr ? globals::SCREEN_HEIGHT - 50 : _topCloud->getY();
    int randY = globals::randInt(MAX_DISTANCE / 2.5, globals::randInt(1,3) == 1 ? MAX_DISTANCE * 2 : globals::SCREEN_HEIGHT * 2);
    int randX =  globals::randInt(0, globals::SCREEN_WIDTH - 16 * globals::SPRITE_SCALE);
    
    return Vector2<int>{randX, prevY - randY};
}

void World::resetPlatform(Platform* platform) {
    platform->deleteSpring();
    
    Vector2<int> nextPos = getNextPlatformPos();
    platform->setX(nextPos.X);
    platform->setY(nextPos.Y);
    if(globals::randInt(1, SPRING_PROBABILITY) == 1){
        platform->addSpring(globals::randInt(0, 1), *_graphics);
    }
    _topPlatform = platform;
}

void World::resetCloud(Cloud* cloud) {
    Vector2<int> nextPos = getNextCloudPos();
    cloud->setX(nextPos.X);
    cloud->setY(nextPos.Y);
    _topCloud = cloud;

}

void World::resetAll(){
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->deleteSpring();
        delete _platforms[i];
    }
    
    for(int i = 0; i < MAX_CLOUDS; i++){
        delete _clouds[i];
    }
    
    _nPlatforms = 0;
    initPlatforms();
    _topCloud = nullptr;
    initClouds();

}

void World::shift(){
    _shifting = true;
    _shiftCount = 0;
    
    
}

void World::resetScore() {
    _score = 0;
}



