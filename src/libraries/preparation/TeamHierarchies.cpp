// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: TeamHierarchies constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
TeamHierarchies::TeamHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : EntityHierarchies( controller, holder, superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TeamHierarchies destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
TeamHierarchies::~TeamHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::DoSerialize
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void TeamHierarchies::DoSerialize( xml::xostream& xos ) const
{
    xos << start( "formations" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "formation" );
        it->second->Serialize( xos );
        xos << end();
    }
    xos << end();
}
