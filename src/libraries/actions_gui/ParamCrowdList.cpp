// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamCrowd.h"
#include "ParamCrowdList.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamCrowdList constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamCrowdList::ParamCrowdList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller )
    : EntityListParameter< kernel::Population_ABC >( parent, parameter, actions, controller )
    , parameter_( parameter )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamCrowdList destructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamCrowdList::~ParamCrowdList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamCrowdList::AddToMenu
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamCrowdList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tools::translate( "ListParameter", "%1: add item" ).arg( GetName() ), menu );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter< kernel::Population_ABC >* ParamCrowdList::CreateElement
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
EntityParameter< kernel::Population_ABC >* ParamCrowdList::CreateElement( const kernel::Population_ABC& potential )
{
    return new ParamCrowd( this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "agent", false ), potential, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamCrowdList::CommitTo
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamCrowdList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ParameterList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}


// -----------------------------------------------------------------------------
// Name: ParamCrowdList::SetName
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamCrowdList::SetName( const QString& name )
{
    this->name_ = name;
    ListParameter::SetLabel( name );
}
