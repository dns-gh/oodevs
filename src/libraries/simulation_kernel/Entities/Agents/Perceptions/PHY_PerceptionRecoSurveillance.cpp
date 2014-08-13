// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoSurveillance.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 17:16 $
// $Revision: 6 $
// $Workfile: PHY_PerceptionRecoSurveillance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionRecoSurveillance.h"
#include "DetectionComputer.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"

namespace
{
    double rForestSurveillanceTime_ = std::numeric_limits< double >::max();
    double rUrbanSurveillanceTime_  = std::numeric_limits< double >::max();
    double rEmptySurveillanceTime_  = std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillanceReco constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoSurveillanceReco::PHY_PerceptionRecoSurveillanceReco( const TER_Localisation& localisation )
    : localisation_             ( localisation )
    , nForestDetectionTimeStep_ ( MIL_Time_ABC::GetTime().GetCurrentTimeStep() )
    , nUrbanDetectionTimeStep_  ( nForestDetectionTimeStep_ )
    , nEmptyDetectionTimeStep_  ( nForestDetectionTimeStep_ )
{
    unsigned int nForestSurface = 0;
    unsigned int nEmptySurface  = 0;
    unsigned int nUrbanSurface  = 0;

    MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObjectsInSurface( localisation, nEmptySurface, nForestSurface, nUrbanSurface );

    const_cast< unsigned int& >( nForestDetectionTimeStep_ ) += (unsigned int)( nForestSurface * rForestSurveillanceTime_ );
    const_cast< unsigned int& >( nEmptyDetectionTimeStep_  ) += (unsigned int)( nEmptySurface  * rEmptySurveillanceTime_  );
    const_cast< unsigned int& >( nUrbanDetectionTimeStep_  ) += (unsigned int)( nUrbanSurface  * rUrbanSurveillanceTime_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::Initialize
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoSurveillance::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "alat-monitoring-times" )
            >> xml::list( "alat-monitoring-time", &PHY_PerceptionRecoSurveillance::ReadAlatTime )
        >> xml::end;
}

namespace
{
    void UpdateTime( xml::xistream& xis, double& time )
    {
        tools::ReadTimeAttribute( xis, "time", time );
        time = MIL_Tools::ConvertSecondsToSim( time ); // second.hectare-1 => dT.hectare-1
        time /= 10000.;                                // dT.hectare-1     => dT.m-2
        if( time != 0. )
            time = 1. / ( 1. / time );                     // dT.m-2           => dT.px-2
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::ReadAlatTime
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoSurveillance::ReadAlatTime( xml::xistream& xis )
{
    const std::string terrain = xis.attribute< std::string >( "terrain" );
    if( terrain == "Vide" )
        UpdateTime( xis, rEmptySurveillanceTime_ );
    else if( terrain == "Foret" )
        UpdateTime( xis, rForestSurveillanceTime_ );
    else if( terrain == "Urbain" )
        UpdateTime( xis, rUrbanSurveillanceTime_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillanceReco::IsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoSurveillanceReco::IsInside( const MT_Vector2D& vPoint ) const
{
    if( !localisation_.IsInside( vPoint ) )
        return false;

    const unsigned int                       nCurrentTime = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    const envBits env          = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObject( vPoint );

    if( env == PHY_RawVisionData::eVisionEmpty )
        return nCurrentTime >= nEmptyDetectionTimeStep_;

    bool bResult = true;

    if( env & PHY_RawVisionData::eVisionUrban )
        bResult &= nCurrentTime >= nUrbanDetectionTimeStep_;

    if( env & PHY_RawVisionData::eVisionForest )
        bResult &= nCurrentTime >= nForestDetectionTimeStep_;

    return bResult;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillanceReco::GetAgentsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoSurveillanceReco::GetAgentsInside( TER_Agent_ABC::T_AgentPtrVector& result ) const
{
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( localisation_, result );
    for( auto it = result.begin(); it != result.end(); )
        if( IsInside( (*it)->GetPosition() ) )
            ++it;
        else
            it = result.erase( it );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoSurveillance::PHY_PerceptionRecoSurveillance( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_PerceptionWithLocation< PHY_PerceptionRecoSurveillanceReco >( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::~PHY_PerceptionRecoSurveillance
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoSurveillance::~PHY_PerceptionRecoSurveillance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoSurveillance::AddLocalisation( const TER_Localisation& localisation )
{
    return Add( new PHY_PerceptionRecoSurveillanceReco( localisation ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::RemoveLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoSurveillance::RemoveLocalisation( int id )
{
    Remove( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoSurveillance::Compute( const MT_Vector2D& vPoint ) const
{
    for( auto itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
        if( itReco->IsInside( vPoint ) )
            return PHY_PerceptionLevel::recognized_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoSurveillance::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    for( auto itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        perceivableAgents.clear();
        itReco->GetAgentsInside( perceivableAgents );
        for( auto it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
            if( !DEC_Knowledge_Agent::detectDestroyedUnits_ && target.IsDead() )
                continue;
            detection::DetectionComputer detectionComputer( target );
            perceiver_.GetPion().Execute( detectionComputer );
            target.Execute( detectionComputer );
            if( detectionComputer.CanBeSeen() )
                perceiver_.NotifyPerception( target, PHY_PerceptionLevel::recognized_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoSurveillance::Compute( const MIL_Agent_ABC& agent ) const
{
    return Compute( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoSurveillance::HasLocalisationToHandle() const
{
    return !recos_.empty();
}
