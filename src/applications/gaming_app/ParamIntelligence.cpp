// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamIntelligence.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "actions/Intelligence.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamIntelligence constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ParamIntelligence::ParamIntelligence( QObject* parent, const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Intelligence_ABC& entity )
    : EntityParameter< Intelligence_ABC >( parent, parameter, entity )
    , parameter_( parameter )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamIntelligence destructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ParamIntelligence::~ParamIntelligence()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamIntelligence::CommitTo
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
void ParamIntelligence::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Intelligence > param( new actions::parameters::Intelligence( parameter_, converter_ ) );
    EntityParameter< Intelligence_ABC >::CommitTo( *param );
    param->CommitToChildren();
    action.AddParameter( *param.release() );
}
