//
//  globals.h
//  ElsaJump
//
//  Created by Simón Zeiger on 12/30/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#ifndef globals_h
#define globals_h

#include <vector>

namespace globals {
    const int SCREEN_WIDTH = 480;
    const int SCREEN_HEIGHT = 640;
    const int SPRITE_SCALE = 3;
}

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};


template<typename T>
class Vector2 {
public:
    T X;
    T Y;
    inline Vector2(T x, T y) : X(x), Y(y) {}
    inline Vector2(){};
};

template<typename T>
class Vector3 {
public:
    T X;
    T Y;
    T Z;
    inline Vector3(T x, T y, T z) : X(x), Y(y), Z(z) {}
    inline Vector3(){}
};


#endif /* globals_h */
