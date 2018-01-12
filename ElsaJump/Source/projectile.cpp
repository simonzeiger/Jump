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

float Projectile::timer;

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
    if(_loaded)
        Sprite::draw(graphics, _x, _y, 1);
}

void Projectile::incrementTimer(float incr) {
    timer += incr;
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


