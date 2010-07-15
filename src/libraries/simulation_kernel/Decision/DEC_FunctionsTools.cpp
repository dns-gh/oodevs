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
#include "Entities/MIL_EntityManager.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
MIL_Object_ABC* DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( unsigned int nID )
{
    return MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( nID );
}
