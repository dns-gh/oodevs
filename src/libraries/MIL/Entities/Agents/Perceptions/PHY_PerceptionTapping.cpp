// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionTapping.cpp $
// $Author: Nld $
// $Modtime: 1/04/05 10:35 $
// $Revision: 7 $
// $Workfile: PHY_PerceptionTapping.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_PerceptionTapping.h"

#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Tools/MIL_Tools.h"

#include "TER_World.h"

MT_Float PHY_PerceptionTapping::rRadius_                   = 0.;
MT_Float PHY_PerceptionTapping::rNonPcDetectionTime_       = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionTapping::rNonPcRecognitionTime_     = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionTapping::rNonPcIdentificationTime_  = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionTapping::rPcDetectionTime_          = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionTapping::rPcRecognitionTime_        = std::numeric_limits< MT_Float >::max();
MT_Float PHY_PerceptionTapping::rPcIdentificationTime_     = std::numeric_limits< MT_Float >::max();

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::PHY_PerceptionTapping::sAgentPerceptionData::sAgentPerceptionData
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
PHY_PerceptionTapping::sAgentPerceptionData::sAgentPerceptionData()
    : nFirstTimeStepPerceived_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    , bUpdated_               ( true )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::sAgentPerceptionData
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
PHY_PerceptionTapping::sAgentPerceptionData::sAgentPerceptionData( const sAgentPerceptionData& rhs )
    : nFirstTimeStepPerceived_( rhs.nFirstTimeStepPerceived_ )
    , bUpdated_               ( rhs.bUpdated_ )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::sTapping constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionTapping::sTapping::sTapping( const TER_Localisation& localisation )
    : localisation_    ( localisation )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::sTapping::GetAgentsInside
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
void PHY_PerceptionTapping::sTapping::GetAgentsInside( TER_Agent_ABC::T_AgentPtrVector& result ) const
{
    result.clear();
    TER_World::GetWorld().GetListAgentWithinLocalisation( localisation_, result );
}

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::Initialize
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
void PHY_PerceptionTapping::Initialize( MIL_InputArchive& archive )
{
    archive.Section( "Ecoute" );

    archive.ReadField( "RayonAction", rRadius_, CheckValueGreaterOrEqual( 0. ) );
    rRadius_ = MIL_Tools::ConvertMeterToSim( rRadius_ );

    MT_Float rMinBound = 0.;
    archive.Section( "PionPC" );    
    if( archive.ReadTimeField( "TempsDetection", rPcDetectionTime_, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) ) 
        rMinBound = rPcDetectionTime_;   
    if( archive.ReadTimeField( "TempsReconnaissance", rPcRecognitionTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
        rMinBound = rPcRecognitionTime_;
    archive.ReadTimeField( "TempsIdentification", rPcIdentificationTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

    if( rPcDetectionTime_ != std::numeric_limits< MT_Float >::max() )
        rPcDetectionTime_ = MIL_Tools::ConvertSecondsToSim( rPcDetectionTime_ );

    if( rPcRecognitionTime_ != std::numeric_limits< MT_Float >::max() )
        rPcRecognitionTime_ = MIL_Tools::ConvertSecondsToSim( rPcRecognitionTime_ );

    if( rPcIdentificationTime_ != std::numeric_limits< MT_Float >::max() )
        rPcIdentificationTime_ = MIL_Tools::ConvertSecondsToSim( rPcIdentificationTime_ );
    archive.EndSection(); // PionPC

    rMinBound = 0.;
    archive.Section( "PionNonPC" );    
    if( archive.ReadTimeField( "TempsDetection", rNonPcDetectionTime_, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) ) 
        rMinBound = rNonPcDetectionTime_;   
    if( archive.ReadTimeField( "TempsReconnaissance", rNonPcRecognitionTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
        rMinBound = rNonPcRecognitionTime_;
    archive.ReadTimeField( "TempsIdentification", rNonPcIdentificationTime_, CheckValueGreaterOrEqual( rMinBound ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

    if( rNonPcDetectionTime_ != std::numeric_limits< MT_Float >::max() )
        rNonPcDetectionTime_ = MIL_Tools::ConvertSecondsToSim( rNonPcDetectionTime_ );

    if( rNonPcRecognitionTime_ != std::numeric_limits< MT_Float >::max() )
        rNonPcRecognitionTime_ = MIL_Tools::ConvertSecondsToSim( rNonPcRecognitionTime_ );

    if( rNonPcIdentificationTime_ != std::numeric_limits< MT_Float >::max() )
        rNonPcIdentificationTime_ = MIL_Tools::ConvertSecondsToSim( rNonPcIdentificationTime_ );
    archive.EndSection(); // PionNonPC

    archive.EndSection(); // Ecoute
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::Terminate
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
void PHY_PerceptionTapping::Terminate()
{
    // NOTHING
}


// =============================================================================
// INSTANCE
// =============================================================================


// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionTapping::PHY_PerceptionTapping( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::~PHY_PerceptionTapping
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PHY_PerceptionTapping::~PHY_PerceptionTapping()
{
    for( IT_TappingVector it = tappings_.begin(); it != tappings_.end(); ++it )
        delete *it;
    tappings_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::AddTapping
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void* PHY_PerceptionTapping::AddTapping( const TER_Localisation& localisation )
{
    sTapping* pNewTapping = new sTapping( localisation );
    
    assert( pNewTapping );
    tappings_.push_back( pNewTapping );
    return pNewTapping;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::RemoveTapping
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PHY_PerceptionTapping::RemoveTapping( void* pId )
{
    sTapping* pTapping = static_cast< sTapping* >( pId );
    IT_TappingVector it = std::find( tappings_.begin(), tappings_.end(), pTapping );

    if ( it != tappings_.end() )
    {
        delete pTapping;
        tappings_.erase( it );    
    }
}

// =============================================================================
// AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTapping::Compute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const MT_Vector2D& /*vPoint*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::ComputeLevel
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& PHY_PerceptionTapping::ComputeLevel( const MIL_Agent_ABC& agent, uint nFirstTimeStepPerceived ) const
{
    if( !agent.GetRole< PHY_RoleInterface_Communications >().CanCommunicate() )
        return PHY_PerceptionLevel::notSeen_;

    const uint nTimePerceived = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() - nFirstTimeStepPerceived;

    if( agent.IsPC() )
    {
        if( nTimePerceived >= rPcIdentificationTime_ )
            return PHY_PerceptionLevel::identified_;
        if( nTimePerceived >= rPcRecognitionTime_ )
            return PHY_PerceptionLevel::recognized_;
        if( nTimePerceived >= rPcDetectionTime_ )
            return PHY_PerceptionLevel::detected_;
        return PHY_PerceptionLevel::notSeen_;
    }
    else 
    {
        if( nTimePerceived >= rNonPcIdentificationTime_ )
            return PHY_PerceptionLevel::identified_;
        if( nTimePerceived >= rNonPcRecognitionTime_ )
            return PHY_PerceptionLevel::recognized_;
        if( nTimePerceived >= rNonPcDetectionTime_ )
            return PHY_PerceptionLevel::detected_;
        return PHY_PerceptionLevel::notSeen_;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionTapping::Execute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    const MT_Vector2D& vPerceiverPos = perceiver_.GetPion().GetRole< PHY_RolePion_Location >().GetPosition();

    for( CIT_TappingVector itTapping = tappings_.begin(); itTapping != tappings_.end(); ++itTapping  )
    {
        TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
        (*itTapping )->GetAgentsInside( perceivableAgents );

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
// Name: PHY_PerceptionTapping::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTapping::Compute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const MIL_Agent_ABC& /*agent*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTapping::Compute( const PHY_RolePion_Perceiver::T_SurfaceAgentMap& /*surfaces*/, const DEC_Knowledge_Agent & /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// =============================================================================
// OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTapping::Compute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const MIL_RealObject_ABC& /*object*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionTapping::Compute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const DEC_Knowledge_Object& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionTapping::Execute( const PHY_RolePion_Perceiver::T_SurfaceObjectMap& /*surfaces*/, const TER_DynaObject_ABC::T_DynaObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}
