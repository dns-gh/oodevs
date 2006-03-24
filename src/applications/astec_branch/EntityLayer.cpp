// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "EntityLayer.h"
#include "Entity_ABC.h"

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::EntityLayerBase
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
EntityLayerBase::EntityLayerBase( const GlTools_ABC& tools, MapWidget& widget )
    : tools_      ( tools )
    , widget_     ( widget )
    , selected_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::~EntityLayerBase
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
EntityLayerBase::~EntityLayerBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Paint
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Paint( const geometry::Rectangle2f& viewport )
{
    for( unsigned i = 0; i < entities_.size(); ++i )
        if( i != selected_ )
            Draw( *entities_.at( i ), viewport );
    if( selected_ < entities_.size() )
        Draw( *entities_.at( selected_ ), viewport );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Draw( const Entity_ABC& e, const geometry::Rectangle2f& viewport )
{
    Entity_ABC& entity = const_cast< Entity_ABC& >( e ); // $$$$ AGE 2006-03-16: 
    const Positions& positions = entity.Get< Positions >();
    if( positions.IsIn( viewport ) )
    {
        SelectColor( e );
        const geometry::Point2f position = positions.GetPosition();
        entity.Apply( Drawable_ABC::Draw, position, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::HandleMousePress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool EntityLayerBase::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( entities_.empty() || !event || event->state() == Qt::NoButton )
        return false;
    const int button = event->button();
    
    if( selected_ >= entities_.size() 
     || ! IsInSelection( *entities_[ selected_ ], point ) 
     || ( button == Qt::LeftButton && ++selected_ >= entities_.size() ) )
        selected_ = 0;

    for( ; selected_ < entities_.size(); ++selected_ )
    {
        const Entity_ABC& entity = *entities_[ selected_ ];
        if( IsInSelection( entity, point ) )
        {
            if( button == Qt::LeftButton )
                Select( entity );
            else if( button == Qt::RightButton )
                ContextMenu( entity, event->globalPos() );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::IsInSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool EntityLayerBase::IsInSelection( const Entity_ABC& entity, const geometry::Point2f& point ) const
{
    return entity.Get< Positions >().IsAt( point );
}
    
// -----------------------------------------------------------------------------
// Name: EntityLayerBase::AddEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::AddEntity( const Entity_ABC& entity )
{
    entities_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::RemoveEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::RemoveEntity( const Entity_ABC& entity )
{
    IT_Entities it = std::find( entities_.begin(), entities_.end(), &entity );
    if( it != entities_.end() )
    {
        if( entities_[ selected_ ] == *it )
            selected_ = it - entities_.begin();
        std::swap( *it, entities_.back() );
        entities_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ActivateEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::ActivateEntity( const Entity_ABC& entity )
{
    widget_.Center( entity.Get< Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::SelectColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::SelectColor( const Entity_ABC& )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Select( const Entity_ABC& )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ContextMenu
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::ContextMenu( const Entity_ABC&, const QPoint&  )
{
    // NOTHING
}
