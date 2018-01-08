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
    
    
    gameLoop();
}

Game::~Game(){
    SDL_Quit();
}

void Game::gameLoop(){
    Graphics graphics;
    
    Sprite::addTexture("Elsa", SDL_CreateTextureFromSurface(graphics.renderer(), graphics.loadImage( "Content/Sprites/ElsaChar.png")));
    Sprite::addTexture("Spring",  SDL_CreateTextureFromSurface(graphics.renderer(), graphics.loadImage( "Content/Sprites/Spring.png")));
    // Sprite::addTexture("Platform", SDL_CreateTextureFromSurface(graphics.renderer(), graphics.loadImage( "Content/Sprites/Platform")));
    
    _player = Player(graphics, 150, 680);
    _world = World(&_player, &graphics);
    SDL_Event event;
    Input input;
    
    int LAST_UPDATE_TIME = SDL_GetTicks();
    
    //start game loop
    
    
   // int secondTimer = 0;
    //int frames = 0;
    

    for(;;){
        
        input.beginNewFrame();
        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        
        //movement
        if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]){
            _player.moveLeft();
        }
        else if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]){
            _player.moveRight();
        } else {
            _player.stopMoving();
        }
      
        const int CURRENT_TIME_MS = SDL_GetTicks();
        const int ELAPSED_TIME = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        int elapsedTime = ELAPSED_TIME;
        
        
        for (int i = 0; i < 8 && elapsedTime > 0.0 ; i++ ){
            float dt = std::min(elapsedTime, MAX_FRAME_TIME);
            fixedUpdate(dt);
            elapsedTime -= dt;
        }
       
        //TODO: this uses an outdated elapsedtime, so animations might be wack with low framerates?
        update(ELAPSED_TIME);
        draw(graphics);

        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        //secondTimer += ELAPSED_TIME;
        
       /* if(secondTimer > 1000){
            printf("%d ", frames);
            frames = 0;
            secondTimer = 0;
        }*/
       
        
       \
    }
}

void Game::draw(Graphics &graphics){
    graphics.clear();

    _world.draw(graphics);
    
    graphics.flip();
}

void Game::update(float elsapedTime){
    
    _world.update(elsapedTime);
    
    
}

void Game::fixedUpdate(float fixedTime) {
    
    _world.fixedUpdate(fixedTime);
    
}

