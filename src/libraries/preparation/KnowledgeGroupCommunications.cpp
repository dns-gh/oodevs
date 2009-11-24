// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "KnowledgeGroupCommunications.h"
#include "AutomatDecisions.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupCommunications constructor
// Created: FHD 2009-11-20
// -----------------------------------------------------------------------------
KnowledgeGroupCommunications::KnowledgeGroupCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : EntityCommunications( controller, holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupCommunications destructor
// Created: FHD 2009-11-20
// -----------------------------------------------------------------------------
KnowledgeGroupCommunications::~KnowledgeGroupCommunications()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupCommunications::SerializeAttributes
// Created: FHD 2009-11-20
// -----------------------------------------------------------------------------
void KnowledgeGroupCommunications::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const kernel::Entity_ABC& element = *it->second;
        if( ! element.Retrieve< AutomatDecisions >() )
        {
            xos << xml::start( "knowledge-group" );
            element.Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
            xos << xml::end();
        }
    }
}
