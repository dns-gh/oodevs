// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamCommunications.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamCommunications constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
TeamCommunications::TeamCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : EntityCommunications( controller, holder, superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TeamCommunications destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
TeamCommunications::~TeamCommunications()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamCommunications::DoSerialize
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TeamCommunications::DoSerialize( xml::xostream& xos ) const
{
    xos << start( "communication" );
    Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        xos << start( "knowledge-group" );
        it.NextElement().Serialize( xos );
        xos << end();
    }
    xos << end();
}
