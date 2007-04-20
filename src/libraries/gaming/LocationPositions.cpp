// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LocationPositions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace kernel;

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
// Name: LocationPositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f LocationPositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Draw
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void LocationPositions::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! viewport.IsVisible( boundingBox_ ) || points_.empty() )
        return;

    const bool selected = tools.Select( false );
    tools.Select( selected );
    glPushAttrib( GL_LINE_BIT );
    if( selected )
    {
        glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 0, 0, 0, 0.5f );
        glLineWidth( 6.f );
        Draw( tools );
        glPopAttrib();
    }
    glLineWidth( 2.f );
    Draw( tools );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Draw
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void LocationPositions::Draw( const kernel::GlTools_ABC& tools ) const
{
    if( points_.size() >= 2 )
        tools.DrawLines( points_ );
    else
        tools.DrawCross( points_.front(), GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Update
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void LocationPositions::Update( const ASN1T_Localisation& asn )
{
    points_.clear(); points_.reserve( asn.vecteur_point.n );
    center_ = geometry::Point2f( 0, 0 );
    boundingBox_.Set( 0, 0, 0, 0 );
    AddLocation( asn );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Update
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
void LocationPositions::Update( const ASN1T_Localisation& asn, const geometry::Point2f& startPoint )
{
    points_.clear(); points_.reserve( asn.vecteur_point.n + 1 );
    center_ = geometry::Point2f( 0, 0 );
    boundingBox_.Set( 0, 0, 0, 0 );
    AddPoint( startPoint );
    AddLocation( asn );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::AddLocation
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
void LocationPositions::AddLocation( const ASN1T_Localisation& asn )
{
    for( uint i = 0; i < asn.vecteur_point.n; ++i )
        AddPoint( converter_.ConvertToXY( asn.vecteur_point.elem[i] ) );
    if( !points_.empty() )
        center_.Set( center_.X() / points_.size(), center_.Y() / points_.size() );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::AddPoint
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
void LocationPositions::AddPoint( const geometry::Point2f& point )
{
    points_.push_back( point );
    center_ += geometry::Vector2f( point.X(), point.Y() );
    boundingBox_.Incorporate( point );
}
