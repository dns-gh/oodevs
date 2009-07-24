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
#include "DEC_ExtinguishFireFunctions.h"

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Objects/MIL_FireFunctor.h"
#include "Entities/Objects/MIL_FireClass.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Entities/Objects/FireAttribute.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Decision/DEC_Tools.h"

namespace
{
    // -----------------------------------------------------------------------------
    // Name: GetFireHoseRange
    // Created: RFT 2004-09-16
    // -----------------------------------------------------------------------------
    int GetFireHoseRange( const MIL_AgentPion& pPion, MIL_FireFunctor& functor )
    {
        pPion.GetRole< PHY_RolePion_Composantes >().Apply( functor );

        const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = pPion.GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
        for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
            (*itReinforcement)->GetRole< PHY_RolePion_Composantes >().Apply( functor );
    
        return functor.GetFireHoseRange();
    }

    // -----------------------------------------------------------------------------
    // Name: GetObject
    // Created: RFT
    // -----------------------------------------------------------------------------
    MIL_Object_ABC* GetObject( MIL_AgentPion& pPion, uint nKnowledgeObjectID )
    {
        DEC_Knowledge_Object* pKnowledge = pPion.GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nKnowledgeObjectID );
        if( !pKnowledge )
            return 0;

        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if( pObject )
          return pObject;

        pPion.GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
        return 0;
    }
}