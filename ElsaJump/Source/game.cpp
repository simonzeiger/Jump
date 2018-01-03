//
//  game.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/30/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#include "game.hpp"
#include "globals.h"
#include "graphics.hpp"
#include "globals.h"
#include "input.hpp"
#include <SDL2/SDL.h>

namespace {
    const int FPS = 60;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game(){
   
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    
    gameLoop();
}

Game::~Game(){
    SDL_Quit();
}

void Game::gameLoop(){
    Graphics graphics;
    _player = Player(graphics, 50, 200);
    SDL_Event event;
    Input input;
    
    int LAST_UPDATE_TIME = SDL_GetTicks();
    
    //start game loop
    
    for(;;){
        input.beginNewFrame();
        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        
        //continuous-response keys
        if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]){
            _player.moveLeft();
        }
        if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]){
            _player.moveRight();
        }
        
      
        const int CURRENT_TIME_MS = SDL_GetTicks();
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
        LAST_UPDATE_TIME = CURRENT_TIME_MS;

       
        
        draw(graphics);
        
      

        
        
    }
}

void Game::draw(Graphics &graphics){
    graphics.clear();

    _player.draw(graphics);
    _background.draw(graphics);
    
    graphics.flip();
}

void Game::update(float elsapedTime){
    
    _player.update(elsapedTime);
    
    if(!_player._isJumping){
        int y  = _player.checkPlatformCollisions(_background.platforms(), _background.nPlatforms());
        if(y >= 0){
            _player.jump(y);
            _background.shift();
        }
    }
    
    
    _background.update(elsapedTime);
    
}

