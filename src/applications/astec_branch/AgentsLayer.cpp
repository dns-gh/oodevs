// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentsLayer.h"
#include "Controller.h"
#include "ActionController.h"
#include "Agent.h"
#include "Positions.h"
#include "Drawable_ABC.h"
#include "Positions.h"
#include "ColorStrategy_ABC.h"
#include "GlTools_ABC.h"
#include "Positions.h"
#include "graphics/MapWidget.h"
#include "Controllers.h"

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, MapWidget& widget )
    : controllers_( controllers )
    , tools_      ( tools )
    , strategy_   ( strategy )
    , widget_     ( widget )
    , selected_   ( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Paint
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::Paint( const geometry::Rectangle2f& viewport )
{
    for( unsigned i = 0; i < agents_.size(); ++i )
        if( i != selected_ )
            Draw( *agents_.at( i ), viewport );
    if( selected_ < agents_.size() )
        Draw( *agents_.at( selected_ ), viewport );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void AgentsLayer::Draw( const Agent& a, const geometry::Rectangle2f& viewport )
{
    Entity_ABC& agent = const_cast< Agent& >( a ); // $$$$ AGE 2006-03-16: 
    const geometry::Point2f position = agent.Get< Positions >().GetPosition();
    if( viewport.IsInside( position ) )
    {
        strategy_.SelectColor( a );
        agent.Apply( Drawable_ABC::Draw, position, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifyCreated
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyCreated( const Agent& agent )
{
    agents_.push_back( &agent );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifyUpdated
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyUpdated( const Agent& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifyDeleted
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyDeleted( const Agent& agent )
{
    // $$$$ AGE 2006-03-16: std::collection
    IT_Agents it = std::find( agents_.begin(), agents_.end(), &agent );
    if( it != agents_.end() )
    {
        if( agents_[ selected_ ] == *it )
            selected_ = it - agents_.begin();
        std::swap( *it, agents_.back() );
        agents_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifyActivated
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyActivated( const Agent& agent )
{
    widget_.Center( agent.Get< Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleMousePress
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( agents_.empty() || !event || event->state() == Qt::NoButton )
        return false;
    
    if( selected_ >= agents_.size() || ! IsInSelection( *agents_[ selected_ ], point ) || ++selected_ >= agents_.size() )
        selected_ = 0;

    for( unsigned i = selected_; i < agents_.size(); ++i )
    {
        const Agent& agent = *agents_[ i ];
        if( IsInSelection( agent, point ) )
        {
            int button = event->button();
            if( button == Qt::LeftButton )
                controllers_.actions_.Select( agent );
            else if( button == Qt::RightButton )
                controllers_.actions_.ContextMenu( agent, event->globalPos() );
            selected_ = i;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsInSelection
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
bool AgentsLayer::IsInSelection( const Agent& agent, const geometry::Point2f& point ) const
{
    return agent.Get< Positions >().IsAt( point );
}
