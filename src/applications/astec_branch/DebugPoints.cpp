// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "DebugPoints.h"

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
// Name: DebugPoints::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void DebugPoints::Update( const DebugPointsMessage& message )
{
    uint32 size;
    message >> size;

    points_.clear();
    points_.reserve( size );
    for( uint i = 0; i < size; ++i )
    {
        MT_Vector2D vPos;
        message >> vPos;
        points_.push_back( vPos );
    }
}
