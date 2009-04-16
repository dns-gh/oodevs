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
#include "IndicatorTypeResolver.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorType constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorType::IndicatorType( xml::xistream& xis )
    : type_( xml::attribute< std::string >( xis, "type" ) )
    , resolver_( boost::shared_ptr< IndicatorTypeResolver >( new IndicatorTypeResolver() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorType constructor
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
IndicatorType::IndicatorType( const std::string& type )
    : type_( type )
    , resolver_( boost::shared_ptr< IndicatorTypeResolver >( new IndicatorTypeResolver() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorType constructor
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
IndicatorType::IndicatorType( const IndicatorType& type, boost::shared_ptr< IndicatorTypeResolver > resolver )
    : type_( type.type_ )
    , resolver_( resolver )
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
// Name: IndicatorType::Resolve
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
std::string IndicatorType::Resolve() const
{
    return resolver_->Resolve( type_ );
}

// -----------------------------------------------------------------------------
// Name: IndicatorType::operator
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
bool IndicatorType::operator!=( const IndicatorType& rhs ) const
{
    return false; //ToString() != rhs.ToString(); // $$$$ SBO 2009-04-15: TODO, check type compatibility
}
