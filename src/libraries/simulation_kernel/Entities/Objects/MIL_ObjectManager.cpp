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
#include "CapacityFactory.h"
#include "MIL_ObjectManager.h"
#include "MIL_AgentServer.h"
#include "MIL_ObjectFactory.h"
#include "MIL_Object_ABC.h"
#include "MIL_ObjectLoader.h"
#include "MIL_ObjectManipulator_ABC.h"
#include "MIL_Singletons.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Network/NET_Publisher_ABC.h"
#include "MT_Tools/MT_ScipioException.h"
#include "protocol/ClientSenders.h"
#include <urban/TerrainObject_ABC.h>
#include <hla/HLA_Federate.h>
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ObjectManager )

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_ObjectManager::MIL_ObjectManager()
    : builder_( new MIL_ObjectFactory() )
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
            if( MIL_Singletons::GetHla() )
                MIL_Singletons::GetHla()->Unregister( object );
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

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::RegisterObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_ObjectManager::RegisterObject( MIL_Object_ABC* pObject )
{
    if( !pObject )
        return;
    if( MIL_Singletons::GetHla() )
        MIL_Singletons::GetHla()->Register( *pObject );
    RegisterDistantObject( pObject );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::RegisterDistantObject
// Created: SLI 2010-10-04
// -----------------------------------------------------------------------------
void MIL_ObjectManager::RegisterDistantObject( MIL_Object_ABC* pObject )
{
    if( !pObject )
        return;
    if( !objects_.insert( std::make_pair( pObject->GetID(), pObject ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    pObject->SendCreation(); //$$$ a d�placer ...
    if( pObject->GetArmy() )
        pObject->GetArmy()->GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( *pObject ); //$$$ A CHANGER DE PLACE QUAND REFACTOR OBJETS -- NB : ne doit pas �tre fait dans RealObject::InitializeCommon <= crash dans connaissance, si initialisation objet failed
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
    MIL_Object_ABC* pObject = builder_->BuildObject( xis, army );
    if( !pObject )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown object" ); //@TODO MGD propagate reference
    RegisterObject( pObject );
    return *pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode MIL_ObjectManager::CreateObject( const sword::MissionParameters& message, const tools::Resolver< MIL_Army_ABC >& armies )
{  //@TODO MGD Try to externalize ASN when protobuff will be merged
   //@HBD : Verify later that conversion from MIL_Army to MIL_Army_ABC was right
    if( message.elem_size() != 5 ) // type, location, name, team, attributes
        return sword::ObjectMagicActionAck::error_invalid_specific_attributes;
    MIL_Army_ABC* pArmy = armies.Find( message.elem( 3 ).value().Get( 0 ).party().id() );
    if( !pArmy )
        return sword::ObjectMagicActionAck::error_invalid_party;
    sword::ObjectMagicActionAck_ErrorCode value = sword::ObjectMagicActionAck::no_error;
    RegisterObject( builder_->BuildObject( message, *pArmy, value ) );
    return value;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation )
{
    MIL_Object_ABC* pObject = builder_->BuildObject( "", type, army, localisation, sword::ObstacleType_DemolitionTargetType_preliminary );
    RegisterObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateDistantObject
// Created: SLI 2010-10-04
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateDistantObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, const std::string& name )
{
    MIL_Object_ABC* pObject = builder_->BuildObject( name, type, army, localisation, sword::ObstacleType_DemolitionTargetType_preliminary );
    RegisterDistantObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, sword::ObstacleType_DemolitionTargetType obstacleType )
{
    if( pLocalisation )
    {
        MIL_Object_ABC* pObject = builder_->BuildObject( "", type, army, *pLocalisation, obstacleType );
        RegisterObject( pObject );
        return pObject;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder )
{
    MIL_Object_ABC* pObject = builder_->BuildObject( builder, army );
    RegisterObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateUrbanObject
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateUrbanObject( const urban::TerrainObject_ABC& object )
{
    MIL_Object_ABC* pObject = builder_->BuildUrbanObject( object );
    RegisterObject( pObject );
    urbanIds_.push_back( pObject->GetID() );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::UpdateCapacity
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void MIL_ObjectManager::UpdateCapacity( const std::string& capacity, xml::xistream& xis, MIL_Object_ABC& object )
{
    MIL_ObjectLoader::GetLoader().GetCapacityFactory().Update( object, capacity, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::ReadUrbanState
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void MIL_ObjectManager::ReadUrbanState( xml::xistream& xis )
{
    unsigned int id = xis.attribute< unsigned int >( "id" );
    UrbanObjectWrapper* wrapper = FindUrbanWrapper( id );
    if( wrapper )
        xis >> xml::list( *this, &MIL_ObjectManager::UpdateCapacity, *wrapper );
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
// Name: MIL_ObjectManager::OnReceiveObjectMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveObjectMagicAction( const sword::ObjectMagicAction& msg, unsigned int nCtx, const tools::Resolver< MIL_Army_ABC >& armies )
{
    sword::ObjectMagicActionAck_ErrorCode nErrorCode = sword::ObjectMagicActionAck::no_error;

    if( msg.type() == sword::ObjectMagicAction::create )
        nErrorCode = CreateObject( msg.parameters(), armies );
    else if( msg.type() == sword::ObjectMagicAction::destroy )
    {
        MIL_Object_ABC* pObject = Find( msg.object().id() );
        if( !pObject )
            nErrorCode = sword::ObjectMagicActionAck::error_invalid_object;
        else
            (*pObject)().Destroy();
    }
    else if( msg.type() == sword::ObjectMagicAction::update )
    {
        MIL_Object_ABC* pObject = Find( msg.object().id() );
        if( !pObject )
            nErrorCode = sword::ObjectMagicActionAck::error_invalid_object;
        else
        {
            const sword::MissionParameters& params = msg.parameters();
            if( params.elem_size() && params.elem( 0 ).value_size() && params.elem( 0 ).value().Get( 0 ).list_size() )
                nErrorCode = pObject->OnUpdate( params.elem( 0 ).value() );
        }
    }
    else if( msg.type() == sword::ObjectMagicAction::request )
    {
        MIL_Object_ABC* pObject = Find( msg.object().id() );
        if( !pObject )
            nErrorCode = sword::ObjectMagicActionAck::error_invalid_object;
        else
        {
            const sword::MissionParameters& params = msg.parameters();
            if( params.elem_size() && params.elem( 0 ).value_size() && params.elem( 0 ).value().Get( 0 ).list_size() )
                nErrorCode = pObject->OnRequest( params.elem( 0 ).value() );
        }
    }
    client::ObjectMagicActionAck asnReplyMsg;
    asnReplyMsg().set_error_code( nErrorCode );
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::OnReceiveUrbanMagicAction
// Created: SLG 2010-12-22
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveUrbanMagicAction( const sword::UrbanMagicAction& msg, unsigned int nCtx )
{
    sword::UrbanMagicActionAck_ErrorCode nErrorCode = sword::UrbanMagicActionAck::no_error;
    UrbanObjectWrapper* object = FindUrbanWrapper( msg.id().id() );
    if( !object )
        nErrorCode = sword::UrbanMagicActionAck::error_invalid_urban_block;
    else
        nErrorCode = object->OnUpdateStructuralState( msg.structural_state() );
    client::UrbanMagicActionAck asnReplyMsg;
    asnReplyMsg().set_error_code( nErrorCode );
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::OnReceiveChangeResourceLinks
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveChangeResourceLinks( const sword::MagicAction& message, unsigned int nCtx )
{
    sword::MagicActionAck_ErrorCode nErrorCode = sword::MagicActionAck::no_error;
    const sword::MissionParameters& params = message.parameters();
    unsigned int id = params.elem( 0 ).value().Get( 0 ).identifier();
    bool urban = params.elem( 1 ).value().Get( 0 ).booleanvalue();
    MIL_Object_ABC* object = 0;
    if( urban )
        object = FindUrbanWrapper( id );
    else 
        object = Find( id );
    if( object == 0 )
        nErrorCode = sword::MagicActionAck::error_invalid_parameter;
    else if( params.elem( 2 ).value_size() > 0 )
        nErrorCode = object->OnUpdateResourceLinks( params.elem( 2 ).value() );
    client::MagicActionAck asnReplyMsg;
    asnReplyMsg().set_error_code( nErrorCode );
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher(), nCtx );
}
