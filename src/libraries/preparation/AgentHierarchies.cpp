// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentHierarchies.h"
#include "AutomatDecisions.h"
#include "clients_kernel/Entity_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
AgentHierarchies::AgentHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : EntityHierarchies( controller, holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
AgentHierarchies::~AgentHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoSerialize
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
void AgentHierarchies::DoSerialize( xml::xostream& xos ) const
{
    if( !GetEntity().Retrieve< AutomatDecisions >() ) // $$$$ SBO 2006-09-22: bof bof
        return;
    xos << start( "units" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "unit" );
        it->second->Serialize( xos );
        xos << end();
    }
    xos << end();
}
