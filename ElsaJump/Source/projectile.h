//
//  projectile.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/11/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef projectile_h
#define projectile_h

#include "sprite.h"

class Graphics;
class Player;

class Projectile : public Sprite {
    
public:
    Projectile(Graphics &graphics, Player* player);
    void fixedUpdate(float fixedTime);
    void draw(Graphics &graphics);
    void addTarget(int targetX, int targetY);
    void load();
    bool isLoaded();
    
private:
    float _dx;
    float _dy;
    float _x;
    float _y;
    bool _loaded;
    Player* _player;
    
};
#endif /* projectile_h */
