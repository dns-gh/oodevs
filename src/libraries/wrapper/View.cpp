// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "View.h"
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

using namespace sword::wrapper;

// -----------------------------------------------------------------------------
// Name: View constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
View::View()
    : model_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: View constructor
// Created: VPR 2012-01-24
// -----------------------------------------------------------------------------
View::View( const SWORD_Model* model )
    : model_( model )
{
    if( ! model )
        throw std::invalid_argument( "cannot create a view from a null model" );
}

// -----------------------------------------------------------------------------
// Name: View::operator bool()
// Created: BAX 2012-01-27
// -----------------------------------------------------------------------------
View::operator bool() const
{
    int result;
    if( ! ::SWORD_GetBool( model_, &result ) )
        throw std::runtime_error( "unable to get bool" );
    return !!result;
}

// -----------------------------------------------------------------------------
// Name: View::operator <TYPE>() const
// Created: BAX 2012-01-25
// -----------------------------------------------------------------------------
#define DECLARE_GET( TYPE_NAME, TYPE ) \
View::operator TYPE() const \
{ \
    TYPE result; \
    if( ! ::SWORD_Get##TYPE_NAME ( model_, &result ) ) \
        throw std::runtime_error( "unable to get " #TYPE ); \
    return result; \
}
DECLARE_GET( Int8,   int8_t );
DECLARE_GET( Int16,  int16_t );
DECLARE_GET( Int32,  int32_t );
DECLARE_GET( Int64,  int64_t );
DECLARE_GET( Uint8,  uint8_t );
DECLARE_GET( Uint16, uint16_t );
DECLARE_GET( Uint32, uint32_t );
DECLARE_GET( Uint64, uint64_t );
DECLARE_GET( Float,  float );
DECLARE_GET( Double, double );
#undef DECLARE_GET

// -----------------------------------------------------------------------------
// Name: View::operator std::string
// Created: VPR 2012-02-13
// -----------------------------------------------------------------------------
View::operator std::string() const
{
    const char* result;
    size_t size;
    if( ! ::SWORD_GetText( model_, &result, &size ) )
        throw std::runtime_error( "unable to get text" );
    return std::string( result, size );
}

// -----------------------------------------------------------------------------
// Name: View::GetUserData
// Created: SLI 2012-01-02
// -----------------------------------------------------------------------------
void* View::GetUserData() const
{
    void* result = 0;
    if( ! ::SWORD_GetUserData( model_, &result ) )
        throw std::runtime_error( "could not retrieve user data" );
    return result;
}

// -----------------------------------------------------------------------------
// Name: View::operator[]
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
View View::operator[]( const std::string& key ) const
{
    if( key.empty() )
        return model_;
    const SWORD_Model* child = ::SWORD_GetNamedChild( model_, key.c_str() );
    if( !child )
        throw std::runtime_error( "could not retrieve child '" + key + "'" );
    return child;
}

// -----------------------------------------------------------------------------
// Name: View::operator[]
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
View View::operator[]( const char* key ) const
{
    if( ! key )
        return model_;
    const SWORD_Model* child = ::SWORD_GetNamedChild( model_, key );
    if( !child )
        throw std::runtime_error( "could not retrieve child '" + std::string( key ) + "'" );
    return child;
}

// -----------------------------------------------------------------------------
// Name: View::operator[]
// Created: SLI 2012-01-19
// -----------------------------------------------------------------------------
View View::operator[]( std::size_t key ) const
{
    const SWORD_Model* child = ::SWORD_GetIdentifiedChild( model_, boost::numeric_cast< unsigned int >( key ) );
    if( !child )
        throw std::runtime_error( "could not retrieve child '" + boost::lexical_cast< std::string >( key ) + "'" );
    return child;
}

// -----------------------------------------------------------------------------
// Name: View::GetSize
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
std::size_t View::GetSize() const
{
    size_t size;
    if( ! ::SWORD_GetSize( model_, &size ) )
        throw std::runtime_error( "could not retrieve size" );
    return size;
}

// -----------------------------------------------------------------------------
// Name: View::GetElement
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
View View::GetElement( std::size_t index ) const
{
    const SWORD_Model* element = ::SWORD_GetElement( model_, index );
    if( !element )
        throw std::runtime_error( "could not retrieve element '" + boost::lexical_cast< std::string >( index ) + "'" );
    return element;
}

// -----------------------------------------------------------------------------
// Name: View::operator const SWORD_Model*
// Created: MCO 2012-06-14
// -----------------------------------------------------------------------------
View::operator const SWORD_Model*() const
{
    return model_;
}

// -----------------------------------------------------------------------------
// Name: View::operator==
// Created: MCO 2012-06-14
// -----------------------------------------------------------------------------
bool View::operator==( const View& rhs ) const
{
    return model_ == rhs.model_;
}

// -----------------------------------------------------------------------------
// Name: View::operator!=
// Created: MCO 2012-06-14
// -----------------------------------------------------------------------------
bool View::operator!=( const View& rhs ) const
{
    return model_ != rhs.model_;
}
