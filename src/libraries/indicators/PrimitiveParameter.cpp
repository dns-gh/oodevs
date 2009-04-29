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
#include "ElementType.h"
#include "Element_ABC.h"
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: PrimitiveParameter constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
PrimitiveParameter::PrimitiveParameter( xml::xistream& xis )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , attribute_( xml::attribute< std::string >( xis, "attribute", "input" ) )
    , type_( new ElementType( xis ) )
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
// Name: PrimitiveParameter::GetAttribute
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
std::string PrimitiveParameter::GetAttribute() const
{
    return attribute_;
}

// -----------------------------------------------------------------------------
// Name: PrimitiveParameter::GetName
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
QString PrimitiveParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PrimitiveParameter::GetType
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
const ElementType& PrimitiveParameter::GetType() const
{
    return *type_;
}
