//
//  animatedsprite.hpp
//  ElsaJump
//
//  Created by Simón Zeiger on 12/31/17.
//  Copyright © 2017 Simón Zeiger. All rights reserved.
//

#ifndef animatedsprite_hpp
#define animatedsprite_hpp

#include <map>
#include <string>
#include <vector>

#include "sprite.hpp"
#include "globals.h"

class Graphics;

/* AnimatedSprite class
 * Holds logic for animating sprites
 */

class AnimatedSprite : public Sprite {
public:
    AnimatedSprite();
    AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
                   float posX, float posY, float timeToUpdate);
    
    /* void playAnimation
     * Plays the animation provided if it's not already playing
     */
    void playAnimation(std::string animation, bool once = false);
    
    /* void update
     * Updates the animated sprite (timer)
     */
    void update(int elapsedTime); //TODO: figure out why is int and not float
    
    /* void draw
     * Draws the sprite to the screen
     */
    void draw(Graphics &graphics, int x, int y,  int scale = globals::SPRITE_SCALE);
    
    
protected:
    double _timeToUpdate;
    bool _currentAnimationOnce;
    std::string _currentAnimation;
    
    /* void addAnimation
     * Adds an animation to the map of animations for the sprite
     */
    void addAnimation(int frames, int x, int y, std::string name, int width, int height);
    
    /* void resetAnimations
     * Resets all animations associated with this sprite
     */
    void resetAnimations();
    
    /* void stopAnimation
     * Stops the current animation
     */
    void stopAnimation();
    
    /* void setVisible
     * Changes the visibility of the animated sprite
     */
    void setVisible(bool visible);
    
    /* void animationDone
     * Logic that happens when an animation ends
     */
    virtual void animationDone(std::string currentAnimation) = 0;
    
    /* void setupAnimations
     * A required function that sets up all animations for a sprite
     */
    virtual void setupAnimations() = 0;
    
    
private:
    std::map<std::string, std::vector<SDL_Rect>> _animations;
    
    int _frameIndex;
    double _timeElapsed;
    bool _visible;
};


#endif /* animatedsprite_hpp */