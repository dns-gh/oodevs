// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 16:22 $
// $Revision: 9 $
// $Workfile: DEC_FireFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_DynamicFireFunctions.h"

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Objects/MIL_FireFunctor.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Entities/Objects/FireAttribute.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"

namespace
{
    // -----------------------------------------------------------------------------
    // Name: GetFireHoseRange
    // Created: RFT 2004-09-16
    // -----------------------------------------------------------------------------
    int GetFireHoseRange( const MIL_AgentPion& pPion, MIL_FireFunctor& functor )
    {
        //pPion.GetRole< PHY_RolePion_Composantes >().Apply( functor );

        const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = pPion.GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
        for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        {
            //(*itReinforcement)->GetRole< PHY_RolePion_Composantes >().Apply( functor );
        }
    
        return functor.GetFireHoseRange();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetRangeToExtinguish
// Created: RFT 2008
// -----------------------------------------------------------------------------
float DEC_DynamicFireFunctions::GetRangeToExtinguish( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge )
{
    if( !knowledge || !knowledge->IsValid() )
    {
        //If the fire is extinguished the range is set to -1
        return -1.f;
    }
    MIL_Object_ABC* pObject = knowledge->GetObjectKnown();
    if( !pObject || pObject->IsMarkedForDestruction() )
    {
        return -1.f;
    }
    MIL_FireFunctor functor( pObject->GetAttribute< FireAttribute >().GetClass() );
    return ( float ) GetFireHoseRange( callerAgent, functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_DynamicFireFunctions::AnalyzeFireClass
// Created: RFT 2008
// -----------------------------------------------------------------------------
unsigned int  DEC_DynamicFireFunctions::AnalyzeFireClass( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge )
{
    //Si la distance entre le pion et l'objet est supérieur à un threshold, ne rien renvoye
    //Sinon renvoyé la classe!
    //TODO!!

    if( !knowledge || !knowledge->IsValid() )
    {
        //If the fire is extinguished the range is set to -1
        return 0;
    }
    MIL_Object_ABC* pObject = knowledge->GetObjectKnown();
    if( !pObject || pObject->IsMarkedForDestruction() )
    {
        return 0;
    }

    callerAgent.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );

    return ( int ) pObject->GetAttribute< FireAttribute >().GetClass().GetID();
}