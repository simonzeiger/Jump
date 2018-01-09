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
#include <SDL2/SDL.h>

namespace {
    const int FPS = 60;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

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
    
    for(int i = 0; i < _nNumSprites; i++){
        _numSprites[i] = new NumSprite(*_graphics, globals::SCREEN_WIDTH - 40 - 32 * i, 16);
        _numSprites[i]->num = -1;
        if(i == 0)
            _numSprites[i]->num = 0;
    }
    
    
    gameLoop();
}

Game::~Game(){
    SDL_Quit();
}

void Game::gameLoop(){

    SDL_Event event;
    
    int lastUpdateTime = SDL_GetTicks();
    
    //start game loop
    
    
     int secondTimer = 0;
     int frames = 0;
    

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
    
    if(_world->score() > 0){
        for (int i = 0; i < ((int)(log10(_world->score())+1)); i++){
            _numSprites[i]->num = ((int)( _world->score() / pow(10 , i)) % 10);
        }
    } else {
        for (int i = 0; i < _nNumSprites; i++){
            _numSprites[i]->num = -1;
            if(i ==0)
                _numSprites[i]->num = 0;
        }
    }
    
    
    
    
    _world->update();
    
    
}

void Game::fixedUpdate(float fixedTime) {
    
    //SDL_Delay(10); //for reducing fps for testing
    _world->fixedUpdate(fixedTime);
    
   
    
}

