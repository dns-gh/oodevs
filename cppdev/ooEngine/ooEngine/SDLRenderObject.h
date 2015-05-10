#ifndef _SDLRENDEROBJECT_H
#define _SDLRENDEROBJECT_H

#include "EngineObject.h"
#include "SceneObject_ABC.h"
#include "SDLRenderResource.h"
#include "SDL_rect.h"
#include "SDL_pixels.h"

#include <memory>

class SDLRenderObject : public EngineObject
                      , public SceneObject_ABC
{
public:
    SDLRenderObject();
    virtual ~SDLRenderObject();
    void SetResourceObject( std::shared_ptr< Resource_ABC > renderResource );
    virtual void Update(){};

    // Getters
    SDLRenderResource_ABC* GetRenderResource( ) const;
    SDL_Rect GetRenderRect() const;
    float X() const;
    float Y() const;
    bool IsVisible() const;

    // Setters
    void SetPosition( float x, float y );
    void SetVisible( bool visibility );
    void SetColorKeying( bool enable, unsigned int r = 0, unsigned int g = 0, unsigned int b = 0 ); // color keying is black by default

protected:
    SDLRenderResource_ABC* renderResource_;
    SDL_Rect renderRect_;
    float posX_;
    float posY_;
    bool visible_;
    SDL_Color colorKey_;
    bool colorKeyEnabled_;
};

#endif // SDLRenderObject.h