#include "animatedsprite.h"
#include "graphics.h"
#include "sprite.h"

/* AnimatedSprite class
 * Animates our sprites
 */

std::map<std::string, std::vector<SDL_Rect> > AnimatedSprite::_animations;


AnimatedSprite::AnimatedSprite(Graphics &graphics, std::string name, int sourceX, int sourceY,
                               int width, int height, float posX, float posY, float timeToUpdate) :
Sprite(graphics, name, sourceX, sourceY, width, height, posX, posY),
_frameIndex(0),
_timeElapsed(0),
_timeToUpdate(timeToUpdate),
_visible(true),
_currentAnimationOnce(false),
_currentAnimation("")
{}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height) {
    
    if(_animations.count(name) == 0){
        std::vector<SDL_Rect> rectangles;
        for (int i = 0; i < frames; i++) {
            SDL_Rect newRect = { i * width + x, y, width, height };
            rectangles.push_back(newRect);
        }
        
        _animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
    }
}

void AnimatedSprite::resetAnimations() {
    _animations.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
    _currentAnimationOnce = once;
    if (_currentAnimation != animation) {
        _currentAnimation = animation;
        _frameIndex = 0;
    }
}

void AnimatedSprite::setVisible(bool visible) {
    _visible = visible;
}

void AnimatedSprite::stopAnimation() {
    _frameIndex = 0;
    animationDone(_currentAnimation);
}

void AnimatedSprite::fixedUpdate(float fixedTime) {
        
    _timeElapsed += fixedTime;
    if (_timeElapsed > _timeToUpdate) {
        _timeElapsed -= _timeToUpdate;
        if (_frameIndex < _animations[_currentAnimation].size() - 1) {
            _frameIndex++;
        }
        else {
            if (_currentAnimationOnce == true) {
                setVisible(false);
            }
            _frameIndex = 0;
            animationDone(_currentAnimation);
        }
    }
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y, int scale) {
    
    if (_visible && _y + _height * scale > -20) {
        SDL_Rect destinationRectangle;
        destinationRectangle.x = x;
        destinationRectangle.y = y;
        destinationRectangle.w = _sourceRect.w * scale;
        destinationRectangle.h = _sourceRect.h * scale;
        
        SDL_Rect sourceRect = _animations[_currentAnimation][_frameIndex];
        graphics.blitSurface(_spriteSheet, &sourceRect, &destinationRectangle);
    }
}


