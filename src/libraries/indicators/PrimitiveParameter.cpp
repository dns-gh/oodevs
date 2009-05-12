// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "PrimitiveParameter.h"
#include "DataTypeFactory.h"
#include "Function.h"
#include "FunctionParameter.h"
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: PrimitiveParameter constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
PrimitiveParameter::PrimitiveParameter( xml::xistream& xis, const DataTypeFactory& types )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , attribute_( xml::attribute< std::string >( xis, "attribute", "input" ) )
    , type_( xml::attribute< std::string >( xis, "type" ) )
    , types_( types )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PrimitiveParameter destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
PrimitiveParameter::~PrimitiveParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PrimitiveParameter::GetName
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
QString PrimitiveParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PrimitiveParameter::Declare
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
void PrimitiveParameter::Declare( Function& function, boost::shared_ptr< ElementTypeResolver > resolver ) const
{
    function.DeclareParameter( attribute_, Instanciate( resolver ) );
}

// -----------------------------------------------------------------------------
// Name: PrimitiveParameter::Instanciate
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > PrimitiveParameter::Instanciate( boost::shared_ptr< ElementTypeResolver > resolver ) const
{
    boost::shared_ptr< Element_ABC > element( new FunctionParameter( name_, attribute_, types_.Instanciate( type_, resolver ) ) );
    return element;
}
