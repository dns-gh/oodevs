// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgentList.h"
#include "ParamAgent.h"
#include "gaming/ActionParameterAgentList.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/OrderParameter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAgentList::ParamAgentList( QObject* parent, const OrderParameter& parameter, ActionController& controller )
    : EntityListParameter< Agent_ABC >( parent, parameter, controller )
    , parameter_( parameter )
    , count_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamAgentList destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAgentList::~ParamAgentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tr( "Add agent" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::Agent_ABC >* ParamAgentList::CreateElement( const kernel::Agent_ABC& potential )
{
    return new ParamAgent( this, OrderParameter( tr( "Agent %1" ).arg( ++count_ ), "agent", false ), potential );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterAgentList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}
