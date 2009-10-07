// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ObjectManager.cpp $
// $Author: Nld $
// $Modtime: 10/06/05 10:18 $
// $Revision: 9 $
// $Workfile: MIL_ObjectManager.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ObjectManager.h"
#include "MIL_ObjectFactory.h"
#include "MIL_Object_ABC.h"
#include "MIL_ObjectManipulator_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Hla/HLA_Federate.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( MIL_ObjectManager, "MIL_ObjectManager" )

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_ObjectManager::MIL_ObjectManager()
    : builder_ ( new MIL_ObjectFactory( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_ObjectManager::~MIL_ObjectManager()
{
    for( CIT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
        if ( it->second )
            delete it->second;
}


// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> objects_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << objects_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::ProcessEvents
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::ProcessEvents()
{
    for( CIT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
        it->second->ProcessEvents();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::UpdateStates
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::UpdateStates()
{
    for( IT_ObjectMap it = objects_.begin(); it != objects_.end(); )
    {
        MIL_Object_ABC& object = *it->second;
        if( object.IsReadyForDeletion() )
        {
            object.SendDestruction();
            delete &object;
            it = objects_.erase( it );
        }
        else
        {
            object.UpdateState();   
            ++it;
        }
    }
}

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::RegisterObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_ObjectManager::RegisterObject( MIL_Object_ABC& object )
{
    if( MIL_AgentServer::GetWorkspace().GetHLAFederate() )
        MIL_AgentServer::GetWorkspace().GetHLAFederate()->Register( object );
    bool bOut = objects_.insert( std::make_pair( object.GetID(), &object ) ).second;
    assert( bOut );
    object.SendCreation(); //$$$ a déplacer ...
    object.GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( object ); //$$$ A CHANGER DE PLACE QUAND REFACTOR OBJETS -- NB : ne doit pas être fait dans RealObject::InitializeCommon <= crash dans connaissance, si initialisation objet failed
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::GetType
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_ObjectManager::FindType( const std::string& type ) const
{
    return builder_->FindType( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::CreateObject( xml::xistream& xis, MIL_Army_ABC& army )
{
    builder_->BuildObject( xis, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ObjectManager::CreateObject( const ASN1T_MagicActionCreateObject& asn )
{  
    MIL_Army* pArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( asn.team );
    if( !pArmy )
        return EnumObjectErrorCode::error_invalid_camp;
    return builder_->BuildObject( asn, *pArmy );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation )
{    
    std::vector< double > noParameter;
    return builder_->BuildObject( type, army, localisation, EnumDemolitionTargetType::preliminary, noParameter );    
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, ASN1T_EnumDemolitionTargetType obstacleType, const std::vector< double >& parameters )
{
	if ( pLocalisation )
        return builder_->BuildObject( type, army, *pLocalisation, obstacleType, parameters );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder )
{
    return builder_->BuildObject( builder, army );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::OnReceiveMsgObjectMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveMsgObjectMagicAction( const ASN1T_MsgObjectMagicAction& asnMsg, uint nCtx )
{
    ASN1T_EnumObjectErrorCode nErrorCode = EnumObjectErrorCode::no_error;

    switch ( asnMsg.action.t )
    {
    case T_MsgObjectMagicAction_action_create_object:
        nErrorCode = CreateObject( *asnMsg.action.u.create_object );
        break;
    case T_MsgObjectMagicAction_action_destroy_object:
        {
            MIL_Object_ABC* pObject = Find( asnMsg.action.u.destroy_object );
            if( !pObject )
                nErrorCode = EnumObjectErrorCode::error_invalid_object;
            else
                (*pObject)().Destroy();
            break;
        }
    case T_MsgObjectMagicAction_action_update_object:
        {
            MIL_Object_ABC* pObject = Find( asnMsg.action.u.update_object->oid );
            if( !pObject )
                nErrorCode = EnumObjectErrorCode::error_invalid_object;
            else
                nErrorCode = pObject->OnUpdate( asnMsg.action.u.update_object->attributes );
            break;
        }
    }
    
    NET_ASN_MsgObjectMagicActionAck asnReplyMsg;
    asnReplyMsg().error_code = nErrorCode;
    asnReplyMsg.Send( nCtx );
}
