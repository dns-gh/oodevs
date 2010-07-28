// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Diplomacy.h"
#include "Mapping.h"

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy( xml::xisubstream xis, const Mapping& mapping )
    : mapping_( &mapping )
{
    xis >> xml::start( "target-side-ref" )
            >> xml::content( "id", target_ )
        >> xml::end
        >> xml::start( "relationship" )
            >> xml::start( "value" )
                >> xml::content( "enumeration-value", type_ );
}

// -----------------------------------------------------------------------------
// Name: Diplomacy destructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Diplomacy::~Diplomacy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: MCO 2010-07-13
// -----------------------------------------------------------------------------
xml::xostream& operator<<( xml::xostream& xos, const Diplomacy& diplomacy )
{
    return xos << xml::start( "relationship" )
                   << xml::attribute( "diplomacy", (*diplomacy.mapping_)[ diplomacy.type_ ] )
                   << xml::attribute( "side", (*diplomacy.mapping_)[ diplomacy.target_ ] )
               << xml::end;
}
