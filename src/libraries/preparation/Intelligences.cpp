// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Intelligences.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Intelligences constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
Intelligences::Intelligences()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligences destructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
Intelligences::~Intelligences()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Intelligences::AddIntelligence
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void Intelligences::AddIntelligence( Intelligence_ABC& intelligence )
{
    Register( intelligence.GetId(), intelligence );
}

// -----------------------------------------------------------------------------
// Name: Intelligences::SerializeAttributes
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void Intelligences::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "intelligences" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "intelligence" );
        it->second->Interface().Apply( &Serializable_ABC::SerializeAttributes, xos );
        xos << end();
    }
    xos << end();
}
