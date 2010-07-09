// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Diplomacy.h"

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy( const std::string& target, const std::string& type )
: target_( target )
, type_( type )
{
    // NOTHING
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
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Diplomacy constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Diplomacy::Diplomacy( const Diplomacy& rhs )
: target_( rhs.target_ )
, type_( rhs.type_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Diplomacy::operator=
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Diplomacy& Diplomacy::operator=( const Diplomacy& rhs )
{
    target_ = rhs.target_;
    type_ = rhs.type_;
    return *this;
}
