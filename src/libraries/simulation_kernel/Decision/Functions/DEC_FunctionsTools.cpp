// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FunctionsTools.cpp $
// $Author: Jvt $
// $Modtime: 17/03/05 17:03 $
// $Revision: 5 $
// $Workfile: DEC_FunctionsTools.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_FunctionsTools.h"

#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Decision/DEC_Tools.h"
#include "MIL_AgentServer.h"

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_FunctionsTools::GetKnowledgeObjectFromDia
// Created: NLD 2004-03-31
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_FunctionsTools::GetKnowledgeObjectFromDia( unsigned int nKnowledgeID, const MIL_Army_ABC& caller )
{
    return caller.GetKnowledge().GetKnowledgeObjectFromID( nKnowledgeID ); 
}

// =============================================================================
// POPULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
MIL_Object_ABC* DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( unsigned int nID )
{
    return MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( nID );
}
