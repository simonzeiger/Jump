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
#include "player.h"
#include "numsprite.h"
#include <cstring>
#include <cstring>
#include <cstring>
#include <SDL2/SDL.h>
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif



using std::string;
using std::vector;
using std::pair;


namespace {
    const int FPS = 60;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

std::vector<std::pair<std::string, int> > Game::_highScores;

#ifdef __EMSCRIPTEN__
SDL_Event event;

int lastUpdateTime = SDL_GetTicks();

bool ballLoaded = false;


std::string inputText = "Elsa";

std::string _playerName = "Elsa";
std::string _path = "";

static void userdata_sync(){
    EM_ASM(
           FS.syncfs(false, function(error) {
        if (error) {
            console.log("Error while syncing", error);
        }
        console.log("Finished syncing");
        
    });
           );
}



#endif



void readFiles (std::string &_playerName, std::vector<std::pair<std::string, int> > &_highScores, std::string &_path) {
    
#ifndef __EMSCRIPTEN__
    char* temp = SDL_GetBasePath();
    _path = temp;
    SDL_free(temp);
#endif
    
    _path += "user_data/";
    
    FILE * hSFile;
    hSFile = fopen((_path + "high_score.txt").c_str(), "r");
    
    int scoreString = 0;
    char name[8];
    
    if(hSFile != NULL){
        
        
        while(fscanf(hSFile, "%d %s", &scoreString, name) == 2){
            _highScores.push_back(pair<string, int>( name, scoreString));
        }
        
        fclose(hSFile);

    } else {
        printf("HS File null\n");
        
    }
    
    std::sort(_highScores.begin(), _highScores.end(), [] (const std::pair<std::string,int> &a, const std::pair<std::string,int> &b) {
        return (a.second < b.second);
    }
              );
    char cName[8];
    _playerName = "Elsa";
    FILE * nameFile;
    nameFile =  fopen((_path + "name.txt").c_str(), "r");
    if(nameFile != NULL){
        
        if(fscanf(nameFile, "%s", cName) == 1)
            _playerName = cName;
       
        
        
        if(_playerName == "")
            _playerName = "Elsa";
        fclose(nameFile);

    } else {
        _playerName = "Elsa";
        printf("Name File null\n");
    }
#ifdef __EMSCRIPTEN__
    inputText = _playerName;
#endif
    
}


#ifdef __EMSCRIPTEN__
extern "C" {
    void main_loop(void* game){
        class Game *gem = (Game*)game;
        gem->gameLoop(event, lastUpdateTime, ballLoaded, inputText);
    }
}


extern "C" {
    static void EMSCRIPTEN_KEEPALIVE finishedSync(void *){
        readFiles(_playerName, Game::_highScores, _path);
    }
}

#endif

Game::Game(){
#ifdef __EMSCRIPTEN__
    EM_ASM(
        FS.mkdir('/user_data');
        FS.mount(IDBFS, {}, '/user_data');
        FS.syncfs(true, function(err) {
            if(err) console.log('ERROR!', err);
            console.log('finished syncing..');
            ccall('finishedSync', 'v');
        });
        
   );
#endif
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
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
    Sprite::addTexture("PlayAgain", SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Playagainbutton.png")));
    Sprite::addTexture("PowerUp", SDL_CreateTextureFromSurface(_graphics->renderer(), _graphics->loadImage( "Content/Sprites/Powerup.png")));

   
    for(int i = 0; i < _nNumSprites; i++){
        _numSprites[i] = new NumSprite(*_graphics, globals::SCREEN_WIDTH - 40 - 32 * i, 16);
        _numSprites[i]->num = -1;
        if(i == 0)
            _numSprites[i]->num = 0;
    }
    
#ifndef __EMSCRIPTEN__
    
    readFiles(_playerName, _highScores, _path);
    
#endif
  
    
  /*  for(int i = 0; i < _highScores.size(); i++){
        printf("%s %s\n", _highScores[i].first.c_str(), std::to_string(_highScores[i].second).c_str());

    }*/
    
    _player = new Player(*_graphics, 150, 680);
    _world = new World(_player, _graphics);
    _editTextTimer = 0;
    _startEditing = false;
    _textTimer = 0;
    
   
    
    #ifdef __EMSCRIPTEN__
    inputText = _playerName;
    emscripten_set_main_loop_arg(main_loop, this, 0 , 1);
    #else
    SDL_Event event;
    
    int lastUpdateTime = SDL_GetTicks();
    
    bool ballLoaded = false;
    
    
    std::string inputText = _playerName;
    
    for(;;){
        if(!gameLoop(event, lastUpdateTime, ballLoaded, inputText))
           break;
    }
    #endif
}

Game::~Game(){
    delete _player;
    delete _world;
    delete _graphics;
    Sprite::flush();
    SDL_Quit();
}

bool Game::gameLoop(SDL_Event &event, int &lastUpdateTime, bool &ballLoaded, std::string &inputText){

    
    //start game loop
    
        
        if(_player->isDead())
            SDL_StartTextInput();

        int mouseX, mouseY;

        
        const int CURRENT_TIME = SDL_GetTicks();
        int elapsedTime = CURRENT_TIME - lastUpdateTime;
        
        bool renderText = false;
    
    
    int maxRepeat = 8;
    #ifdef __EMSCRIPTEN__
    maxRepeat = 1;
    #endif
        
        for (int i = 0; i < maxRepeat && elapsedTime > 0.0 ; i++ ){
            
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                return false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN){
                if(_player->isDead()){
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if(inputText == _playerName && checkSpriteClick(_endGameSprites[0], mouseX, mouseY)){
                        _endGameSprites[0].setSpriteSheet(nullptr, 0, 0);
                        _endGameSprites[1].setX(globals::SCREEN_WIDTH / 2);
                        _startEditing = true;
                        inputText = "";
                    }
                    if(checkSpriteClick(_endGameSprites[_endGameSprites.size() - 1], mouseX, mouseY)){
                        if (_player->isDead()){
                            _world->resetScore();
                            _player->revive();
                            _startEditing = false;
                            _endGameSprites.clear();
                            if(inputText != "") _playerName = inputText;
                            else
                                inputText = _playerName;
                            SDL_StopTextInput();

                            
                        }
                    }
                }
                else if(!ballLoaded && _world->score() > 0)
                    _player->loadBall();
                ballLoaded = true;
            } else if(event.type == SDL_MOUSEBUTTONUP && ballLoaded  && !_player->isDead() && _world->score() > 0){
                SDL_GetMouseState(&mouseX, &mouseY);
                _player->throwBall(mouseX, mouseY);
                ballLoaded = false;
            }
            
            float dt = std::min(elapsedTime,MAX_FRAME_TIME); 
            fixedUpdate(dt);
            elapsedTime -= dt;
            lastUpdateTime += dt;
            _textTimer += dt;
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
            return false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN){
            if(_player->isDead()){
                SDL_GetMouseState(&mouseX, &mouseY);
                if(inputText == _playerName && checkSpriteClick(_endGameSprites[0], mouseX, mouseY)){
                    _endGameSprites[0].setSpriteSheet(nullptr, 0, 0);
                    _endGameSprites[1].setX(globals::SCREEN_WIDTH / 2);
                    _startEditing = true;
                    inputText = "";
                }
                if(checkSpriteClick(_endGameSprites[_endGameSprites.size() - 1], mouseX, mouseY)){
                    if (_player->isDead()){
                        _world->resetScore();
                        _player->revive();
                        _startEditing = false;
                        _endGameSprites.clear();
                        if(inputText != "") _playerName = inputText;
                        else
                            inputText = _playerName;
                        SDL_StopTextInput();

                        
                    }
                }
            }
            else if(!ballLoaded)
                _player->loadBall();
                ballLoaded = true;
        } else if (!_startEditing && keystate[SDL_SCANCODE_BACKSPACE]) {
            if(!_startEditing){
                _endGameSprites[0].setSpriteSheet(nullptr, 0, 0);
                _endGameSprites[1].setX(globals::SCREEN_WIDTH / 2);
                _startEditing = true;
                inputText = "";
            }
            
        } else if(event.type == SDL_MOUSEBUTTONUP && ballLoaded && !_player->isDead()){
            SDL_GetMouseState(&mouseX, &mouseY);
            _player->throwBall(mouseX, mouseY);
            ballLoaded = false;
        } else if (keystate[SDL_SCANCODE_SPACE] || keystate[SDL_SCANCODE_RETURN]){
            if(!ballLoaded  && !_player->isDead() && _world->score() > 10){
                _player->loadBall();
                ballLoaded = true;
            }
            if (_player->isDead()){
                _world->resetScore();
                _player->revive();
                _startEditing = false;
                _endGameSprites.clear();
                if(inputText != "") _playerName = inputText;
                else
                    inputText = _playerName;
                SDL_StopTextInput();

                
            }
            
        } else if(event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_SPACE && ballLoaded  && !_player->isDead() && _world->score() >= 19){
            _player->throwBall(_player->getX(), -300);
            ballLoaded = false;
        }
        //Special key input
        else if( _startEditing && event.type == SDL_KEYDOWN )
        {
            //Handle backspace
            if( event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
            {
                if(_textTimer > 100){
                    //lop off character
                    inputText.pop_back();
                    renderText = true;
                    _textTimer = 0;

                }
            
            }
        }
        //Special text input event
        else if( _startEditing && event.type == SDL_TEXTINPUT )
        {
            //Not copy or pasting
            if(  event.text.text[ 0 ] != ' ' && event.text.text[ 0 ] != '\\' && event.text.text[ 0 ] != '\"' && event.text.text[ 0 ] != '\'' && event.text.text[ 0 ] != '%')
            {
                if(event.text.text[0] != inputText[inputText.size() - 1] || _textTimer > 200){
                    //Append character
                    inputText += event.text.text;
                    renderText = true;
                    _textTimer = 0;
                }
               
            }
        }
        
        //Rerender text if needed
        if( renderText )
        {
            if(inputText.size() > 0){
                inputText = inputText.substr(0, 7);
                TTF_CloseFont(font);
                font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 72);
                
                SDL_Surface* textSurface = TTF_RenderText_Solid( font, inputText.c_str(), {255,255,255} );
                SDL_Texture* texture =  SDL_CreateTextureFromSurface( _graphics->renderer(), textSurface);
                
                int width = textSurface->w;
                int height = textSurface->h;
                SDL_FreeSurface(textSurface);
                _endGameSprites[0].setSpriteSheet(texture, width, height);
                _endGameSprites[0].setX(globals::SCREEN_WIDTH / 2 - width / 2);
                _endGameSprites[1].setX(globals::SCREEN_WIDTH / 2 + width / 2);
                TTF_CloseFont(font);
                font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 16);
            } else{
                _endGameSprites[0].setSpriteSheet(nullptr, 0, 0);
                _endGameSprites[1].setX(globals::SCREEN_WIDTH / 2);
            }
            renderText = false;

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


      
        
       
    
    return true;
}

void Game::draw(){
    _graphics->clear();

    _world->draw();
    
    if(!_player->isDead()){
        for(int i = 0; i < _nNumSprites; i++){
            _numSprites[i]->draw(*_graphics);
        }
    } else {
        for(int i = 0; i < _endGameSprites.size(); i++){
            if(i == 1){
                if(_editTextTimer < 600)
                    _endGameSprites[1].draw(*_graphics, _endGameSprites[i].getX(), _endGameSprites[i].getY(), 1);

            } else {
                _endGameSprites[i].draw(*_graphics, _endGameSprites[i].getX(), _endGameSprites[i].getY(), 1);

            }
            
        }
    }
    
    _graphics->flip();
}


void Game::update(){
    
    
    
    if(!_player->isDead()){
        int decimalPlaces = (static_cast<int>(log10(_world->score())+1));
        for (int i = 0; i < decimalPlaces; i++){
            _numSprites[i]->num = (static_cast<int>( _world->score() / pow(10 , i)) % 10);
        }
        
    } else {
        if(_endGameSprites.empty()){
            for (int i = 0; i < _nNumSprites; i++){
                _numSprites[i]->num = -1;
                if(i ==0)
                    _numSprites[i]->num = 0;
            }
#ifdef __EMSCRIPTEN__
            _path = "user_data/";
#endif
             FILE * hSFile;
             hSFile = fopen ((_path + "high_score.txt").c_str(), "a");
            if (hSFile != NULL){
                if(std::find(_highScores.begin(), _highScores.end(), std::pair<string, int>(_playerName, _world->score())) == _highScores.end()){
                    
                    
#ifdef __EMSCRIPTEN__
                    _highScores.push_back(std::pair<string, int>(inputText, _world->score()));
                    
                    fprintf(hSFile, "%d %s\n", _world->score(), inputText.c_str());
#else
                    _highScores.push_back(std::pair<string, int>(_playerName, _world->score()));
                    
                    fprintf(hSFile, "%d %s\n", _world->score(), _playerName.c_str());
#endif
                    fclose(hSFile);
                    
                    
                    std::sort(_highScores.begin(), _highScores.end(), [] (const std::pair<std::string,int> &a, const std::pair<std::string,int> &b) {
                            return (a.second < b.second);
                        }
                    );
                    
                 
                    
                }
            }
            else
                printf("Unable to open file\n");
            
            FILE * nameFile;
            nameFile = fopen ((_path  + "name.txt").c_str(), "w");
            if (nameFile != NULL){
                fprintf(nameFile, "%s", _playerName.c_str());
                fclose(nameFile);
                #ifdef __EMSCRIPTEN__
                userdata_sync();
                #endif
            }
            
            if(_endGameSprites.empty()){
                displayEndGame();
            }
        }
     
        
        
    
        
    }
    
    
        
    

    _world->update();
    
    
}

void Game::displayEndGame() {
    TTF_CloseFont(font);
    font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 72);
    
    
#ifdef __EMSCRIPTEN__
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, inputText.c_str(), {255,255,255} );
#else
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, _playerName.c_str(), {255,255,255} );
#endif

    SDL_Texture* texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
    
    int width = textSurface->w;
    int height = textSurface->h;
    SDL_FreeSurface(textSurface);
    Sprite::addTexture(_playerName, texture);
    
    _endGameSprites.push_back(Sprite(*_graphics, _playerName, 0, 0, width, height, globals::SCREEN_WIDTH / 2 - width/2, 20));
    
    std::string str = "|";
    textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
    texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
    
    int w = textSurface->w;
    height = textSurface->h;
    SDL_FreeSurface(textSurface);
    Sprite::addTexture("edit", texture);
    
    _endGameSprites.push_back(Sprite(*_graphics, "edit", 0, 0, w, height, globals::SCREEN_WIDTH / 2  + width/2, 12));

    
    TTF_CloseFont(font);
    font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 64);
    
    str = "got";
    textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
    texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
    
    width = textSurface->w;
    height = textSurface->h;
    SDL_FreeSurface(textSurface);
    Sprite::addTexture("got", texture);
    
    _endGameSprites.push_back(Sprite(*_graphics, "got", 0, 0, width, height, globals::SCREEN_WIDTH / 2   - width/2, 100));
    
    str = std::to_string(_world->score());
    textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
    texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
    
    width = textSurface->w;
    height = textSurface->h;
    SDL_FreeSurface(textSurface);
    Sprite::addTexture("score", texture);
    
    _endGameSprites.push_back(Sprite(*_graphics, "score", 0, 0, width, height, globals::SCREEN_WIDTH / 2 - width/2, 170));
    
    if(!_highScores.empty()){
        str = "Highscore:";
        textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
        texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
        
        width = textSurface->w;
        height = textSurface->h;
        SDL_FreeSurface(textSurface);
        Sprite::addTexture("Highscore", texture);
        
        _endGameSprites.push_back(Sprite(*_graphics, "Highscore", 0, 0, width, height,  globals::SCREEN_WIDTH / 2  - width/2, 330));
        
        str = _highScores[_highScores.size() - 1].first;
        textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
        texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
        
        width = textSurface->w;
        height = textSurface->h;
        SDL_FreeSurface(textSurface);
        Sprite::addTexture("Highname", texture);
        
        _endGameSprites.push_back(Sprite(*_graphics, "Highname", 0, 0, width, height,  globals::SCREEN_WIDTH / 2  - width/2, 400));
        
        str = std::to_string(_highScores[_highScores.size() - 1].second);
        textSurface = TTF_RenderText_Solid(font, str.c_str(), {255,255,255} );
        texture =  SDL_CreateTextureFromSurface( _graphics->renderer(),textSurface);
        
        width = textSurface->w;
        height = textSurface->h;
        SDL_FreeSurface(textSurface);
        Sprite::addTexture("hs", texture);
        
        _endGameSprites.push_back(Sprite(*_graphics, "hs", 0, 0, width, height,  globals::SCREEN_WIDTH / 2  - width/2, 470));
        _startEditing = true;
        
       
    }
    
    _endGameSprites.push_back(Sprite(*_graphics, "PlayAgain", 0, 0, 128, 64,  330, 555));
    TTF_CloseFont(font);
    font = TTF_OpenFont("Content/Font/Ubuntu-Bold.ttf", 16);
    
   
}

