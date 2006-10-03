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
#include "clients_kernel/DataDictionary.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
AgentHierarchies::AgentHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior, kernel::DataDictionary& dictionary )
    : EntityHierarchies( controller, holder, superior )
{
    if( superior )
        dictionary.Register( tools::translate( "Agent", "Hiérarchie/Supérieur" ), superior );
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
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "unit" );
        it->second->Serialize( xos );
        xos << end();
    }
}
