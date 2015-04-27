#ifndef _RENDERRESOURCE_H
#define _RENDERRESOURCE_H

#include "Resource.h"
#include "SDL_surface.h"

class RenderResource : public Resource
{
public:
    virtual ~RenderResource(){};
    virtual void Load(){};
    virtual void UnLoad(){};

private:
    SDL_Surface* surface_;
};

#endif // RenderResource.h