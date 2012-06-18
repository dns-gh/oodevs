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
#include "AltitudeModifierAttribute.h"
#include "CapacityFactory.h"
#include "CrowdCapacity.h"
#include "FloodAttribute.h"
#include "LogisticAttribute.h"
#include "MIL_ObjectFactory.h"
#include "MIL_Object_ABC.h"
#include "MIL_ObjectLoader.h"
#include "MIL_ObjectManipulator_ABC.h"
#include "UrbanObjectWrapper.h"
#include "Entities/MIL_Army_ABC.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Network/NET_Publisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_ScipioException.h"
#include "protocol/ClientSenders.h"
#include <urban/TerrainObject_ABC.h>
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
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
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> objects_;
    for( CIT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
    {
        if( UrbanObjectWrapper* wrapper = dynamic_cast< UrbanObjectWrapper* >( it->second ) )
            urbanObjects_.insert( std::make_pair( &wrapper->GetObject(), wrapper ) );
        if( it->second->IsUniversal() )
            universalObjects_.insert( it->second );
    }
    FinalizeObjects();
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
    {
        try
        {
            it->second->ProcessEvents();
        }
        catch( std::exception& e )
        {
            MT_LOG_ERROR_MSG( "Object event error : " << it->second->GetID() << " : " << e.what() );
        }
    }
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
            try
            {
                if( const AltitudeModifierAttribute* altitude = object.RetrieveAttribute< AltitudeModifierAttribute >() )
                {
                    const TER_Localisation& localisation = object.GetLocalisation();
                    altitude->ResetAltitude( localisation );
                    for( IT_ObjectMap obj = objects_.begin(); obj != objects_.end(); ++obj )
                        if( FloodAttribute* flood = obj->second->RetrieveAttribute< FloodAttribute >() )
                            if( localisation.IsIntersecting( flood->GetLocalisation() ) )
                                flood->GenerateFlood( true );
                }
                object.SendDestruction();
                if( UrbanObjectWrapper* wrapper = dynamic_cast< UrbanObjectWrapper* >( &object ) )
                    urbanObjects_.erase( &wrapper->GetObject() );
            }
            catch( std::exception& e )
            {
                MT_LOG_ERROR_MSG( "Error updating object " << object.GetID() << " before destruction : " << e.what() );
            }
            universalObjects_.erase( &object );
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
    if( !objects_.insert( std::make_pair( pObject->GetID(), pObject ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    if( pObject->IsUniversal() )
        universalObjects_.insert( pObject );
    pObject->SendCreation();
    if( pObject->GetArmy() )
        pObject->GetArmy()->GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( *pObject ); //$$$ A CHANGER DE PLACE QUAND REFACTOR OBJETS -- NB : ne doit pas �tre fait dans RealObject::InitializeCommon <= crash dans connaissance, si initialisation objet failed
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::Count
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
unsigned long MIL_ObjectManager::Count() const
{
    return static_cast< unsigned long >( objects_.size() - urbanObjects_.size() );
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
// Name: MIL_ObjectManager::GetUrbanObjectWrapper
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper& MIL_ObjectManager::GetUrbanObjectWrapper( const urban::TerrainObject_ABC& object )
{
    CIT_UrbanObjectMap it = urbanObjects_.find( &object );
    if( it == urbanObjects_.end() )
        throw std::exception( "Unable to get UrbanObjectWrapper from TerrainObject" );
    return *( it->second );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::ConvertUrbanIdToSimId
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
unsigned int MIL_ObjectManager::ConvertUrbanIdToSimId( unsigned int urbanId )
{
    for( CIT_UrbanObjectMap it = urbanObjects_.begin(); it != urbanObjects_.end(); ++it )
        if( it->first->GetId() == urbanId )
            return it->second->GetID();
    throw std::exception( ( "Cannot find urban object with id = " + boost::lexical_cast< std::string >( urbanId ) ).c_str() );
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
{
    if( !( message.elem_size() == 4 || message.elem_size() == 5 ) ) // type, location, name, team, attributes
        return sword::ObjectMagicActionAck::error_invalid_specific_attributes;

    MIL_Army_ABC* pArmy = 0;
    const sword::MissionParameter_Value& value = message.elem( 3 ).value( 0 );
    if( value.has_identifier() )
        pArmy = armies.Find( value.identifier() );
    else if( value.has_party() )
        pArmy = armies.Find( value.party().id() );
    if( !pArmy )
        return sword::ObjectMagicActionAck::error_invalid_party;
    sword::ObjectMagicActionAck_ErrorCode errorCode = sword::ObjectMagicActionAck::no_error;
    MIL_Object_ABC* pObject = builder_->BuildObject( message, *pArmy, errorCode );
    if( pObject && pObject->RetrieveAttribute< AltitudeModifierAttribute >() )
    {
        const TER_Localisation& localisation = pObject->GetLocalisation();
        for( IT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
            if( FloodAttribute* flood = it->second->RetrieveAttribute< FloodAttribute >() )
                if( localisation.IsIntersecting( flood->GetLocalisation() ) )
                    flood->GenerateFlood( true );
    }
    RegisterObject( pObject );
    return errorCode;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation )
{
    MIL_Object_ABC* pObject = builder_->BuildObject( "", type, army, localisation, sword::ObstacleType_DemolitionTargetType_preliminary, 0u );
    RegisterObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JSR 2011-10-26
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, unsigned int forcedId )
{
    MIL_Object_ABC* pObject = builder_->BuildObject( "", type, army, localisation, sword::ObstacleType_DemolitionTargetType_preliminary, 0u, forcedId );
    RegisterObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation,
                                                 sword::ObstacleType_DemolitionTargetType obstacleType, unsigned int externalIdentifier, const std::string& name )
{
    if( pLocalisation )
    {
        MIL_Object_ABC* pObject = builder_->BuildObject( name, type, army, *pLocalisation, obstacleType, externalIdentifier );
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
    urbanObjects_.insert( std::make_pair( &object, static_cast< UrbanObjectWrapper* >( pObject ) ) );
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
    try
    {
        MIL_Object_ABC* urbanObject = Find( ConvertUrbanIdToSimId( xis.attribute< unsigned int >( "id" ) ) );
        if( urbanObject )
            xis >> xml::list( *this, &MIL_ObjectManager::UpdateCapacity, *urbanObject );
    }
    catch( std::exception& )
    {
        // Avoid crash if id in urban state is not coherent with urban.xml
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::FinalizeObjects
// Created: JSR 2011-05-20
// -----------------------------------------------------------------------------
void MIL_ObjectManager::FinalizeObjects()
{
    // $$$$ _RC_ JSR 2011-11-04: TODO passer par une interface? (attention, respecter l'ordre, floods en derniers)
    for( IT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
    {
        if( AltitudeModifierAttribute* altitude = it->second->RetrieveAttribute< AltitudeModifierAttribute >() )
            altitude->ModifyAltitude( it->second->GetLocalisation() );
        if( LogisticAttribute* logistic = it->second->RetrieveAttribute< LogisticAttribute >() )
            logistic->Finalize();
    }
    for( IT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
        if( FloodAttribute* flood = it->second->RetrieveAttribute< FloodAttribute >() )
            flood->GenerateFlood();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::SendCreation
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::SendCreation()
{
    for( IT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
        it->second->SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::SendFullState
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::SendFullState()
{
    for( IT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
        it->second->SendFullState();
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
        if( !pObject || pObject->Retrieve< CrowdCapacity >() )
            nErrorCode = sword::ObjectMagicActionAck::error_invalid_object;
        else
        {
            MIL_Army_ABC* army = pObject->GetArmy();
            if( army )
                army->GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pObject );
            ( *pObject )().Destroy();
        }
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
            else
                nErrorCode = sword::ObjectMagicActionAck::error_invalid_specific_attributes;
        }
    }
    client::ObjectMagicActionAck asnReplyMsg;
    asnReplyMsg().set_error_code( nErrorCode );
    asnReplyMsg().set_type( msg.type() );
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
    MIL_Object_ABC* object = Find( id );
    if( object == 0 )
        nErrorCode = sword::MagicActionAck::error_invalid_parameter;
    else if( params.elem( 1 ).value_size() > 0 )
        nErrorCode = object->OnUpdateResourceLinks( params.elem( 1 ).value() );
    client::MagicActionAck asnReplyMsg;
    asnReplyMsg().set_error_code( nErrorCode );
    asnReplyMsg().set_type( message.type() );
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: std::set< MIL_Object_ABC* >& MIL_ObjectManager::GetUniversalObjects
// Created: LDC 2012-01-26
// -----------------------------------------------------------------------------
const std::set< MIL_Object_ABC* >& MIL_ObjectManager::GetUniversalObjects() const
{
    return universalObjects_;
}