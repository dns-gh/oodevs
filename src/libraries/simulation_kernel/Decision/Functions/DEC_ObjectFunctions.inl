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

/*
namespace 
{
    template <typename Functor> 
    bool HasCapacity( MIL_Object_ABC* pObject, Functor& functor )
    {
        if ( pObject )        
            return (*pObject)().functor();
        return false;
    }
}
*/

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::ActivateObject
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
template< typename T > 
bool DEC_ObjectFunctions::ActivateObject( const T& caller, unsigned int nID )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( nID, caller.GetArmy() );
    if( !pKnowledge )
        return false;

    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject || !(*pObject)().CanBeActivated() )
        return false;

    (*pObject)().Activate();
    return true;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::MagicCreateObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_ObjectFunctions::MagicCreateObject( const T& caller, const std::string& type, const TER_Localisation* pLocalisation )
{
    //$$$ A réencapsuler    
    std::vector< double > noParameter;
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( caller.GetArmy(), type, pLocalisation, EnumDemolitionTargetType::preliminary, noParameter );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::MagicDestroyObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_ObjectFunctions::MagicDestroyObject( const T& caller, int knowledgeId )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( knowledgeId, caller.GetArmy() );
    if( pKnowledge )
    {
        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if( pObject && (*pObject)().CanBeDestroyed() )
            (*pObject)().Destroy(); // AddObjectKnowledgeToForget done
    }
}
