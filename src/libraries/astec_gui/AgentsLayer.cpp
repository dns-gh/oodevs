// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "AgentsLayer.h"
#include "moc_AgentsLayer.cpp"
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/Aggregatable_ABC.h"
#include "astec_gaming/AutomatDecisions.h"
#include "astec_gaming/Attributes.h"
#include "astec_gaming/Reports.h"
#include "astec_gaming/AutomatDecisions.h"
#include "astec_gaming/Decisions.h"
#include "astec_kernel/Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : EntityLayer< Agent_ABC >( controllers, tools, strategy, view )
    , selected_( controllers )
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
void AgentsLayer::Aggregate( const Agent_ABC& automat )
{
    Toggle( automat, true );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Disaggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Disaggregate( const Agent_ABC& automat )
{
    Toggle( automat, false );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Toggle
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void AgentsLayer::Toggle( const Agent_ABC& automat, bool aggregate )
{
    if( automat.GetSuperior() )
        return;
    Iterator< const Agent_ABC& > children = automat.CreateIterator();
    while( children.HasMoreElements() )
    {
        const Agent_ABC& child = children.NextElement();
        child.Interface().Apply( & Aggregatable_ABC::Aggregate, aggregate );    
        if( aggregate )
            RemoveEntity( child );
        else
            AddEntity( child );
    }
    automat.Interface().Apply( & Aggregatable_ABC::Aggregate, aggregate );

    if( aggregate )
        aggregated_.insert( &automat );
    else
        aggregated_.erase( &automat );
}   

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Engage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AgentsLayer::Engage( const Agent_ABC& automat )
{
    if( automat.GetSuperior() )
        return;
    automat.Retrieve< AutomatDecisions >()->Engage();
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AgentsLayer::Disengage( const Agent_ABC& automat )
{
    if( automat.GetSuperior() )
        return;
    automat.Retrieve< AutomatDecisions >()->Disengage();
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifyContextMenu
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyContextMenu( const Agent_ABC& agent, ::ContextMenu& menu )
{
    if( agent.GetSuperior() )
        return;

    selected_ = &agent;
    if( agent.Retrieve< AutomatDecisions >() )
    {
        if( ! agent.Retrieve< AutomatDecisions >()->IsEmbraye() )
            menu.InsertItem( "Commande", tr( "Embrayer" ), this, SLOT( Engage() ) );
        else
            menu.InsertItem( "Commande", tr( "Debrayer" ), this, SLOT( Disengage() ) );
    }

    if( aggregated_.find( &agent ) == aggregated_.end() )
        menu.InsertItem( "Interface", tr( "Aggreger" ), this, SLOT( Aggregate() ) );
    else
        menu.InsertItem( "Interface", tr( "Désaggreger" ), this, SLOT( Disaggregate() ) );
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
    const Agent_ABC& agent = static_cast< const Agent_ABC& >( entity );
    const Agent_ABC* superior = agent.GetSuperior();
    if( shift && superior )
    {
        controllers_.actions_.Select( *superior );
        controllers_.actions_.Activate( *superior );
    }
    else
        controllers_.actions_.Select( agent );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::AddToTooltip
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename Extension >
void AgentsLayer::AddToTooltip( const Agent_ABC& entity, Displayer_ABC& displayer )
{
    if( const Extension* extension = entity.Retrieve< Extension >() )
        extension->DisplayInTooltip( displayer );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void AgentsLayer::DisplayTooltip( const Agent_ABC& agent, Displayer_ABC& displayer )
{
    displayer.Display( "", agent );
    AddToTooltip< Attributes >      ( agent, displayer );
    AddToTooltip< Decisions >       ( agent, displayer );
    AddToTooltip< AutomatDecisions >( agent, displayer );
    AddToTooltip< Reports >         ( agent, displayer );
}
