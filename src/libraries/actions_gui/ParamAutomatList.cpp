// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAutomatList.h"
#include "ParamAutomat.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAutomatList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::ParamAutomatList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller )
    : EntityListParameter< kernel::Automat_ABC >( parent, parameter, actions, controller )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::~ParamAutomatList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAutomatList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tools::translate( "ListParameter", "%1: add item" ).arg( GetName() ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::Automat_ABC >* ParamAutomatList::CreateElement( const kernel::Automat_ABC& potential )
{
    return new ParamAutomat( this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "automat", false ), potential, controller_ );
}
