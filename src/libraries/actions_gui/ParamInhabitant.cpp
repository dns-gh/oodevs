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
#include "actions/Inhabitant.h"
#include "clients_kernel/Inhabitant_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamInhabitant constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamInhabitant::ParamInhabitant(  const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::Inhabitant_ABC >( builder, parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitant destructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamInhabitant::~ParamInhabitant()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitant::CommitTo
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamInhabitant::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::unique_ptr< actions::parameters::Entity< kernel::Inhabitant_ABC > > param( new actions::parameters::Inhabitant( parameter_, controllers_.controller_ ) );
    EntityParameter< kernel::Inhabitant_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitant::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamInhabitant::Visit( const actions::parameters::Inhabitant& param )
{
    InternalVisit( param );
}
