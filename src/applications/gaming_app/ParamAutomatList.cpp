// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAutomatList.h"
#include "ParamAutomat.h"
#include "gaming/ActionParameterAutomatList.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/OrderParameter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAutomatList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::ParamAutomatList( QObject* parent, const OrderParameter& parameter, ActionController& controller )
    : EntityListParameter< Automat_ABC >( parent, parameter, controller )
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
    MakeMenu( tr( "Add automat" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::Automat_ABC >* ParamAutomatList::CreateElement( const kernel::Automat_ABC& potential )
{
    return new ParamAutomat( this, OrderParameter( tr( "Automat %1" ).arg( ++count_ ), "automat", false ), potential );
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAutomatList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterAutomatList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}
