// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#pragma warning( disable : 4018 4389 4702 ) // $$$$ SBO 2009-05-19: signed/unsigned mismatch
#include "Types.h"

namespace
{
    class PlusVisitor : public boost::static_visitor< NumericValue >
    {
    public:
        template< typename T, typename U >
        NumericValue operator()( const T& t, const U& u ) const
        {
            return NumericValue( t + u );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Types::operator+
// Created: SBO 2009-05-19
// -----------------------------------------------------------------------------
NumericValue NumericValue::operator+( const NumericValue& rhs ) const
{
    PlusVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class MinusVisitor : public boost::static_visitor< NumericValue >
    {
    public:
        template< typename T, typename U >
        NumericValue operator()( const T& t, const U& u ) const
        {
            return NumericValue( t - u );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator-
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
NumericValue NumericValue::operator-( const NumericValue& rhs ) const
{
    MinusVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class MultiplyVisitor : public boost::static_visitor< NumericValue >
    {
    public:
        template< typename T, typename U >
        NumericValue operator()( const T& t, const U& u ) const
        {
            return NumericValue( t * u );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator*
// Created: JSR 2014-10-07
// -----------------------------------------------------------------------------
NumericValue NumericValue::operator*( const NumericValue& rhs ) const
{
    MultiplyVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class DivideVisitor : public boost::static_visitor< NumericValue >
    {
    public:
        template< typename T, typename U >
        NumericValue operator()( const T& t, const U& u ) const
        {
            return NumericValue( t / u );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator/
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
NumericValue NumericValue::operator/( const NumericValue& rhs ) const
{
    DivideVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class PlusEqualVisitor : public boost::static_visitor< NumericValue& >
    {
    public:
        explicit PlusEqualVisitor( NumericValue& value ) : value_( &value ) {}
        template< typename T, typename U >
        NumericValue& operator()( const T& t, const U& u )
        {
            *value_ = t + u;
            return *value_;
        }

        NumericValue* value_;
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator+=
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
NumericValue& NumericValue::operator+=( const NumericValue& rhs )
{
    PlusEqualVisitor visitor( *this );
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class LessThanVisitor : public boost::static_visitor< bool >
    {
    public:
        template< typename T, typename U >
        bool operator()( const T& t, const U& u ) const
        {
            return t < u;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator<
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
bool NumericValue::operator<( const NumericValue& rhs ) const
{
    LessThanVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class LessThanOrEqualVisitor : public boost::static_visitor< bool >
    {
    public:
        template< typename T, typename U >
        bool operator()( const T& t, const U& u ) const
        {
            return t <= u;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator<=
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
bool NumericValue::operator<=( const NumericValue& rhs ) const
{
    LessThanOrEqualVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class GreaterThanVisitor : public boost::static_visitor< bool >
    {
    public:
        template< typename T, typename U >
        bool operator()( const T& t, const U& u ) const
        {
            return t > u;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator>
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
bool NumericValue::operator>( const NumericValue& rhs ) const
{
    GreaterThanVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class GreaterThanOrEqualVisitor : public boost::static_visitor< bool >
    {
    public:
        template< typename T, typename U >
        bool operator()( const T& t, const U& u ) const
        {
            return t >= u;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator>=
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
bool NumericValue::operator>=( const NumericValue& rhs ) const
{
    GreaterThanOrEqualVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class EqualVisitor : public boost::static_visitor< bool >
    {
    public:
        template< typename T, typename U >
        bool operator()( const T& t, const U& u ) const
        {
            return t == u;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator==
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
bool NumericValue::operator==( const NumericValue& rhs ) const
{
    EqualVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class DifferentVisitor : public boost::static_visitor< bool >
    {
    public:
        template< typename T, typename U >
        bool operator()( const T& t, const U& u ) const
        {
            return t != u;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator!=
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
bool NumericValue::operator!=( const NumericValue& rhs ) const
{
    DifferentVisitor visitor;
    return boost::apply_visitor( visitor, *this, rhs );
}

namespace
{
    class DoubleCastVisitor : public boost::static_visitor< double >
    {
    public:
        template< typename T >
        double operator()( const T& t ) const
        {
            return static_cast< double >( t );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: NumericValue::operator double
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
NumericValue::operator double() const
{
    DoubleCastVisitor visitor;
    return boost::apply_visitor( visitor, *this );
}

namespace
{
    class DeserializeVisitor : public boost::static_visitor< std::istream& >
    {
    public:
        explicit DeserializeVisitor( std::istream& stream ) : stream_( &stream ) {}
        template< typename T >
        std::istream& operator()( T& t )
        {
            *stream_ >> t;
            return *stream_;
        }
        std::istream* stream_;
    };
}

// -----------------------------------------------------------------------------
// Name: operator>>
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
std::istream& operator>>( std::istream& is, NumericValue& rhs )
{
    DeserializeVisitor visitor( is );
    return boost::apply_visitor( visitor, rhs );
}

namespace
{
    class SerializeVisitor : public boost::static_visitor< std::ostream& >
    {
    public:
        explicit SerializeVisitor( std::ostream& stream ) : stream_( &stream ) {}
        template< typename T >
        std::ostream& operator()( const T& t )
        {
            *stream_ << t;
            return *stream_;
        }
        std::ostream* stream_;
    };
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: SBO 2009-05-15
// -----------------------------------------------------------------------------
std::ostream& operator<<( std::ostream& os, const NumericValue& rhs )
{
    SerializeVisitor visitor( os );
    return boost::apply_visitor( visitor, rhs );
}
