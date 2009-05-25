// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAgentList.h"
#include "moc_ParamAgentList.cpp"
#include "ParamAgent.h"
#include "actions/AgentList.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAgentList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAgentList::ParamAgentList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller )
    : EntityListParameter< kernel::Agent_ABC >( parent, parameter, actions, controller )
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
void ParamAgentList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tr( "Add agent" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::Agent_ABC >* ParamAgentList::CreateElement( const kernel::Agent_ABC& potential )
{
    return new ParamAgent( this, kernel::OrderParameter( tr( "Agent %1:%2" ).arg( ++count_ ).arg( potential.GetName() ).ascii(), "agent", false ), potential, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::AgentList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::NotifyContextMenu
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void ParamAgentList::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    superior_ = &entity;
    menu.InsertItem( "Parameter", tr( "Add hierarchy" ), this, SLOT( AddHierarchy() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::NotifyContextMenu
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void ParamAgentList::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
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
void ParamAgentList::AddHierarchy( const kernel::Entity_ABC& superior )
{
    // $$$$ AGE 2007-10-15: 
    if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( &superior ) )
        Add( *agent );
    else if( const kernel::Hierarchies* h = superior.Retrieve< kernel::TacticalHierarchies >() )
    {
        kernel::Iterator< const kernel::Entity_ABC& > children = h->CreateSubordinateIterator();
        while( children.HasMoreElements() )
            AddHierarchy( children.NextElement() );
    }
}
