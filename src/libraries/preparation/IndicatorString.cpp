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
IndicatorString::IndicatorString( const std::string& value, const IndicatorType& type )
    : IndicatorConstant< std::string >( value, value, type )
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
void IndicatorString::Serialize( xml::xostream& xos ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorString::SerializeDeclaration
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void IndicatorString::SerializeDeclaration( xml::xostream& ) const
{
    // NOTHING
}
