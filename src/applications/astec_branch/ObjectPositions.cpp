// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectPositions.h"
#include "CoordinateConverter.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( const CoordinateConverter& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions destructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::~ObjectPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetPosition
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
geometry::Point2f ObjectPositions::GetPosition() const
{
    if( ! points_.empty() )
        return points_.front();
    return geometry::Point2f();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetHeight
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float ObjectPositions::GetHeight() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::DoUpdate
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectPositions::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    if( message.m.localisationPresent )
        Update( message.localisation );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::DoUpdate
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectPositions::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    Update( message.localisation );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Update
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectPositions::Update( const ASN1T_Localisation& localisation )
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

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Draw
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectPositions::Draw( const geometry::Point2f&, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
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
// Name: ObjectPositions::IsAt
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ObjectPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
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
// Name: ObjectPositions::IsIn
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ObjectPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return ! boundingBox_.Intersect( rectangle ).IsEmpty();
}
