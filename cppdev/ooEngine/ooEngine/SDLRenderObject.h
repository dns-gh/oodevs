#ifndef _SDLRENDEROBJECT_H
#define _SDLRENDEROBJECT_H

#include "EngineObject.h"
#include "Geometry2D.h"
#include "Resource_ABC.h"
#include "SceneObject.h"

#include "SDLRenderResource.h"
#include "SDL_rect.h"
#include "SDL_pixels.h"

#include <memory>

using namespace Geometry2D;

class SDLRenderObject : public EngineObject
                      , public SceneObject
{
public:
    SDLRenderObject();
    virtual ~SDLRenderObject();

    virtual void SetResourceObject( const std::shared_ptr< Resource_ABC >& renderResource );
    virtual void SetColorKeying( bool enable, unsigned int r = 0, unsigned int g = 0, unsigned int b = 0 ); // color keying is black by default

protected:
    SDL_Color colorKey_;
    bool colorKeyEnabled_;

private:
    void SetRenderRect( const SDL_Rect& rect );
};

#endif // SDLRenderObject.h