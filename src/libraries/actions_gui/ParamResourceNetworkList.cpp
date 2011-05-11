// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamResourceNetworkList.h"
#include "ParamResourceNetwork.h"
#include "actions/ParameterList.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkList constructor
// Created: LDC 2011-05-11
// -----------------------------------------------------------------------------
ParamResourceNetworkList::ParamResourceNetworkList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller )
    : ListParameter( parent, parameter, actions )
    , parameter_( parameter )
    , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkList destructor
// Created: LDC 2011-05-11
// -----------------------------------------------------------------------------
ParamResourceNetworkList::~ParamResourceNetworkList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkList::CommitTo
// Created: LDC 2011-05-11
// -----------------------------------------------------------------------------
void ParamResourceNetworkList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ParameterList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamResourceNetworkList::CreateElement
// Created: LDC 2011-05-11
// -----------------------------------------------------------------------------
Param_ABC* ParamResourceNetworkList::CreateElement()
{
    return new ParamResourceNetwork( parent(), parameter_, controller_ );
}
