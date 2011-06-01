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
ParamInhabitant::ParamInhabitant( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : EntityParameter< kernel::Inhabitant_ABC >( parent, parameter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitant constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamInhabitant::ParamInhabitant( QObject* parent, const kernel::OrderParameter& parameter, const kernel::Inhabitant_ABC& entity, kernel::Controller& controller )
    : EntityParameter< kernel::Inhabitant_ABC >( parent, parameter, entity, controller )
    , parameter_( parameter )
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
    std::auto_ptr< actions::parameters::Entity< kernel::Inhabitant_ABC > > param( new actions::parameters::Inhabitant( parameter_, controller_ ) );
    EntityParameter< kernel::Inhabitant_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamInhabitant::SetName
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamInhabitant::SetName( const QString& name )
{
    this->name_ = name;
    EntityParameterBase::SetLabel( name );
}
