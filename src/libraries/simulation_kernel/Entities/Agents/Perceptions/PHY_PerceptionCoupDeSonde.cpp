//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_PerceptionCoupDeSonde.h"
#include "PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_kernel/DetectionComputer.h"

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionCoupDeSonde::PHY_PerceptionCoupDeSonde( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
    , rWidth_           ( perceiver.GetPion().GetType().GetUnitType().GetCoupDeSondeWidth () )
    , rLength_          ( perceiver.GetPion().GetType().GetUnitType().GetCoupDeSondeLength() )
{
    assert( rLength_ >= rWidth_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PHY_PerceptionCoupDeSonde::~PHY_PerceptionCoupDeSonde()
{
    // NOTHING
}

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

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Compute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionCoupDeSonde::Compute( const MIL_Agent_ABC& target ) const
{
    if( target.BelongsTo( *perceiver_.GetKnowledgeGroup() ) || perceiver_.IsIdentified( target ) )
        return GetMaxHostilePerceptionLevel( perceiver_.GetPion(), target, PHY_PerceptionLevel::recognized_ );

    const MT_Vector2D& vSourcePos = GetPerceiverPosition();
    const MT_Vector2D& vTargetPos = target.GetRole< PHY_RoleInterface_Location >().GetPosition();
    if( vSourcePos.Distance( vTargetPos ) >  rLength_ )
        return PHY_PerceptionLevel::notSeen_;

    if( fabs( ( vSourcePos - vTargetPos ) * GetPerceiverDirection() ) <= rWidth_ )
        return GetMaxHostilePerceptionLevel( perceiver_.GetPion(), target, PHY_PerceptionLevel::recognized_ );

    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PHY_PerceptionCoupDeSonde::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{
    TER_Agent_ABC::T_AgentPtrVector vAgentDetectedList;
    TER_World::GetWorld().GetAgentManager().GetListWithinCircle( GetPerceiverPosition(), rLength_, vAgentDetectedList );

    for ( auto itAgent = vAgentDetectedList.begin(); itAgent != vAgentDetectedList.end(); ++itAgent )
    {
        MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **itAgent ).GetAgent();
        if( !DEC_Knowledge_Agent::detectDestroyedUnits_ && agent.IsDead() )
            continue;
        detection::DetectionComputer detectionComputer( agent );
        perceiver_.GetPion().Execute( detectionComputer );
        agent.Execute( detectionComputer );

        if( detectionComputer.CanBeSeen() )
            perceiver_.NotifyPerception( agent, Compute( agent ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionCoupDeSonde::FinalizePerception
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
void PHY_PerceptionCoupDeSonde::FinalizePerception()
{
    // NOTHING
}
