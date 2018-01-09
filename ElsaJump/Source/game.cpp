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
    // Sprite::addTexture("Platform", SDL_CreateTextureFromSurface(_graphics.renderer(), _graphics.loadImage( "Content/Sprites/Platform")));
    
    _player = new Player(*_graphics, 150, 680);
    _world = new World(_player, _graphics);
    
    
    
    gameLoop();
}

Game::~Game(){
    SDL_Quit();
}

void Game::gameLoop(){

    SDL_Event event;
    Input input;
    
    long long LAST_UPDATE_TIME = SDL_GetPerformanceCounter();
    
    //start game loop
    
    
    /* int secondTimer = 0;
     int frames = 0;*/
    

    for(;;){
        
        input.beginNewFrame();
        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        
        //movement
        if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]){
            _player->moveLeft();
        }
        else if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]){
            _player->moveRight();
        } else {
            _player->stopMoving();
        }
      
        const long long CURRENT_TIME_MS = SDL_GetPerformanceCounter();
        const long long ELAPSED_TIME = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        long long elapsedTime = ELAPSED_TIME;
        long freq = SDL_GetPerformanceFrequency();
        
        for (int i = 0; i < 8 && elapsedTime > 0.0 ; i++ ){
            elapsedTime = 1000 * elapsedTime / freq;
            float dt = elapsedTime < MAX_FRAME_TIME ? elapsedTime : MAX_FRAME_TIME; // std::min(elapsedTime, MAX_FRAME_TIME);
            fixedUpdate(dt);
            elapsedTime -= dt;
        }
       
        //TODO: this uses an outdated elapsedtime, so animations might be wack with low framerates?
        update(1000 * ELAPSED_TIME / freq);
        LAST_UPDATE_TIME = CURRENT_TIME_MS;

       // frames++;
        
        draw();
        
        //secondTimer += ELAPSED_TIME;
        
       /* if(secondTimer > 1000){
            printf("%d\n", frames);
            frames = 0;
            secondTimer = 0;
        }
        */

        
      
        
       
    }
}

void Game::draw(){
    _graphics->clear();

    _world->draw(*_graphics);
    
    _graphics->flip();
}

void Game::update(float elsapedTime){
    
    _world->update(elsapedTime);
    
    
}

void Game::fixedUpdate(float fixedTime) {
   
    _world->fixedUpdate(fixedTime);
    
}

