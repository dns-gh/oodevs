#include "ErrorLogManager.h"
#include "SDLRenderResource.h"
#include "SDL2DRenderManager.h"

#include "SDL_image.h"

SDLRenderResource::SDLRenderResource()
{

}

SDLRenderResource::~SDLRenderResource()
{
    UnLoad();
}

void SDLRenderResource::Load()
{
    UnLoad();
    SDL_Renderer* renderer = SDL2DRenderManager::GetInstance()->GetRenderer();
    if( !renderer )
        OOTHROW( 0, "Error when creating a texture from a file. There is no renderer available at this time." );

    //Load image at specified path
    SDL_Surface* surface = IMG_Load( GetResourceFilename().c_str() );
    if( !surface )
        OOTHROW( 0, "Error when creating a texture fom a file. Could not load the specified filename : " + GetResourceFilename() );
    texture_ = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_RenderCopy( renderer, texture_, NULL, NULL );
    SDL_FreeSurface( surface );
}

void SDLRenderResource:: UnLoad()
{
    if( texture_ )
        SDL_DestroyTexture( texture_ );
    texture_ = 0;
    Reset();
}