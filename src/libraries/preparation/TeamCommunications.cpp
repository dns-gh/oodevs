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
#include <xeumeuleu/xml.h>

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
// Name: TeamCommunications::SerializeAttributes
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TeamCommunications::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "communication" );
    WriteKnowledgeGroup( xos );
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: TeamCommunications::SerializeAttributes
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TeamCommunications::WriteKnowledgeGroup( xml::xostream& xos ) const
{
    tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        xos << start( "knowledge-group" );
        it.NextElement().Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        // read inner knowledge groups
        bool bHasSubKnowledgeGroups = false;
        if( bHasSubKnowledgeGroups )
            WriteKnowledgeGroup( xos );
        xos << end();
    }
}
