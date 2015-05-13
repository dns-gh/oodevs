#include "SDLRenderObject.h"
#include "SDL2DRenderManager.h"
#include "ErrorLogManager.h"

#include "SDL_pixels.h"
#include "SDL_render.h"

SDLRenderObject::SDLRenderObject()
    : colorKeyEnabled_( false )
{
    // NOTHING
}

SDLRenderObject::~SDLRenderObject()
{
    renderResource_->UnLoad();
}

void SDLRenderObject::SetResourceObject( const std::shared_ptr< Resource_ABC >& resource )
{
    if( !resource || resource->GetResourceType() != RESOURCE_GRAPHIC )
        return;

    renderResource_ = resource;
    auto sdlResource = std::dynamic_pointer_cast< SDLRenderResource_ABC >( resource );
    if( !renderResource_ )
        OOTHROW( 1, "Cannot retrieve derived class for the given Resource" );
    renderResource_->Load();
    SDL_QueryTexture( sdlResource->GetTexture(), NULL, NULL, &renderRect_.w, &renderRect_.h );
}


void SDLRenderObject::SetColorKeying( bool enable, unsigned int r, unsigned int g, unsigned int b )
{
    if( !renderResource_ )
        OOTHROW( 1, "The rendering resource is not yet set. You may call SetResourceObject to do so." )

    colorKeyEnabled_ = enable;
    auto sdlResource = std::dynamic_pointer_cast< SDLRenderResource_ABC >( renderResource_ );
    SDL_Surface* surface = sdlResource->GetSurface();
    if( !surface )
        OOTHROW( 1, "Problem when initializing color keying : no surface set for the resource" );
    SDL_PixelFormat* format = surface->format;
    unsigned int colorKey = SDL_MapRGB( format, colorKey_.r, colorKey_.g, colorKey_.b );    
    SDL_SetColorKey( surface, colorKeyEnabled_, colorKey );
    sdlResource->UpdateTexture( surface );
}

void SDLRenderObject::SetRenderRect( const SDL_Rect& rect )
{
    memcpy( &renderRect_, &rect, sizeof( rect ) );
}
