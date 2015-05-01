#ifndef _SDLRENDEROBJECT_H
#define _SDLRENDEROBJECT_H

#include "EngineObject.h"
#include "SDLRenderResource.h"
#include "SDL_rect.h"
#include "SDL_pixels.h"

class SDLRenderObject : public EngineObject
{
public:
    void SetResourceObject( SDLRenderResource* renderResource );
    virtual void Update(){};

    SDLRenderResource* GetRenderResource() const;
    SDL_Rect GetRenderRect() const;
    float X() const;
    float Y() const;
    bool IsVisible() const;

protected:
    SDLRenderResource* renderResource_;
    SDL_Rect renderRect_;
    float posX_;
    float posY_;
    bool visible_;
    /*SDL_Color colorKey_;
    bool colorKeyEnabled_;
    */
};

#endif // SDLRenderObject.h