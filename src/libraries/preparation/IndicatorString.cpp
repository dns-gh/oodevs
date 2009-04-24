// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorString.h"

// -----------------------------------------------------------------------------
// Name: IndicatorString constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
IndicatorString::IndicatorString( const std::string& value )
    : IndicatorConstant< std::string >( value, "string", value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorString destructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
IndicatorString::~IndicatorString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorString::Serialize
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void IndicatorString::Serialize( xml::xostream& /*xos*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorString::Clone
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
IndicatorElement_ABC& IndicatorString::Clone() const
{
    IndicatorElement_ABC* clone = new IndicatorString( value_ );
    return *clone;
}
