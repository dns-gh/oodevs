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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.h>


namespace
{
    MT_Float rForestSurveillanceTime_ = std::numeric_limits< MT_Float >::max();
    MT_Float rUrbanSurveillanceTime_  = std::numeric_limits< MT_Float >::max();
    MT_Float rEmptySurveillanceTime_  = std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillanceReco constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRecoSurveillanceReco::PHY_PerceptionRecoSurveillanceReco( const TER_Localisation& localisation )
    : localisation_             ( localisation )
    , nForestDetectionTimeStep_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    , nUrbanDetectionTimeStep_  ( nForestDetectionTimeStep_ )
    , nEmptyDetectionTimeStep_  ( nForestDetectionTimeStep_ )
{
    uint nForestSurface = 0;
    uint nEmptySurface  = 0;
    uint nUrbanSurface  = 0;

    MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObjectsInSurface( localisation, nEmptySurface, nForestSurface, nUrbanSurface );

    const_cast< uint& >( nForestDetectionTimeStep_ ) += (uint)( nForestSurface * rForestSurveillanceTime_ );
    const_cast< uint& >( nEmptyDetectionTimeStep_  ) += (uint)( nEmptySurface  * rEmptySurveillanceTime_  );
    const_cast< uint& >( nUrbanDetectionTimeStep_  ) += (uint)( nUrbanSurface  * rForestSurveillanceTime_ );
}

struct PHY_PerceptionRecoSurveillance::LoadingWrapper
{
    void ReadAlatTime( xml::xistream& xis )
    {
        PHY_PerceptionRecoSurveillance::ReadAlatTime( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::Initialize
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoSurveillance::Initialize( xml::xistream& xis )
{
    LoadingWrapper loader;

    xis >> xml::start( "alat-monitoring-times" )
            >> xml::list( "alat-monitoring-time", loader, &LoadingWrapper::ReadAlatTime )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::ReadAlatTime
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoSurveillance::ReadAlatTime( xml::xistream& xis )
{
    tools::ReadTimeAttribute( xis, "empty", rEmptySurveillanceTime_ );
    tools::ReadTimeAttribute( xis, "forest", rForestSurveillanceTime_ );
    tools::ReadTimeAttribute( xis, "urban", rUrbanSurveillanceTime_ );
       
    rEmptySurveillanceTime_ = MIL_Tools::ConvertSecondsToSim( rEmptySurveillanceTime_ );                // second.hectare-1 => dT.hectare-1
    rEmptySurveillanceTime_ /= 10000.;                                                                  // dT.hectare-1     => dT.m-2
    rEmptySurveillanceTime_ = 1. / MIL_Tools::ConvertMeterSquareToSim( 1. / rEmptySurveillanceTime_ );  // dT.m-2           => dT.px-2

    rForestSurveillanceTime_ = MIL_Tools::ConvertSecondsToSim( rForestSurveillanceTime_ );                 // second.hectare-1 => dT.hectare-1
    rForestSurveillanceTime_ /= 10000.;                                                                    // dT.hectare-1     => dT.m-2
    rForestSurveillanceTime_ = 1. / MIL_Tools::ConvertMeterSquareToSim( 1. / rForestSurveillanceTime_ );   // dT.m-2           => dT.px-2

    rUrbanSurveillanceTime_ = MIL_Tools::ConvertSecondsToSim( rUrbanSurveillanceTime_ );                // second.hectare-1 => dT.hectare-1
    rUrbanSurveillanceTime_ /= 10000.;                                                                  // dT.hectare-1     => dT.m-2
    rUrbanSurveillanceTime_ = 1. / MIL_Tools::ConvertMeterSquareToSim( 1. / rUrbanSurveillanceTime_ );  // dT.m-2           => dT.px-2
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillanceReco::IsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoSurveillanceReco::IsInside( const MT_Vector2D& vPoint ) const
{
    if ( !localisation_.IsInside( vPoint ) )
        return false;

    const uint                       nCurrentTime = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep(); 
    const PHY_RawVisionData::envBits env          = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObject( vPoint );

    if ( env == PHY_RawVisionData::eVisionEmpty )
        return nCurrentTime >= nEmptyDetectionTimeStep_;

    bool bResult = true;

    if ( env & PHY_RawVisionData::eVisionUrban )
        bResult &= nCurrentTime >= nUrbanDetectionTimeStep_;

    if ( env & PHY_RawVisionData::eVisionForest )
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

    for ( TER_Agent_ABC::IT_AgentPtrVector it = result.begin(); it != result.end(); )
        if ( IsInside( (*it)->GetPosition() ) )
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
    for( IT_RecoVector it = recos_.begin(); it != recos_.end(); ++it )
        delete *it;
    recos_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int PHY_PerceptionRecoSurveillance::AddLocalisation( const TER_Localisation& localisation )
{
    PHY_PerceptionRecoSurveillanceReco* pNewReco = new PHY_PerceptionRecoSurveillanceReco( localisation );    
    assert( pNewReco );
    return Add( pNewReco );
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
    for ( CIT_RecoVector itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        if ( (*itReco)->IsInside( vPoint ) )
            return PHY_PerceptionLevel::recognized_;
    }

    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRecoSurveillance::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    
    for ( CIT_RecoVector itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        (*itReco)->GetAgentsInside( perceivableAgents );
        
        for ( TER_Agent_ABC::CIT_AgentPtrVector it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
            if( target.GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver_.GetPion() ) )
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
    if( !agent.GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver_.GetPion() ) )
        return PHY_PerceptionLevel::notSeen_;
    return Compute( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRecoSurveillance::Compute( const DEC_Knowledge_Agent & knowledge ) const
{
    return Compute( knowledge.GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_PerceptionRecoSurveillance::HasLocalisationToHandle() const
{
    return !recos_.empty();
}
