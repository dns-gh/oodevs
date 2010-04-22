// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.cpp $
// $Author: Rft $
// $Modtime: 2/05/05 16:22 $
// $Revision: 9 $
// $Workfile: DEC_FireFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_NbcFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Entities/Objects/NBCTypeAttribute.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"

// -----------------------------------------------------------------------------
// Name: DEC_NbcFunctions::AnalyzeNbcAgent
// Created: RFT 2008
// -----------------------------------------------------------------------------
int DEC_NbcFunctions::AnalyzeNbcAgent( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge )
{
    assert( knowledge );
    MIL_Object_ABC* pObject = knowledge->GetObjectKnown();
    
    if( !pObject || pObject->IsMarkedForDestruction() )
    {
        return 0;
    }

    callerAgent.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );

    int value = 0; // $$$$ TODO : ( int ) pObject->GetAttribute< NBCTypeAttribute >().GetAgentType().GetID();
    return value;
}

// -----------------------------------------------------------------------------
// Name: DEC_NbcFunctions::AnalyzeNbcAgent
// Created: RFT 2008
// -----------------------------------------------------------------------------
void DEC_NbcFunctions::DecontaminateAgent( MIL_AgentPion& /*callerAgent*/ )
{
    //Changer le MIL_Injury_Contaminate
}

// -----------------------------------------------------------------------------
// Name: DEC_NbcFunctions::AnalyzeNbcAgent
// Created: RFT 2008
// -----------------------------------------------------------------------------
void DEC_NbcFunctions::DecontaminateSite( MIL_AgentPion& /*callerAgent*/ )
{
    //Changer la concentration en agent nbc
    //Eventuellement baisser la quantité de ressource disponible
    //Peut etre s'agit comme dans le cas de extinguish fire, d'une PHY_Action????
}