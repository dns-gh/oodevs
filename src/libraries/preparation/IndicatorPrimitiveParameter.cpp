// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorPrimitiveParameter.h"
#include "IndicatorType.h"
#include "IndicatorElement_ABC.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitiveParameter constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorPrimitiveParameter::IndicatorPrimitiveParameter( xml::xistream& xis )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , attribute_( xml::attribute< std::string >( xis, "attribute", "input" ) )
    , type_( new IndicatorType( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitiveParameter destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorPrimitiveParameter::~IndicatorPrimitiveParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitiveParameter::GetAttribute
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
std::string IndicatorPrimitiveParameter::GetAttribute() const
{
    return attribute_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitiveParameter::GetType
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
const IndicatorType& IndicatorPrimitiveParameter::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitiveParameter::Serialize
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
void IndicatorPrimitiveParameter::Serialize( xml::xostream& xos, const IndicatorElement_ABC& element ) const
{
    xos << xml::attribute( attribute_, element.GetId() );
}
