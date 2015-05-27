#ifndef _SCENEOBJECT_H
#define _SCENEOBJECT_H

#include "CollisionProperty.h"
#include "Resource_ABC.h"
#include "EngineObject.h"
#include "Geometry2D.h"

#include <memory>

using namespace Geometry2D;

class SceneManager2D;

class SceneObject : public EngineObject
                  , public CollisionProperty
{
public:
    SceneObject( const SceneManager2D* sceneManager );
    SceneObject();
    virtual ~SceneObject(){};

    virtual void Update(){};

    const std::shared_ptr< Resource_ABC >& GetRenderResource() const;
    RenderRect GetRenderRect() const;
    float X() const;
    float Y() const;
    bool IsVisible() const;
    const Circle& GetCollisionCircle() const;

    virtual void SetResourceObject( const std::shared_ptr< Resource_ABC >& renderResource );
    virtual void SetColorKeying( bool enable, unsigned int r = 0, unsigned int g = 0, unsigned int b = 0 ){};
    void SetPosition( float x, float y );
    void SetVisible( bool visibility );
    void SetCollisionCircle( Circle circle );
    void SetSceneManager2D( const SceneManager2D* sceneManager );
    void Move( const Geometry2D::Vector2D& dir );

protected:
    const SceneManager2D* sceneManager_;
    std::shared_ptr< Resource_ABC > renderResource_;
    RenderRect renderRect_;
    float x_;
    float y_;
    bool visible_;
    Circle collisionCircle_;
};

#endif // SceneObject.h