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
#include "gaming/ActionParameterAutomat.h"
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAutomat constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomat::ParamAutomat( QObject* parent, const OrderParameter& parameter )
    : EntityParameter< Automat_ABC >( parent, parameter )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ParamAutomat::ParamAutomat( QObject* parent, const OrderParameter& parameter, const Automat_ABC& entity )
    : EntityParameter< Automat_ABC >( parent, parameter, entity )
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
void ParamAutomat::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameterEntity< Automat_ABC > > param( new ActionParameterAutomat( parameter_ ) );
    EntityParameter< Automat_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

