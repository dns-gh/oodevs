// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPointList.h"
#include "ParamLocation.h"
#include "actions/Action_ABC.h"
#include "actions/PointList.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPointList constructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPointList::ParamPointList( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller )
    : ParamLocationList( parent, parameter, layer, converter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPointList destructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPointList::~ParamPointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ParamPointList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::PointList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPointList::CreateElement()
{
    Param_ABC* param = ParamLocationList::CreateElement();
    static_cast< ParamLocation* >( param )->SetShapeFilter( true, false, false, false, false );
    return param;
}
