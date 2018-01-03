//
//  collidable.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/2/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "collidable.hpp"

Collidable::Collidable(float x, float y){
    _x = x;
    _y = y;
}

void Collidable::draw(Graphics &graphics){
    
}
void Collidable::update(float elapsedTime){
    
}
bool Collidable::checkCollision(float playerX, float playerY){
    return false;
}

int Collidable::getY() const{
    return _y;
}

void Collidable::setY(float y){
    _y = y;
}
void Collidable::setX(float x){
    _x = x;
}

void Collidable::shift(float y){
    _y += y;
}
