//
//  background.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/1/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "world.h"
#include "globals.h"
#include "game.h"
#include "enemy.h"
#include "cloud.h"
#include "enemy.h"
#include "projectile.h"
#include <iostream>
#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_ttf.h>
#else
#include <SDL2_ttf/SDL_ttf.h>
#endif



namespace world_constants {
    const int SHIFT_DELAY = 3; // 3
    const int MAX_DISTANCE = 178    ;
    const int MAX_SPRING_DISTANCE = 600;
    const int MIN_DISTANCE = 35;
    const int SPRING_PROBABILITY = 10; //10
    const int ONLY_ONCE_PLAT_PROBABILITY = 14;
    const int FAKE_PLAT_PROBABILITY = 3;
    const int MOVING_PLAT_PROBABILITY = 14;
    const int ENEMY_PROBABILITY = 20;
}



//TODO: fix spring who is barely off screen

TTF_Font* font;

World::World(Player* player, Graphics* graphics) :
_player(player),
_graphics(graphics)
{
    _nPlatforms = 0;
    _shifting = false;
    _shiftCount = 0;
    _shiftDistance = 0;
    _prevPlayerY = globals::SCREEN_HEIGHT;
    _topPlatform = nullptr;
    _topCloud = nullptr;
    _score = 0;
    _highScoreCounter = 0;
    _enemyPlatform = nullptr;
    _highScoreY = -20;
    
    TTF_Init();

    font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 16);
    
    _highScores = Game::highScores();
    
    for(int i = 0; i < _highScores.size(); i++){
        printf("%d\n", _highScores[i].second);
    }
    
    initPlatforms();
    initClouds();
}

World::~World(){
    for(int i = 0; i < MAX_PLATFORMS; i++){
        _platforms[i]->deleteSpring();
        delete _platforms[i];
    }
    for(int i = 0; i < MAX_CLOUDS; i++){
        delete _clouds[i];
    }
    
    TTF_CloseFont(font);
    TTF_Quit();
 
}



void World::update( ){
    
    
    _player->update();
    
    
    
    
    if(_highScoreCounter < _highScores.size() && score() + _player->getY() / 10 + 20 >= _highScores[_highScoreCounter].second){
        
        bool make = true;
        for(int i = 0; i < _nPlatforms; i++){
            if(_platforms[i]->getY() < 0 && _platforms[i]->getY() > -40 && _platforms[i]->Sprite::getX() < 80){
                make = false;
                _highScoreCounter++;
            }
        }
        if(make){
            
            bool same = false;
            if(_highScoreCounter > 0)
                same = _highScores[_highScoreCounter].second == _highScores[_highScoreCounter - 1].second;
            
            if(!same)
                _highScoreY = -20;
            
            if(_highScoreY > -80){
                
                std::string textureText = "- " + _highScores[_highScoreCounter].first.substr(0, 7);
                
                SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), {255,255,255} );
                SDL_Texture* texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
                
                int width = textSurface->w;
                int height = textSurface->h;
                SDL_FreeSurface(textSurface);
                Sprite::addTexture(textureText, texture);
                
                
                
                if(_highScoreCounter > 0 && same){
                    _highScoreY -= 30;
                }
                
                _scoreSprites.push_back(Sprite(*_graphics, textureText, 0, 0, width, height,
                                               0, _highScoreY));
            }
            
            _highScoreCounter++;
        }
    }
    
    if(_player->getY() > globals::SCREEN_HEIGHT + 100){
        _player->kill();
        _prevPlayerY = globals::SCREEN_HEIGHT;
    }
  

}

