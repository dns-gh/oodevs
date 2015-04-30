#include "SDLRenderObject.h"

void SDLRenderObject::SetResourceObject( SDLRenderResource* renderResource )
{
    if( !renderResource )
        return;

    renderResource_ = renderResource;
    SDL_QueryTexture( renderResource->GetTexture(), NULL, NULL, &renderRect_.w, &renderRect_.h );
    
    // TODO Color Keying
}

SDL_Rect SDLRenderObject::GetRenderRect() const
{
    return renderRect_;
}

SDLRenderResource* SDLRenderObject::GetRenderResource( ) const
{
    return renderResource_;
}

float  SDLRenderObject::X( ) const
{
    return posX_;
}

float  SDLRenderObject::Y( ) const
{
    return posY_;
}

bool  SDLRenderObject::IsVisible( ) const
{
    return visible_;
}