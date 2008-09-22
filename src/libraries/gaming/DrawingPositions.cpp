// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DrawingPositions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

// -----------------------------------------------------------------------------
// Name: DrawingPositions constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingPositions::DrawingPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingPositions::DrawingPositions( const kernel::CoordinateConverter_ABC& converter, const ASN1T_MsgShapeCreation& asn )
{
    for( unsigned int i = 0; i < asn.shape.points.n; ++i )
        AddPoint( converter.ConvertToXY( asn.shape.points.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingPositions::~DrawingPositions()
{
    // NOTHING
}
