// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorVariable.h"
#include "IndicatorType.h"
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorVariable constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorVariable::IndicatorVariable( const std::string& name, const std::string& type, const std::string& value )
    : IndicatorConstant< std::string >( std::string( "$" ) + name, type, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariable constructor
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
IndicatorVariable::IndicatorVariable( xml::xistream& xis )
    : IndicatorConstant< std::string >( xml::attribute< std::string >( xis, "id" )
                                      , xml::attribute< std::string >( xis, "type" )
                                      , xml::attribute< std::string >( xis, "value" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariable destructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorVariable::~IndicatorVariable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariable::Serialize
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
void IndicatorVariable::Serialize( xml::xostream& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariable::SerializeDeclaration
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
void IndicatorVariable::SerializeDeclaration( xml::xostream& xos ) const
{
    IndicatorConstant< std::string >::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariable::Clone
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
IndicatorElement_ABC& IndicatorVariable::Clone() const
{
    return *new IndicatorVariable( boost::erase_first_copy( GetInput(), "$" ), type_.ToString(), value_ );
}
