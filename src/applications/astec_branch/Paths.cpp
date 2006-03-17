// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Paths.h"
#include "CoordinateConverter.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: Paths constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Paths::Paths( const CoordinateConverter& converter )
    : converter_( converter ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Paths destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Paths::~Paths()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Paths::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Paths::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    // $$$$ AGE 2006-02-13: Jamais vidé ??
    if( message.m.positionPresent )
        previousPath_.push_back( converter_.ConvertToXY( message.position ) );
}   

// -----------------------------------------------------------------------------
// Name: Paths::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Paths::DoUpdate( const ASN1T_MsgUnitPathFind& message )
{
    plannedPath_.clear(); plannedPath_.reserve( message.itineraire.vecteur_point.n );
    for( uint i = 0; i < message.itineraire.vecteur_point.n; ++i )
        plannedPath_.push_back( converter_.ConvertToXY( message.itineraire.vecteur_point.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: Paths::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void Paths::Draw( const geometry::Point2f& /*where*/, const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );

    glColor4d( COLOR_PATH );
    glLineWidth( 3 );
    glEnable( GL_LINE_STIPPLE );
    glLineStipple( 1, tools.StipplePattern()  );
    tools.DrawLines( plannedPath_ );

    glDisable( GL_LINE_STIPPLE );
    glColor4d( COLOR_BLACK );
    glLineWidth( 3 );
    tools.DrawLines( previousPath_ );
    glColor4d( COLOR_OLDPATH );
    glLineWidth( 2 );
    tools.DrawLines( previousPath_ );

    glPopAttrib();
}
