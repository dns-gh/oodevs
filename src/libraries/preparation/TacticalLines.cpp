// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TacticalLines.h"
#include "TacticalLine_ABC.h"
#include <xeumeuleu/xml.h>

using namespace xml;

// -----------------------------------------------------------------------------
// Name: TacticalLines constructor
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
TacticalLines::TacticalLines()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLines destructor
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
TacticalLines::~TacticalLines()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TacticalLines::AddLine
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLines::AddLine( TacticalLine_ABC& line )
{
    Register( line.GetId(), line );
}

// -----------------------------------------------------------------------------
// Name: TacticalLines::SerializeAttributes
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLines::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( it->second->IsLimit() ? "limit" : "lima" );
        it->second->Apply( &Serializable_ABC::SerializeAttributes, xos );
        xos << end();
    }
}
