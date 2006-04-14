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

#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"

BOOST_CLASS_EXPORT_GUID( DEC_KS_Fire, "DEC_KS_Fire" )

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::DEC_KS_Fire( DEC_KnowledgeBlackBoard_AgentPion& blackBoard )
    : DEC_KnowledgeSource_ABC ( blackBoard, 1 )
    , pBlackBoard_            ( &blackBoard )
    , pionsAttacking_         ()
    , populationsAttacking_   ()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::DEC_KS_Fire()
    : DEC_KnowledgeSource_ABC ()
    , pBlackBoard_            ( 0 )
    , pionsAttacking_         ()
    , populationsAttacking_   ()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_Fire::~DEC_KS_Fire()
{
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

    const MIL_AgentPion& agentInteracting = pBlackBoard_->GetPion();
    
    for( CIT_PionSet itAttacker = pionsAttacking_.begin(); itAttacker != pionsAttacking_.end(); ++itAttacker )
    {
        MIL_AgentPion& attacker = **itAttacker;

        // On ne gére la connaissance que lorsque le tireur est à distance inférieure de la distance max de détection
        if( agentInteracting.GetRole< PHY_RolePion_Perceiver >().GetMaxAgentPerceptionDistance() <= agentInteracting.GetRole< PHY_RolePion_Location >().GetPosition().Distance( attacker.GetRole< PHY_RolePion_Location >().GetPosition() ) )
            continue;

        DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerceptionContainer().GetKnowledgeAgentPerception( attacker );
        if ( !pKnowledge )
            pKnowledge = &pBlackBoard_->GetKnowledgeAgentPerceptionContainer().CreateKnowledgeAgentPerception( agentInteracting, attacker );       
        pKnowledge->Update( PHY_PerceptionLevel::recognized_, false );
        pKnowledge->NotifyAttacker();
    }
    pionsAttacking_.clear();

    // Population
    for( CIT_PopulationSet itAttacker = populationsAttacking_.begin(); itAttacker != populationsAttacking_.end(); ++itAttacker )
    {
        DEC_Knowledge_PopulationPerception* pKnowledge = pBlackBoard_->GetKnowledgePopulationPerceptionContainer().GetKnowledgePopulationPerception( **itAttacker );
        if( pKnowledge )
            pKnowledge->NotifyAttacker();
    }
    populationsAttacking_.clear();
}
