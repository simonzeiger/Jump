//
//  game.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/30/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#include "game.h"
#include "globals.h"
#include "graphics.h"
#include "globals.h"
#include "input.h"
#include <fstream>
#include <SDL2/SDL.h>

namespace {
    const int FPS = 60;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

std::string Game::_path;

Game::Game(){
   
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    _graphics = new Graphics;
    Sprite::addTexture("Elsa", SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/ElsaChar.png")));
    Sprite::addTexture("Spring",  SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Spring.png")));
    Sprite::addTexture("Cloud",  SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Cloud.png")));
    Sprite::addTexture("Nums",  SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/NumSheet.png")));
    // Sprite::addTexture("Platform", SDL_CreateTextureFromSurface(_graphics.renderer(), _graphics.loadImage( "Content/Sprites/Platform")));
    
    _player = new Player(*_graphics, 150, 680);
    _world = new World(_player, _graphics);
    _playerName = "Elsa";
    
    for(int i = 0; i < _nNumSprites; i++){
        _numSprites[i] = new NumSprite(*_graphics, globals::SCREEN_WIDTH - 40 - 32 * i, 16);
        _numSprites[i]->num = -1;
        if(i == 0)
            _numSprites[i]->num = 0;
    }
    
    _path = SDL_GetBasePath();
    _path += "Content/high_score.txt";
    printf("%s\n", _path.c_str());
    
    std::ifstream load( _path);
    std::string scoreString;
    std::string name;
    
    while( load >> scoreString >> name){
        printf("%s %s\n", scoreString.c_str(), name.c_str());
        _highScores.push_back(std::pair<std::string, int>( name, stoi(scoreString)));
    }
    
    gameLoop();
}

Game::~Game(){
    SDL_Quit();
}

void Game::gameLoop(){

    SDL_Event event;
    
    int lastUpdateTime = SDL_GetTicks();
    
    
    
     int secondTimer = 0;
     int frames = 0;
    
    //start game loop
    for(;;){
        
        const int CURRENT_TIME = SDL_GetTicks();
        int elapsedTime = CURRENT_TIME - lastUpdateTime;
        
        for (int i = 0; i < 8 && elapsedTime > 0.0 ; i++ ){
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                return;
            }
            
            float dt = std::min(elapsedTime,MAX_FRAME_TIME); // std::min(elapsedTime, MAX_FRAME_TIME);
            fixedUpdate(dt);
            elapsedTime -= dt;
            lastUpdateTime += dt;
            //secondTimer += dt;

        }
        
        update();

        draw();
                
       /* if(secondTimer > 1000){
            printf("%d\n", frames);
            frames = 0;
            secondTimer = 0;
        } else {
            //frames++;
        }
        */


      
        
       
    }
}

void Game::draw(){
    _graphics->clear();

    _world->draw();
    
    for(int i = 0; i < _nNumSprites; i++){
        _numSprites[i]->draw(*_graphics);
    }
    
    _graphics->flip();
}

void Game::update(){
    
     const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    //movement
    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]){
        _player->moveLeft();
    }
    else if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]){
        _player->moveRight();
    } else {
        _player->stopMoving();
    }
    
    if(!_player->isDead()){
        int decimalPlaces = ((int)(log10(_world->score())+1));
        for (int i = 0; i < decimalPlaces; i++){
            _numSprites[i]->num = ((int)( _world->score() / pow(10 , i)) % 10);
        }
    } else {
        for (int i = 0; i < _nNumSprites; i++){
            _numSprites[i]->num = -1;
            if(i ==0)
                _numSprites[i]->num = 0;
        }
        
        std::ofstream myfile (_path, std::ios_base::app);
        if (myfile.is_open()){
            myfile << _world->score() << " " << _playerName << "\n";
            myfile.close();
        }
        else
            printf("Unable to open file\n");
        
        _player->revive();
        _world->resetScore();
        
    }
    
    
    
    
    _world->update();
    
    
}

void Game::fixedUpdate(float fixedTime) {
    
    //SDL_Delay(10); //for reducing fps for testing
    _world->fixedUpdate(fixedTime);
    
   
    
}

