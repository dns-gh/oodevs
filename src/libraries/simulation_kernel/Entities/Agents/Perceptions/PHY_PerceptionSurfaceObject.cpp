//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionSurfaceObject.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeObject.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_PerceptionSurfaceObject )

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceObject::PHY_PerceptionSurfaceObject()
    : pSensorType_( 0  )
    , rHeight_    ( 0. )
    , vOrigin_    ( -1., -1. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceObject::PHY_PerceptionSurfaceObject( const PHY_SensorTypeObject& sensorType, const MT_Vector2D& vOrigin, double rHeight )
    : vOrigin_    ( vOrigin    )
    , pSensorType_( &sensorType )
    , rHeight_    ( rHeight    )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceObject::~PHY_PerceptionSurfaceObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject::load
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceObject::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> pSensorType_;
    file >> vOrigin_;
    file >> rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject::save
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceObject::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pSensorType_;
    file << vOrigin_;
    file << rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject::ComputePerception
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceObject::ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_Object_ABC& target ) const
{
    return pSensorType_->ComputePerception( perceiver.GetPion(), target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceObject::ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const DEC_Knowledge_Object& target ) const
{
    return pSensorType_->ComputePerception( perceiver.GetPion(), target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject::IsInitialized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PHY_PerceptionSurfaceObject::IsInitialized() const
{
    return pSensorType_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject::operator=
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceObject& PHY_PerceptionSurfaceObject::operator=( const PHY_PerceptionSurfaceObject& rhs )
{
    pSensorType_ = rhs.pSensorType_;
    vOrigin_     = rhs.vOrigin_;
    rHeight_     = rhs.rHeight_;
    return *this;
}
