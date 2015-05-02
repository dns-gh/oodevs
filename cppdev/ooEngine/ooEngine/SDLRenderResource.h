#ifndef _SDLRENDERRESOURCE_H
#define _SDLRENDERRESOURCE_H

#include "Resource_ABC.h"
#include "SDL_render.h"
#include "SDL_pixels.h"

class SDLRenderResource : public Resource_ABC
{
public:
    SDLRenderResource();
    virtual ~SDLRenderResource();
    virtual void Load();
    virtual void UnLoad();

    void UpdateTexture( SDL_Surface* surface );
    void RenderTextureAtPos( int x, int y );
    SDL_Texture* GetTexture() const { return texture_; };
    SDL_Surface* GetSurface() const { return surface_; };
private:
    SDL_Texture* texture_;
    SDL_Surface* surface_;
};

#endif // SDLRenderResource.h