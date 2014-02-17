// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "DashArray.h"
#include "RenderingContext_ABC.h"
#include <iterator>
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: DashArray constructor
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
DashArray::DashArray( const std::string& array )
    : disable_  ( array == "none" )
    , pixels_   ( false )
    , pattern_  ( 0 )
    , bitLength_( 0 )
{
    if( ! disable_ )
        Parse( array );
}

// -----------------------------------------------------------------------------
// Name: DashArray destructor
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
DashArray::~DashArray()
{
    // NOTHING
}

namespace
{

    // -----------------------------------------------------------------------------
    // Name: std::vector< float > Repeat
    // Created: AGE 2006-09-04
    // -----------------------------------------------------------------------------
    std::vector< float > Repeat( const std::vector< float >& values )
    {
        std::vector< float > result( values );
        std::copy( values.begin(), values.end(), std::back_inserter( result ) );
        return result;
    }

    // -----------------------------------------------------------------------------
    // Name: Accumulate
    // Created: AGE 2006-09-04
    // -----------------------------------------------------------------------------
    void Accumulate( std::vector< float >& values )
    {
        float last = 0;
        for( std::vector< float >::iterator it = values.begin(); it != values.end(); ++it )
            last = *it += last;
    }

}

// -----------------------------------------------------------------------------
// Name: DashArray::Parse
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DashArray::Parse( const std::string& array )
{
    std::vector< float > values;
    std::string value;
    std::stringstream input( array );
    while( std::getline( input, value, ',' ) )
    {
        std::stringstream str( value );
        float r;
        str >> r;
        values.push_back( r );
    }
    if( values.empty() )
    { 
        disable_ = true;
        return;
    }
    pixels_ = array.find( "px" ) != std::string::npos;
    if( ( values.size() % 2 ) != 0 )
        values = Repeat( values );
    Accumulate( values );
    bitLength_ = values.back() / 16.f;
    GenerateMask( values );
}


namespace
{
    // -----------------------------------------------------------------------------
    // Name: Find
    // Created: AGE 2006-09-04
    // -----------------------------------------------------------------------------
    int Find( const std::vector< float >& values, float value )
    {
        for( std::vector< float >::const_iterator it = values.begin(); it != values.end(); ++it )
            if( *it > value )
                return static_cast< int >( it - values.begin() );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: DashArray::GenerateMask
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DashArray::GenerateMask( const std::vector< float >& values )
{
    for( unsigned i = 0; i < 16; ++i )
    {
        float length = ( i + 0.5f ) * bitLength_;
        bool set = ( Find( values, length ) % 2 ) == 0;
        if( set )
            SetBit( i );
    }
}

// -----------------------------------------------------------------------------
// Name: DashArray::SetBit
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DashArray::SetBit( unsigned index )
{
    unsigned short mask = unsigned short( 1 << index );
    pattern_ |= mask;
}

// -----------------------------------------------------------------------------
// Name: DashArray::Clone
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
DashArray& DashArray::Clone() const
{
    return *new DashArray( *this );
}

namespace
{
    unsigned short RefinePattern( float factor, unsigned short base )
    {
        unsigned short result = 0;
        for( unsigned i = 0; i < 16; ++i )
        {
            unsigned offseted = unsigned( float( i ) / factor ) % 16;
            if( base & ( 1 << offseted ) )
                result |= ( 1 << i );
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: DashArray::ReachTop
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DashArray::ReachTop( const RenderingContext_ABC& context )
{
    if( disable_ )
        glDisable( GL_LINE_STIPPLE );
    else
    {
        float pixels = context.Pixels();
        float factor = pixels_ ? bitLength_ : bitLength_ / pixels;
        unsigned short pattern = pattern_;
        if( factor < 1 )
        {
            pattern = RefinePattern( factor, pattern );
            factor = 1;
        }
        if( factor > 255 ) factor = 255;
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( int( factor ), pattern );
    }
}
