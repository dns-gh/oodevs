#include "SDLRenderObject.h"
#include "SDL2DRenderManager.h"
#include "ErrorLogManager.h"

#include "SDL_pixels.h"
#include "SDL_render.h"

SDLRenderObject::SDLRenderObject()
    : visible_( false )
    , colorKeyEnabled_( false )
{
    // NOTHING
}

SDLRenderObject::~SDLRenderObject()
{
    renderResource_->UnLoad();
}

void SDLRenderObject::SetResourceObject( std::shared_ptr< Resource_ABC > resource )
{
    if( !resource || resource->GetResourceType( ) != RESOURCE_GRAPHIC )
        return;

    renderResource_ = dynamic_cast< SDLRenderResource_ABC* >( &*resource );
    if( !renderResource_ )
        OOTHROW( 1, "Cannot retrieve derived class for the given Resource" );
    renderResource_->Load();
    SDL_QueryTexture( renderResource_->GetTexture( ), NULL, NULL, &renderRect_.w, &renderRect_.h );
}

void SDLRenderObject::SetPosition( float x, float y )
{
    posX_ = x;
    posY_ = y;
}

void SDLRenderObject::SetVisible( bool visibility )
{
    visible_ = visibility;
}

void SDLRenderObject::SetColorKeying( bool enable, unsigned int r, unsigned int g, unsigned int b )
{
    if( !renderResource_ )
        OOTHROW( 1, "The rendering resource is not yet set. You may call SetResourceObject to do so." )

    colorKeyEnabled_ = enable;
    SDL_Surface* surface = renderResource_->GetSurface();
    if( !surface )
        OOTHROW( 1, "Problem when initializing color keying : no surface set for the resource" );
    SDL_PixelFormat* format = surface->format;
    unsigned int colorKey = SDL_MapRGB( format, colorKey_.r, colorKey_.g, colorKey_.b );    
    SDL_SetColorKey( surface, colorKeyEnabled_, colorKey );
    renderResource_->UpdateTexture( surface );
}

SDL_Rect SDLRenderObject::GetRenderRect() const
{
    return renderRect_;
}

SDLRenderResource_ABC* SDLRenderObject::GetRenderResource( ) const
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