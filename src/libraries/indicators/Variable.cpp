// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Variable.h"
#include "ElementType.h"
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Variable constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
Variable::Variable( const std::string& name, const std::string& type, const std::string& value )
    : Constant< std::string >( std::string( "$" ) + name, type, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Variable constructor
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
Variable::Variable( xml::xistream& xis )
    : Constant< std::string >( xml::attribute< std::string >( xis, "id" )
                             , xml::attribute< std::string >( xis, "type" )
                             , xml::attribute< std::string >( xis, "value" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Variable destructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
Variable::~Variable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Variable::Clone
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
Element_ABC& Variable::Clone() const
{
    return *new Variable( boost::erase_first_copy( GetInput(), "$" ), type_.ToString(), value_ );
}
