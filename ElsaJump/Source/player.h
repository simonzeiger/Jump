//
//  player.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include "animatedsprite.h"
#include "globals.h"
#include "platform.h"

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
    
    
    bool _isDead;
    
    int checkPlatformCollisions(Platform** platforms, int nPlatforms);
    
    void killed();
    
    bool _isJumping;

    void shift(float amt);
    
    void animationStateMachine();
    
private:
    float _dx, _dy;
    bool _hasShield;
    bool _maxJumpHeightReached;
    Direction _facing;

};

#endif /* player_hpp */
