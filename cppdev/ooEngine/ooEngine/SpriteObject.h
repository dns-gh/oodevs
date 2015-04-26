#ifndef _SPRITEOBJECT_H
#define _SPRITEOBJECT_H

#include "SDLRenderObject.h"

class SpriteObject : public SDLRenderObject
{
public:
    SpriteObject();
    void Update();
    void Play();
    void Stop();
    void setFrameRect( int frameNuber );
    unsigned int totalFrames_;
    unsigned int framesPerRow_;
    unsigned int framesPerColumn_;
    unsigned int currentFrame_;
    unsigned int startFrame_;
    float speed_;
    unsigned int frameWidth_;
    unsigned int frameHeight_;

protected:
    unsigned long lastFrameTime_;

};

#endif // SpriteObject.h