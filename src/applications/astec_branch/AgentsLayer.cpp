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
#include "moc_AgentsLayer.cpp"
#include "Agent.h"
#include "Aggregatable_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : EntityLayer< Agent >( controllers, tools, strategy, view )
    , selected_( 0 )
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
// Name: AgentsLayer::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Aggregate( const Agent& automat )
{
    if( automat.GetSuperior() )
        return;
    
    aggregatedAutomats_.push_back( &automat );
    Iterator< const Agent& > children = automat.CreateIterator();
    while( children.HasMoreElements() )
        RemoveEntity( children.NextElement() );
    Entity_ABC& entity = const_cast< Agent& >( automat );  // $$$$ AGE 2006-04-11: 
    bool aggregate = true;
    entity.Apply( Aggregatable_ABC::Aggregate, aggregate );
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

    Iterator< const Agent& > children = automat.CreateIterator();
    while( children.HasMoreElements() )
        AddEntity( children.NextElement() );
    Entity_ABC& entity = const_cast< Agent& >( automat );  // $$$$ AGE 2006-04-11: 
    bool aggregate = false;
    entity.Apply( Aggregatable_ABC::Aggregate, aggregate );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifyContextMenu
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    if( agent.GetSuperior() )
        return;
    if( menu.count() > 0 )
        menu.insertSeparator();

    selected_ = &agent;
    if( std::find( aggregatedAutomats_.begin(), aggregatedAutomats_.end(), selected_ ) == aggregatedAutomats_.end() )
        menu.insertItem( tr( "Aggreger" ), this, SLOT( Aggregate() ) );
    else
        menu.insertItem( tr( "Désaggreger" ), this, SLOT( Disaggregate() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Aggregate()
{
    if( selected_ )
        Aggregate( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Disaggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Disaggregate()
{
    if( selected_ )
        Disaggregate( *selected_ );
}
