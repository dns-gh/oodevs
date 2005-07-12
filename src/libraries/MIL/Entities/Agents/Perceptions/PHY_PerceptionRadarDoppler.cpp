// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRadarDoppler.cpp $
// $Author: Nld $
// $Modtime: 1/04/05 10:30 $
// $Revision: 7 $
// $Workfile: PHY_PerceptionRadarDoppler.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_PerceptionRadarDoppler.h"

#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Tools/MIL_Tools.h"

#include "TER_World.h"

MT_Float PHY_PerceptionRadarDoppler::rRadius_              = 0.;
MT_Float PHY_PerceptionRadarDoppler::rDetectionTime_       = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionRadarDoppler::rRecognitionTime_     = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionRadarDoppler::rIdentificationTime_  = std::numeric_limits< MT_Float >::max();

PHY_PerceptionRadarDoppler::T_DetectableConsumptionVector PHY_PerceptionRadarDoppler::detectableConsumptions_;

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::PHY_PerceptionRadarDoppler::sAgentPerceptionData::sAgentPerceptionData
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
PHY_PerceptionRadarDoppler::sAgentPerceptionData::sAgentPerceptionData()
    : nFirstTimeStepPerceived_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    , bUpdated_               ( true )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::sAgentPerceptionData
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
PHY_PerceptionRadarDoppler::sAgentPerceptionData::sAgentPerceptionData( const sAgentPerceptionData& rhs )
    : nFirstTimeStepPerceived_( rhs.nFirstTimeStepPerceived_ )
    , bUpdated_               ( rhs.bUpdated_ )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::sRadar constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRadarDoppler::sRadar::sRadar( const TER_Localisation& localisation )
    : localisation_    ( localisation )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::sRadar::GetAgentsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
