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
// Name: ReadDirections::ReadDirection
// Created: SBO 2010-04-07
// -----------------------------------------------------------------------------
MT_Vector2D weather::ReadDirection( const sword::MsgHeading& message )
{
    MT_Vector2D result( 0, 1 ); // North vector
    double radAngle = message.heading() * MT_PI / 180.;
    result.Rotate( radAngle );
    result.Normalize();
    assert( !result.IsZero() );
    return result;
}
