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
#include "MIL_AgentServer.h"
#include "MIL_ObjectFactory.h"
#include "MIL_Object_ABC.h"
#include "MIL_ObjectManipulator_ABC.h"
#include "MIL_Singletons.h"
#include "Hla/HLA_Federate.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Network/NET_Publisher_ABC.h"
#include <protocol/ClientSenders.h>
#include <urban/TerrainObject_ABC.h>
#include <xeumeuleu/xml.h>
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ObjectManager )

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
        if( it->second )
            delete it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> objects_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
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
    if( MIL_Singletons::GetHla() )
        MIL_Singletons::GetHla()->Register( object );
    if( ! objects_.insert( std::make_pair( object.GetID(), &object ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    object.SendCreation(); //$$$ a déplacer ...
    if( object.GetArmy() )
        object.GetArmy()->GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( object ); //$$$ A CHANGER DE PLACE QUAND REFACTOR OBJETS -- NB : ne doit pas être fait dans RealObject::InitializeCommon <= crash dans connaissance, si initialisation objet failed
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
MIL_Object_ABC& MIL_ObjectManager::CreateObject( xml::xistream& xis, MIL_Army_ABC& army )
{
    return builder_->BuildObject( xis, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode MIL_ObjectManager::CreateObject( const Common::MsgMissionParameters& message, const tools::Resolver< MIL_Army_ABC >& armies )
{  //@TODO MGD Try to externalize ASN when protobuff will be merged
   //@HBD : Verify later that conversion from MIL_Army to MIL_Army_ABC was right

    if( message.elem_size() != 5 ) // type, location, name, team, attributes
        return MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_specific_attributes;

    MIL_Army_ABC* pArmy = armies.Find( message.elem( 3 ).value().army().oid() );
    if( !pArmy )
        return MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_camp;
    return builder_->BuildObject( message, *pArmy );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation )
{
    return builder_->BuildObject( type, army, localisation, Common::ObstacleType_DemolitionTargetType_preliminary );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, Common::ObstacleType_DemolitionTargetType obstacleType )
{
    if( pLocalisation )
        return builder_->BuildObject( type, army, *pLocalisation, obstacleType );
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

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateUrbanObject
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateUrbanObject( const urban::TerrainObject_ABC& object )
{
    MIL_Object_ABC* obj = builder_->BuildUrbanObject( object );
    urbanIds_.push_back( obj->GetID() );
    return obj;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::ReadBlock
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void MIL_ObjectManager::ReadUrbanState( xml::xistream& xis )
{
    unsigned int id;
    xis >> xml::attribute( "id", id );
    UrbanObjectWrapper* wrapper = FindUrbanWrapper( id );
    if( wrapper )
    {
        xis >> xml::optional() >> xml::start( "capacities" )
            >> xml::list( *wrapper, &UrbanObjectWrapper::UpdateCapacities )
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::FindUrbanWrapper
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
UrbanObjectWrapper* MIL_ObjectManager::FindUrbanWrapper( unsigned int nId )
{
    for( std::vector< unsigned int >::const_iterator it = urbanIds_.begin(); it != urbanIds_.end(); ++it )
    {
        UrbanObjectWrapper* wrapper = static_cast< UrbanObjectWrapper* >( objects_[ *it ] );
        if( wrapper->GetObject().GetId() == nId )
            return wrapper;
    }
    return 0;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::SendCreation
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::SendCreation()
{
    for( IT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
    {
        MIL_Object_ABC& object = *it->second;
        object.SendCreation();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::SendFullState
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::SendFullState()
{
    for( IT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
    {
        MIL_Object_ABC& object = *it->second;
        object.SendFullState();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::OnReceiveMsgObjectMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveMsgObjectMagicAction( const MsgsClientToSim::MsgObjectMagicAction& msg, unsigned int nCtx, const tools::Resolver< MIL_Army_ABC >& armies )
{
    MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode nErrorCode = MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_no_error;

    if( msg.type() == MsgsClientToSim::MsgObjectMagicAction_Type_create )
    {
        nErrorCode = CreateObject( msg.parametres(), armies );
    }
    else if( msg.type() == MsgsClientToSim::MsgObjectMagicAction_Type_destroy )
    {
        MIL_Object_ABC* pObject = Find( msg.oid() );
        if( !pObject )
            nErrorCode = MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_object;
        else
            (*pObject)().Destroy();
    }

    else if( msg.type() == MsgsClientToSim::MsgObjectMagicAction_Type_update )
    {
        MIL_Object_ABC* pObject = Find( msg.oid() );
        if( !pObject )
            nErrorCode = MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_object;
        else
        {
            const Common::MsgMissionParameters& params = msg.parametres();
            if( params.elem_size() && params.elem( 0 ).has_value() && params.elem( 0 ).value().list_size() )
                nErrorCode = pObject->OnUpdate( params.elem( 0 ).value() );
        }
    }

    client::ObjectMagicActionAck asnReplyMsg;
    asnReplyMsg().set_error_code( nErrorCode );
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher(), nCtx );
}
