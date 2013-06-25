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
#include "Adapters/Sink_ABC.h"
#include "MIL_ObjectManipulator_ABC.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Network/NET_Publisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_FormatString.h"
#include "protocol/ClientSenders.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "Urban/PHY_RoofShapeType.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ObjectManager )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_ObjectManager* manager, const unsigned int /*version*/ )
{
    const sword::Sink_ABC* sink = &manager->sink_;
    archive << sink;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_ObjectManager* manager, const unsigned int /*version*/ )
{
    sword::Sink_ABC* sink;
    archive >> sink;
    ::new( manager )MIL_ObjectManager( MIL_AgentServer::GetWorkspace().GetObjectFactory(), *sink );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_ObjectManager::MIL_ObjectManager( MIL_ObjectFactory& factory, sword::Sink_ABC& sink )
    : factory_  ( factory )
    , nbObjects_( 0 )
    , sink_     ( sink )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_ObjectManager::~MIL_ObjectManager()
{
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> objects_;
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
    {
        if( dynamic_cast< MIL_UrbanObject_ABC* >( it->second ) == 0 )
            ++nbObjects_;
        if( it->second->IsUniversal() )
            universalObjects_.push_back( it->second );
    }
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
// Name: MIL_ObjectManager::Clean
// Created: JSR 2013-01-29
// -----------------------------------------------------------------------------
void MIL_ObjectManager::Clean()
{
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
        it->second->Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::ProcessEvents
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::ProcessEvents()
{
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
    {
        try
        {
            it->second->ProcessEvents();
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( "Object event error : " << it->second->GetID() << " : " << tools::GetExceptionMsg( e ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::UpdateStates
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::UpdateStates( const propagation::FloodModel_ABC& floodModel )
{
    for( auto it = objects_.begin(); it != objects_.end(); )
    {
        MIL_Object_ABC& object = *it->second;
        if( object.IsReadyForDeletion() )
        {
            try
            {
                if( const AltitudeModifierAttribute* altitude = object.RetrieveAttribute< AltitudeModifierAttribute >() )
                {
                    const TER_Localisation& localisation = object.GetLocalisation();
                    altitude->ResetAltitude( localisation, object.GetID() );
                    for( auto obj = objects_.begin(); obj != objects_.end(); ++obj )
                        if( FloodAttribute* flood = obj->second->RetrieveAttribute< FloodAttribute >() )
                            if( localisation.IsIntersecting( flood->GetLocalisation() ) )
                                flood->GenerateFlood( floodModel );
                }
                object.SendDestruction();
            }
            catch( const std::exception& e )
            {
                MT_LOG_ERROR_MSG( "Error updating object " << object.GetID() << " before destruction : " << tools::GetExceptionMsg( e ) );
            }
            if( dynamic_cast< MIL_UrbanObject_ABC* >( &object ) == 0 )
                --nbObjects_;
            boost::remove_erase( universalObjects_, &object );
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
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    ++nbObjects_;
    if( pObject->IsUniversal() )
        universalObjects_.push_back( pObject );
    pObject->SendCreation();
    if( pObject->GetArmy() )
        pObject->GetArmy()->GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( *pObject ); //$$$ A CHANGER DE PLACE QUAND REFACTOR OBJETS -- NB : ne doit pas être fait dans RealObject::InitializeCommon <= crash dans connaissance, si initialisation objet failed
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::Count
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
unsigned long MIL_ObjectManager::Count() const
{
    return nbObjects_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC& MIL_ObjectManager::CreateObject( xml::xistream& xis, MIL_Army_ABC* army )
{
    MIL_Object_ABC* pObject = factory_.CreateObject( sink_, xis, army );
    if( !pObject )
        throw MASA_EXCEPTION( "Unknown object" ); //@TODO MGD propagate reference
    RegisterObject( pObject );
    return *pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode MIL_ObjectManager::CreateObject( const sword::MissionParameters& message, const tools::Resolver< MIL_Army_ABC >& armies,
                                                                       const propagation::FloodModel_ABC& floodModel )
{
    if( !( message.elem_size() >= 4 && message.elem_size() <= 6 ) ) // type, location, name, team, attributes, extension
        return sword::ObjectMagicActionAck::error_invalid_specific_attributes;

    MIL_Army_ABC* pArmy = 0;
    const sword::MissionParameter_Value& value = message.elem( 3 ).value( 0 );
    if( ! ( value.has_identifier() || value.has_party() ) )
        return sword::ObjectMagicActionAck::error_invalid_party;
    unsigned int id = value.has_identifier() ? value.identifier() : value.party().id();
    if( id != 0 )
    {
        pArmy = armies.Find( id );
        if( !pArmy )
            return sword::ObjectMagicActionAck::error_invalid_party;
    }
    sword::ObjectMagicActionAck_ErrorCode errorCode = sword::ObjectMagicActionAck::no_error;
    MIL_Object_ABC* pObject = factory_.CreateObject( sink_, message, pArmy, errorCode );
    if( pObject )
    {
        if( FloodAttribute* flood = pObject->RetrieveAttribute< FloodAttribute >() )
            flood->GenerateFlood( floodModel );
        if( pObject->RetrieveAttribute< AltitudeModifierAttribute >() )
        {
            const TER_Localisation& localisation = pObject->GetLocalisation();
            for( auto it = objects_.begin(); it != objects_.end(); ++it )
                if( FloodAttribute* flood = it->second->RetrieveAttribute< FloodAttribute >() )
                    if( localisation.IsIntersecting( flood->GetLocalisation() ) )
                        flood->GenerateFlood( floodModel );
        }
        if( message.elem_size() == 6 )
        {
            MIL_DictionaryExtensions extensions;
            extensions.ReadExtensions( message.elem( 5 ).value( 0 ).extensionlist() );
            pObject->SetExtensions( extensions );
        }
    }
    RegisterObject( pObject );
    return errorCode;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation )
{
    MIL_Object_ABC* pObject = factory_.CreateObject( sink_, "", type, army, localisation, sword::ObstacleType_DemolitionTargetType_preliminary, 0u, 0u );
    RegisterObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JSR 2011-10-26
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation, unsigned int forcedId )
{
    MIL_Object_ABC* pObject = factory_.CreateObject( sink_, "", type, army, localisation, sword::ObstacleType_DemolitionTargetType_preliminary, 0u, forcedId );
    RegisterObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( MIL_Army_ABC* army, const std::string& type, const TER_Localisation* pLocalisation,
                                                 sword::ObstacleType_DemolitionTargetType obstacleType, unsigned int externalIdentifier, const std::string& name, double density )
{
    if( pLocalisation )
    {
        MIL_Object_ABC* pObject = factory_.CreateObject( sink_, name, type, army, *pLocalisation, obstacleType == sword::ObstacleType_DemolitionTargetType_reserved, externalIdentifier, 0, density );
        RegisterObject( pObject );
        return pObject;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateObject( MIL_Army_ABC* army, const MIL_ObjectBuilder_ABC& builder )
{
    MIL_Object_ABC* pObject = factory_.CreateObject( sink_, builder, army );
    RegisterObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateUrbanObject
// Created: JSR 2012-08-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::CreateUrbanObject( xml::xistream& xis, MIL_UrbanObject_ABC* parent )
{
    MIL_UrbanObject_ABC* pObject = factory_.CreateUrbanObject( sink_, xis, parent );
    if( pObject->IsBlock() )
    {
        const UrbanPhysicalCapacity* pPhysical = pObject->Retrieve< UrbanPhysicalCapacity >();
        if( pPhysical && ( !PHY_MaterialCompositionType::Find( pPhysical->GetMaterial() ) || !PHY_RoofShapeType::Find( pPhysical->GetRoofShape() ) ) )
            MT_LOG_INFO_MSG( "The architecture of the urban block '" << pObject->GetUrbanId() << "' ('" << pObject->GetName() << "') is not consistent with the architecture described in the urban file" );
        if( pObject->GetLocalisation().GetPoints().empty() )
        {
            MT_LOG_ERROR_MSG( MT_FormatString( "Urban block %d ignored for lack of geometry", pObject->GetUrbanId() ) );
            delete pObject;
            return 0;
        }
    }
    RegisterObject( pObject );
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::UpdateCapacity
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void MIL_ObjectManager::UpdateCapacity( const std::string& capacity, xml::xistream& xis, MIL_Object_ABC& object )
{
    factory_.Update( capacity, xis, object );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::WriteODB
// Created: JSR 2011-11-15
// -----------------------------------------------------------------------------
void MIL_ObjectManager::WriteODB( xml::xostream& xos ) const
{
    bool noSideObjectsFound = false;
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
    {
        MIL_Object_ABC* obj = it->second;
        if( obj && dynamic_cast< MIL_UrbanObject_ABC* >( obj ) == 0 && obj->GetArmy() == 0  )
        {
            if( !noSideObjectsFound )
            {
                noSideObjectsFound = true;
                xos << xml::start( "no-party")
                        << xml::start( "objects" );
            }
            obj->WriteODB( xos );
        }
    }
    if( noSideObjectsFound )
        xos << xml::end  // no-party
            << xml::end; // objects
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::ReadUrbanState
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void MIL_ObjectManager::ReadUrbanState( xml::xistream& xis )
{
    try
    {
        MIL_Object_ABC* urbanObject = Find( MIL_AgentServer::GetWorkspace().GetEntityManager().ConvertUrbanIdToSimId( xis.attribute< unsigned int >( "id" ) ) );
        if( urbanObject )
            xis >> xml::list( *this, &MIL_ObjectManager::UpdateCapacity, *urbanObject );
    }
    catch( const std::exception& )
    {
        // Avoid crash if id in urban state is not coherent with urban.xml
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::FinalizeObjects
// Created: JSR 2011-05-20
// -----------------------------------------------------------------------------
void MIL_ObjectManager::FinalizeObjects( const propagation::FloodModel_ABC& floodModel )
{
    // $$$$ _RC_ JSR 2011-11-04: TODO passer par une interface? (attention, respecter l'ordre, floods en derniers)
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
    {
        if( AltitudeModifierAttribute* altitude = it->second->RetrieveAttribute< AltitudeModifierAttribute >() )
            altitude->ModifyAltitude( it->second->GetLocalisation(), it->second->GetID() );
        if( LogisticAttribute* logistic = it->second->RetrieveAttribute< LogisticAttribute >() )
            logistic->Finalize();
    }
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
        if( FloodAttribute* flood = it->second->RetrieveAttribute< FloodAttribute >() )
            flood->GenerateFlood( floodModel );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::SendCreation
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::SendCreation()
{
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
        it->second->SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::SendFullState
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::SendFullState()
{
    for( auto it = objects_.begin(); it != objects_.end(); ++it )
        it->second->SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::OnReceiveObjectMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveObjectMagicAction( const sword::ObjectMagicAction& msg, unsigned int nCtx, const tools::Resolver< MIL_Army_ABC >& armies,
                                                    const propagation::FloodModel_ABC& floodModel )
{
    sword::ObjectMagicActionAck_ErrorCode nErrorCode = sword::ObjectMagicActionAck::no_error;

    if( msg.type() == sword::ObjectMagicAction::create )
        nErrorCode = CreateObject( msg.parameters(), armies, floodModel );
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
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::OnReceiveChangeResourceLinks
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveChangeResourceLinks( const sword::MagicAction& message, unsigned int nCtx, unsigned int clientId )
{
    sword::MagicActionAck_ErrorCode nErrorCode = sword::MagicActionAck::no_error;
    const sword::MissionParameters& params = message.parameters();
    if( params.elem_size() > 1 && params.elem( 0 ).value_size() > 0 
      && params.elem( 0 ).value().Get( 0 ).has_identifier() )
    {        
        unsigned int id = params.elem( 0 ).value().Get( 0 ).identifier();
        MIL_Object_ABC* object = Find( id );
        if( object == 0 )
            nErrorCode = sword::MagicActionAck::error_invalid_parameter;
        else
            nErrorCode = object->OnUpdateResourceLinks( params.elem( 1 ).value() );
    }
    else
        nErrorCode = sword::MagicActionAck::error_invalid_parameter;
    client::MagicActionAck asnReplyMsg;
    asnReplyMsg().set_error_code( nErrorCode );
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::VisitUniversalObjects
// Created: LDC 2012-01-26
// -----------------------------------------------------------------------------
void MIL_ObjectManager::VisitUniversalObjects( const boost::function< void( MIL_Object_ABC& ) >& visitor ) const
{
    for( auto it = universalObjects_.begin(); it != universalObjects_.end(); ++it )
        if( !(*it)->IsMarkedForDestruction() )
            visitor( **it );
}

// -----------------------------------------------------------------------------
// Name:MIL_ObjectManager::GetObjects
// Created: SLI 2012-09-19
// -----------------------------------------------------------------------------
const std::map< unsigned int, MIL_Object_ABC* >& MIL_ObjectManager::GetObjects() const
{
    return objects_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::Find
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectManager::Find( unsigned int nID ) const
{
    auto it = objects_.find( nID );
    if( it == objects_.end() )
        return 0;
    return it->second;
}
