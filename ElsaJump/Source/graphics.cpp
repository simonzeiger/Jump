//
//  graphics.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/30/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#include "graphics.hpp"
#include <SDL2_image/SDL_image.h>
#include "globals.h"


/* Graphics class
 * Holds all information dealing with graphics for the game
 */

Graphics::Graphics() {
    
    IMG_Init(IMG_INIT_PNG);
    
    if (SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &_window, &_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
    }
    
    
    
    // Select the color for drawing. It is set to blue here.
    SDL_SetRenderDrawColor(_renderer, 102, 102, 153, 255);
    
    SDL_SetWindowTitle(_window, "Elsa's Game");
    
    
}

Graphics::~Graphics() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    IMG_Quit();
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
    if (_spriteSheets.count(filePath) == 0) {
        _spriteSheets[filePath] = IMG_Load(filePath.c_str());
        
    }
    return _spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle){
    SDL_RenderCopy(_renderer, source, sourceRectangle, destinationRectangle);
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
