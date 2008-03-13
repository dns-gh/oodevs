// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgentList.h"
#include "moc_ParamAgentList.cpp"
#include "ParamAgent.h"
#include "gaming/ActionParameterAgentList.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAgentList::ParamAgentList( QObject* parent, const OrderParameter& parameter, ActionController& controller )
    : EntityListParameter< Agent_ABC >( parent, parameter, controller )
    , parameter_( parameter )
    , count_( 0 )
    , superior_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamAgentList destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAgentList::~ParamAgentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentList::AddToMenu( ContextMenu& menu )
{
    MakeMenu( tr( "Add agent" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< Agent_ABC >* ParamAgentList::CreateElement( const Agent_ABC& potential )
{
    return new ParamAgent( this, OrderParameter( tr( "Agent %1:%2" ).arg( ++count_ ).arg( potential.GetName() ).ascii(), "agent", false ), potential );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterAgentList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::NotifyContextMenu
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void ParamAgentList::NotifyContextMenu( const Automat_ABC& entity, ContextMenu& menu )
{
    superior_ = &entity;
    menu.InsertItem( "Parameter", tr( "Add hierarchy" ), this, SLOT( AddHierarchy() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::NotifyContextMenu
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void ParamAgentList::NotifyContextMenu( const Formation_ABC& entity, ContextMenu& menu )
{
    superior_ = &entity;
    menu.InsertItem( "Parameter", tr( "Add hierarchy" ), this, SLOT( AddHierarchy() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::AddHierarchy
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void ParamAgentList::AddHierarchy()
{
    if( superior_ )
        AddHierarchy( *superior_ );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::AddHierarchy
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void ParamAgentList::AddHierarchy( const Entity_ABC& superior )
{
    // $$$$ AGE 2007-10-15: 
    if( const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &superior ) )
        Add( *agent );
    else if( const Hierarchies* h = superior.Retrieve< TacticalHierarchies >() )
    {
        Iterator< const Entity_ABC& > children = h->CreateSubordinateIterator();
        while( children.HasMoreElements() )
            AddHierarchy( children.NextElement() );
    }
}
