//
//  input.cpp
//  ElsaJump
//
//  Created by Simón Zeiger on 1/2/18.
//  Copyright © 2018 Simón Zeiger. All rights reserved.
//

#include "input.h"

/* Input class
 * Keeps track of keyboard state
 */

//This function gets called at the beginning of each new frame
//to reset the keys that are no longer relevant
void Input::beginNewFrame() {
    _pressedKeys.clear();
    _releasedKeys.clear();
}

//This gets called when a key has been pressed
void Input::keyDownEvent(const SDL_Event& event) {
    _pressedKeys[event.key.keysym.scancode] = true;
    _heldKeys[event.key.keysym.scancode] = true;
}

//This gets called when a key is released
void Input::keyUpEvent(const SDL_Event& event) {
    _releasedKeys[event.key.keysym.scancode] = true;
    _heldKeys[event.key.keysym.scancode] = false;
}

//Check if a certain key was pressed during the current frame
bool Input::wasKeyPressed(SDL_Scancode key) {
    return _pressedKeys[key];
}

//Check if a certain key was released during the current frame
bool Input::wasKeyReleased(SDL_Scancode key) {
    return _releasedKeys[key];
}

//Check if a certain key is currently being held
bool Input::isKeyHeld(SDL_Scancode key) {
    return _heldKeys[key];
}
