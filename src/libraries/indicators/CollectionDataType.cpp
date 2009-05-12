// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "CollectionDataType.h"
#include "ElementTypeResolver.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace indicators;

namespace
{
    std::vector< std::string > Split( const std::string& type )
    {
        std::string str = boost::replace_first_copy( type, "list(", "" );
        boost::replace_last( str, ")", "" );
        std::vector< std::string > result;
        return boost::split( result, str, boost::algorithm::is_any_of( "," ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CollectionDataType constructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
CollectionDataType::CollectionDataType( const std::string& type, boost::shared_ptr< ElementTypeResolver > resolver )
    : fullType_( type )
    , types_( Split( type ) )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CollectionDataType destructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
CollectionDataType::~CollectionDataType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CollectionDataType::Resolve
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
std::string CollectionDataType::Resolve() const
{
    return resolver_->Resolve( fullType_ );
}

// -----------------------------------------------------------------------------
// Name: CollectionDataType::AddParameter
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
void CollectionDataType::AddParameter( const DataType_ABC& definition, const DataType_ABC& instance )
{
    resolver_->AddElement( definition, instance );
}

// -----------------------------------------------------------------------------
// Name: CollectionDataType::ToString
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
std::string CollectionDataType::ToString() const
{
    return fullType_;
}

// -----------------------------------------------------------------------------
// Name: CollectionDataType::operator==
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
bool CollectionDataType::operator==( const DataType_ABC& rhs ) const
{
    return IsCompatible( rhs.ToString() ) || rhs.IsCompatible( fullType_ ) || rhs.IsCompatible( types_.back() );
}

// -----------------------------------------------------------------------------
// Name: CollectionDataType::operator!=
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
bool CollectionDataType::operator!=( const DataType_ABC& rhs ) const
{
    return ! ( *this == rhs );
}

// -----------------------------------------------------------------------------
// Name: CollectionDataType::IsCompatible
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
bool CollectionDataType::IsCompatible( const std::string& type ) const
{
    return ElementTypeResolver::IsCompatible( fullType_, type ) || ElementTypeResolver::IsCompatible( types_.back(), type );
}
