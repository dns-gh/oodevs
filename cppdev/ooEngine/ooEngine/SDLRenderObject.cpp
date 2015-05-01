#include "SDLRenderObject.h"

SDLRenderObject::~SDLRenderObject()
{
    renderResource_->UnLoad();
}

void SDLRenderObject::SetResourceObject( Resource* renderResource )
{
    if( !renderResource || renderResource->GetResourceType() != RESOURCE_GRAPHIC )
        return;

    renderResource_ = renderResource;
    renderResource_->Load();
    SDL_QueryTexture( dynamic_cast< SDLRenderResource* >( renderResource )->GetTexture(), NULL, NULL, &renderRect_.w, &renderRect_.h );
    
    // TODO Color Keying
}

void SDLRenderObject::SetPosition( float x, float y )
{
    posX_ = x;
    posY_ = y;
}

SDL_Rect SDLRenderObject::GetRenderRect() const
{
    return renderRect_;
}

Resource* SDLRenderObject::GetRenderResource( ) const
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