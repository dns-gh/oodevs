// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectsLayer.h"
#include "Controllers.h"
#include "ActionController.h"
#include "ColorStrategy_ABC.h"
#include "graphics/MapWidget.h"
#include "Positions.h"
#include "Object.h"
#include "Drawable_ABC.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, MapWidget& widget )
    : controllers_( controllers )
    , tools_      ( tools )
    , strategy_   ( strategy )
    , widget_     ( widget )
    , selected_   ( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::Paint
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectsLayer::Paint( const geometry::Rectangle2f& viewport ) // $$$$ AGE 2006-03-22: 
{
    for( CIT_Objects it = objects_.begin(); it != objects_.end(); ++it )
    {
        strategy_.SelectColor( **it );
        Object& object = const_cast< Object& >( **it ); // $$$$ AGE 2006-03-22: 
        geometry::Point2f pos = object.Get< Positions >().GetPosition();
        object.Apply( Drawable_ABC::Draw, pos, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleMousePress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( objects_.empty() || !event || event->state() == Qt::NoButton )
        return false;
    
    if( selected_ >= objects_.size() || ! IsInSelection( *objects_[ selected_ ], point ) || ++selected_ >= objects_.size() )
        selected_ = 0;

    for( unsigned i = selected_; i < objects_.size(); ++i )
    {
        const Object& object = *objects_[ i ];
        if( IsInSelection( object, point ) )
        {
            int button = event->button();
            if( button == Qt::LeftButton )
                controllers_.actions_.Select( object );
            else if( button == Qt::RightButton )
                controllers_.actions_.ContextMenu( object, event->globalPos() );
            selected_ = i;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifyCreated
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifyCreated( const Object& object )
{
    objects_.push_back( &object );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifyUpdated
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifyUpdated( const Object& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifyDeleted
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifyDeleted( const Object& object )
{
    IT_Objects it = std::find( objects_.begin(), objects_.end(), &object );
    if( it != objects_.end() )
        objects_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifyActivated
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifyActivated( const Object& object )
{
    widget_.Center( object.Get< Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::IsInSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ObjectsLayer::IsInSelection( const Object& object, const geometry::Point2f& point ) const
{
    return object.Get< Positions >().IsAt( point, 10* tools_.Pixels() );
}
