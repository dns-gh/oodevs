#ifndef _SDLRENDERRESOURCE_H
#define _SDLRENDERRESOURCE_H

#include "Resource.h"
#include "SDL_render.h"

class SDLRenderResource : public Resource
{
public:
    SDLRenderResource();
    virtual ~SDLRenderResource();
    virtual void Load();
    virtual void UnLoad();

private:
    SDL_Texture* texture_;
};

#endif // SDLRenderResource.h