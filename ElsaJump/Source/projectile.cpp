//
//  projectile.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/11/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "projectile.h"
#include "sprite.h"
#include "graphics.h"
#include "player.h"


Projectile::Projectile(Graphics &graphics, Player* player) :
Sprite(graphics, "Ball", 0, 0, 16, 16, player->getX(), player->getY()),
_player(player)

{
    _x =  player->getX();
    _y = player->getY();
    _dx = 0;
    _dy = 0;
    _loaded = false;
}

void Projectile::fixedUpdate(float fixedTime){
    
    if(_loaded && _dx == 0 && _dy == 0){
        
        if(_player->facing() == RIGHT){
            _x = _player->getX() + 27;
            _y = _player->getY() + 20;
        } else {
            _x = _player->getX() + 4;
            _y = _player->getY() + 20;
        }
    } else if(_loaded){
        
        _x += _dx * fixedTime;
        _y -= _dy * fixedTime;
        
        
        if(_x > globals::SCREEN_WIDTH || _x + _width < 0 || _y > globals::SCREEN_HEIGHT || _y + _height < 0){
            _loaded = false;
            _dx = 0;
            _dy = 0;
        }
    } else {
        _dx = 0;
        _dy = 0;
    }
}

void Projectile::draw(Graphics &graphics){
    if(_loaded){
        Sprite::draw(graphics, _x, _y, 1);
      
            
    }
}

bool Projectile::checkCollision(float enemyX, float enemyY){
    /*SDL_Rect rect1 = {(int)enemyX, (int) enemyY, 64, 48};
    SDL_Rect rect2 = {(int)Sprite::_x, (int) Sprite::_y, 16, 16};
    
    printf("%f %f %f %f \n", _x, _y, enemyX, enemyY);
    
    if (rect1.x < rect2.x + rect2.w &&
        rect1.x + rect1.w > rect2.x &&
        rect1.y < rect2.y + rect2.h &&
        rect1.h + rect1.y > rect2.y)
        return 1;
    
    return 0; */
    
    if(_x + 16 > enemyX && _x < enemyX + 64){
        printf("%f %f %f %f \n", _x, _y, enemyX, enemyY + 48);
        if(_y < enemyY && _y + 16 < enemyY + 48){
            
            return true;
        }
    }
    return false;
}


void Projectile::addTarget(int targetX, int targetY){
    
    
    _dx = (targetX - _x);
    _dy = (_y - targetY);
    
    float mag = sqrt(_dx * _dx + _dy * _dy);
    _dx = _dx / mag;
    _dy = _dy / mag;
}

void Projectile::load(){
    _loaded = true;
}

bool Projectile::isLoaded() {
    return _loaded;
}


