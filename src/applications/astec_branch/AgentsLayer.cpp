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
#include "Agent.h"
#include "Aggregations.h"

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : EntityLayer< Agent >( controllers, tools, strategy, view )
    , tools_( tools )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Paint
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Paint( const geometry::Rectangle2f& viewport )
{
    EntityLayer< Agent >::Paint( viewport );
    for( CIT_Agents it = aggregatedAutomats_.begin(); it != aggregatedAutomats_.end(); ++it )
    {
        const Agent& agent = **it;
        agent.Get< Aggregations >().Draw( agent.Get< Positions >().GetPosition(), tools_ );
    };
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Aggregate( const Agent& automat )
{
    if( automat.GetSuperior() || ! RemoveEntity( automat ) )
        return;
    
    aggregatedAutomats_.push_back( &automat );
    Iterator< const Agent& > children = automat.CreateIterator();
    while( children.HasMoreElements() )
        RemoveEntity( children.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Disaggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Disaggregate( const Agent& automat )
{
    IT_Agents it = std::find( aggregatedAutomats_.begin(), aggregatedAutomats_.end(), &automat );
    if( it == aggregatedAutomats_.end() )
        return;

    std::swap( *it, aggregatedAutomats_.back() );
    aggregatedAutomats_.pop_back();

    AddEntity( automat );
    Iterator< const Agent& > children = automat.CreateIterator();
    while( children.HasMoreElements() )
        AddEntity( children.NextElement() );
}
