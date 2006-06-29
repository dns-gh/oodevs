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
#include "AutomatDecisions.h"
#include "Attributes.h"
#include "Reports.h"
#include "AutomatDecisions.h"
#include "Decisions.h"

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
    
    Iterator< const Agent& > children = automat.CreateIterator();
    while( children.HasMoreElements() )
        RemoveEntity( children.NextElement() );
    bool aggregate = true;
    automat.Interface().Apply( & Aggregatable_ABC::Aggregate, aggregate );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Disaggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Disaggregate( const Agent& automat )
{
    if( automat.GetSuperior() )
        return;

    Iterator< const Agent& > children = automat.CreateIterator();
    while( children.HasMoreElements() )
        AddEntity( children.NextElement() );
    bool aggregate = false;
    automat.Interface().Apply( & Aggregatable_ABC::Aggregate, aggregate );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Engage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AgentsLayer::Engage( const Agent& automat )
{
    if( automat.GetSuperior() )
        return;
    automat.Retrieve< AutomatDecisions >()->Engage();
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AgentsLayer::Disengage( const Agent& automat )
{
    if( automat.GetSuperior() )
        return;
    automat.Retrieve< AutomatDecisions >()->Disengage();
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
    if( agent.Retrieve< AutomatDecisions >() )
    {
        if( ! agent.Retrieve< AutomatDecisions >()->IsEmbraye() )
            menu.insertItem( tr( "Embrayer" ), this, SLOT( Engage() ) );
        else
            menu.insertItem( tr( "Debrayer" ), this, SLOT( Disengage() ) );
    }

    if( ! agent.IsAggregated() )
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

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Engage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AgentsLayer::Engage()
{
    if( selected_ )
        Engage( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AgentsLayer::Disengage()
{
    if( selected_ )
        Disengage( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Select
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const Entity_ABC& entity, bool shift )
{
    const Agent& agent = static_cast< const Agent& >( entity );
    const Agent* superior = agent.GetSuperior();
    if( shift && superior )
    {
        controllers_.actions_.Select( *superior );
        controllers_.actions_.Activate( *superior );
    }
    else
        controllers_.actions_.Select( agent );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void AgentsLayer::DisplayTooltip( const Agent& agent, Displayer_ABC& displayer )
{
    displayer.Display( "", agent );
    agent.Get< Attributes >().DisplayInTooltip( displayer );
    {
        const Decisions* decisions = agent.Retrieve< Decisions >();
        if( decisions )
            decisions->DisplayInTooltip( displayer );
    }
    {
        const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >();
        if( decisions )
            decisions->DisplayInTooltip( displayer );

    }
    agent.Get< Reports >().DisplayInTooltip( displayer );
}
