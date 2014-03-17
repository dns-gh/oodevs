// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionIllumination.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination constructor
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
PHY_ActionIllumination::PHY_ActionIllumination( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, DEC_Decision_ABC* allie )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_                       ( pion.GetRole< PHY_RoleAction_DirectFiring >() )
    , pEnemy_                     ( pEnemy )
    , knowledgeGroup_             ( allie->GetPion().GetKnowledgeGroup() )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination destructor
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
PHY_ActionIllumination::~PHY_ActionIllumination()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionIllumination::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination::Execute
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
void PHY_ActionIllumination::Execute()
{
    int nResult = role_.IlluminatePion( pEnemy_ );
    if( nResult == 4 || nResult == 5 )
    {
        auto bbKg = knowledgeGroup_->GetKnowledge();
        if( bbKg && !bbKg->IsKnown( pEnemy_->GetAgentKnown() ) )
            bbKg->GetKnowledgeAgentContainer().CreateKnowledgeAgent( *knowledgeGroup_, pEnemy_->GetAgentKnown() );
    }
    Callback( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination::ExecuteSuspended
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_ActionIllumination::ExecuteSuspended()
{
    role_.FirePionSuspended( pEnemy_, false );
}
