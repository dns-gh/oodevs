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


void SDLRenderResource::RenderTextureAtPos( int x, int y )
{
    // Position of the texture to be drawn
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    // Get width and height of the texture
    SDL_QueryTexture( texture_, NULL, NULL, &dst.w, &dst.h );
    SDL_Renderer* renderer = SDL2DRenderManager::GetInstance()->GetRenderer();
    if( !renderer )
        OOTHROW( 0, "Error when trying to render a texture in a rendering context" );
    SDL_RenderCopy( renderer, texture_, NULL, &dst );
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
    SDL_FreeSurface( surface );
}

void SDLRenderResource:: UnLoad()
{
    if( texture_ )
        SDL_DestroyTexture( texture_ );
    texture_ = 0;
    Reset();
}