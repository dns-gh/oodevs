// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamCommunications.h"
#include "preparation/KnowledgeGroupCommunications.h"
#include <xeumeuleu/xml.hpp>
#include "KnowledgeGroup.h"

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
    xos << xml::start( "communication" );
    tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        xos << xml::start( "knowledge-group" );
        it.NextElement().Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end;
    }
    xos << xml::end;
}