void Game::fixedUpdate(float fixedTime) {
    
    //SDL_Delay(10); //for reducing fps for testing
    

    _world->fixedUpdate(fixedTime);
    
    if(_player->isDead()){
        _editTextTimer += fixedTime;
        if(_editTextTimer >= 1200){
            _editTextTimer = 0;
        }
            
    }
    
}

bool Game::checkSpriteClick(const Sprite &sprite, int mouseX, int mouseY){
    if (mouseX > sprite.getX() - 25 && mouseX < sprite.getX() + sprite.width() + 25
        && mouseY > sprite.getY() - 10 && mouseY < sprite.getY() + sprite.height() + 10){
        return true;
    } else
        return false;
        
}


std::vector<std::pair<std::string, int> > Game::highScores() {
    std::vector<std::pair<std::string, int> > copy;
    std::vector<std::pair<std::string, int> > bunch;
    for(int i = 0; i < _highScores.size(); i++){
        float rounded =  (float)_highScores[i].second / 100;
        rounded = std::round(rounded);
        int finalRound = rounded * 100;
        
        if(bunch.size() == 0){
            if(finalRound > 100)
                bunch.push_back(std::pair<std::string, int> (_highScores[i].first, finalRound));
        }

        else if(finalRound != bunch.back().second){
            std::random_shuffle(bunch.begin(), bunch.end());
            for(int j = 0; j < bunch.size(); j++){
                copy.push_back(bunch[j]);
            }
            bunch.clear();
            bunch.push_back(std::pair<std::string, int> (_highScores[i].first, finalRound));
        } else if(i == 0 || _highScores[i].first != bunch.back().first){
            if (finalRound > 100 && std::find(bunch.begin(), bunch.end(), std::pair<std::string, int> (_highScores[i].first, finalRound)) == bunch.end())
                bunch.push_back(std::pair<std::string, int> (_highScores[i].first, finalRound));
        }

    }
    
    if(copy.size() == 0 || bunch.size() != 0){
        std::random_shuffle(bunch.begin(), bunch.end());
        copy.insert(copy.end(), bunch.begin(), bunch.end());
    }
    return copy;
}

