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
#include "projectile.h"
#include "input.h"
#include "world.h"
#include <cstring>
#include <fstream>
#include <cstring>
#include <SDL2/SDL.h>


using std::string;
using std::vector;
using std::pair;


namespace {
    const int FPS = 60;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

std::vector<std::pair<std::string, int> > Game::_highScores;

Game::Game(){
   
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    
    
    _graphics = new Graphics;
    
    Sprite::addTexture("Elsa", SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/ElsaChar.png")));
    Sprite::addTexture("Spring",  SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Spring.png")));
    Sprite::addTexture("Cloud",  SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Cloud.png")));
    Sprite::addTexture("Nums",  SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/NumSheet.png")));
    Sprite::addTexture("Platform", SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Platform.png")));
    Sprite::addTexture("Ball", SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Ball.png")));
    Sprite::addTexture("Enemy", SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Enemy.png")));

   
    
    for(int i = 0; i < _nNumSprites; i++){
        _numSprites[i] = new NumSprite(*_graphics, globals::SCREEN_WIDTH - 40 - 32 * i, 16);
        _numSprites[i]->num = -1;
        if(i == 0)
            _numSprites[i]->num = 0;
    }
    
    
    char* temp = SDL_GetBasePath();
    _path = temp;
    SDL_free(temp);
    _path += "Content/high_score.txt";
    
    std::ifstream load( _path);
    string scoreString;
    string name;
    
    while( load >> scoreString >> name){
        _highScores.push_back(pair<string, int>( name, stoi(scoreString)));
    }
    std::sort(_highScores.begin(), _highScores.end(), [] (const std::pair<std::string,int> &a, const std::pair<std::string,int> &b) {
            return (a.second < b.second);
        }
    );
    
    
    for(int i = 0; i < _highScores.size(); i++){
        printf("%s %s\n", _highScores[i].first.c_str(), std::to_string(_highScores[i].second).c_str());

    }
    
    _player = new Player(*_graphics, 150, 680);
    _world = new World(_player, _graphics);
    _playerName = "Tiger";
    

    gameLoop();
}

Game::~Game(){
    delete _player;
    delete _world;
    delete _graphics;
    Sprite::flush();
  
    SDL_Quit();
}

void Game::gameLoop(){

    SDL_Event event;
    
    int lastUpdateTime = SDL_GetTicks();
    
    int mouseX, mouseY;
    
     int secondTimer = 0;
     int frames = 0;
    
    bool ballLoaded = false;
    
    
   // std::string inputText;
    //SDL_StartTextInput();
    
    //start game loop
    for(;;){
        
        const int CURRENT_TIME = SDL_GetTicks();
        int elapsedTime = CURRENT_TIME - lastUpdateTime;
        
      //  bool renderText = false;
        
        for (int i = 0; i < 8 && elapsedTime > 0.0 ; i++ ){
            
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                return;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && !ballLoaded  && !_player->isDead()){
                _player->loadBall();
                ballLoaded = true;
            } else if(event.type == SDL_MOUSEBUTTONUP && ballLoaded  && !_player->isDead()){
                SDL_GetMouseState(&mouseX, &mouseY);
                _player->throwBall(mouseX, mouseY);
                ballLoaded = false;
            }
            
            float dt = std::min(elapsedTime,MAX_FRAME_TIME); 
            fixedUpdate(dt);
            elapsedTime -= dt;
            lastUpdateTime += dt;
            //secondTimer += dt;

        }
        
        
        
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        
        
        //movement
        if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]){
            _player->moveLeft();
        }
        else if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]){
            _player->moveRight();
        }
        else {
            _player->stopMoving();
        }
      

        
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            return;
        } else if (event.type == SDL_MOUSEBUTTONDOWN && !ballLoaded){
            _player->loadBall();
            ballLoaded = true;
        } else if(event.type == SDL_MOUSEBUTTONUP && ballLoaded && !_player->isDead()){
            SDL_GetMouseState(&mouseX, &mouseY);
            _player->throwBall(mouseX, mouseY);
            ballLoaded = false;
            printf("x %d y %d\n", mouseX, mouseY);
        } else if (keystate[SDL_SCANCODE_SPACE] && !ballLoaded  && !_player->isDead()){
            _player->loadBall();
            ballLoaded = true;
        } else if(event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_SPACE && ballLoaded  && !_player->isDead()){
            _player->throwBall(_player->getX(), -300);
            ballLoaded = false;
        }
        /*//Special key input
        else if( event.type == SDL_KEYDOWN )
        {
            //Handle backspace
            if( event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
            {
                //lop off character
                inputText.pop_back();
                renderText = true;

            }
            //Handle copy
            else if( event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
            {
                SDL_SetClipboardText( inputText.c_str() );
            }
            //Handle paste
            else if( event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
            {
                inputText = SDL_GetClipboardText();
                renderText = true;

            }
        }
        //Special text input event
        else if( event.type == SDL_TEXTINPUT )
        {
            //Not copy or pasting
            if( !( ( event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' ) && ( event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
            {
                //Append character
                inputText += event.text.text;
                renderText = true;

            }
        }
        
        //Rerender text if needed
        if( renderText )
        {
            printf("%s \n", inputText.c_str());

        }*/
        
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
    //TODO:
    //  SDL_StopTextInput();

}

void Game::draw(){
    _graphics->clear();

    _world->draw();
    
    for(int i = 0; i < _nNumSprites; i++){
        _numSprites[i]->draw(*_graphics);
    }
    
    if(_player->isDead()){
        for(int i = 0; i < _endGameSprites.size(); i++)
            _endGameSprites[i].draw(*_graphics, _endGameSprites[i].getX(), _endGameSprites[i].getY(), 1);
    }
        
   
    
    _graphics->flip();
}


void Game::update(){
    
    
    
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
            if(_world->score() > 130 && std::find(_highScores.begin(), _highScores.end(), std::pair<string, int>(_playerName, _world->score())) == _highScores.end()){
                _highScores.push_back(std::pair<string, int>(_playerName, _world->score()));
                myfile << _world->score() << " " << _playerName << "\n";
                myfile.close();
                
              
                std::sort(_highScores.begin(), _highScores.end(), [] (const std::pair<std::string,int> &a, const std::pair<std::string,int> &b) {
                        return (a.second < b.second);
                    }
                );
                
                for(int i = 0; i < _highScores.size(); i++){
                    printf("%s %s\n", _highScores[i].first.c_str(), std::to_string(_highScores[i].second).c_str());
                    
                }
                
                
            }
        }
        else
            printf("Unable to open file\n");
        
        if(_endGameSprites.empty()){
            displayEndGame();
        }
        
     
        
        
       // _player->revive();
        //_world->resetScore();
        
    }
    
    if(_player->isDead()){
        //update player text
    }
        
    

    _world->update();
    
    
}

void Game::displayEndGame() {
    TTF_CloseFont(font);
    font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 86);
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, _playerName.c_str(), {255,255,255} );
    SDL_Texture* texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
    
    int width = textSurface->w;
    int height = textSurface->h;
    SDL_FreeSurface(textSurface);
    Sprite::addTexture(_playerName, texture);
    
    _endGameSprites.push_back(Sprite(*_graphics, _playerName, 0, 0, width, height, globals::SCREEN_WIDTH / 2 - 40 * ((float)_playerName.size() / 2), 20));
    
    
    TTF_CloseFont(font);
    font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 72);
    
    std::string str = "got";
    textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
    texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
    
    width = textSurface->w;
    height = textSurface->h;
    SDL_FreeSurface(textSurface);
    Sprite::addTexture("got", texture);
    
    _endGameSprites.push_back(Sprite(*_graphics, "got", 0, 0, width, height, globals::SCREEN_WIDTH / 2 - 53, 220));
    
    str = std::to_string(_world->score());
    textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
    texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
    
    width = textSurface->w;
    height = textSurface->h;
    SDL_FreeSurface(textSurface);
    Sprite::addTexture("score", texture);
    
    _endGameSprites.push_back(Sprite(*_graphics, "score", 0, 0, width, height, globals::SCREEN_WIDTH / 2 - 35* ((float)std::to_string(_world->score()).size() / 2), 320));
    
    str = "points";
    textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
    texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
    
    width = textSurface->w;
    height = textSurface->h;
    SDL_FreeSurface(textSurface);
    Sprite::addTexture("points", texture);
    
    _endGameSprites.push_back(Sprite(*_graphics, "points", 0, 0, width, height,  globals::SCREEN_WIDTH / 2 - 105, 420));
    
    TTF_CloseFont(font);
    font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 16);
}

