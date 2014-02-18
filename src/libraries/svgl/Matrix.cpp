// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Matrix.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: Matrix constructor
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix::Matrix( float d /*= 0*/ )
    : a00_( d ), a01_( 0 ), a02_( 0 )
    , a10_( 0 ), a11_( d ), a12_( 0 )
    , z1_ ( 0 ), z2_ ( 0 ), z3_ ( 0 )
    , z4_ ( 0 ), z5_ ( 0 ), z6_ ( 0 )
    , z7_ ( 0 ), z8_ ( 0 )
    , o1_ ( 1 ), o2_ ( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Matrix constructor
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix::Matrix( float a, float b, float c, float d, float e, float f )
    : a00_( a ), a01_( c ), a02_( e )
    , a10_( b ), a11_( d ), a12_( f )
    , z1_ ( 0 ), z2_ ( 0 ), z3_ ( 0 )
    , z4_ ( 0 ), z5_ ( 0 ), z6_ ( 0 )
    , z7_ ( 0 ), z8_ ( 0 )
    , o1_ ( 1 ), o2_ ( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Matrix::CreateScale
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix Matrix::CreateScale( float sx, float sy )
{
    return Matrix( sx, 0, 0, sy, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: Matrix::CreateTranslate
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix Matrix::CreateTranslate( float tx, float ty )
{
    return Matrix( 1, 0, 0, 1, tx, ty );
}

// -----------------------------------------------------------------------------
// Name: Matrix::CreateRotation
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix Matrix::CreateRotation( float angle )
{
    const float cosa = std::cos( angle );
    const float sina = std::sin( angle );
    return Matrix( cosa, sina, -sina, cosa, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: Matrix::CreateSkewOnX
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix Matrix::CreateSkewOnX( float angle )
{
    return Matrix( 1, 0, std::tan(angle), 1, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: Matrix::CreateSkewOnY
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix Matrix::CreateSkewOnY( float angle )
{
    return Matrix( 1, std::tan(angle), 0, 1, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: Matrix destructor
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix::~Matrix()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Matrix::operator*
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix Matrix::operator*( const Matrix& r ) const
{
    Matrix result;
    result.a00_ = a00_*r.a00_ + a01_*r.a10_;
    result.a10_ = a10_*r.a00_ + a11_*r.a10_;
    result.a01_ = a00_*r.a01_ + a01_*r.a11_;
    result.a11_ = a10_*r.a01_ + a11_*r.a11_;
    result.a02_ = a00_*r.a02_ + a01_*r.a12_ + a02_;
    result.a12_ = a10_*r.a02_ + a11_*r.a12_ + a12_;
    return result;
}

// -----------------------------------------------------------------------------
// Name: Matrix::operator*=
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix& Matrix::operator*=( const Matrix& rhs )
{
    (*this) = (*this) * rhs;
    return *this;
}
    
// -----------------------------------------------------------------------------
// Name: Matrix::Multiply
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix& Matrix::Multiply( float a, float b, float c, float d, float e, float f )
{
    return (*this) *= Matrix( a, b, c, d, e, f );
}
    
// -----------------------------------------------------------------------------
// Name: Matrix::Scale
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix& Matrix::Scale( float sx, float sy )
{
    return (*this) *= CreateScale( sx, sy );
}

// -----------------------------------------------------------------------------
// Name: Matrix::Translate
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix& Matrix::Translate( float tx, float ty )
{
    return (*this) *= CreateTranslate( tx, ty );
}

// -----------------------------------------------------------------------------
// Name: Matrix::Rotate
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix& Matrix::Rotate( float angle )
{
    return (*this) *= CreateRotation( angle );
}

// -----------------------------------------------------------------------------
// Name: Matrix::SkewOnX
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix& Matrix::SkewOnX( float angle )
{   
    return (*this) *= CreateSkewOnX( angle );
}

// -----------------------------------------------------------------------------
// Name: Matrix::SkewOnY
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Matrix& Matrix::SkewOnY( float angle )
{
    return (*this) *= CreateSkewOnY( angle );
}
