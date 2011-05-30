//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionSurfaceAgent.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include <boost/serialization/vector.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_PerceptionSurfaceAgent )

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceAgent::PHY_PerceptionSurfaceAgent()
    : vOrigin_    ()
    , pSensorType_( 0  )
    , rHeight_    ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceAgent::PHY_PerceptionSurfaceAgent( const PHY_SensorTypeAgent& sensorType, const MT_Vector2D& vOrigin, double rHeight )
    : vOrigin_    ( vOrigin    )
    , pSensorType_( &sensorType )
    , rHeight_    ( rHeight    )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceAgent::~PHY_PerceptionSurfaceAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::load
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceAgent::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int nID;
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
void PHY_PerceptionSurfaceAgent::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pSensorType_ );

    unsigned type = pSensorType_->GetType().GetID();
    file << type
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
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MT_Vector2D& vTargetPos ) const
{
    assert( pSensorType_ );
    return pSensorType_->ComputePerception( perceiver.GetPion(), vTargetPos, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::ComputePerception
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_Agent_ABC& target ) const
{
    assert( pSensorType_ );
    const MT_Vector2D& vTargetPos = target.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( !( perceiver.IsKnown( target ) || ( perceiver.IsPeriphericalVisionEnabled() && pSensorType_->CanScan() ) || IsInside( vTargetPos ) ) )
        return PHY_PerceptionLevel::notSeen_;

    const PHY_PerceptionLevel& level = pSensorType_->ComputePerception( perceiver.GetPion(), target, rHeight_ );

    return GetLevelWithDelay( level, &target );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::GetLevelWithDelay
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::GetLevelWithDelay( const PHY_PerceptionLevel& level, const void* target ) const
{
    int delay = pSensorType_->GetDelay();
    if( delay && level > PHY_PerceptionLevel::notSeen_ )
    {
        CIT_PerceptionTickMap it = perceptionsUnderway_.find( target );
        int tick = 0;
        if( it != perceptionsUnderway_.end() )
            tick = it->second;
        perceptionsBuffer_[ target ] = tick + 1;
        if( tick <= delay )
            return PHY_PerceptionLevel::notSeen_;
    }
    return level;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::FinalizePerception
// Created: LDC 2010-05-04
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceAgent::FinalizePerception()
{
    perceptionsUnderway_ = perceptionsBuffer_;
    perceptionsBuffer_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const DEC_Knowledge_Agent&  target ) const
{
    assert( pSensorType_ );
    const PHY_PerceptionLevel& level = pSensorType_->ComputePerception( perceiver.GetPion(), target, rHeight_ );
    return GetLevelWithDelay( level, &target );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_PopulationConcentration& target ) const
{
    const PHY_PerceptionLevel& level = pSensorType_->ComputePerception( perceiver.GetPion(), target, rHeight_ );
    return GetLevelWithDelay( level, &target );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::ComputePerceptionAccuracy
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
double PHY_PerceptionSurfaceAgent::ComputePerceptionAccuracy( const PHY_RoleInterface_Perceiver& perceiver, const MIL_PopulationFlow& target ) const
{
    return pSensorType_->ComputePerceptionAccuracy( perceiver.GetPion(), target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionSurfaceAgent::ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_PopulationFlow& target, T_PointVector& shape ) const
{
    const PHY_PerceptionLevel& level = pSensorType_->ComputePerception( perceiver.GetPion(), target, rHeight_, shape );
    return GetLevelWithDelay( level, &target );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::SendFullState
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceAgent::SendFullState( sword::VisionCone& msg ) const
{
    NET_ASN_Tools::WritePoint( vOrigin_, *msg.mutable_origin() );
    msg.set_height( static_cast< int >( rHeight_ ) );
    msg.set_sensor( pSensorType_->GetType().GetName() );
    msg.mutable_directions();
    for( unsigned int i = 0u; i < sectors_.size(); ++i )
        NET_ASN_Tools::WriteDirection( sectors_[i].GetDirection(), *msg.mutable_directions()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::IsInitialized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PHY_PerceptionSurfaceAgent::IsInitialized()
{
    return pSensorType_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::GetSensorTypeName
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
const std::string& PHY_PerceptionSurfaceAgent::GetSensorTypeName() const
{
    assert( pSensorType_ );

    return pSensorType_->GetType().GetName();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::TransfertPerception
// Created: SLG 2010-05-07
// -----------------------------------------------------------------------------
void PHY_PerceptionSurfaceAgent::TransfertPerception( std::map< const void*, std::pair< unsigned int, double > > urbanPerceptionMap ) const
{
    perceptionsBuffer_.clear();
    for( std::map< const void*, std::pair< unsigned int, double > >::const_iterator it = urbanPerceptionMap.begin(); it != urbanPerceptionMap.end(); ++it )
        perceptionsBuffer_[ it->first ] = it->second.first;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::GetTargetsPerception
// Created: SLG 2010-05-07
// -----------------------------------------------------------------------------
std::map< const void*, unsigned int > PHY_PerceptionSurfaceAgent::GetTargetsPerception() const
{
    return perceptionsUnderway_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::operator=
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_PerceptionSurfaceAgent& PHY_PerceptionSurfaceAgent::operator=( const PHY_PerceptionSurfaceAgent& rhs )
{
    pSensorType_ = rhs.pSensorType_;
    vOrigin_     = rhs.vOrigin_;
    rHeight_     = rhs.rHeight_;
    sectors_     = rhs.sectors_;
    return *this;
}
