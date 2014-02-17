// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PolyEllipticalArc.h"
#include "PathParser.h"
#include "EllipticalArc.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: PolyEllipticalArc constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PolyEllipticalArc::PolyEllipticalArc( PathParser& data, PathContext& context )
{
    do
        elements_.push_back( new EllipticalArc( data, context ) );
    while( data.IsCoordinate() );
}

// -----------------------------------------------------------------------------
// Name: PolyEllipticalArc destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PolyEllipticalArc::~PolyEllipticalArc()
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: PolyEllipticalArc::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void PolyEllipticalArc::DrawLine( float expectedPrecision )
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        (*it)->DrawLine( expectedPrecision );
}

// -----------------------------------------------------------------------------
// Name: PolyEllipticalArc::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void PolyEllipticalArc::DrawArea( Tesselator_ABC& tesselator, float expectedPrecision )
{
     for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        (*it)->DrawArea( tesselator, expectedPrecision );
}

