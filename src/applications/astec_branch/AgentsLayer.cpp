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

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controller& controller, ActionController& actions, SelectionProxy& proxy, const CoordinateConverter& converter, const GlTools_ABC& tools, ColorStrategy_ABC& strategy )
    : actions_  ( actions )
    , converter_( converter )
    , proxy_    ( proxy )
    , tools_    ( tools )
    , strategy_ ( strategy )
    , selected_ ( 0 )
{
    controller.Register( *this );
    proxy_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    // $$$$ AGE 2006-03-16: 
    //    controller_.Remove( *this );
    //    proxy_.Remove( *this );
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
        {
            selected_ = it - agents_.begin();
            proxy_.NotifyFocusLost( *this );
        }
        std::swap( *it, agents_.back() );
        agents_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleMousePress
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
// $$$$ AGE 2006-03-17: deal with button !
bool AgentsLayer::HandleMousePress( Qt::ButtonState button, const geometry::Point2f& point )
{
    if( ! HasFocus() || agents_.empty() )
        return false;
    
    if( selected_ >= agents_.size() || ! IsInSelection( *agents_[ selected_ ], point ) )
        selected_ = 0;

    for( unsigned i = selected_; i < agents_.size(); ++i )
        if( IsInSelection( *agents_[ i ], point ) )
        {
            actions_.Select( *agents_[ i ] );
            selected_ = i;
            return true;
        }
    proxy_.NotifyFocusLost( *this );
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsInSelection
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
bool AgentsLayer::IsInSelection( const Agent& agent, const geometry::Point2f& point ) const
{
    return agent.Get< Positions >().GetPosition().Distance( point ) < 100.f; // $$$$ SBO 2006-03-16: 
}
