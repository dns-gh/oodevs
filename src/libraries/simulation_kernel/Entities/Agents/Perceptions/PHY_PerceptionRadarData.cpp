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
#include "DetectionComputer_ABC.h"
#include "DetectionComputerFactory_ABC.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/Units/Radars/PHY_RadarType.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionDataIterator.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_World.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::sAcquisitionData::sAcquisitionData
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadarData::sAcquisitionData::sAcquisitionData()
    : nFirstTimeStepPerceived_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    , bUpdated_               ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::PHY_PerceptionRadarData::sAcquisitionData::sAcquisitionData
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadarData::sAcquisitionData::sAcquisitionData( const sAcquisitionData& rhs )
    : nFirstTimeStepPerceived_( rhs.nFirstTimeStepPerceived_ )
    , bUpdated_               ( rhs.bUpdated_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadarData::PHY_PerceptionRadarData()
    : pRadarType_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData constructor
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
PHY_PerceptionRadarData::PHY_PerceptionRadarData( const PHY_RadarType& radarType )
    : pRadarType_( &radarType )
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
    bool CanPerceive( const MT_Vector2D& sourcePosition, const MT_Vector2D& targetPosition, double perceiverAltitude, double targetAltitude )
    {
        static const double sensorHeight = 10.0;
        static const double targetHeight = 2.0;
        const MT_Vector3D vSource3D( sourcePosition.rX_, sourcePosition.rY_, sensorHeight + MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( sourcePosition.rX_, sourcePosition.rY_ ) + perceiverAltitude );
        const MT_Vector3D vTarget3D( targetPosition.rX_, targetPosition.rY_, targetHeight + MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( targetPosition.rX_, targetPosition.rY_ ) + targetAltitude );

        PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
        while ( !(++it).End() )
        {
            if( PHY_RawVisionData::eVisionGround & it.GetCurrentEnv() )
                return false;
        }
        return true;
    }

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::AcquireTargets
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarData::AcquireTargets( PHY_RoleInterface_Perceiver& perceiver, TER_Agent_ABC::T_AgentPtrVector& targets, const detection::DetectionComputerFactory_ABC& detectionComputerFactory )
{
    const MT_Vector2D& perceiverPosition = perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    for( TER_Agent_ABC::CIT_AgentPtrVector it = targets.begin(); it != targets.end(); ++it )
    {
        MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        std::auto_ptr< detection::DetectionComputer_ABC > detectionComputer = detectionComputerFactory.Create( target );
        perceiver.GetPion().Execute( *detectionComputer );
        target.Execute( *detectionComputer );

        if( detectionComputer->CanBeSeen() && pRadarType_->CanAcquire( perceiver.GetPion(), target ) )
        {
            const MT_Vector2D& targetPosition = target.GetRole< PHY_RoleInterface_Location >().GetPosition();
            double perceiverAltitude = perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetHeight();
            double targetAltitude = target.GetRole< PHY_RoleInterface_Location >().GetHeight();
            if( CanPerceive( perceiverPosition, targetPosition, perceiverAltitude, targetAltitude ) )
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
    for( IT_AgentAcquisitionMap itAcquisitionData = acquisitionData_.begin(); itAcquisitionData != acquisitionData_.end(); )
    {
        sAcquisitionData& data   =  itAcquisitionData->second;
        MIL_Agent_ABC&    target = *itAcquisitionData->first;
        if( !data.bUpdated_ )
        {
            itAcquisitionData = acquisitionData_.erase( itAcquisitionData );
            continue;
        }
        perceiver.NotifyPerception( target, pRadarType_->ComputeAcquisitionLevel( target, data.nFirstTimeStepPerceived_ ) );
        data.bUpdated_ = false;
        ++itAcquisitionData;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarData::Acquire
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarData::Acquire( PHY_RoleInterface_Perceiver& perceiver, const T_ZoneSet& zones, bool bAcquireOnPerceiverPosition, const detection::DetectionComputerFactory_ABC& detectionComputer )
{
    assert( pRadarType_ );
    TER_Agent_ABC::T_AgentPtrVector targets;
    for( CIT_ZoneSet itZone = zones.begin(); itZone != zones.end(); ++itZone )
    {
        targets.clear();
        TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( **itZone, targets );
        AcquireTargets( perceiver, targets, detectionComputer );
    }

    if( bAcquireOnPerceiverPosition )
    {
        targets.clear();
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( perceiver.GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), pRadarType_->GetRadius(), targets );
        AcquireTargets( perceiver, targets, detectionComputer );
    }

    Update( perceiver );
}

