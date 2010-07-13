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
Diplomacy::Diplomacy( xml::xisubstream xis )
{
    xis >> xml::start( "ns5:target-side-ref" )
            >> xml::content( "ns2:id", target_ )
        >> xml::end
        >> xml::start( "ns5:relationship" )
            >> xml::start( "ns2:value" )
                >> xml::content( "ns2:enumeration-value", type_ );
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
// Name: Diplomacy::Write
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Diplomacy::Write( xml::xostream& xos, const Mapping& mapping ) const
{
    xos << xml::start( "relationship" )
            << xml::attribute( "diplomacy", mapping[ type_ ] )
            << xml::attribute( "side", mapping[ target_ ] )
        << xml::end;
}
