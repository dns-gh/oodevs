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
#include "AgentDrawer.h"

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controller& controller, ActionController& actions, const CoordinateConverter& converter )
    : actions_  ( actions )
    , converter_( converter )
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
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Paint
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::Paint( const geometry::Rectangle2f& viewport )
{
    for( CIT_Agents it = agents_.begin(); it != agents_.end(); ++it )
        (*it)->Get< AgentDrawer >().Draw( viewport );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifyCreated
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyCreated( const Agent& a )
{
    Agent& agent = const_cast< Agent& >( a ); // $$$$ AGE 2006-03-16: 
    if( ! agent.Retrieve< AgentDrawer >() )
        agent.Attach( *new AgentDrawer( agent, converter_, *this ) );
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
        std::swap( *it, agents_.back() );
        agents_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::DrawCross
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::DrawCross( const MT_Vector2D& at ) const
{

}
