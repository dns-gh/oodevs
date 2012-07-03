// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Sensor.h"

using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: Sensor constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
Sensor::Sensor( const SensorType& type, double rHeight )
    : type_   ( type )
    , rHeight_( rHeight )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sensor destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
Sensor::~Sensor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sensor::GetHeight
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
double Sensor::GetHeight() const
{
    return rHeight_;
}

// -----------------------------------------------------------------------------
// Name: Sensor::GetType
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const SensorType& Sensor::GetType() const
{
    return type_;
}
