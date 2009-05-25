// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamIntelligence.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "actions/Intelligence.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamIntelligence constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ParamIntelligence::ParamIntelligence( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Intelligence_ABC& entity, kernel::Controller& controller )
    : EntityParameter< kernel::Intelligence_ABC >( parent, parameter, entity, controller )
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
    std::auto_ptr< actions::parameters::Intelligence > param( new actions::parameters::Intelligence( parameter_, converter_, controller_ ) );
    EntityParameter< kernel::Intelligence_ABC >::CommitTo( *param );
    param->CommitToChildren();
    action.AddParameter( *param.release() );
}
