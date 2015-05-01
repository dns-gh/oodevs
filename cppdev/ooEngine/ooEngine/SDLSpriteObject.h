#ifndef _SDLSPRITEOBJECT_H
#define _SDLSPRITEOBJECT_H

#include "SDLRenderObject.h"

class SDLSpriteObject : public SDLRenderObject
{
public:
    SDLSpriteObject();
    virtual void Update();
    void Play( unsigned int startFrame );
    void Stop();

private:
    unsigned int imageNumber_;
    unsigned int imagesPerRow_;
    unsigned int imagesPerColumn_;
    unsigned int currentImage_;
    unsigned int startImage_;

    unsigned long timeBetweenImages_;
    unsigned int imageWidth_;
    unsigned int imageHeight_;

    // Set the sprite rendering rect to the given frame
    void SetRenderRect( int imageNumber );

protected:
    unsigned long lastFrameTime_;

};

#endif // SDLSpriteObject.h