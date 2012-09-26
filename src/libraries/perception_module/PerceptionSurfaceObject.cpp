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
// Name: PerceptionSurfaceObject::ComputeObjectPerception
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceObject::ComputeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target ) const
{
    return pSensorType_->ComputeObjectPerception( perceiver, target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceObject::ComputeKnowledgeObjectPerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceObject::ComputeKnowledgeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target ) const
{
    return pSensorType_->ComputeKnowledgeObjectPerception( perceiver, target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceObject::IsInitialized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PerceptionSurfaceObject::IsInitialized() const
{
    return pSensorType_ != 0;
}
