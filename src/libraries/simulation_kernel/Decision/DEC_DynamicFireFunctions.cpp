// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_DynamicFireFunctions.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Objects/MIL_FireFunctor.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Entities/Objects/FireAttribute.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"

/*
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
}*/

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetRangeToExtinguish
// Created: RFT 2008
// -----------------------------------------------------------------------------
float DEC_DynamicFireFunctions::GetRangeToExtinguish( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge )
{
    //if( !knowledge || !knowledge->IsValid() )
    //    return -1.f; //If the fire is extinguished the range is set to -1
    //MIL_Object_ABC* pObject = knowledge->GetObjectKnown();
    //if( !pObject || pObject->IsMarkedForDestruction() )
    //    return -1.f;
    //MIL_FireFunctor functor( pObject->GetAttribute< FireAttribute >().GetClass() );
    //return static_cast< float >( GetFireHoseRange( callerAgent, functor ) );

    int todo = 0;
    return -1.0f;
}

// -----------------------------------------------------------------------------
// Name: DEC_DynamicFireFunctions::AnalyzeFireClass
// Created: RFT 2008
// -----------------------------------------------------------------------------
//unsigned int  DEC_DynamicFireFunctions::AnalyzeFireClass( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge )
//{
//    //Si la distance entre le pion et l'objet est supérieur à un threshold, ne rien renvoye
//    //Sinon renvoyé la classe!
//    //TODO!!
//    if( !knowledge || !knowledge->IsValid() )
//        return 0; //If the fire is extinguished the range is set to -1
//    MIL_Object_ABC* pObject = knowledge->GetObjectKnown();
//    if( !pObject || pObject->IsMarkedForDestruction() )
//        return 0;
//    callerAgent.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
//    return static_cast< int >( pObject->GetAttribute< FireAttribute >().GetClass().GetID() );
//}
