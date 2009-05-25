// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAutomatList.h"
#include "ParamAutomat.h"
#include "actions/AutomatList.h"
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
    , parameter_( parameter )
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
    MakeMenu( tools::translate( "ParamAutomatList", "Add automat" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::Automat_ABC >* ParamAutomatList::CreateElement( const kernel::Automat_ABC& potential )
{
    return new ParamAutomat( this, kernel::OrderParameter( tools::translate( "ParamAutomatList", "Automat %1" ).arg( ++count_ ).ascii(), "automat", false ), potential, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAutomatList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::AutomatList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}
