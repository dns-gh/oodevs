// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRadarData.cpp $
// $Author: Nld $
// $Modtime: 3/06/05 17:40 $
// $Revision: 2 $
// $Workfile: PHY_PerceptionRadarData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionRadarData.h"
#include "DetectionComputer.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/RawVisionData/PHY_RawVisionDataIterator.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_World.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::sAcquisitionData::sAcquisitionData
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadarData::sAcquisitionData::sAcquisitionData()
    : nFirstTimeStepPerceived_( MIL_Time_ABC::GetTime().GetCurrentTimeStep() )
    , bUpdated_               ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadarData::PHY_PerceptionRadarData()
    : pRadarType_( 0 )
    , sensorHeight_( 10. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadarData::PHY_PerceptionRadarData( const PHY_RadarType& radarType, const double sensorHeight )
    : pRadarType_( &radarType )
    , sensorHeight_( sensorHeight )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData destructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadarData::~PHY_PerceptionRadarData()
{
    // NOTHING
}

namespace
{
    double ComputeExtinction( const PHY_RawVisionDataIterator& env, double rVisionNRJ, const PHY_RadarType& type ) 
    {
        const double epsilon = 1e-8;
        double rDistanceModificator = type.ComputeEnvironmentFactor( env.GetCurrentEnv() );
        return rDistanceModificator <= epsilon ? -1. : rVisionNRJ - env.Length() / rDistanceModificator ;
    }

    bool CanPerceive( const MT_Vector2D& sourcePosition, const MT_Vector2D& targetPosition, double sensorHeight, double perceiverAltitude, double targetAltitude, const PHY_RadarType& type )
    {
        static const double targetHeight = 2.0;
        const MT_Vector3D vSource3D( sourcePosition.rX_, sourcePosition.rY_, sensorHeight + MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( sourcePosition.rX_, sourcePosition.rY_ ) + perceiverAltitude );
        const MT_Vector3D vTarget3D( targetPosition.rX_, targetPosition.rY_, targetHeight + MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( targetPosition.rX_, targetPosition.rY_ ) + targetAltitude );

        PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
        double rVisionNRJ = type.GetRadius();
        if( rVisionNRJ > 0 )
            rVisionNRJ = it.End() ? std::numeric_limits< double >::max() : ComputeExtinction( it, rVisionNRJ, type );

        while( rVisionNRJ > 0 && !(++it).End() )
            rVisionNRJ = ComputeExtinction( it, rVisionNRJ, type );

        return ( rVisionNRJ > 0 );
    }

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::AcquireTargets
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarData::AcquireTargets( PHY_RoleInterface_Perceiver& perceiver, TER_Agent_ABC::T_AgentPtrVector& targets )
{
    const MT_Vector2D& perceiverPosition = perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    double perceiverAltitude = perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetHeight();
    for( auto it = targets.begin(); it != targets.end(); ++it )
    {
        MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        detection::DetectionComputer detectionComputer( target );
        perceiver.GetPion().Execute( detectionComputer );
        target.Execute( detectionComputer );
        if( detectionComputer.CanBeSeen() && pRadarType_->CanAcquire( perceiver.GetPion(), target ) )
        {
            const MT_Vector2D& targetPosition = target.GetRole< PHY_RoleInterface_Location >().GetPosition();
            double targetAltitude = target.GetRole< PHY_RoleInterface_Location >().GetHeight();
            if( CanPerceive( perceiverPosition, targetPosition, sensorHeight_, perceiverAltitude, targetAltitude, *pRadarType_ ) )
            {
                sAcquisitionData& agentData = acquisitionData_[ &target ];
                agentData.bUpdated_ = true;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::Update
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarData::Update( PHY_RoleInterface_Perceiver& perceiver )
{
    assert( pRadarType_ );
    for( auto it = acquisitionData_.begin(); it != acquisitionData_.end(); )
    {
        sAcquisitionData& data   =  it->second;
        MIL_Agent_ABC&    target = *it->first;
        if( !data.bUpdated_ )
        {
            it = acquisitionData_.erase( it );
            continue;
        }
        perceiver.NotifyPerception( target, pRadarType_->ComputeAcquisitionLevel( target, data.nFirstTimeStepPerceived_ ) );
        data.bUpdated_ = false;
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::Acquire
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarData::Acquire( PHY_RoleInterface_Perceiver& perceiver, const T_ZoneSet& zones, bool bAcquireOnPerceiverPosition )
{
    assert( pRadarType_ );
    for( auto it = zones.begin(); it != zones.end(); ++it )
    {
        TER_Agent_ABC::T_AgentPtrVector targets;
        TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( **it, targets );
        AcquireTargets( perceiver, targets );
    }
    if( bAcquireOnPerceiverPosition )
    {
        TER_Agent_ABC::T_AgentPtrVector targets;
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), pRadarType_->GetRadius(), targets );
        AcquireTargets( perceiver, targets );
    }
    Update( perceiver );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::SetMinHeight
// Created: LDC 2013-04-08
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarData::SetMinHeight( double height )
{
    sensorHeight_ = std::max( sensorHeight_, height );
}
    
