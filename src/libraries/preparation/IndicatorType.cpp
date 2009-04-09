// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorType.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorType constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorType::IndicatorType( xml::xistream& xis )
    : type_( xml::attribute< std::string >( xis, "type" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorType constructor
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
IndicatorType::IndicatorType( const std::string& type )
    : type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorType destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorType::~IndicatorType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorType::ToString
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
std::string IndicatorType::ToString() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorType::operator==
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
bool IndicatorType::operator==( const IndicatorType& rhs ) const
{
    return true; // $$$$ SBO 2009-04-09: TODO
}

// -----------------------------------------------------------------------------
// Name: IndicatorType::operator
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
bool IndicatorType::operator!=( const IndicatorType& rhs ) const
{
    return !( *this == rhs );
}
