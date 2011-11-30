//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamNumericList.h"
#include "ParamNumericField.h"
#include "actions/Action_ABC.h"
#include "actions/ParameterList.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"
#include "clients_gui/ValuedListItem.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamNumericList constructor
// Created: MMC 2011-03-22
// -----------------------------------------------------------------------------
ParamNumericList::ParamNumericList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, bool isReal )
    : ListParameter( parent, parameter, actions )
    , isReal_( isReal )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamNumericList destructor
// Created: MMC 2011-03-22
// -----------------------------------------------------------------------------
ParamNumericList::~ParamNumericList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamNumericList::CreateElement
// Created: MMC 2011-03-22
// -----------------------------------------------------------------------------
Param_ABC* ParamNumericList::CreateElement()
{
    return new ParamNumericField ( kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "Numeric", false ), isReal_ );
}

