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
#include "SelectionProxy.h"
#include "ColorStrategy_ABC.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controller& controller, ActionController& actions, const CoordinateConverter& converter, const GlTools_ABC& tools, ColorStrategy_ABC& strategy )
    : actions_  ( actions )
    , converter_( converter )
    , tools_    ( tools )
    , strategy_ ( strategy )
    , selected_ ( 0 )
    , menu_( new QPopupMenu() ) // $$$$ AGE 2006-03-20: don't make the menu_ orphan
{
    controller.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    // $$$$ AGE 2006-03-16: 
    //    controller_.Remove( *this );
    delete menu_;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Paint
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::Paint( const geometry::Rectangle2f& viewport )
{
    for( CIT_Agents it = agents_.begin(); it != agents_.end(); ++it )
    {
        Entity_ABC& agent = const_cast< Agent& >( **it ); // $$$$ AGE 2006-03-16: 
        const geometry::Point2f position = agent.Get< Positions >().GetPosition();
        if( viewport.IsInside( position ) )
        {
            strategy_.SelectColor( **it );
            agent.Apply( Drawable_ABC::Draw, position, tools_ );
        }
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
// Name: AgentsLayer::HandleMousePress
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( agents_.empty() || !event || event->state() == Qt::NoButton )
        return false;
    
    if( selected_ >= agents_.size() || ! IsInSelection( *agents_[ selected_ ], point ) )
        selected_ = 0;

    for( unsigned i = selected_; i < agents_.size(); ++i )
    {
        const Agent& agent = *agents_[ i ];
        if( IsInSelection( agent, point ) )
        {
            int button = event->button();
            if( button == Qt::LeftButton )
                actions_.Select( agent );
            else if( button == Qt::RightButton )
            {
                menu_->clear();
                actions_.ContextMenu( agent, *menu_ );
                if( menu_->count() > 0 )
                    menu_->popup( event->globalPos() );
            }
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
    const float halfSizeX = 500.f * 0.5f; // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f;
    const geometry::Point2f& center = agent.Get< Positions >().GetPosition();
    const geometry::Rectangle2f agentBBox( center.X() - halfSizeX, center.Y(), center.X() + halfSizeX, center.Y() + sizeY );
    return agentBBox.IsInside( point );
}