void World::fixedUpdate(float fixedTime){
    
    //could go in update but seeing if this is more responsive
    if(!_player->isJumping() || _enemyPlatform != nullptr){
        float y  = _player->checkPlatformCollisions(_platforms, _nPlatforms);
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
            
           
            if(_enemyPlatform != nullptr && abs(y - _enemyPlatform->getY() + 48) < 2){
                _enemyPlatform->deleteEnemy();
                _enemyPlatform = nullptr;
            }
            
        }
    }
    
    if(_enemyPlatform != nullptr){
        for(int i = 0; i < 14; i++){
           // printf("%d %d %d \n", i, (int)_player->balls()[i]->getX(), (int) _player->balls()[i]->getY() );

            if(_player->balls()[i]->isLoaded() && _player->balls()[i]->checkCollision(_enemyPlatform->Sprite::getX(), _enemyPlatform->getY() - 48)){
                _enemyPlatform->deleteEnemy();
                _enemyPlatform = nullptr;
                break;
            }
               
        }
    }
  
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
        
        

        if(_shiftCount > _prevPlayerY / world_constants::SHIFT_DELAY || _player->isDead()){    //   if(_player->getY() <= globals::SCREEN_HEIGHT / 2){
            
            
            for(int i = 0; i < _nPlatforms; i++){
                _platforms[i]->shift(_shiftDistance);
                if(_platforms[i]->getY() > globals::SCREEN_HEIGHT + 50){
                    resetPlatform(_platforms[i]);
                }
            }
            
            
            
            for(int i = 0; i < _scoreSprites.size(); i++){
                if( _scoreSprites[i].getY() < globals::SCREEN_HEIGHT)
                    _scoreSprites[i].setY(_scoreSprites[i].getY() -_player->getDY() * fixedTime);
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
        
        if(!_player->isJumping()){
            _shifting = false;
            _shiftCount = 0;
        }
        
    }
    
    _player->fixedUpdate(fixedTime);
    
  
    
}

