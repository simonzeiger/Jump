//
//  player.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#ifndef player_h
#define player_h

#include "animatedsprite.h"
#include "globals.h"
#include "collidables.h"

class Graphics;

class Player : public AnimatedSprite {
public:
    Player(Graphics &graphics, float x, float y);
    void draw(Graphics &graphics);
    void update();
    void fixedUpdate(float fixedTime);
    
    /* void moveLeft
     * Moves the player by -dx (left)
     */
    void moveLeft();
    
    /* void moveRight
     * Moves the player by dx (right)
     */
    void moveRight();
    
    /* void stopMoving
     * Stops moving the player
     */
    void stopMoving();
    
    /* void jump
     * Starts jumping
     */
    void jump(float y);
    
    void animationDone(std::string currentAnimation);
    void setupAnimations(); 
    
    float getDY() const;
    
    
    
    int checkPlatformCollisions(Platform** platforms, int nPlatforms);
    
    void killed();
    
    bool _isJumping;

    void shift(float amt);
    
    void animationStateMachine();
    
    bool isDead();
    
    void revive();
    
private:
    float _dx, _dy;
    bool _hasShield;
    bool _maxJumpHeightReached;
    bool _isDead;

    Direction _facing;

};

#endif /* player_hpp */
