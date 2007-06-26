// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MSEllipse.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MSEllipse constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
MSEllipse::MSEllipse( const ASN1T_Location& localisation, const CoordinateConverter_ABC& converter )
{
    if( localisation.type != EnumLocationType::ellipse || localisation.coordinates.n != 3 )
        throw std::runtime_error( "MSEllipse is not an ellipse" );
    origin_ = converter.ConvertToXY( localisation.coordinates.elem[0] );
    const Point2f majorHigh = converter.ConvertToXY( localisation.coordinates.elem[1] );
    const Point2f minorHigh = converter.ConvertToXY( localisation.coordinates.elem[2] );

    minorAxis_ = origin_.Distance( minorHigh );
    majorAxis_ = origin_.Distance( majorHigh );
    Vector2f axis( origin_, majorHigh ); axis.Normalize();
    angle_     = std::acos( axis.X() ) * 180.f / std::acos( -1.f );
    if( axis.Y() < 0 )
        angle_ = - angle_;
}

// -----------------------------------------------------------------------------
// Name: MSEllipse destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
MSEllipse::~MSEllipse()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MSEllipse::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void MSEllipse::Draw( const GlTools_ABC& tools ) const
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef( origin_.X(), origin_.Y(), 0 );
        glRotatef( angle_, 0, 0, 1 );
        glScalef( majorAxis_, minorAxis_, 1 );
        tools.DrawDisc( Point2f(), 1 );
    glPopMatrix();
}

