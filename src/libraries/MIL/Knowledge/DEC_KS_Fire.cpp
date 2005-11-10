// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Fire.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 16:32 $
// $Revision: 6 $
// $Workfile: DEC_KS_Fire.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_Fire.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"

#include "TER/TER_Localisation.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::DEC_KS_Fire( DEC_KnowledgeBlackBoard& blackBoard, MIL_AgentPion& agentInteracting )
    : DEC_KnowledgeSource_ABC ( blackBoard       )
    , pAgentInteracting_      ( &agentInteracting )
    , pionsAttacking_         ()
    , concentrationsAttacking_()
    , flowsAttacking_         ()
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this );
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::~DEC_KS_Fire()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Fire::Talk()
{
    assert( pBlackBoard_ );
    assert( pAgentInteracting_ );
    
    for( CIT_PionSet itAttacker = pionsAttacking_.begin(); itAttacker != pionsAttacking_.end(); ++itAttacker )
    {
        MIL_AgentPion& attacker = **itAttacker;

        // On ne gére la connaissance que lorsque le tireur est à distance inférieure de la distance max de détection
        if ( pAgentInteracting_->GetRole< PHY_RolePion_Perceiver >().GetMaxAgentPerceptionDistance() <= pAgentInteracting_->GetRole< PHY_RolePion_Location >().GetPosition().Distance( attacker.GetRole< PHY_RolePion_Location >().GetPosition() ) )
            continue;

        DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerception( attacker );
        if ( !pKnowledge )
            pKnowledge = &pBlackBoard_->CreateKnowledgeAgentPerception( *pAgentInteracting_, attacker );       
        pKnowledge->Update( PHY_PerceptionLevel::recognized_, false );
        pKnowledge->NotifyAttacker();
    }
    pionsAttacking_.clear();

    // Population
    for( CIT_ConcentrationSet itAttacker = concentrationsAttacking_.begin(); itAttacker != concentrationsAttacking_.end(); ++itAttacker )
    {
        MIL_PopulationConcentration& concentration = **itAttacker;
        DEC_Knowledge_PopulationPerception* pKnowledge = pBlackBoard_->GetKnowledgePopulationPerception( concentration.GetPopulation() );
        if( pKnowledge )
            pKnowledge->NotifyAttacker();
    }
    concentrationsAttacking_.clear();

    for( CIT_FlowSet itAttacker = flowsAttacking_.begin(); itAttacker != flowsAttacking_.end(); ++itAttacker )
    {
        MIL_PopulationFlow& flow = **itAttacker;
        DEC_Knowledge_PopulationPerception* pKnowledge = pBlackBoard_->GetKnowledgePopulationPerception( flow.GetPopulation() );
        if( pKnowledge )
            pKnowledge->NotifyAttacker();
    }
    flowsAttacking_.clear();
}
