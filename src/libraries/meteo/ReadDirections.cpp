// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ReadDirections.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: weather::ReadDirection
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
MT_Vector2D weather::ReadDirection( double orientation )
{
    MT_Vector2D result( 0, 1 ); // North vector
    double radAngle = orientation * MT_PI / 180.;
    result.Rotate( radAngle );
    result.Normalize();
    assert( !result.IsZero() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: weather::ReadDirection
// Created: SBO 2010-04-07
// -----------------------------------------------------------------------------
MT_Vector2D weather::ReadDirection( const sword::Heading& message )
{
    return ReadDirection( message.heading() );
}
