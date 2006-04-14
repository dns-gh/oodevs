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

#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_KnowledgeBlackBoard_Automate.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"

BOOST_CLASS_EXPORT_GUID( DEC_KS_RapFor, "DEC_KS_RapFor" )

// -----------------------------------------------------------------------------
// Name: DEC_KS_RapFor constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_RapFor::DEC_KS_RapFor( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 2 )
    , pBlackBoard_           ( &blackBoard )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_RapFor constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_RapFor::DEC_KS_RapFor()
    : DEC_KnowledgeSource_ABC( )
    , pBlackBoard_           ( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: DEC_KS_RapFor destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_RapFor::~DEC_KS_RapFor()
{
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
    assert( pBlackBoard_ );

    const T_KnowledgeAgentVector& enemies = pBlackBoard_->GetKnowledgeGroup().GetKnowledge().GetEnemies();
    const T_KnowledgeAgentVector& friends = pBlackBoard_->GetKnowledgeGroup().GetKnowledge().GetFriends();

    // Compute and update the RapFor for all the subordinates
    const MIL_KnowledgeGroup::T_AutomateVector& automates = pBlackBoard_->GetKnowledgeGroup().GetAutomates();
    for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
    {
        MIL_Automate& automate = **itAutomate;
        automate.GetKnowledge().GetKnowledgeRapForGlobal().Update( automate, enemies, friends );

        const MIL_Automate::T_PionVector& pions = automate.GetPions();
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            (**itPion).GetKnowledge().GetKnowledgeRapForLocal().Update( **itPion, enemies, friends );
    }
}

    
