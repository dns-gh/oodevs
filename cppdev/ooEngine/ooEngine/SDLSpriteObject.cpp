#include "SDLSpriteObject.h"

#include "tools.h"

SDLSpriteObject::SDLSpriteObject()
{
    // NOTHING
}

void SDLSpriteObject::Update()
{
    unsigned long timeSinceLastFrame = tools::GetCurrenTime() - lastFrameTime_;

    if( timeSinceLastFrame >= timeBetweenImages_ )
    {
        currentImage_++;
        if( currentImage_ >= imageNumber_ )
            currentImage_ = startImage_;
        SetRenderRect( currentImage_ );
        lastFrameTime_ = tools::GetCurrenTime();
    }
}

void SDLSpriteObject::Play( unsigned int startImage = -1 )
{
    SDL_Rect rect;
    SDL_QueryTexture( renderResource_->GetTexture(), NULL, NULL, &rect.w, &rect.h );
    imageWidth_ = rect.w / imagesPerRow_;
    imageHeight_ = rect.h / imagesPerColumn_;
    if( startImage < 0 || startImage >= imageNumber_ )
        currentImage_ = startImage_;
    else
        startImage_ = startImage;
    SetRenderRect( currentImage_ );
    lastFrameTime_ = tools::GetCurrenTime();
}

void SDLSpriteObject::SetRenderRect( int imageNumber )
{
    auto row = floor( imageNumber / imagesPerRow_ );
    auto col = imageNumber - row * imagesPerRow_;

    renderRect_.x = static_cast< int >( col * imageWidth_ );
    renderRect_.y = static_cast< int >( row * imageHeight_ );
    renderRect_.w = imageWidth_;
    renderRect_.h = imageHeight_;
}