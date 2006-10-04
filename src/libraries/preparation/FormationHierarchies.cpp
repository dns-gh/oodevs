// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationHierarchies.h"
#include "AutomatDecisions.h"
#include "clients_kernel/Entity_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: FormationHierarchies constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationHierarchies::FormationHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : TacticalHierarchies( controller, holder, superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: FormationHierarchies destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationHierarchies::~FormationHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::DoSerialize
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void FormationHierarchies::DoSerialize( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( it->second->Retrieve< AutomatDecisions >() ? "automat" : "formation" ); // $$$$ SBO 2006-09-22: bof bof
        it->second->Serialize( xos );
        xos << end();
    }
}
