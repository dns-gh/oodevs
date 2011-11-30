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
#include "actions/ParameterList.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAgentList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAgentList::ParamAgentList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller )
    : EntityListParameter< kernel::Agent_ABC >( parent, parameter, actions, controller )
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
    MakeMenu( tools::translate( "ListParameter", "%1: add item" ).arg( GetName() ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::Agent_ABC >* ParamAgentList::CreateElement( const kernel::Agent_ABC& potential )
{
    return new ParamAgent( this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "agent", false ), potential, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::NotifyContextMenu
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void ParamAgentList::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    superior_ = &entity;
    menu.InsertItem( "Parameter", tools::translate( "ListParameter", "Add hierarchy" ), this, SLOT( AddHierarchy() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::NotifyContextMenu
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void ParamAgentList::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    superior_ = &entity;
    menu.InsertItem( "Parameter", tools::translate( "ListParameter", "Add hierarchy" ), this, SLOT( AddHierarchy() ) );
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
        tools::Iterator< const kernel::Entity_ABC& > children = h->CreateSubordinateIterator();
        while( children.HasMoreElements() )
            AddHierarchy( children.NextElement() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::SetName
// Created: ABR 2011-01-21
// -----------------------------------------------------------------------------
void ParamAgentList::SetName( const QString& name )
{
    this->name_ = name;
    ListParameter::SetLabel( name );
}
