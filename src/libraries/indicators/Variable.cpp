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
#include "DataType_ABC.h"
#include "DataTypeFactory.h"
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Variable constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
Variable::Variable( const std::string& name, boost::shared_ptr< DataType_ABC > type, const std::string& value )
    : Constant< std::string >( std::string( "$" ) + name, type, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Variable constructor
// Created: SBO 2009-05-12
// -----------------------------------------------------------------------------
Variable::Variable( const Element_ABC& element, const DataTypeFactory& types )
    : Constant< std::string >( element.GetInput(), types.Instanciate( element.GetType().ToString() ), element.GetValue() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Variable constructor
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
Variable::Variable( xml::xistream& xis, const DataTypeFactory& types )
    : Constant< std::string >( xis.attribute< std::string >( "id" )
                             , types.Instanciate( xis.attribute< std::string >( "type" ) )
                             , xis.attribute< std::string >( "value" ) )
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
