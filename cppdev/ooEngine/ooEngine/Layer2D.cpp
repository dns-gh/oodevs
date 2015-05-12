#include "Layer2D.h"

Layer2D::Layer2D( bool visible, unsigned int zorder, float x, float y, std::string name )
    : isVisible_( visible )
    , zOrder_( zorder )
    , x_( x )
    , y_( y )
    , name_( name )
{
    // NOTHING
}

float  Layer2D::X() const
{
    return x_;
}

float  Layer2D::Y() const
{
    return y_;
}

const std::string& Layer2D::GetName( ) const
{
    return name_;
}

unsigned int Layer2D::GetZOrder() const
{
    return zOrder_;
}

bool Layer2D::IsVisible() const
{
    return isVisible_;
}

const std::list< std::shared_ptr< SceneObject_ABC > > Layer2D::GetSceneObjects( ) const
{
    return sceneObjects_;
}

void Layer2D::SetVisible( bool visibility )
{
    isVisible_ = visibility;
}

void Layer2D::InsertSceneObject( const std::shared_ptr< SceneObject_ABC >& object )
{
    if( object )
        sceneObjects_.push_back( object );
}