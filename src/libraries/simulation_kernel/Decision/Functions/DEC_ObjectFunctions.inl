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
#include "Entities/Automates/MIL_Automate.h"
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
void DEC_ObjectFunctions::ActivateObject( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( !pKnowledge )
    {
        call.GetResult().SetValue( false );
        return;
    }

    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject || !(*pObject)().CanBeActivated() )
    {
        call.GetResult().SetValue( false );
        return;
    }

    (*pObject)().Activate();
    call.GetResult().SetValue( true );
    return;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::MagicCreateObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_ObjectFunctions::MagicCreateObject( DIA_Call_ABC& call, const T& caller )
{
    //$$$ A réencapsuler
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( caller.GetArmy(), call.GetParameters(), 0, EnumDemolitionTargetType::preliminary );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::MagicDestroyObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_ObjectFunctions::MagicDestroyObject( DIA_Call_ABC& call, const T& caller )
{
    DEC_Tools::CheckTypeConnaissanceObjet( call.GetParameter( 0 ) );

    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), caller.GetArmy() );
    if( pKnowledge )
    {
        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if( pObject && (*pObject)().CanBeDestroyed() )
            (*pObject)().Destroy(); // AddObjectKnowledgeToForget done
    }
}