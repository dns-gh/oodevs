// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LocationPositions.h"
#include "CoordinateConverter_ABC.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: LocationPositions constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
LocationPositions::LocationPositions( const CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationPositions destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
LocationPositions::~LocationPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::GetPosition
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
geometry::Point2f LocationPositions::GetPosition() const
{
    if( ! points_.empty() )
        return points_.front();
    return geometry::Point2f();
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::GetHeight
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
float LocationPositions::GetHeight() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::IsAt
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
bool LocationPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    precision*=precision;
    if( points_.empty() )
        return false;
    if( points_.size() == 1 )
        return points_.front().SquareDistance( pos ) <= precision;

    CIT_PointVector previous = points_.begin();
    for( CIT_PointVector current = previous + 1; current != points_.end(); ++current )
    {
        const geometry::Segment2f segment( *previous, *current );
        if( segment.SquareDistance( pos ) < precision )
            return true;
        previous = current;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::IsIn
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
bool LocationPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return ! boundingBox_.Intersect( rectangle ).IsEmpty();
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Draw
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void LocationPositions::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.Intersect( boundingBox_ ).IsEmpty() || points_.empty() )
        return;

    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 2.f );
    if( points_.size() >= 2 )
        tools.DrawLines( points_ );
    else
        tools.DrawCross( points_.front(), GL_CROSSSIZE );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Update
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void LocationPositions::Update( const ASN1T_Localisation& localisation )
{
    points_.clear(); points_.reserve( localisation.vecteur_point.n );
    center_ = geometry::Point2f( 0, 0 );
    boundingBox_.Set( 0, 0, 0, 0 );
    for( uint i = 0; i < localisation.vecteur_point.n; ++i )
    {
        const geometry::Point2f p = converter_.ConvertToXY( localisation.vecteur_point.elem[i] );
        points_.push_back( p );
        center_ += geometry::Vector2f( p.X(), p.Y() );
        boundingBox_.Incorporate( p );
    }

    if( localisation.vecteur_point.n )
        center_.Set( center_.X() / localisation.vecteur_point.n, center_.Y() / localisation.vecteur_point.n );
}