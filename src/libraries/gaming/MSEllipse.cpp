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
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MSEllipse constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
MSEllipse::MSEllipse( const sword::Location& localisation, const CoordinateConverter_ABC& converter )
{
    if( localisation.type() != sword::Location_Geometry_ellipse || localisation.coordinates().elem_size() != 3 )
        throw MASA_EXCEPTION( "MSEllipse is not an ellipse" );
    origin_ = converter.ConvertToXY( localisation.coordinates().elem(0) );
    const Point2f majorHigh = converter.ConvertToXY( localisation.coordinates().elem(1) );
    const Point2f minorHigh = converter.ConvertToXY( localisation.coordinates().elem(2) );

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
void MSEllipse::Draw( const gui::GLView_ABC& tools ) const
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef( origin_.X(), origin_.Y(), 0 );
        glRotatef( angle_, 0, 0, 1 );
        glScalef( majorAxis_, minorAxis_, 1 );
        tools.DrawDisc( Point2f(), 1 );
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: MSEllipse::IsInside
// Created: HBD 2010-04-06
// -----------------------------------------------------------------------------
bool MSEllipse::IsInside( geometry::Point2f point ) const
{
    point.Set( point.X() - origin_.X(), point.Y() - origin_.Y())  ;
    const float value = ( majorAxis_ * majorAxis_ * point.X() * point.X()
        + majorAxis_ * majorAxis_  * point.Y() * point.Y()
        + minorAxis_ * minorAxis_ * point.X() * point.Y()
        +  ( - majorAxis_ * majorAxis_  * minorAxis_ * minorAxis_  ) );
    return value <= 0.;
}
