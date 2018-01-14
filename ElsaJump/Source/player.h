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
class Projectile;
class Enemy;

class Player : public AnimatedSprite {
public:
    Player(Graphics &graphics, float x, float y);
    ~Player();
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
    
    bool isJumping();
    
    Direction facing();
    
    int checkPlatformCollisions(Platform** platforms, int nPlatforms);
    
    bool checkEnemyCollision(Enemy* enemy);
    
    void kill();
    
    void shift(float amt);
    
    void animationStateMachine();
    
    bool isDead();
    
    void revive();
    
    void loadBall();
    
    void throwBall(int mouseX, int mouseY);
    
private:
    float _dx, _dy;
    bool _hasShield;
    bool _maxJumpHeightReached;
    bool _isDead;
    bool _isJumping;
    static const int MAX_BALLS = 14;
    Projectile* _balls[MAX_BALLS];
    
    
    Direction _facing;

};

#endif /* player_hpp */
