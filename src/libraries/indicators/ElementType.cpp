// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "ElementType.h"
#include "ElementTypeResolver.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: ElementType constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
ElementType::ElementType( xml::xistream& xis )
    : type_( xml::attribute< std::string >( xis, "type" ) )
    , resolver_( boost::shared_ptr< ElementTypeResolver >( new ElementTypeResolver() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElementType constructor
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
ElementType::ElementType( const std::string& type )
    : type_( type )
    , resolver_( boost::shared_ptr< ElementTypeResolver >( new ElementTypeResolver() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElementType constructor
// Created: SBO 2009-04-10
// -----------------------------------------------------------------------------
ElementType::ElementType( const ElementType& type, boost::shared_ptr< ElementTypeResolver > resolver )
    : type_( type.type_ )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElementType destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
ElementType::~ElementType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElementType::ToString
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
std::string ElementType::ToString() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: ElementType::Resolve
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
std::string ElementType::Resolve() const
{
    return resolver_->Resolve( type_ );
}

// -----------------------------------------------------------------------------
// Name: ElementType::operator
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
bool ElementType::operator!=( const ElementType& rhs ) const
{
    return false; //ToString() != rhs.ToString(); // $$$$ SBO 2009-04-15: TODO, check type compatibility
}
