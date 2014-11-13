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
#include "clients_kernel/TacticalHierarchies.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: TacticalLines constructor
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
TacticalLines::TacticalLines( const kernel::TacticalHierarchies& hierarchy )
    : hierarchy_( hierarchy )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLines destructor
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
TacticalLines::~TacticalLines()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLines::SerializeAttributes
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void TacticalLines::SerializeAttributes( xml::xostream& xos ) const
{
    tools::Iterator< const kernel::Entity_ABC& > it = hierarchy_.CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        const kernel::TacticalLine_ABC* line = dynamic_cast< const kernel::TacticalLine_ABC* >( &child );
        if( line )
        {
            xos << xml::start( line->IsLimit() ? "limit" : "lima" );
            line->GetInterfaces().Apply( &Serializable_ABC::SerializeAttributes, xos );
            xos << xml::end;
        }
    }
}
