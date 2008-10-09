// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAutomat.h"
#include "actions/Automat.h"
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAutomat constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomat::ParamAutomat( QObject* parent, const OrderParameter& parameter, kernel::Controller& controller )
    : EntityParameter< Automat_ABC >( parent, parameter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ParamAutomat::ParamAutomat( QObject* parent, const OrderParameter& parameter, const Automat_ABC& entity, kernel::Controller& controller )
    : EntityParameter< Automat_ABC >( parent, parameter, entity, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomat destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomat::~ParamAutomat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomat::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAutomat::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< Automat_ABC > > param( new actions::parameters::Automat( parameter_, controller_ ) );
    EntityParameter< Automat_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

