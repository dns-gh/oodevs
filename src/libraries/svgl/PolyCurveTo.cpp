// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PolyCurveTo.h"
#include "PathParser.h"
#include "CubicCurveTo.h"
#include "QuadraticCurveTo.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: PolyCurveTo constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PolyCurveTo::PolyCurveTo( PathParser& data, PathContext& context, char command )
{
    const bool shortHand = command == 's' || command == 't';
    const bool cubic = command == 'c' || command == 's';
    do
    {
        PathCommand_ABC* command = cubic ? (PathCommand_ABC*)new CubicCurveTo( data, context, shortHand )
                                         : (PathCommand_ABC*)new QuadraticCurveTo( data, context, shortHand );
        elements_.push_back( command );
    }
    while( data.IsCoordinate() );
}

// -----------------------------------------------------------------------------
// Name: PolyCurveTo destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PolyCurveTo::~PolyCurveTo()
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: PolyCurveTo::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void PolyCurveTo::DrawLine( float expectedPrecision )
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        (*it)->DrawLine( expectedPrecision );
}

// -----------------------------------------------------------------------------
// Name: PolyCurveTo::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void PolyCurveTo::DrawArea( Tesselator_ABC& tesselator, float expectedPrecision )
{
     for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        (*it)->DrawArea( tesselator, expectedPrecision );
}