void PHY_PerceptionRadarDoppler::sRadar::GetAgentsInside( TER_Agent_ABC::T_AgentPtrVector& result ) const
{
    result.clear();
    TER_World::GetWorld().GetListAgentWithinLocalisation( localisation_, result );
}

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Initialize
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarDoppler::Initialize( MIL_InputArchive& archive )
{
    archive.Section( "RadarDoppler" );

    archive.ReadField( "RayonAction", rRadius_, CheckValueGreaterOrEqual( 0. ) );
    rRadius_ = MIL_Tools::ConvertMeterToSim( rRadius_ );

    // temps de détection
    MT_Float rMinBound = 0.;
    if( archive.ReadTimeField( "TempsDetection", rDetectionTime_, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) ) 
        rMinBound = rDetectionTime_;   
    if( archive.ReadTimeField( "TempsReconnaissance", rRecognitionTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
        rMinBound = rRecognitionTime_;
    archive.ReadTimeField( "TempsIdentification", rIdentificationTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

    if( rDetectionTime_ != std::numeric_limits< MT_Float >::max() )
        rDetectionTime_ = MIL_Tools::ConvertSecondsToSim( rDetectionTime_ );

    if( rRecognitionTime_ != std::numeric_limits< MT_Float >::max() )
        rRecognitionTime_ = MIL_Tools::ConvertSecondsToSim( rRecognitionTime_ );

    if( rIdentificationTime_ != std::numeric_limits< MT_Float >::max() )
        rIdentificationTime_ = MIL_Tools::ConvertSecondsToSim( rIdentificationTime_ );

    // mode de consommatiopn détectés
    const PHY_ConsumptionType::T_ConsumptionTypeMap& consumptionTypes = PHY_ConsumptionType::GetConsumptionTypes();

    archive.Section( "ActivitePionsDetectables" );
    detectableConsumptions_.clear();
    detectableConsumptions_.resize( consumptionTypes.size() ,false );
    for ( PHY_ConsumptionType::CIT_ConsumptionTypeMap it = consumptionTypes.begin(); it != consumptionTypes.end(); ++it )
    {
        const PHY_ConsumptionType& conso = *it->second;

        if ( archive.Section( conso.GetName(), MIL_InputArchive::eNothing ) )
        {
            detectableConsumptions_[ conso.GetID() ] = true;
            archive.EndSection();
        }
    }
    archive.EndSection(); // ActivitePionsDetectables

    archive.EndSection(); // RadarDoppler
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Terminate
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarDoppler::Terminate()
{
   detectableConsumptions_.clear();
}


// =============================================================================
// INSTANCE
// =============================================================================


// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRadarDoppler::PHY_PerceptionRadarDoppler( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::~PHY_PerceptionRadarDoppler
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionRadarDoppler::~PHY_PerceptionRadarDoppler()
{
    for( IT_RadarVector it = radars_.begin(); it != radars_.end(); ++it )
        delete *it;
    radars_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::AddRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void* PHY_PerceptionRadarDoppler::AddRadar( const TER_Localisation& localisation )
{
    sRadar* pNewRadar = new sRadar( localisation );
    
    assert( pNewRadar );
    radars_.push_back( pNewRadar );
    return pNewRadar;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::RemoveRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarDoppler::RemoveRadar( void* pId )
{
    sRadar* pRadar = static_cast< sRadar* >( pId );

    IT_RadarVector it = std::find( radars_.begin(), radars_.end(), pRadar );

    if ( it != radars_.end() )
    {
        delete pRadar;
        radars_.erase( it );
    }
}

// =============================================================================
// AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarDoppler::Compute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const MT_Vector2D& /*vPoint*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::ComputeLevel
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& PHY_PerceptionRadarDoppler::ComputeLevel( const MIL_Agent_ABC& agent, uint nFirstTimeStepPerceived ) const
{
    if ( !detectableConsumptions_[ agent.GetRole< PHY_RoleInterface_Dotations >().GetConsumptionMode().GetID() ] )
        return PHY_PerceptionLevel::notSeen_;

    const uint nTimePerceived = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() - nFirstTimeStepPerceived;

    if( nTimePerceived >= rIdentificationTime_ )
        return PHY_PerceptionLevel::identified_;
    if( nTimePerceived >= rRecognitionTime_ )
        return PHY_PerceptionLevel::recognized_;
    if( nTimePerceived >= rDetectionTime_ )
        return PHY_PerceptionLevel::detected_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarDoppler::Execute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    const MT_Vector2D& vPerceiverPos = perceiver_.GetPion().GetRole< PHY_RolePion_Location >().GetPosition();

    for( CIT_RadarVector itRadar = radars_.begin(); itRadar != radars_.end(); ++itRadar  )
    {
        TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
        (*itRadar )->GetAgentsInside( perceivableAgents );

        // Update map
        for ( TER_Agent_ABC::CIT_AgentPtrVector it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
        {
            if( vPerceiverPos.Distance( (**it).GetPosition() ) > rRadius_ )
                continue;

            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
            if( target.GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver_.GetPion() ) )
            {
                sAgentPerceptionData& agentData = perceivedAgents_[ &target ];
                agentData.bUpdated_ = true;
            }
        }
    }

    for( IT_AgentPerceptionMap itAgent = perceivedAgents_.begin(); itAgent != perceivedAgents_.end(); )
    {
        sAgentPerceptionData& data  =  itAgent->second;
        MIL_Agent_ABC&        agent = *itAgent->first;
        if( !data.bUpdated_ )
        {
            itAgent = perceivedAgents_.erase( itAgent );
            continue;
        }
        perceiver_.NotifyAgentPerception( agent, ComputeLevel( agent, data.nFirstTimeStepPerceived_ ) );
        data.bUpdated_ = false;
        ++itAgent;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarDoppler::Compute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const MIL_Agent_ABC& /*agent*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarDoppler::Compute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const DEC_Knowledge_Agent & /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// =============================================================================
// OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarDoppler::Compute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const MIL_RealObject_ABC& /*object*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarDoppler::Compute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const DEC_Knowledge_Object& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarDoppler::Execute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const TER_DynaObject_ABC::T_DynaObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}
