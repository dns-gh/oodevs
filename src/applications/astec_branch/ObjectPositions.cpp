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
    return center_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::DoUpdate
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectPositions::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
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
    for( uint i = 0; i < localisation.vecteur_point.n; ++i )
    {
        const geometry::Point2f p = converter_.ConvertToXY( localisation.vecteur_point.elem[i] );
        points_.push_back( p );
        center_ += geometry::Vector2f( p.X(), p.Y() );
    }

    if( localisation.vecteur_point.n )
        center_.Set( center_.X() / localisation.vecteur_point.n, center_.Y() / localisation.vecteur_point.n );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Draw
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectPositions::Draw( const geometry::Point2f&, const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 2.f );
    tools.DrawLines( points_ );
    glPopAttrib();
}
