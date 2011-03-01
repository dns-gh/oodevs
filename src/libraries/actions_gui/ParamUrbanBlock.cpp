// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamUrbanBlock.h"
#include "actions/UrbanBlock.h"
#include "clients_kernel/Object_ABC.h"

using namespace actions::gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock constructor
// Created: LGY 2011-02-25
// -----------------------------------------------------------------------------
ParamUrbanBlock::ParamUrbanBlock( QObject* parent, const OrderParameter& parameter, Controller& controller )
    : EntityParameter< Object_ABC >( parent, parameter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock constructor
// Created: LGY 2011-02-25
// -----------------------------------------------------------------------------
ParamUrbanBlock::ParamUrbanBlock( QObject* parent, const OrderParameter& parameter, const Object_ABC& object, Controller& controller )
    : EntityParameter< Object_ABC >( parent, parameter, object, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock destructor
// Created: LGY 2011-02-25
// -----------------------------------------------------------------------------
ParamUrbanBlock::~ParamUrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::CommitTo
// Created: LGY 2011-02-25
// -----------------------------------------------------------------------------
void ParamUrbanBlock::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< Object_ABC > > param( new actions::parameters::UrbanBlock( parameter_, controller_ ) );
    EntityParameter< Object_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}
