// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_ObjectFunctions.inl $
// $Author: Nld $
// $Modtime: 23/02/05 15:07 $
// $Revision: 3 $
// $Workfile: DEC_ObjectFunctions.inl $
//
// *****************************************************************************

#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/MIL_Army.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "DEC_FunctionsTools.h"

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::MagicCreateObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_ObjectFunctions::MagicCreateObject( const T& caller, const std::string& type, const TER_Localisation* pLocalisation )
{
    //$$$ A réencapsuler    
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( caller.GetArmy(), type, pLocalisation, EnumDemolitionTargetType::preliminary );
}
