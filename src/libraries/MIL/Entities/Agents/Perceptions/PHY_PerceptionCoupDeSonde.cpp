//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "MIL_pch.h"

#include "PHY_PerceptionCoupDeSonde.h"

#include "PHY_PerceptionLevel.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "TER/TER_World.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionCoupDeSonde::PHY_PerceptionCoupDeSonde( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
    , rWidth_            ( perceiver.GetPion().GetType().GetUnitType().GetCoupDeSondeWidth () )
    , rLength_           ( perceiver.GetPion().GetType().GetUnitType().GetCoupDeSondeLength() )
{
    assert( rLength_ >= rWidth_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionCoupDeSonde::~PHY_PerceptionCoupDeSonde()
{

}

// =============================================================================
// PERCEPTION POINT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Compute
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionCoupDeSonde::Compute( const MT_Vector2D& vTargetPos ) const
{
    const MT_Vector2D& vSourcePos = GetPerceiverPosition();
    if( vSourcePos.Distance( vTargetPos ) >  rLength_ )
        return PHY_PerceptionLevel::notSeen_;

    if( fabs( ( vSourcePos - vTargetPos ) * GetPerceiverDirection() ) <= rWidth_ )
        return PHY_PerceptionLevel::recognized_;

    return PHY_PerceptionLevel::notSeen_;
}

// =============================================================================
// PERCEPTION AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionCoupDeSonde::Compute( const DEC_Knowledge_Agent& target ) const
{
    const MT_Vector2D& vSourcePos = GetPerceiverPosition();
    const MT_Vector2D& vTargetPos = target.GetPosition();
    if( vSourcePos.Distance( vTargetPos ) >  rLength_ )
        return PHY_PerceptionLevel::notSeen_;

    if( fabs( ( vSourcePos - vTargetPos ) * GetPerceiverDirection() ) <= rWidth_ )
        return PHY_PerceptionLevel::recognized_;

    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Compute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionCoupDeSonde::Compute( const MIL_Agent_ABC& target ) const
{
    if( !target.GetRole< PHY_RoleInterface_Posture >().CanBePerceived( perceiver_.GetPion() ) )
       return PHY_PerceptionLevel::notSeen_;

    if( target.BelongsTo( perceiver_.GetKnowledgeGroup() ) || perceiver_.IsIdentified( target ) )
        return PHY_PerceptionLevel::recognized_;

    const MT_Vector2D& vSourcePos = GetPerceiverPosition();
    const MT_Vector2D& vTargetPos = target.GetRole< PHY_RoleInterface_Location >().GetPosition();
    if( vSourcePos.Distance( vTargetPos ) >  rLength_ )
        return PHY_PerceptionLevel::notSeen_;

    if( fabs( ( vSourcePos - vTargetPos ) * GetPerceiverDirection() ) <= rWidth_ )
        return PHY_PerceptionLevel::recognized_;

    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_PerceptionCoupDeSonde::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    TER_Agent_ABC::T_AgentPtrVector vAgentDetectedList;
    TER_World::GetWorld().GetListAgentWithinCircle( GetPerceiverPosition(), rLength_, vAgentDetectedList );

    for ( TER_Agent_ABC::CIT_AgentPtrVector itAgent = vAgentDetectedList.begin(); itAgent != vAgentDetectedList.end(); ++itAgent )
    {
        MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **itAgent ).GetAgent();
        perceiver_.NotifyAgentPerception( agent, Compute( agent ) );
    }
}

// =============================================================================
// PERCEPTION OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionCoupDeSonde::Compute( const DEC_Knowledge_Object& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionCoupDeSonde::Compute( const MIL_RealObject_ABC& /*target*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Execute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_PerceptionCoupDeSonde::Execute( const TER_DynaObject_ABC::T_DynaObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}

