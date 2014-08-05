// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamItinerary.h"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/Itinerary.h"
#include "clients_kernel/StaticModel.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamItinerary constructor
// Created: ABR 2014-05-20
// -----------------------------------------------------------------------------
ParamItinerary::ParamItinerary( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::Pathfind_ABC >( builder, parameter )
    , converter_( builder.GetStaticModel().coordinateConverter_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamItinerary destructor
// Created: ABR 2014-05-20
// -----------------------------------------------------------------------------
ParamItinerary::~ParamItinerary()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamItinerary::CommitTo
// Created: ABR 2014-05-20
// -----------------------------------------------------------------------------
void ParamItinerary::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Itinerary > param( new actions::parameters::Itinerary( parameter_, converter_, controllers_.controller_ ) );
    EntityParameter< kernel::Pathfind_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamItinerary::Visit
// Created: ABR 2014-05-20
// -----------------------------------------------------------------------------
void ParamItinerary::Visit( const actions::parameters::Itinerary& param )
{
    InternalVisit( param );
}
