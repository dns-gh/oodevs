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

#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"

#include "TER/TER_Localisation.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::DEC_KS_Fire( DEC_KnowledgeBlackBoard& blackBoard, MIL_AgentPion& agentInteracting )
    : DEC_KnowledgeSource_ABC ( blackBoard       )
    , pAgentInteracting_      ( &agentInteracting )
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
// Name: DEC_KS_Fire::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Fire::Prepare()
{
    assert( pBlackBoard_ );
    pBlackBoard_->NotifyKnowledgesAgentPerceptionNotAttackers();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_Fire::Talk()
{
    assert( pBlackBoard_ );
    
    // Direct fire
    for( CIT_DirectFireAttackerSet itAttacker = directFireAttackerSet_.begin(); itAttacker != directFireAttackerSet_.end(); ++itAttacker )
    {
        MIL_AgentPion& attacker = **itAttacker;

        // On ne gére la connaissance que lorsque le tireur est à distance inférieure de la distance max de détection
        if ( pAgentInteracting_->GetRole< PHY_RolePion_Perceiver >().GetMaxAgentPerceptionDistance() <= pAgentInteracting_->GetRole< PHY_RolePion_Location >().GetPosition().Distance( attacker.GetRole< PHY_RolePion_Location >().GetPosition() ) )
            continue;

        DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerception( attacker );
        if ( !pKnowledge )
        {
            assert( pAgentInteracting_ );
            pKnowledge = &pBlackBoard_->CreateKnowledgeAgentPerception( *pAgentInteracting_, attacker );
        }
        
        pKnowledge->Update( PHY_PerceptionLevel::recognized_, false );

        pBlackBoard_->NotifyKnowledgeAgentPerceptionIsAttacker( *pKnowledge );
    }
    directFireAttackerSet_.clear();
}
