#include "ErrorLogManager.h"
#include "SDLRenderResource.h"
#include "SDL2DRenderManager.h"

#include "SDL_image.h"

SDLRenderResource::SDLRenderResource( const SDL2DRenderManager& sdlRenderManager )
: sdlRenderManager_( sdlRenderManager )
{
    // NOTHING
}

SDLRenderResource::~SDLRenderResource()
{
    UnLoad();
}

void SDLRenderResource::UpdateTexture( SDL_Surface* surface )
{
    if( !surface )
        return;

    if( texture_ )
        SDL_DestroyTexture( texture_ );
    SDL_Renderer* renderer = sdlRenderManager_.GetRenderer();
    texture_ = SDL_CreateTextureFromSurface( renderer, surface_ );
}

void SDLRenderResource::Load()
{
    UnLoad();
    SDL_Renderer* renderer = sdlRenderManager_.GetRenderer();
    if( !renderer )
        OOTHROW( 0, "Error when creating a texture from a file. There is no renderer available at this time." );

    //Load image at specified path
    surface_ = IMG_Load( GetResourceFilename().c_str() );
    if( !surface_ )
        OOTHROW( 0, "Error when creating a texture fom a file. Could not load the specified filename : " + GetResourceFilename() );
    texture_ = SDL_CreateTextureFromSurface( renderer, surface_ );
}

void SDLRenderResource:: UnLoad()
{
    if( texture_ )
        SDL_DestroyTexture( texture_ );
    if( surface_ )
        SDL_FreeSurface( surface_ );
    texture_ = 0;
    surface_ = 0;
}