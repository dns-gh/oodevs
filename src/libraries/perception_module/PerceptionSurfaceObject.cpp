// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionSurfaceObject.h"
#include "SensorTypeObject.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceObject constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionSurfaceObject::PerceptionSurfaceObject()
    : pSensorType_( 0  )
    , rHeight_    ( 0. )
    , vOrigin_    ( -1., -1. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceObject constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionSurfaceObject::PerceptionSurfaceObject( const SensorTypeObject& sensorType, const MT_Vector2D& vOrigin, double rHeight )
    : vOrigin_    ( vOrigin    )
    , pSensorType_( &sensorType )
    , rHeight_    ( rHeight    )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceObject destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionSurfaceObject::~PerceptionSurfaceObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceObject::ComputePerception
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceObject::ComputePerception( const wrapper::View& perceiver, const MIL_Object_ABC& target ) const
{
    return pSensorType_->ComputePerception( perceiver, target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceObject::ComputePerception( const wrapper::View& perceiver, const DEC_Knowledge_Object& target ) const
{
    return pSensorType_->ComputePerception( perceiver, target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceObject::IsInitialized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PerceptionSurfaceObject::IsInitialized() const
{
    return pSensorType_ != 0;
}
