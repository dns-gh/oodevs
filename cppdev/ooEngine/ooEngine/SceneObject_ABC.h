#ifndef _SCENEOBJECT_ABC_H
#define _SCENEOBJECT_ABC_H

#include "SDLRenderResource_ABC.h"
#include "SDL_rect.h"

class SceneObject_ABC
{
public:
    virtual ~SceneObject_ABC(){};
    virtual void Update() = 0;
    virtual SDLRenderResource_ABC* GetRenderResource() const = 0;
    virtual SDL_Rect GetRenderRect() const = 0;
    virtual float X() const = 0;
    virtual float Y() const = 0;
    virtual bool IsVisible() const = 0;
};

#endif // SceneObject_ABC.h