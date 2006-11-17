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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const Profile_ABC& profile )
    : EntityLayer< Agent_ABC >( controllers, tools, strategy, view, profile )
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
void AgentsLayer::Aggregate( const Automat_ABC& automat )
{
    Toggle( automat, true );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Disaggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::Disaggregate( const Automat_ABC& automat )
{
    Toggle( automat, false );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Toggle
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void AgentsLayer::Toggle( const Automat_ABC& automat, bool aggregate )
{
    Iterator< const Entity_ABC& > children = automat.Get< CommunicationHierarchies >().CreateSubordinateIterator();
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
    {
        AddEntity( automat ); // $$$$ AGE 2006-10-06: careful may have side effects
        aggregated_.insert( &automat );
    }
    else
    {
        RemoveEntity( automat );
        aggregated_.erase( &automat );
    }
}   

// -----------------------------------------------------------------------------
// Name: AgentsLayer::NotifyContextMenu
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentsLayer::NotifyContextMenu( const Automat_ABC& agent, ::ContextMenu& menu )
{
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
    const Entity_ABC* superior = entity.Get< CommunicationHierarchies >().GetSuperior();
    if( shift && superior )
    {
        superior->Select( controllers_.actions_ );
        superior->Activate( controllers_.actions_ );
    }
    else
        entity.Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::ContextMenu
// Created: AGE 2006-11-17
// -----------------------------------------------------------------------------
void AgentsLayer::ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& point, const QPoint& where )
{
    const kernel::Agent_ABC& agent = static_cast< const kernel::Agent_ABC& >( entity );
    controllers_.actions_.ContextMenu( agent, entity, point, where );
}
