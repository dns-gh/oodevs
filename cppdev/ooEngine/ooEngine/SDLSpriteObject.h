#ifndef _SDLSPRITEOBJECT_H
#define _SDLSPRITEOBJECT_H

#include "SDLRenderObject.h"

class SDLSpriteObject : public SDLRenderObject
{
public:
    SDLSpriteObject();
    SDLSpriteObject( unsigned imgNum, unsigned imgPRows, unsigned imgPCol, unsigned long timeBImg );
    virtual void Update();
    void Play( unsigned int startFrame = -1 );
    void Pause();
    void Resume(unsigned int resumeFrame = -1 );
    bool IsPaused() const;
    // Update image data to the given frame
    void GoToFrame( unsigned int imgNumber );

private:
    unsigned int imageNumber_;
    unsigned int imagesPerRow_;
    unsigned int imagesPerColumn_;
    unsigned int currentImage_;
    unsigned int startImage_;
    bool paused_;
    bool playedOnce_;

    unsigned long timeBetweenImages_;
    unsigned int imageWidth_;
    unsigned int imageHeight_;

    // Set the sprite rendering rect to the given frame
    void SetRenderRect( int imageNumber );

protected:
    unsigned long lastFrameTime_;

};

#endif // SDLSpriteObject.h