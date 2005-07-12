//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "MIL_pch.h"

#include "PHY_PerceptionSurfaceAgent.h"

#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceAgent::PHY_PerceptionSurfaceAgent()
    : vOrigin_    ()
    , pSensorType_( 0  )
    , rHeight_    ( 0. )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceAgent::PHY_PerceptionSurfaceAgent( const PHY_SensorTypeAgent& sensorType, const MT_Vector2D& vOrigin, MT_Float rHeight )
    : vOrigin_    ( vOrigin    )
    , pSensorType_( &sensorType )
    , rHeight_    ( rHeight    )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceAgent::~PHY_PerceptionSurfaceAgent()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::load
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceAgent::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nID;
    
    file >> nID;
    assert( PHY_SensorType::FindSensorType( nID ) );
    pSensorType_ = PHY_SensorType::FindSensorType( nID )->GetTypeAgent();
    assert( pSensorType_ );
    
    file >> vOrigin_
         >> rHeight_
         >> sectors_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::save
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceAgent::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pSensorType_ );

    file << pSensorType_->GetType().GetID()
         << vOrigin_
         << rHeight_
         << sectors_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::AddDirection
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceAgent::AddDirection( const MT_Vector2D& vDir )
{
    assert( pSensorType_ );
    sectors_.push_back( MT_Sector( vOrigin_, vDir, pSensorType_->GetAngle() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::IsInside
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionSurfaceAgent::IsInside( const MT_Vector2D& vPoint ) const
{
    assert( pSensorType_ );

    if( vOrigin_.SquareDistance( vPoint ) <= pSensorType_->GetSquareProximityDistance() )
        return true;

    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
        if( itSector->IsInSector( vPoint ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::ComputePerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::ComputePerception( const PHY_RolePion_Perceiver& perceiver, const MT_Vector2D& vTargetPos ) const
{
    assert( pSensorType_ );
    return pSensorType_->ComputePerception( perceiver.GetPion(), vTargetPos, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::ComputePerception
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::ComputePerception( const PHY_RolePion_Perceiver& perceiver, const MIL_Agent_ABC& target ) const
{
    assert( pSensorType_ );
    const MT_Vector2D& vTargetPos = target.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( !( perceiver.IsKnown( target ) || ( perceiver.IsPeriphericalVisionEnabled() && pSensorType_->CanScan() ) || IsInside( vTargetPos ) ) )
        return PHY_PerceptionLevel::notSeen_;

    return pSensorType_->ComputePerception( perceiver.GetPion(), target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::ComputePerception( const PHY_RolePion_Perceiver& perceiver, const DEC_Knowledge_Agent&  target ) const
{
    assert( pSensorType_ );
    return pSensorType_->ComputePerception( perceiver.GetPion(), target, rHeight_ );  
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::SendFullState
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceAgent::SendFullState( DIN::DIN_BufferedMessage& msg ) const
{
    msg << vOrigin_;
    msg << rHeight_;
    msg << pSensorType_->GetType().GetName();
    msg << (uint32)sectors_.size();
    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
    {
        const MT_Sector& sector = *itSector;
        msg << sector.GetDirection();
    }
}


