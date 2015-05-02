#include "SDLSpriteObject.h"

#include "ErrorLogManager.h"
#include "tools.h"

SDLSpriteObject::SDLSpriteObject()
{
    // NOTHING
}

SDLSpriteObject::SDLSpriteObject( unsigned imgNum, unsigned imgPRows, unsigned imgPCol, unsigned long timeBImg )
    : imageNumber_( imgNum )
    , imagesPerRow_( imgPRows )
    , imagesPerColumn_( imgPCol )
    , timeBetweenImages_( timeBImg )
    , paused_( true )
    , playedOnce_( false )
{
    // NOTHING
}

void SDLSpriteObject::Update()
{
    if( paused_ )
        return;

    unsigned long timeSinceLastFrame = tools::GetCurrentTime( ) - lastFrameTime_;

    if( timeSinceLastFrame >= timeBetweenImages_ )
    {
        currentImage_++;
        if( currentImage_ >= imageNumber_ )
            currentImage_ = startImage_;
        SetRenderRect( currentImage_ );
        lastFrameTime_ = tools::GetCurrentTime( );
    }
}

void SDLSpriteObject::Play( unsigned int startImage )
{
    if( !renderResource_ )
        return;

    if( playedOnce_ && paused_ )
        return Resume( startImage );

    SDL_Rect rect;
    SDL_QueryTexture( renderResource_->GetTexture( ), NULL, NULL, &rect.w, &rect.h );
    imageWidth_ = rect.w / imagesPerRow_;
    imageHeight_ = rect.h / imagesPerColumn_;
    if( startImage < 0 || startImage >= imageNumber_ )
        currentImage_ = startImage_;
    else
        currentImage_ = startImage;

    SetRenderRect( currentImage_ );
    lastFrameTime_ = tools::GetCurrentTime();
    playedOnce_ = true;
    paused_ = false;
    visible_ = true;
}

void SDLSpriteObject::Pause()
{
    paused_ = true;
}

void SDLSpriteObject::Resume( unsigned int resumeFrame )
{
    if( !playedOnce_ )
        return Play( resumeFrame );

    if( resumeFrame >= 0 && resumeFrame < imageNumber_ )
        currentImage_ = resumeFrame;

    SetRenderRect( currentImage_ );
    lastFrameTime_ = tools::GetCurrentTime( );
    paused_ = false;
}

bool SDLSpriteObject::IsPaused( ) const
{
    return paused_;
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