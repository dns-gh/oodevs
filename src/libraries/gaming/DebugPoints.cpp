// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DebugPoints.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DebugPoints constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
DebugPoints::DebugPoints( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugPoints destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
DebugPoints::~DebugPoints()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugPoints::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void DebugPoints::DoUpdate( const sword::DebugPoints& message )
{
    points_.resize( message.coordinates().elem_size() );
    for( int i = 0; i < message.coordinates().elem_size(); ++i )
        points_[ i ] = converter_.ConvertToXY( message.coordinates().elem(i) );
}

// -----------------------------------------------------------------------------
// Name: DebugPoints::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void DebugPoints::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "DebugPoints" ) )
    {
        GLfloat color[4];
        glGetFloatv( GL_CURRENT_COLOR, color );
        glPushAttrib( GL_LINE_BIT );
        glLineWidth( 6.f );
        for( auto it = points_.begin(); it != points_.end(); ++it )
            if( viewport.IsVisible( *it ) )
            {
                glColor4f( 0, 0, 0, color[3] * 0.5f );
                glLineWidth( 8.f );
                tools.DrawCross( *it, GL_CROSSSIZE );

                glColor4f( 255, 255, 255, 1.0f );
                glLineWidth( 4.f );
                tools.DrawCross( *it, GL_CROSSSIZE );
            }
        glPopAttrib();
    }
}
