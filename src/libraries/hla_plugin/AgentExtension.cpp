// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentExtension.h"
#include "FederateFacade.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent& holder, FederateFacade& federate )
    : holder_( holder )
{
    federate.Register( holder_ );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentExtension::~AgentExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::Serialize
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void AgentExtension::Serialize( UpdateFunctor_ABC& functor, bool bUpdateAll ) const
{
    
}