void World::draw(){
    
    if(!_player->isDead()){
    
    for(int i = 0; i < _scoreSprites.size(); i++){
        _scoreSprites[i].draw(*_graphics, _scoreSprites[i].getX(), _scoreSprites[i].getY(), 1);
    }
    
    for(int i = 0; i < MAX_CLOUDS; i++){
        _clouds[i]->draw(*_graphics);
    }
    
    for(int i = 0; i < _nPlatforms; i++){
        _platforms[i]->draw(*_graphics);
    }
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
        if(globals::randInt(1, world_constants::SPRING_PROBABILITY) == 1){
            _platforms[_nPlatforms]->addSpring(globals::randInt(0, 1), *_graphics);

        }

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
        randY = globals::randInt(120, world_constants::MAX_DISTANCE);
    else if(_topPlatform->hasSpring()){
       /* if(globals::randInt(1, 12) == 1) //long distnat springs cool in theoru but annoying in practice
            randY = globals::randInt(world_constants::MAX_SPRING_DISTANCE * .6,world_constants:: MAX_SPRING_DISTANCE);
        else*/
        randY = globals::randInt(world_constants::MIN_DISTANCE / 2, globals::randInt(1,6) == 1 ? world_constants::MAX_SPRING_DISTANCE * .5 : world_constants::MAX_DISTANCE * 1.4);
    
    } else{
        if(_topPlatform->hasEnemy())
             randY = globals::randInt( world_constants::MIN_DISTANCE * 2,  world_constants::MAX_DISTANCE);
        else{
            //difficulty scaling
            randY = globals::randInt(_topPlatform->isMoving() ? world_constants::MIN_DISTANCE : world_constants::MIN_DISTANCE / 2, globals::randInt(0, 500 - _score / 60) <= 100 && _topPlatform->isReal() ? world_constants::MAX_DISTANCE : world_constants::MAX_DISTANCE / 2.5);
        }
    }
    
    int randX = _nPlatforms == 0 ? globals::randInt(globals::SCREEN_WIDTH / 2 - 40, globals::SCREEN_WIDTH / 2 + 40) : globals::randInt(0, globals::SCREEN_WIDTH -  64);
    
    if(randY < world_constants::MIN_DISTANCE){
        
        //TODO: Figure out when randX to be invalid can be negative and stop it from happening (everytime randInt swaps)
        int prevX;
        
        if(_topPlatform != nullptr)
            prevX = static_cast<Collidable*>(_topPlatform)->getX();
        else
            prevX = globals::SCREEN_WIDTH / 2;
        
        if(globals::SCREEN_WIDTH - prevX < 2 * _topPlatform->getCollidableWidth()){
           randX = globals::randInt(0, prevX - _topPlatform->getCollidableWidth() - 10);
            if(randX > globals::SCREEN_WIDTH - _topPlatform->getCollidableWidth()  || randX < 0)
                printf("1 Bad plat: Randx %d prevX %d\n", randX, prevX);
            
        }else if(prevX < _topPlatform->getCollidableWidth() + 13){
            randX = globals::randInt(prevX + _topPlatform->getCollidableWidth()  + 10, globals::SCREEN_WIDTH - _topPlatform->getCollidableWidth() - 10);
            if(randX > globals::SCREEN_WIDTH - _topPlatform->getCollidableWidth()  || randX < 0)
                printf("2 Bad plat: Randx %d prevX %d\n", randX, prevX);
            
        }else{
            bool isLeft = globals::randInt(0, 1);
            randX = isLeft ?  globals::randInt(0, prevX - _topPlatform->getCollidableWidth() -11) : globals::randInt(prevX + _topPlatform->getCollidableWidth() + 10, globals::SCREEN_WIDTH - _topPlatform->getCollidableWidth() - 10);
            if(randX > globals::SCREEN_WIDTH - _topPlatform->getCollidableWidth()  || randX < 0){
               if(isLeft) printf("3L Bad plat: Randx %d prevX %d\n", randX, prevX);
               else
                   printf("3L Bad plat: Randx %d prevX %d\n", randX, prevX);
            }
           
        }
        
       

        
    }
    
    
    
    randY = prevY - randY;
        
    return Vector2<int>{randX, randY};
}

Vector2<int> World::getNextCloudPos() {
    int prevY = _topCloud == nullptr ? globals::SCREEN_HEIGHT - 50 : _topCloud->getY();
    int randY = globals::randInt(world_constants::MAX_DISTANCE / 2.5, globals::randInt(1,3) == 1 ? world_constants::MAX_DISTANCE * 2 : globals::SCREEN_HEIGHT * 2);
    int randX =  globals::randInt(0, globals::SCREEN_WIDTH - 16 * globals::SPRITE_SCALE);
    
    return Vector2<int>{randX, prevY - randY};
}

void World::resetPlatform(Platform* platform) {
    if(platform->hasEnemy())
        _enemyPlatform = nullptr;
    
    platform->reset();
   
    Vector2<int> nextPos = getNextPlatformPos();
    
   
    
    platform->setX(nextPos.X);

    platform->setY(nextPos.Y);
    
    int scoreDifficult = _score / 30;
    if(scoreDifficult > 800)
        scoreDifficult = 800;
    //difficulty scaling
    if(_topPlatform->getY() - platform->getY() > world_constants::MIN_DISTANCE && globals::randInt(0, world_constants::MOVING_PLAT_PROBABILITY * 100 - scoreDifficult) <= 100 && _topPlatform->isReal()){
        platform->enableMoving(.08 + _score / 80000);
       
    }
    
    if(_enemyPlatform == nullptr && _topPlatform->getY() - platform->getY() < world_constants::MAX_DISTANCE * .7 && globals::randInt(0, world_constants::ENEMY_PROBABILITY) == 1){
        platform->addEnemy(*_graphics);
        _enemyPlatform = platform;
    } else {
        if((!_topPlatform->hasSpring() || _topPlatform->getY() - platform->getY() > world_constants::MAX_DISTANCE * .7) && globals::randInt(1, world_constants::SPRING_PROBABILITY) == 1){
            platform->addSpring(globals::randInt(0, 1), *_graphics);
        } else {
            //difficulty scaling
            if(globals::randInt(0, world_constants::ONLY_ONCE_PLAT_PROBABILITY * 100 - scoreDifficult) <= 200)
                platform->enableOnlyOnce();
            else if (globals::randInt(1, world_constants::FAKE_PLAT_PROBABILITY) == 1 && _topPlatform->isReal() && !_topPlatform->isMoving() && _topPlatform->getY() - platform->getY() < world_constants::MAX_DISTANCE / 2)
                platform->makeFake();
            
        }
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
        _platforms[i]->deleteEnemy();
        delete _platforms[i];
    }
    
    for(int i = 0; i < MAX_CLOUDS; i++){
        delete _clouds[i];
    }
    
    _enemyPlatform = nullptr;
    
    _scoreSprites.clear();
    
    _highScoreCounter = 0;
    _highScores = Game::highScores();
    for(int i = 0; i < _highScores.size(); i++){
        printf("%d\n", _highScores[i].second);
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
    resetAll();
    _score = 0;
}



