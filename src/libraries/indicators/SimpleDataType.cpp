// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "SimpleDataType.h"
#include "ElementTypeResolver.h"
#include <boost/algorithm/string.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: SimpleDataType constructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
SimpleDataType::SimpleDataType( const std::string& type, boost::shared_ptr< ElementTypeResolver > resolver )
    : type_( type )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleDataType destructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
SimpleDataType::~SimpleDataType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleDataType::Resolve
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
std::string SimpleDataType::Resolve() const
{
    return resolver_->Resolve( type_ );
}

// -----------------------------------------------------------------------------
// Name: SimpleDataType::AddParameter
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
void SimpleDataType::AddParameter( const DataType_ABC& definition, const DataType_ABC& instance )
{
    resolver_->AddElement( definition, instance );
}

// -----------------------------------------------------------------------------
// Name: SimpleDataType::ToString
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
std::string SimpleDataType::ToString() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: SimpleDataType::operator==
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
bool SimpleDataType::operator==( const DataType_ABC& rhs ) const
{
    return IsCompatible( rhs.ToString() ) || rhs.IsCompatible( type_ );
}

// -----------------------------------------------------------------------------
// Name: SimpleDataType::operator!=
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
bool SimpleDataType::operator!=( const DataType_ABC& rhs ) const
{
    return ! ( *this == rhs );
}

// -----------------------------------------------------------------------------
// Name: SimpleDataType::IsCompatible
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
bool SimpleDataType::IsCompatible( const std::string& type ) const
{
    return ElementTypeResolver::IsCompatible( type_, type );
}

// -----------------------------------------------------------------------------
// Name: SimpleDataType::IsCollection
// Created: SBO 2009-06-05
// -----------------------------------------------------------------------------
bool SimpleDataType::IsCollection() const
{
    return false;
}
