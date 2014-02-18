// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Ellipse.h"
#include "RenderingContext_ABC.h"
#include "Paint_ABC.h"
#include "Length.h"
#include "DashArray.h"
#include "CompiledEllipse.h"
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Ellipse constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Ellipse::Ellipse( xml::xistream& input, References_ABC& references, const PropertyFactory& factory )
    : Node( input, references, factory )
{
    input >> xml::attribute( "cx", center_.X() ) >> xml::attribute( "cy", center_.Y() )
          >> xml::attribute( "rx", rx_ )
          >> xml::attribute( "ry", ry_ );
    length_ = 2.f * std::acos( -1.f ) * std::sqrt( ( rx_ * rx_ + ry_ * ry_ ) * 0.5f );
}

// -----------------------------------------------------------------------------
// Name: Ellipse constructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Ellipse::Ellipse( xml::xistream& input, References_ABC& references, const PropertyFactory& factory, const geometry::Point& center, float radius )
    : Node( input, references, factory )
    , center_( center )
    , rx_( radius )
    , ry_( radius )
{
    length_ = 2.f * std::acos( -1.f ) * radius;
}

// -----------------------------------------------------------------------------
// Name: Ellipse constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Ellipse::Ellipse( const Ellipse& rhs )
    : Node( rhs )
    , center_( rhs.center_ )
    , rx_    ( rhs.rx_ )
    , ry_    ( rhs.ry_ )
    , length_( rhs.length_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Ellipse destructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Ellipse::~Ellipse()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Ellipse::GetSegmentNumber
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
unsigned Ellipse::GetSegmentNumber( RenderingContext_ABC& context ) const
{
    unsigned nSegments = unsigned( length_ / context.ExpectedPrecision() );
    if( nSegments > 100 )
        nSegments = 100;
    if( nSegments < 4 )
        nSegments = 4;
    return nSegments;
}

// -----------------------------------------------------------------------------
// Name: Ellipse::CompileInternal
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Node& Ellipse::CompileInternal( RenderingContext_ABC& context, References_ABC& ) const
{
    return *new CompiledEllipse( *this, center_, rx_, ry_, GetSegmentNumber( context ) );
}

// -----------------------------------------------------------------------------
// Name: Ellipse::DrawInternal
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void Ellipse::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    const unsigned nSegments = GetSegmentNumber( context );
    const float increment = 2.f * std::acos( -1.f ) / float( nSegments );

    if( context.SetupFill( references ) )
        DrawInternal( GL_POLYGON, nSegments, increment );
    if( context.SetupStroke( references ) )
        DrawInternal( GL_LINE_LOOP, nSegments, increment );
}

// -----------------------------------------------------------------------------
// Name: Ellipse::DrawInternal
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
void Ellipse::DrawInternal( int type, unsigned nSegments, float increment ) const
{
    glBegin( type );
    float angle = 0;
    for( unsigned i = 0; i < nSegments; ++i )
    {
        geometry::Point p( center_.X() + std::cos( angle ) * rx_, 
                             center_.Y() + std::sin( angle ) * ry_ );
        glVertex2fv( (const float*)&p );
        angle += increment;
    }
    glEnd();
}
