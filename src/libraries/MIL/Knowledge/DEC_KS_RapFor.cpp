// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_RapFor.cpp $
// $Author: Jvt $
// $Modtime: 24/03/05 13:12 $
// $Revision: 2 $
// $Workfile: DEC_KS_RapFor.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_RapFor.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_KS_AgentQuerier.h"
#include "DEC_KS_KnowledgeGroupQuerier.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_RapFor constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_RapFor::DEC_KS_RapFor( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup )
    : DEC_KnowledgeSource_ABC( blackBoard, 2 )
    , pKnowledgeGroup_       ( &knowledgeGroup )
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this );        
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_RapFor destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_RapFor::~DEC_KS_RapFor()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );        
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_RapFor::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_RapFor::Prepare()
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_RapFor::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_RapFor::Talk()
{
    T_KnowledgeAgentVector enemies;
    T_KnowledgeAgentVector friends;

    // Get the knowledge group's enemies and friends
    assert( pKnowledgeGroup_ );
    pKnowledgeGroup_->GetKSQuerier().GetEnemies( enemies );
    pKnowledgeGroup_->GetKSQuerier().GetFriends( friends );

    // Compute and update the RapFor for all the subordinates
    const MIL_KnowledgeGroup::T_AutomateVector& automates = pKnowledgeGroup_->GetAutomates();
    for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
    {
        MIL_Automate& automate = **itAutomate;
        automate.GetKSQuerier().GetKnowledgeRapForGlobal().Update( automate, enemies, friends );

        const MIL_Automate::T_PionVector& pions = automate.GetPions();
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            (**itPion).GetKSQuerier().GetKnowledgeRapForLocal().Update( **itPion, enemies, friends );
    }
}

    