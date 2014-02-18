// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Transformations.h"
#include <sstream>
#include <limits>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Transformations constructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Transformations::Transformations( xml::xistream& input )
    : matrix_( 1 )
    , isIdentity_( true )
{
    std::string transforms;
    input >> xml::optional() >> xml::attribute( "transform", transforms );
    while( Transform( transforms ) )
        isIdentity_ = false;
}

// -----------------------------------------------------------------------------
// Name: Transformations constructor
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
Transformations::Transformations()
    : isIdentity_( false )
    , matrix_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transformations destructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Transformations::~Transformations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transformations::Translation
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
Transformations* Transformations::Translation( float x, float y )
{   
    Transformations* result = new Transformations();
    result->matrix_.Translate( x, y );
    return result;
}

namespace
{
    std::string RemoveCrap( const std::string& from )
    {
        std::string result( from );
        for( unsigned i = 0; i < from.size(); ++i )
        {
            char& c = result[i];
            if( c != '.' && c != '-' && ( c < '0' || c > '9' ) ) // $$$$ AGE 2006-08-16: units, ...
                c = ' ';
        }
        return result;
    };
}

// -----------------------------------------------------------------------------
// Name: Transformations::Transform
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
bool Transformations::Transform( std::string& data )
{
    std::string::size_type first = data.find_first_of( '(' );
    std::string::size_type last  = data.find_first_of( ')' );
    if( first != std::string::npos && first > 0 
     && last  != std::string::npos && last  > first )
    {
        std::string::size_type p = data.find_first_not_of( ' ' );
        if( p == std::string::npos || p > first )
            p = 0;
        const std::string transform  = data.substr( p, first - p );
        const std::string parameters = RemoveCrap( data.substr( first + 1, last - first - 1 ) );
        data = data.substr( last + 1 );
        if( transform == "matrix" )
            return Multiply( parameters );
        if( transform == "translate" )
            return Translate( parameters );
        if( transform == "scale" )
            return Scale( parameters );
        if( transform == "rotate" )
            return Rotate( parameters );
        if( transform == "skewX" )
            return SkewOnX( parameters );    
        if( transform == "skewY" )
            return SkewOnY( parameters );    
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Transformations::Apply
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
void Transformations::Apply() const
{
    if( ! isIdentity_ )
    {
        glPushMatrix();
        glMultMatrixf( matrix_ );
    }
}

// -----------------------------------------------------------------------------
// Name: Transformations::Undo
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
void Transformations::Undo() const
{
    if( ! isIdentity_ )
        glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: Transformations::Multiply
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
bool Transformations::Multiply( const std::string& parameters )
{
    std::stringstream str( parameters );
    float a, b, c, d, e, f;
    str >> a >> b >> c >> d >> e >> f;
    matrix_.Multiply( a, b, c, d, e, f );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Transformations::Translate
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
bool Transformations::Translate( const std::string& parameters )
{
    std::stringstream str( parameters );
    float tx = 0, ty = 0;
    str >> tx >> ty;
    matrix_.Translate( tx, ty );
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: Transformations::Scale
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
bool Transformations::Scale( const std::string& parameters )
{
    std::stringstream str( parameters );
    float tx = 0, ty = 0;
    str >> tx; 
    ty = tx; 
    str >> ty;
    matrix_.Scale( tx, ty );
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: Transformations::Rotate
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
bool Transformations::Rotate( const std::string& parameters )
{
    std::stringstream str( parameters );
    const float notSet = std::numeric_limits< float >::infinity();
    float angle, cx = notSet, cy = notSet;
    str >> angle >> cx >> cy;
    if( cx != notSet && cy != notSet )
        matrix_.Translate(  cx,  cy );
    matrix_.Rotate( angle * std::acos( -1.f ) / 180.f );
    if( cx != notSet && cy != notSet )
        matrix_.Translate( -cx, -cy );
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: Transformations::SkewOnX
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
bool Transformations::SkewOnX( const std::string& parameters )
{
    std::stringstream str( parameters );
    float angle;
    str >> angle;
    matrix_.SkewOnX( angle * std::acos( -1.f ) / 180.f );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Transformations::SkewOnY
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
bool Transformations::SkewOnY( const std::string& parameters )
{
    std::stringstream str( parameters );
    float angle;
    str >> angle;
    matrix_.SkewOnY( angle * std::acos( -1.f ) / 180.f );
    return true;
}   
