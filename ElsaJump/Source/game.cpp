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
#include <SDL2/SDL.h>

namespace {
    const int FPS = 60;
    const int MAX_FRAME_TIME = 6 * 1000 / FPS;
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
    
    SDL_Event event;
    
    int LAST_UPDATE_TIME = SDL_GetTicks();
    //start game loop
    for(;;){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
      
        const int CURRENT_TIME_MS = SDL_GetTicks();
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        draw(graphics);
        
    }
}

void Game::draw(Graphics &graphics){
    graphics.flip();
    
    graphics.clear();
}

void Game::update(float elsapedTime){
    
}
