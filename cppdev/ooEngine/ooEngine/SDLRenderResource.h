#ifndef _SDLRENDERRESOURCE_H
#define _SDLRENDERRESOURCE_H

#include "SDLRenderResource_ABC.h"
#include "SDL_render.h"

class SDLRenderResource : public SDLRenderResource_ABC
{
public:
    SDLRenderResource();
    virtual ~SDLRenderResource();
    virtual void Load();
    virtual void UnLoad();

    virtual void UpdateTexture( SDL_Surface* surface );
    virtual SDL_Texture* GetTexture( ) const { return texture_; };
    virtual SDL_Surface* GetSurface( ) const { return surface_; };
private:
    SDL_Texture* texture_;
    SDL_Surface* surface_;
};

#endif // SDLRenderResource.h