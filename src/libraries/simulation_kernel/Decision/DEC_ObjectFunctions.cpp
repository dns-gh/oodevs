// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_ObjectFunctions.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 18:17 $
// $Revision: 3 $
// $Workfile: DEC_ObjectFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_ObjectFunctions.h"
#include "MIL_AgentServer.h"
#include "Decision/DEC_Gen_Object.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/ObstacleAttribute.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_World.h"
#include <boost/make_shared.hpp>

namespace
{


int MagicCreateObjectInArmy( MIL_Army_ABC& army, const std::string& type, const TER_Localisation& localisation )
{
        //$$$ A réencapsuler    
    MIL_Object_ABC* object = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( &army, type, &localisation, sword::ObstacleType_DemolitionTargetType_preliminary );
    if( !object )
        return 0;
    return object->GetID();
}
    
int MagicGetOrCreateObjectInArmy( MIL_Army_ABC& army, const std::string& type, const TER_Localisation& localisation )
{
    std::vector< TER_Object_ABC* > objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinCircle( localisation.ComputeBarycenter(), 1., objects );
    for( auto it = objects.begin(); it != objects.end(); ++it )
    {
        const TER_Object_ABC* object = *it;
        if( object && object->GetLocalisation() == localisation )
        {
            const MIL_Object_ABC* obj = dynamic_cast< const MIL_Object_ABC* >( object );
            if( obj && obj->GetType().GetName() == type && obj->GetArmy() == &army )
                return obj->GetID();
        }
    }
    return MagicCreateObjectInArmy( army, type, localisation );
}

}  // namespace

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectType
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
std::string DEC_ObjectFunctions::GetGenObjectType( const DEC_Gen_Object* object)
{
    if( !object )
        throw MASA_EXCEPTION( "invalid parameter." );
    return object->GetTypeName();
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectExternalIdentifier
// Created: LGY 2011-10-25
// -----------------------------------------------------------------------------
unsigned int DEC_ObjectFunctions::GetGenObjectExternalIdentifier( const DEC_Gen_Object* object )
{
    if( object )
        return object->GetExternalIdentifier();
    return 0u;
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectReservedObstacle
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
bool DEC_ObjectFunctions::GetGenObjectReservedObstacle( const DEC_Gen_Object* object )
{
    if( !object )
        throw MASA_EXCEPTION( "invalid parameter." );
    return !object->GetActivated();
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectTC2
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_ObjectFunctions::GetGenObjectTC2( const DEC_Gen_Object* object )
{
    if( !object )
        throw MASA_EXCEPTION( "invalid parameter." );
    DEC_Decision_ABC* dec = object->GetTC2() ? const_cast< DEC_Decision_ABC* >( &object->GetTC2()->GetDecision() ) : (DEC_Decision_ABC*)0;
    return dec;
}

namespace
{
    std::map< int, std::string > objectNames_;

    void RegisterObject( xml::xistream& xis )
    {
        int id;
        std::string type;
        xis >> xml::attribute( "id", id )
            >> xml::attribute( "type", type );
        objectNames_[ id ] = type;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::RegisterObjectNames
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::RegisterObjectNames( xml::xistream& xis )
{
    xis >> xml::start( "objects" )
        >> xml::list( "object", &RegisterObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::ConvertTypeObjectToString
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
std::string DEC_ObjectFunctions::ConvertTypeObjectToString( int id )
{
    return objectNames_[ id ];
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectLocalisation
// Created: LDC 2009-10-23
// -----------------------------------------------------------------------------
const TER_Localisation& DEC_ObjectFunctions::GetGenObjectLocalisation( boost::shared_ptr< DEC_Gen_Object > knowledgeId )
{
    if( !knowledgeId )
        throw MASA_EXCEPTION( "invalid parameter." );
    return knowledgeId->GetLocalisation();
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetGenObjectLocalisation
// Created: MIA 2013-06-18
// -----------------------------------------------------------------------------
bool DEC_ObjectFunctions::GetGenObjectMining( boost::shared_ptr< DEC_Gen_Object > knowledgeId )
{
    if( !knowledgeId )
        throw MASA_EXCEPTION( "invalid parameter." );
    return knowledgeId->GetMining();
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::GetObjectKnowledgeLocalisation
// Created: DDA 2011-10-26
// -----------------------------------------------------------------------------
const TER_Localisation& DEC_ObjectFunctions::GetObjectKnowledgeLocalisation( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pKnowledge->GetLocalisation();
}

// -----------------------------------------------------------------------------
// Name: static void DEC_ObjectFunctions::ActivateObject
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
bool DEC_ObjectFunctions::ActivateObject( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;
    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject || !( *pObject )().CanBeActivated() )
        return false;
    ( *pObject )().Activate();
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::MagicDestroyObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::MagicDestroyObject( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if( pObject && ( *pObject )().CanBeDestroyed() )
        {
            const MIL_Army_ABC* army = pKnowledge->GetArmy();
            if( army )
            {
                auto knowledges = pKnowledge->GetArmy()->GetKnowledgeGroups();
                for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
                {
                    auto bbKg = it->second->GetKnowledge();
                    if( bbKg )
                        bbKg->GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( pKnowledge );
                }
            }
            ( *pObject )().Destroy();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::MagicDestroyObjectId
// Created: LDC 2012-08-27
// -----------------------------------------------------------------------------
void DEC_ObjectFunctions::MagicDestroyObjectId( int objectId )
{
    MIL_EntityManager_ABC& entityManager = MIL_AgentServer::GetWorkspace().GetEntityManager();
    MIL_Object_ABC* pObject = entityManager.FindObject( objectId );
    if( pObject && ( *pObject )().CanBeDestroyed() )
    {
        ( *pObject )().Destroy();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::CreateDynamicGenObjectFromSharedLocalisation
// Created: MGD 2010-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Gen_Object > DEC_ObjectFunctions::CreateDynamicGenObjectFromSharedLocalisation( std::string type, boost::shared_ptr< TER_Localisation > location, bool preliminary )
{
    return boost::make_shared< DEC_Gen_Object >( type, location.get(), preliminary );
}

// -----------------------------------------------------------------------------
// Name: DEC_ObjectFunctions::CreateDynamicGenObject
// Created: ABR 2012-03-30
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Gen_Object > DEC_ObjectFunctions::CreateDynamicGenObject( std::string type, TER_Localisation* location, bool preliminary )
{
    return boost::make_shared< DEC_Gen_Object >( type, location, preliminary );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::MagicCreateObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
int DEC_ObjectFunctions::MagicCreateObject( const MIL_Entity_ABC& caller,
        const std::string& type, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        return 0;
    return MagicCreateObjectInArmy( caller.GetArmy(), type, *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static int DEC_ObjectFunctions::MagicGetOrCreateObject
// Created: LDC 2013-02-11
// -----------------------------------------------------------------------------
int DEC_ObjectFunctions::MagicGetOrCreateObject( const MIL_Entity_ABC& caller,
        const std::string& type, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        return 0;
    return MagicGetOrCreateObjectInArmy( caller.GetArmy(), type, *pLocalisation );
}
