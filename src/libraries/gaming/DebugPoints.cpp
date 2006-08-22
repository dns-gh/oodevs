// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DebugPoints.h"
#include "Net_Def.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DebugPoints constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
DebugPoints::DebugPoints()
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
void DebugPoints::DoUpdate( const DebugPointsMessage& message )
{
    unsigned long size;
    message >> size;

    points_.clear();
    points_.reserve( size );
    for( unsigned i = 0; i < size; ++i )
    {
        double x, y;
        message >> x >> y;
        points_.push_back( geometry::Point2f( float(x), float(y) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DebugPoints::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void DebugPoints::Draw( const geometry::Point2f& , const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "DebugPoints" ) )
        for( CIT_PointVector it = points_.begin(); it != points_.end(); ++it )
            if( viewport.IsInside( *it ) )
                tools.DrawCross( *it, GL_CROSSSIZE );
}
