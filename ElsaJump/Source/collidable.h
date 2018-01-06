//
//  collidable.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/2/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#ifndef collidable_hpp
#define collidable_hpp

#include "graphics.h"
class Collidable {
public:
    Collidable(float x, float y, int width, int height);
    virtual void draw(Graphics &graphics) = 0;
    virtual bool checkCollision(float playerX, float playerY);
    int getY() const;
    void setY(float y);
    void setX(float x);
    void shift(float y);
    static int getWidth();
protected:
    float _x, _y;
    int _width, _height;
    float _prevPlayerY;
    

};

#endif /* collidable_hpp */
