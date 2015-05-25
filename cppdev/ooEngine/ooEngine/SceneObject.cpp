#include "SceneObject.h"
#include "Resource_ABC.h"
#include "ErrorLogManager.h"

#include <memory>

SceneObject::SceneObject()
    : visible_( false )
    , x_( 0 )
    , y_( 0 )
{
    // NOTHING
}

const std::shared_ptr< Resource_ABC >& SceneObject::GetRenderResource() const
{
    return renderResource_;
}

RenderRect SceneObject::GetRenderRect() const
{
    return renderRect_;
}

float  SceneObject::X() const
{
    return x_;
}

float  SceneObject::Y() const
{
    return y_;
}

bool  SceneObject::IsVisible() const
{
    return visible_;
}

void SceneObject::SetResourceObject( const std::shared_ptr< Resource_ABC >& resource )
{
    OOTHROW( 2, "SetResourceObject should have a proper body" );
}

void SceneObject::SetPosition( float x, float y )
{
    x_ = x;
    y_ = y;
}

void SceneObject::SetVisible( bool visibility )
{
    visible_ = visibility;
}

void SceneObject::SetCollisionCircle( Circle circle)
{
    collisionCircle_ = circle;
}
