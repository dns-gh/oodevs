// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamInhabitant.h"
#include "ParamInhabitantList.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamInhabitantList constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamInhabitantList::ParamInhabitantList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller )
    : EntityListParameter< kernel::Inhabitant_ABC >( parent, parameter, actions, controller )
    , parameter_( parameter )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitantList destructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamInhabitantList::~ParamInhabitantList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitantList::AddToMenu
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamInhabitantList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tools::translate( "ListParameter", "%1: add item" ).arg( GetName() ), menu );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter< kernel::Inhabitant_ABC >* ParamInhabitantList::CreateElement
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
EntityParameter< kernel::Inhabitant_ABC >* ParamInhabitantList::CreateElement( const kernel::Inhabitant_ABC& potential )
{
    return new ParamInhabitant( this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "agent", false ), potential, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitantList::CommitTo
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamInhabitantList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ParameterList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitantList::SetName
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamInhabitantList::SetName( const QString& name )
{
    this->name_ = name;
    ListParameter::SetLabel( name );
}
