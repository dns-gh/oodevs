#ifndef _SCENEOBJECT_ABC_H
#define _SCENEOBJECT_ABC_H

#include "Resource_ABC.h"
#include "Geometry.h"

using namespace RenderGeometry;

class SceneObject_ABC
{
public:
    virtual ~SceneObject_ABC(){};
    virtual void Update() = 0;
    virtual Resource_ABC* GetRenderResource() const = 0;
    virtual RenderRect GetRenderRect() const = 0;
    virtual float X() const = 0;
    virtual float Y() const = 0;
    virtual bool IsVisible() const = 0;
};

#endif // SceneObject_ABC.h