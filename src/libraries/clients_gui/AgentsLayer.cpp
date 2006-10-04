// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AgentsLayer.h"
#include "moc_AgentsLayer.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace gui;

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
    Iterator< const Entity_ABC& > children = automat.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Entity_ABC& child = children.NextElement();
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
// Name: AgentsLayer::NotifyContextMenu
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyContextMenu( const Agent_ABC& agent, ::ContextMenu& menu )
{
    if( agent.GetSuperior() )
        return;

    selected_ = &agent;
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
// Name: AgentsLayer::Select
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const Entity_ABC& entity, bool shift )
{
    const Agent_ABC& agent = static_cast< const Agent_ABC& >( entity );
    const Agent_ABC* superior = agent.GetSuperior();
    if( shift && superior )
    {
        superior->Select( controllers_.actions_ );
        superior->Activate( controllers_.actions_ );
    }
    else
        agent.Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void AgentsLayer::DisplayTooltip( const Agent_ABC& agent, Displayer_ABC& displayer )
{
    // NOTHING
}
