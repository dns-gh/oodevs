#ifndef _SDLRENDERRESOURCE_ABC_H
#define _SDLRENDERRESOURCE_ABC_H

#include "Resource_ABC.h"
#include "SDL_render.h"

class SDLRenderResource_ABC : public Resource_ABC
{
public:
    virtual ~SDLRenderResource_ABC(){};
    virtual void UpdateTexture( SDL_Surface* surface ) = 0;
    virtual SDL_Texture* GetTexture() const = 0;
    virtual SDL_Surface* GetSurface() const = 0;
};

#endif // SDLRenderResource_ABC.h