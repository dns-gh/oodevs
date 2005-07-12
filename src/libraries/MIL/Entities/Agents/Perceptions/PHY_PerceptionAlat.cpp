//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "MIL_pch.h"

#include "PHY_PerceptionAlat.h"

#include "PHY_PerceptionLevel.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "MIL_AgentServer.h"
#include "TER/TER_World.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionAlat::PHY_PerceptionAlat( PHY_RolePion_Perceiver& perceiver, const TER_Localisation& localisation )
    : PHY_Perception_ABC( perceiver )
    , localisation_     ( localisation )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionAlat::~PHY_PerceptionAlat()
{

}

// =============================================================================
// PERCEPTION POINT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat::Compute
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionAlat::Compute( const MT_Vector2D& /*vTargetPos*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// =============================================================================
// PERCEPTION AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionAlat::Compute( const DEC_Knowledge_Agent& /*target*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat::Compute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionAlat::Compute( const MIL_Agent_ABC& /*target*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_PerceptionAlat::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    const PHY_RolePion_Location& perceiverRoleLocation = perceiver_.GetPion().GetRole< PHY_RolePion_Location >(); 
    const MT_Float               rDetectionSemiHeight  = perceiver_.GetMaxAgentPerceptionDistance() / 2.;
    const PHY_RawVisionData&     rawVisionData         = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData();

    // Recherche des pions dans la localisation
    TER_Agent_ABC::T_AgentPtrVector agentsDetected;
    TER_World::GetWorld().GetListAgentWithinLocalisation( localisation_, agentsDetected );
   
    // Enregistrement des pions vus
    for( TER_Agent_ABC::CIT_AgentPtrVector itAgent = agentsDetected.begin(); itAgent != agentsDetected.end(); ++itAgent )
    {        
        PHY_RoleInterface_Location& targetRoleLocation = static_cast< PHY_RoleInterface_Location& >( **itAgent );
        if( targetRoleLocation.GetAgent().GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver_.GetPion() ) && fabs( ( targetRoleLocation.GetPosition() - perceiverRoleLocation.GetPosition() ) * perceiverRoleLocation.GetDirection() ) <= rDetectionSemiHeight )
        {
            const bool bPerceptionDelayed = ( rawVisionData.GetVisionObject( targetRoleLocation.GetPosition() ) != PHY_RawVisionData::eVisionEmpty );
            perceiver_.NotifyAgentPerception( targetRoleLocation.GetAgent(), PHY_PerceptionLevel::recognized_, bPerceptionDelayed );
        }
    }
}

// =============================================================================
// PERCEPTION OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionAlat::Compute( const DEC_Knowledge_Object& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionAlat::Compute( const MIL_RealObject_ABC& /*target*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionAlat::Execute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_PerceptionAlat::Execute( const TER_DynaObject_ABC::T_DynaObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}

