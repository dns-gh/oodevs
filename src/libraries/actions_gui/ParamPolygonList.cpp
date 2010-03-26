// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPolygonList.h"
#include "ParamLocation.h"
#include "actions/Action_ABC.h"
#include "actions/PolygonList.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPolygonList constructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPolygonList::ParamPolygonList( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller )
    : ParamLocationList( parent, parameter, layer, converter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList destructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPolygonList::~ParamPolygonList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ParamPolygonList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::PolygonList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPolygonList::CreateElement()
{
    Param_ABC* param = ParamLocationList::CreateElement();
    static_cast< ParamLocation* >( param )->SetShapeFilter( false, false, true, false, false );
    return param;
}
