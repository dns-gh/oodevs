// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionTappingRadar.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 14:28 $
// $Revision: 3 $
// $Workfile: PHY_PerceptionTappingRadar.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_PerceptionTappingRadar.h"

#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Tools/MIL_Tools.h"

#include "TER_World.h"

MT_Float PHY_PerceptionTappingRadar::rRadius_              = 0.;
MT_Float PHY_PerceptionTappingRadar::rDetectionTime_       = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionTappingRadar::rRecognitionTime_     = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionTappingRadar::rIdentificationTime_  = std::numeric_limits< MT_Float >::max();

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::PHY_PerceptionTappingRadar::sAgentPerceptionData::sAgentPerceptionData
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
PHY_PerceptionTappingRadar::sAgentPerceptionData::sAgentPerceptionData()
    : nFirstTimeStepPerceived_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    , bUpdated_               ( true )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::sAgentPerceptionData
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
PHY_PerceptionTappingRadar::sAgentPerceptionData::sAgentPerceptionData( const sAgentPerceptionData& rhs )
    : nFirstTimeStepPerceived_( rhs.nFirstTimeStepPerceived_ )
    , bUpdated_               ( rhs.bUpdated_ )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::sRadar constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionTappingRadar::sRadar::sRadar( const TER_Localisation& localisation )
    : localisation_    ( localisation )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::sRadar::GetAgentsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
void PHY_PerceptionTappingRadar::sRadar::GetAgentsInside( TER_Agent_ABC::T_AgentPtrVector& result ) const
{
    result.clear();
    TER_World::GetWorld().GetListAgentWithinLocalisation( localisation_, result );
}

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::Initialize
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
void PHY_PerceptionTappingRadar::Initialize( MIL_InputArchive& archive )
{
    archive.Section( "EcouteRadar" );

    archive.ReadField( "RayonAction", rRadius_, CheckValueGreaterOrEqual( 0. ) );
    rRadius_ = MIL_Tools::ConvertMeterToSim( rRadius_ );

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

    archive.EndSection(); // EcouteRadar
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::Terminate
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
void PHY_PerceptionTappingRadar::Terminate()
{
    // NOTHING
}


// =============================================================================
// INSTANCE
// =============================================================================


// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionTappingRadar::PHY_PerceptionTappingRadar( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::~PHY_PerceptionTappingRadar
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionTappingRadar::~PHY_PerceptionTappingRadar()
{
    for( IT_RadarVector it = radars_.begin(); it != radars_.end(); ++it )
        delete *it;
    radars_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::AddRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void* PHY_PerceptionTappingRadar::AddRadar( const TER_Localisation& localisation )
{
    sRadar* pNewRadar = new sRadar( localisation );
    
    assert( pNewRadar );
    radars_.push_back( pNewRadar );
    return pNewRadar;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::RemoveRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionTappingRadar::RemoveRadar( void* pId )
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
// Name: PHY_PerceptionTappingRadar::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTappingRadar::Compute( const MT_Vector2D& /*vPoint*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::ComputeLevel
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& PHY_PerceptionTappingRadar::ComputeLevel( uint nFirstTimeStepPerceived ) const
{
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
// Name: PHY_PerceptionTappingRadar::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionTappingRadar::Execute(  const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
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
            if(    target.GetRole< PHY_RoleInterface_Perceiver >().IsUsingRadar  () 
                && target.GetRole< PHY_RoleInterface_Posture   >().CanBePerceived( perceiver_.GetPion() ) )
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
        perceiver_.NotifyAgentPerception( agent, ComputeLevel( data.nFirstTimeStepPerceived_ ) );
        data.bUpdated_ = false;
        ++itAgent;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTappingRadar::Compute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const MIL_Agent_ABC& /*agent*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTappingRadar::Compute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const DEC_Knowledge_Agent & /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// =============================================================================
// OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTappingRadar::Compute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const MIL_RealObject_ABC& /*object*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTappingRadar::Compute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const DEC_Knowledge_Object& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionTappingRadar::Execute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const TER_DynaObject_ABC::T_DynaObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}
