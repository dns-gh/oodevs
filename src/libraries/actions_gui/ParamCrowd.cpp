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
#include "actions/Crowd.h"
#include "clients_kernel/Population_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamCrowd constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamCrowd::ParamCrowd(  const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::Population_ABC >( builder, parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamCrowd destructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamCrowd::~ParamCrowd()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamCrowd::CommitTo
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamCrowd::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::unique_ptr< actions::parameters::Entity< kernel::Population_ABC > > param( new actions::parameters::Crowd( parameter_, controllers_.controller_ ) );
    EntityParameter< kernel::Population_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamCrowd::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamCrowd::Visit( const actions::parameters::Crowd& param )
{
    InternalVisit( param );
}
