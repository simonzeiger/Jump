//
//  graphics.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/30/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#include "graphics.h"
#ifdef __EMSCRIPTEN__
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include "globals.h"


/* Graphics class
 * Holds all information dealing with graphics for the game
 */

Graphics::Graphics() {
    
    IMG_Init(IMG_INIT_PNG);
    
    _window = SDL_CreateWindow("Elsa Jump", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    
    _glContext = SDL_GL_CreateContext(_window);
    SDL_GL_SetSwapInterval(1);
    
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    
    /*if (SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, SDL_WINDOW_OPENGL, &_window, &_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
    }*/
    

    // Select the color for drawing. It is set to blue here.
    SDL_SetRenderDrawColor(_renderer, 130, 166, 233, 255);
    
    SDL_SetWindowTitle(_window, "Elsa Jump");
    
    
}

Graphics::~Graphics() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    std::map<std::string, SDL_Surface*>::iterator i = _spriteSheets.begin();
    for(; i != _spriteSheets.end();++i)
        SDL_FreeSurface(i->second);
    IMG_Quit();
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
    if (_spriteSheets.count(filePath) == 0) {
        _spriteSheets[filePath] = IMG_Load(filePath.c_str());
    }
    return _spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle){
    if(SDL_RenderCopy(_renderer, source, sourceRectangle, destinationRectangle) < 0){
       // printf("%p %s\n", source, SDL_GetError());
    }
    
    
}




void Graphics::flip() {
    SDL_RenderPresent(_renderer);
}

void Graphics::clear() {
    SDL_SetRenderDrawColor(_renderer, 130, 166, 233, 255);
    SDL_RenderClear(_renderer);
}


SDL_Renderer* Graphics::renderer() const {
    return _renderer;
}
