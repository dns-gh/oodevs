// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorActionParameter.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionParameter constructor
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
SuccessFactorActionParameter::SuccessFactorActionParameter( const QString& name, const QString& value )
    : name_( name )
    , value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionParameter constructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionParameter::SuccessFactorActionParameter( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name" ).c_str() )
    , value_( xis.attribute< std::string >( "value" ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionParameter destructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionParameter::~SuccessFactorActionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionParameter::GetName
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
QString SuccessFactorActionParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionParameter::GetValue
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
QString SuccessFactorActionParameter::GetValue() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionParameter::Serialize
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionParameter::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "parameter" )
            << xml::attribute( "name", name_.toStdString() )
            << xml::attribute( "value", value_.toStdString() )
        << xml::end;
}
