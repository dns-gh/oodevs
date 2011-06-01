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
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamCrowd::ParamCrowd( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : EntityParameter< kernel::Population_ABC >( parent, parameter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamCrowd constructor
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
ParamCrowd::ParamCrowd( QObject* parent, const kernel::OrderParameter& parameter, const kernel::Population_ABC& entity, kernel::Controller& controller )
    : EntityParameter< kernel::Population_ABC >( parent, parameter, entity, controller )
    , parameter_( parameter )
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
    std::auto_ptr< actions::parameters::Entity< kernel::Population_ABC > > param( new actions::parameters::Crowd( parameter_, controller_ ) );
    EntityParameter< kernel::Population_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamCrowd::SetName
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
void ParamCrowd::SetName( const QString& name )
{
    this->name_ = name;
    EntityParameterBase::SetLabel( name );
}
