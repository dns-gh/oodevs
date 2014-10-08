// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Types_h_
#define __Types_h_

#pragma warning( push, 0 )
#include <boost/variant.hpp>
#pragma warning( pop )
#include <iosfwd>

typedef boost::variant< int, unsigned int, long, unsigned long, float, double > NumericVariant;

class NumericValue : public NumericVariant
{
public:
    NumericValue() : NumericVariant()
    {
        // NOTHING
    }
    template< typename T >
    NumericValue( const T& t ) : NumericVariant( t )
    {
        // NOTHING
    }
    virtual ~NumericValue()
    {
        // NOTHING
    }

    // MSVC++ bug fix: https://svn.boost.org/trac/boost/ticket/592
    //@{
    NumericValue( const NumericValue& other ) : NumericVariant( static_cast< const NumericVariant& >( other ) )
    {
        // NOTHING
    }
    NumericValue& operator=( const NumericValue& other )
    {
        *static_cast< NumericVariant* >( this ) = static_cast< const NumericVariant& >( other );
        return *this;
    }
    //@}

    NumericValue operator+( const NumericValue& rhs ) const;
    NumericValue operator-( const NumericValue& rhs ) const;
    NumericValue operator*( const NumericValue& rhs ) const;
    NumericValue operator/( const NumericValue& rhs ) const;
    NumericValue& operator+=( const NumericValue& rhs );
    bool operator<( const NumericValue& rhs ) const;
    bool operator<=( const NumericValue& rhs ) const;
    bool operator>( const NumericValue& rhs ) const;
    bool operator>=( const NumericValue& rhs ) const;
    bool operator==( const NumericValue& rhs ) const;
    bool operator!=( const NumericValue& rhs ) const;
    operator double() const;

    friend std::istream& operator>>( std::istream& os, NumericValue& rhs );
    friend std::ostream& operator<<( std::ostream& os, const NumericValue& rhs );
};

#endif // __Types_h_
