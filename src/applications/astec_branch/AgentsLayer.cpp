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
    {
        Entity_ABC& agent = const_cast< Agent& >( **it ); // $$$$ AGE 2006-03-16: 
        const geometry::Point2f position = agent.Get< Positions >().GetPosition();
        if( viewport.IsInside( position ) )
            agent.Apply( Drawable_ABC::Draw, position, *this );
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
        std::swap( *it, agents_.back() );
        agents_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::DrawCross
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::DrawCross( const geometry::Point2f& at ) const
{
    const float rSize = 200.f; // $$$$ AGE 2006-03-16: arg, ...
    glBegin( GL_LINES );
        glVertex2f(  at.X() - rSize, at.Y() - rSize );
        glVertex2f(  at.X() + rSize, at.Y() + rSize );
        glVertex2f(  at.X() + rSize, at.Y() - rSize );
        glVertex2f(  at.X() - rSize, at.Y() + rSize );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::DrawLine
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentsLayer::DrawLine( const geometry::Point2f& from, const geometry::Point2f& to ) const
{
    glBegin( GL_LINES );
        glVertex2f(  from.X(), from.Y() );
        glVertex2f(  to.X()  , to.Y() );
    glEnd();
}
