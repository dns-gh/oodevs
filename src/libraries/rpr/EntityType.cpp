// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "EntityType.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <vector>

using namespace rpr;

// -----------------------------------------------------------------------------
// Name: EntityType constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
EntityType::EntityType()
    : entityKind_ ( 0 )
    , domain_     ( 0 )
    , countryCode_( 0 )
    , category_   ( 0 )
    , subcategory_( 0 )
    , specific_   ( 0 )
    , extra_      ( 0 )
{
    // NOTHING
}

namespace
{
    template< typename T >
    void Assign( const std::string& s, T& v )
    {
        v = static_cast< T >( boost::lexical_cast< int, std::string >( s ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityType constructor
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
EntityType::EntityType( const std::string& value )
    : entityKind_ ( 0 )
    , domain_     ( 0 )
    , countryCode_( 0 )
    , category_   ( 0 )
    , subcategory_( 0 )
    , specific_   ( 0 )
    , extra_      ( 0 )
{
    if( ! value.empty() )
    {
        std::vector< std::string > split;
        boost::algorithm::split( split, value, boost::algorithm::is_any_of( " " ) );
        // Impressive for/switch pattern
        for( unsigned i = 0; i < split.size(); ++i )
        {
            switch( i )
            {
            case 0: Assign( split[i], entityKind_  ); break;
            case 1: Assign( split[i], domain_      ); break;
            case 2: Assign( split[i], countryCode_ ); break;
            case 3: Assign( split[i], category_    ); break;
            case 4: Assign( split[i], subcategory_ ); break;
            case 5: Assign( split[i], specific_    ); break;
            case 6: Assign( split[i], extra_       ); break;
            }
        }
    }
}


// -----------------------------------------------------------------------------
// Name: EntityType destructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
EntityType::~EntityType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityType::operator==
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
bool EntityType::operator==( const EntityType& rhs ) const
{
    return entityKind_  == rhs.entityKind_
        && domain_      == rhs.domain_
        && countryCode_ == rhs.countryCode_
        && category_    == rhs.category_
        && subcategory_ == rhs.subcategory_
        && specific_    == rhs.specific_
        && extra_       == rhs.extra_;
}

// -----------------------------------------------------------------------------
// Name: EntityType::operator<
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
bool EntityType::operator<=( const EntityType& rhs ) const
{
    return str() <= rhs.str();
}

// -----------------------------------------------------------------------------
// Name: EntityType::operator<
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
bool EntityType::operator<( const EntityType& rhs ) const
{
    return str() < rhs.str();
}

// -----------------------------------------------------------------------------
// Name: EntityType::str
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
std::string EntityType::str() const
{
    std::stringstream ss;
    ss << static_cast< unsigned short >( entityKind_ )  << " "
       << static_cast< unsigned short >( domain_ )      << " "
       << countryCode_                                  << " "
       << static_cast< unsigned short >( category_ )    << " "
       << static_cast< unsigned short >( subcategory_ ) << " "
       << static_cast< unsigned short >( specific_ )    << " "
       << static_cast< unsigned short >( extra_ );
    return ss.str();
}
