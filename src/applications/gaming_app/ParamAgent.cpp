// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgent.h"
#include "gaming/ActionParameterAgent.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgent constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgent::ParamAgent( QObject* parent, const kernel::OrderParameter& parameter )
    : EntityParameter< Agent_ABC >( parent, parameter )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ParamAgent::ParamAgent( QObject* parent, const kernel::OrderParameter& parameter, const kernel::Agent_ABC& entity )
    : EntityParameter< Agent_ABC >( parent, parameter, entity )
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
void ParamAgent::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameterEntity< Agent_ABC > > param( new ActionParameterAgent( parameter_ ) );
    EntityParameter< Agent_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

