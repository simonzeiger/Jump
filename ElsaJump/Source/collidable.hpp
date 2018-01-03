//
//  collidable.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/2/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef collidable_hpp
#define collidable_hpp

#include "graphics.hpp"
class Collidable {
public:
    Collidable(float x, float y);
    virtual void draw(Graphics &graphics);
    virtual void update(float elapsedTime);
    virtual bool checkCollision(float playerX, float playerY);
    int getY() const;
    void setY(float y);
    void setX(float x);
    void shift(float y);
protected:
    float _x, _y;
};

#endif /* collidable_hpp */
