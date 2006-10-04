// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntityLayer.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "View_ABC.h"
#include "GlTooltip.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::EntityLayerBase
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
EntityLayerBase::EntityLayerBase( Controllers& controllers, const GlTools_ABC& tools, View_ABC& view )
    : controllers_( controllers )
    , tools_      ( tools )
    , view_       ( view )
    , tooltiped_  ( std::numeric_limits< unsigned >::max() ) 
    , tooltip_    ( new GlTooltip() )
    , selected_   ( 0 )
    , currentTeam_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::~EntityLayerBase
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
EntityLayerBase::~EntityLayerBase()
{
    delete tooltip_;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Paint
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Paint( const geometry::Rectangle2f& viewport )
{
    for( unsigned i = 0; i < entities_.size(); ++i )
        if( i != selected_ )
            Draw( *entities_[ i ], viewport );
    if( selected_ < entities_.size() )
        Draw( *entities_[ selected_ ], viewport );
    if( tooltiped_ < entities_.size() )
    {
        const Positions& positions = entities_[ tooltiped_ ]->Get< Positions >();
        const geometry::Point2f position = positions.GetPosition();
        tooltip_->Draw( position );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Draw( const Entity_ABC& entity, const geometry::Rectangle2f& viewport )
{
    if( ShouldDisplay( entity ) ) // && positions.IsIn( viewport ) )
    {
        SelectColor( entity );
        const Positions& positions = entity.Get< Positions >();
        const geometry::Point2f position = positions.GetPosition();
        entity.Draw( position, viewport, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ShouldDisplay
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
bool EntityLayerBase::ShouldDisplay( const Entity_ABC& entity )
{
    return ! currentTeam_ || IsInTeam( entity, *currentTeam_ );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::IsInTeam
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
bool EntityLayerBase::IsInTeam( const kernel::Entity_ABC& entity, const kernel::Team_ABC& team )
{
    const TacticalHierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >();
    return !hierarchies || hierarchies->IsSubordinateOf( team );
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
    if( button != Qt::LeftButton && button != Qt::RightButton )
        return false;

    if( selected_ >= entities_.size()
     || ! IsInSelection( *entities_[ selected_ ], point )
     || ! ShouldDisplay( *entities_[ selected_ ] )
     || ( button == Qt::LeftButton && ++selected_ > entities_.size() ) )
        selected_ = 0;

    for( ; selected_ < entities_.size(); ++selected_ )
    {
        const Entity_ABC& entity = *entities_[ selected_ ];
        tooltiped_ = selected_;
        if( ShouldDisplay( entity ) && IsInSelection( entity, point ) )
        {
            if( button == Qt::LeftButton )
                Select( entity, event->state() & Qt::ShiftButton );
            else if( button == Qt::RightButton )
                ContextMenu( entity, point, event->globalPos() );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Select
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
void EntityLayerBase::Select( const Entity_ABC& entity, bool )
{
    entity.Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ContextMenu
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void EntityLayerBase::ContextMenu( const Entity_ABC& entity, const geometry::Point2f&, const QPoint& where )
{
    entity.ContextMenu( controllers_.actions_, where );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::HandleMouseMove
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
bool EntityLayerBase::HandleMouseMove( QMouseEvent* , const geometry::Point2f& point )
{
    if( ! ShouldDisplayTooltip( tooltiped_, point ) )
    {
        tooltiped_ = std::numeric_limits< unsigned >::max();
        tooltip_->Hide();
        if( ! DisplayTooltip( selected_, point ) )
        {
            bool found = false;
            for( unsigned i = 0; i < entities_.size() && !found ; ++i )
                found = i != selected_ && DisplayTooltip( i, point );
        }
    }
    return false;
}   

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ShouldDisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
bool EntityLayerBase::ShouldDisplayTooltip( unsigned i, const geometry::Point2f& point )
{
    return i < entities_.size()
        && ShouldDisplay( *entities_[ i ] )
        && IsInSelection( *entities_[ i ], point );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
bool EntityLayerBase::DisplayTooltip( unsigned i, const geometry::Point2f& point )
{
    return ShouldDisplayTooltip( i, point )
        && DisplayTooltip( *entities_[ i ], *tooltip_ )
        && ( tooltiped_ = i, 1 );  // $$$$ AGE 2006-06-29: duh
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
bool EntityLayerBase::DisplayTooltip( const Entity_ABC&, Displayer_ABC& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::OptionChanged
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void EntityLayerBase::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "CurrentTeam" )
        currentTeam_ = value.To< const Team_ABC* >();
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::IsInSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool EntityLayerBase::IsInSelection( const Entity_ABC& entity, const geometry::Point2f& point ) const
{
    return entity.Get< Positions >().IsAt( point, 10.f * tools_.Pixels() );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::AddEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::AddEntity( const Entity_ABC& entity )
{
    if( std::find( entities_.begin(), entities_.end(), &entity ) == entities_.end() )
        entities_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::RemoveEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool EntityLayerBase::RemoveEntity( const Entity_ABC& entity )
{
    IT_Entities it = std::find( entities_.begin(), entities_.end(), &entity );
    if( it != entities_.end() )
    {
        if( entities_[ selected_ ] == *it )
            selected_ = it - entities_.begin();
        std::swap( *it, entities_.back() );
        entities_.pop_back();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ActivateEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::ActivateEntity( const Entity_ABC& entity )
{
    view_.CenterOn( entity.Get< Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::SelectEntity
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void EntityLayerBase::SelectEntity( const Entity_ABC& entity )
{
    IT_Entities it = std::find( entities_.begin(), entities_.end(), &entity );
    if( it != entities_.end() )
        selected_ = it - entities_.begin();
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::SelectColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::SelectColor( const Entity_ABC& )
{
    // NOTHING
}
