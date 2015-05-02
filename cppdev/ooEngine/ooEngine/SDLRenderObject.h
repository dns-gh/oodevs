#ifndef _SDLRENDEROBJECT_H
#define _SDLRENDEROBJECT_H

#include "EngineObject.h"
#include "SDLRenderResource.h"
#include "SDL_rect.h"
#include "SDL_pixels.h"

class SDLRenderObject : public EngineObject
{
public:
    SDLRenderObject();
    virtual ~SDLRenderObject();
    void SetResourceObject( Resource* renderResource );
    virtual void Update(){};

    // Getters
    Resource* GetRenderResource( ) const;
    SDL_Rect GetRenderRect() const;
    float X() const;
    float Y() const;
    bool IsVisible() const;

    // Setters
    void SetPosition( float x, float y );
    void SetVisible( bool visibility );
    void SetColorKeying( bool enable, unsigned int r = 0, unsigned int g = 0, unsigned int b = 0 ); // color keying is black by default

protected:
    Resource* renderResource_;
    SDL_Rect renderRect_;
    float posX_;
    float posY_;
    bool visible_;
    SDL_Color colorKey_;
    bool colorKeyEnabled_;
};

#endif // SDLRenderObject.h