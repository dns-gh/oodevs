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

// -----------------------------------------------------------------------------
// Name: IndicatorVariable constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorVariable::IndicatorVariable( const std::string& name, const std::string& type )
    : IndicatorElement_ABC( std::string( "$" ) + name )
    , type_( new IndicatorType( type ) )
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
// Name: IndicatorVariable::GetType
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
const IndicatorType& IndicatorVariable::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorVariable::AddParameter
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
void IndicatorVariable::AddParameter( boost::shared_ptr< IndicatorElement_ABC > element )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
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
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
void IndicatorVariable::SerializeDeclaration( xml::xostream& ) const
{
    // NOTHING
}