void Game::fixedUpdate(float fixedTime) {
    
    //SDL_Delay(10); //for reducing fps for testing
    

    _world->fixedUpdate(fixedTime);
    
   
    
}

std::vector<std::pair<std::string, int> > Game::highScores() {
    std::vector<std::pair<std::string, int> > copy;
    std::vector<std::pair<std::string, int> > bunch;
    for(int i = 0; i < _highScores.size(); i++){
        float rounded =  (float)_highScores[i].second / 100;
        rounded = std::round(rounded);
        int finalRound = rounded * 100;
        
        if(bunch.size() == 0)
            bunch.push_back(std::pair<std::string, int> (_highScores[i].first, finalRound));

        else if(finalRound != bunch.back().second){
            std::random_shuffle(bunch.begin(), bunch.end());
            for(int j = 0; j < bunch.size(); j++){
                if(bunch[j].first != "")
                    copy.push_back(bunch[j]);
            }
            bunch.clear();
            bunch.push_back(std::pair<std::string, int> (_highScores[i].first, finalRound));
        } else if(i == 0 || _highScores[i].first != bunch.back().first){
            if (std::find(bunch.begin(), bunch.end(), std::pair<std::string, int> (_highScores[i].first, finalRound)) == bunch.end())
                bunch.push_back(std::pair<std::string, int> (_highScores[i].first, finalRound));
        }

    }
    
    if(copy.size() == 0 || bunch.size() != 0){
        std::random_shuffle(bunch.begin(), bunch.end());
        copy.insert(copy.end(), bunch.begin(), bunch.end());
    }
    return copy;
}

