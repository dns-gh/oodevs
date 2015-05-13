#ifndef _SCENEOBJECT_H
#define _SCENEOBJECT_H

#include "Resource_ABC.h"
#include "Geometry.h"

#include <memory>

using namespace RenderGeometry;

class SceneObject
{
public:
    SceneObject();
    virtual ~SceneObject(){};

    virtual void Update(){};

    const std::shared_ptr< Resource_ABC >& GetRenderResource() const;
    RenderRect GetRenderRect() const;
    float X() const;
    float Y() const;
    bool IsVisible() const;

    virtual void SetResourceObject( const std::shared_ptr< Resource_ABC >& renderResource );
    void SetPosition( float x, float y );
    void SetVisible( bool visibility );

protected:
    std::shared_ptr< Resource_ABC > renderResource_;
    RenderRect renderRect_;
    float x_;
    float y_;
    bool visible_;
};

#endif // SceneObject.h