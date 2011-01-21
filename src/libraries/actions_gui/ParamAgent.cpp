// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAgent.h"
#include "actions/Agent.h"
#include "clients_kernel/Agent_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAgent constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgent::ParamAgent( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : EntityParameter< kernel::Agent_ABC >( parent, parameter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ParamAgent::ParamAgent( QObject* parent, const kernel::OrderParameter& parameter, const kernel::Agent_ABC& entity, kernel::Controller& controller )
    : EntityParameter< kernel::Agent_ABC >( parent, parameter, entity, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgent::~ParamAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ParamAgent::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< kernel::Agent_ABC > > param( new actions::parameters::Agent( parameter_, controller_ ) );
    EntityParameter< kernel::Agent_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::SetName
// Created: ABR 2011-01-21
// -----------------------------------------------------------------------------
void ParamAgent::SetName( const QString& name )
{
    this->name_ = name;
    EntityParameterBase::SetLabel( name );
}
