// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TypeConstraints_h_
#define __TypeConstraints_h_

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <stdexcept>
#include "Types.h"

#pragma warning( push )
#pragma warning( disable : 4702 )

namespace types
{
class bad_type_usage : public std::invalid_argument
{
public:
    bad_type_usage( const std::string& message )
        : std::invalid_argument( "Invalid type usage : " + message )
    {}
};

inline
void check( const boost::true_type&, const char* )
{
    // NOTHING
}

inline
void check( const boost::false_type&, const std::string& message )
{
    throw bad_type_usage( message );
}

template< typename T >
void check( const char* message )
{
    check( T::type(), message );
}

template< typename T >
struct Arithmetic
{
    Arithmetic()
    {
        check< boost::is_arithmetic< T > >( "Type must be arithmetic" );
    }
};

template<>
struct Arithmetic< NumericValue >
{
    Arithmetic() {}
};

}

#pragma warning( pop )

#endif // __TypeConstraints_h_
