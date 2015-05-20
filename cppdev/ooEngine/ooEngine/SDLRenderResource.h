#ifndef _SDLRENDERRESOURCE_H
#define _SDLRENDERRESOURCE_H

#include "EngineObject.h"
#include "SDLRenderResource_ABC.h"
#include "SDL_render.h"

class SDL2DRenderManager;

class SDLRenderResource : public SDLRenderResource_ABC
                        , public EngineObject
{
public:
    SDLRenderResource( const SDL2DRenderManager& sdlRenderManager );
    virtual ~SDLRenderResource();
    virtual void Load();
    virtual void UnLoad();

    virtual void UpdateTexture( SDL_Surface* surface );
    virtual SDL_Texture* GetTexture( ) const { return texture_; };
    virtual SDL_Surface* GetSurface( ) const { return surface_; };

private:
    SDLRenderResource();

private:
    SDL_Texture* texture_;
    SDL_Surface* surface_;
    const SDL2DRenderManager& sdlRenderManager_;
};

#endif // SDLRenderResource.h