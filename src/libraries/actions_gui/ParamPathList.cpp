// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPathList.h"
#include "ParamPath.h"
#include "actions/Action_ABC.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPathList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::ParamPathList( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& entity, kernel::ActionController& controller )
    : ListParameter( parent, parameter, controller )
    , converter_( converter )
    , layer_( layer )
    , entity_( entity )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPathList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::~ParamPathList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPathList::CreateElement()
{
    return new ParamPath( this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "path", false ), layer_, converter_, entity_ );
}
