// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Positions.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: Positions constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
Positions::Positions( const CoordinateConverter& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Positions destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
Positions::~Positions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Positions::GetPosition
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
geometry::Point2f Positions::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: Positions::DoUpdate
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void Positions::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.positionPresent )
        position_ = converter_.ConvertToXY( message.position );
}
