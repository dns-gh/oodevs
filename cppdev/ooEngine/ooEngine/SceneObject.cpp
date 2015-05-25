#include "SceneObject.h"
#include "SceneManager2D.h"
#include "Resource_ABC.h"
#include "ErrorLogManager.h"

#include <memory>

SceneObject::SceneObject( const SceneManager2D* sceneManager )
    : sceneManager_( sceneManager )
    , visible_( false )
    , x_( 0 )
    , y_( 0 )
{

}

SceneObject::SceneObject()
    : sceneManager_( 0 )
    , visible_( false )
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

const Circle& SceneObject::GetCollisionCircle() const
{
    return collisionCircle_;
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

void SceneObject::Move( const Geometry2D::Vector2D& dir )
{
    Vector2D pos( x_ + dir.x_, y_ + dir.y_ );
    Circle newCircle( pos, collisionCircle_.radius_ );
    if( sceneManager_ )
    {
        if( !sceneManager_->CheckCollisions( newCircle ) )
        {
            x_ = pos.x_;
            y_ = pos.y_;
            collisionCircle_ = newCircle;
            return;
        }
        return;
    }
    x_ = pos.x_;
    y_ = pos.y_;
    collisionCircle_ = newCircle;
}
