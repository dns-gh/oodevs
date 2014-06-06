// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAutomat.h"
#include "actions/Automat.h"
#include "clients_kernel/Automat_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAutomat constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamAutomat::ParamAutomat( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::Automat_ABC >( builder, parameter )
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
    std::unique_ptr< actions::parameters::Entity< kernel::Automat_ABC > > param( new actions::parameters::Automat( parameter_, controllers_.controller_ ) );
    EntityParameter< kernel::Automat_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamAutomat::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamAutomat::Visit( const actions::parameters::Automat& param )
{
    InternalVisit( param );
}
