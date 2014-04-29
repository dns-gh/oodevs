// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "UnitTeleporter.h"
#include "ContextHandler_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "ContextFactory_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include "CallsignResolver_ABC.h"
#include "HlaObject_ABC.h"
#include "MissionResolver_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    bool HasSpatialAttributes( const T_AttributeIdentifiers& attributes )
    {
        static const ::hla::AttributeIdentifier spatial("Spatial");
        return std::find( attributes.begin(), attributes.end(), spatial ) != attributes.end();
    }
    
    std::string GetName( xml::xisubstream xis, const std::string& category, const std::string& mission )
    {
        std::string name;
        xis >> xml::start( "missions" )
                >> xml::start( category )
                    >> xml::content( mission, name );
        return name;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter constructor
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
UnitTeleporter::UnitTeleporter( xml::xisubstream xis, const MissionResolver_ABC& resolver, RemoteAgentSubject_ABC& agentSubject, ContextHandler_ABC< sword::UnitCreation >& unitContextHandler,
                                dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory,
                                const LocalAgentResolver_ABC& localResolver, const CallsignResolver_ABC& callsignResolver, dispatcher::Logger_ABC& logger,
                                ContextHandler_ABC< sword::FormationCreation >& formationContextHandler, ContextHandler_ABC< sword::AutomatCreation >& automatContextHandler )
    : cancelId_               ( resolver.ResolveUnit( GetName( xis, "fragOrders", "cancel" ) ) )
    , agentSubject_           ( agentSubject )
    , unitContextHandler_     ( unitContextHandler )
    , formationContextHandler_( formationContextHandler )
    , automatContextHandler_  ( automatContextHandler )
    , publisher_              ( publisher )
    , contextFactory_         ( contextFactory )
    , localResolver_          ( localResolver )
    , callsignResolver_       ( callsignResolver )
    , logger_                 ( logger )
{
    agentSubject_.Register( *this );
    unitContextHandler_.Register( *this );
    formationContextHandler_.Register( *this );
    automatContextHandler_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter destructor
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
UnitTeleporter::~UnitTeleporter()
{
    unitContextHandler_.Unregister( *this );
    formationContextHandler_.Unregister( *this );
    automatContextHandler_.Unregister( *this );
    agentSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Created
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::RemoteCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    object.Register( *this );
    objects_[identifier]=&object;
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Destroyed
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::RemoteDestroyed( const std::string& identifier )
{
    T_Objects::iterator it( objects_.find( identifier ) );
    if( objects_.end() == it)
        return;
    it->second->Unregister( *this );

    T_Identifiers::iterator idIt( identifiers_.find( identifier ) );
    if(  idIt != identifiers_.end() )
    {
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( idIt->second );
        message().set_type( sword::UnitMagicAction::move_to );
        sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
        parameter.set_null_value( false );
        sword::Location& location = *parameter.add_value()->mutable_point()->mutable_location();
        location.set_type( sword::Location::point );
        sword::CoordLatLong& coordinates = *location.mutable_coordinates()->add_elem();
        coordinates.set_latitude( 0. );
        coordinates.set_longitude( 0. );
        message.Send( publisher_, contextFactory_.Create() );

        simulation::UnitMagicAction message2;
        message2().mutable_tasker()->mutable_unit()->set_id( idIt->second );
        message2().set_type( sword::UnitMagicAction::destroy_all );
        message2().mutable_parameters();
        message2.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Moved
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::Moved( const std::string& identifier, double latitude, double longitude )
{
    if( identifiers_.find( identifier ) == identifiers_.end() )
        return;
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( identifiers_[ identifier ] );
    message().set_type( sword::UnitMagicAction::move_to );
    sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
    parameter.set_null_value( false );
    sword::Location& location = *parameter.add_value()->mutable_point()->mutable_location();
    location.set_type( sword::Location::point );
    sword::CoordLatLong& coordinates = *location.mutable_coordinates()->add_elem();
    coordinates.set_latitude( latitude );
    coordinates.set_longitude( longitude );
    message.Send( publisher_, contextFactory_.Create() );
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::SideChanged
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Notify
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    identifiers_[ identifier ] = message.unit().id();
    T_EmbeddedUnitsMap::iterator it( pendingLoaded_.find( identifier ) );
    if( pendingLoaded_.end() != it )
    {
        const std::vector< T_UniqueId >& units( it->second );
        for(std::vector< T_UniqueId >::const_iterator itU=units.begin(); itU!=units.end(); ++itU )
        {
            const T_UniqueId& transportedUniqueId = *itU;
            logger_.LogInfo("UnitTeleporter loading in " + identifier);
            unsigned long transportedId( 0 );
            try
            {
                transportedId = callsignResolver_.ResolveSimulationIdentifier( transportedUniqueId );
            }
            catch( const std::exception& )
            {
                continue;
            }
            simulation::UnitMagicAction magic;
            magic().mutable_tasker()->mutable_unit()->set_id( message.unit().id() );
            magic().set_type( static_cast< sword::UnitMagicAction_Type >( sword::UnitMagicAction::load_unit ) );
            magic().mutable_parameters()->add_elem()->add_value()->mutable_agent()->set_id( transportedId );
            magic.Send( publisher_ );
        }
        pendingLoaded_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::NameChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void UnitTeleporter::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::TypeChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void UnitTeleporter::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::EquipmentUpdated
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void UnitTeleporter::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
                    unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void UnitTeleporter::UniqueIdChanged( const std::string& /*identifier*/, const std::vector< char >& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void UnitTeleporter::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void UnitTeleporter::LocalCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    const unsigned long simId = localResolver_.Resolve( identifier );
    const unsigned long parentId = localResolver_.ParentAutomat( simId );
    if( parentId != 0 )
    {
        identifiers_[ identifier ] = simId;
        automatIds_[ identifier ] = parentId;
        objects_[ identifier ]=&object;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void UnitTeleporter::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void UnitTeleporter::Divested( const std::string& identifier, const T_AttributeIdentifiers& attributes )
{
    T_Identifiers::const_iterator automatIt = automatIds_.find( identifier );
    if( automatIt != automatIds_.end() )
    {
        simulation::SetAutomatMode disengageMessage;
        disengageMessage().mutable_automate()->set_id( automatIt->second );
        disengageMessage().set_mode( sword::disengaged );
        disengageMessage.Send( publisher_, contextFactory_.Create() );
    }
    T_Identifiers::const_iterator unitId = identifiers_.find( identifier );
    if( identifiers_.end() != unitId )
    {
        simulation::FragOrder order;
        order().mutable_tasker()->mutable_unit()->set_id( unitId->second );
        order().mutable_type()->set_id( cancelId_ );
        order.Send( publisher_ );
    }
    T_Objects::iterator it( objects_.find( identifier ) );
    if( objects_.end() == it)
        return;
    if( HasSpatialAttributes( attributes ) )
        it->second->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void UnitTeleporter::Acquired( const std::string& identifier, const T_AttributeIdentifiers& attributes )
{
    T_Objects::iterator it( objects_.find( identifier ) );
    if( objects_.end() == it)
        return;
    if( HasSpatialAttributes( attributes ) )
        it->second->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void UnitTeleporter::EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& units )
{
    logger_.LogInfo("UnitTeleporter EmbeddedUnitListChanged");
    T_Identifiers::const_iterator it( identifiers_.find( identifier ) );
    if( identifiers_.end() == it )
    {
        pendingLoaded_[identifier]=units;
        return;
    }

    unsigned long transporterId( it->second );
    for( std::vector< T_UniqueId >::const_iterator itU=units.begin(); itU!=units.end(); ++itU )
    {
        const T_UniqueId& transportedUniqueId = *itU;
        logger_.LogInfo("UnitTeleporter loading in " + identifier);
        unsigned long transportedId( 0 );
        try
        {
            transportedId = callsignResolver_.ResolveSimulationIdentifier( transportedUniqueId );
        }
        catch( const std::exception&  )
        {
            continue;
        }
        simulation::UnitMagicAction message;
        message().mutable_tasker()->mutable_unit()->set_id( transporterId );
        message().set_type( static_cast< sword::UnitMagicAction_Type >( sword::UnitMagicAction::load_unit ) );
        message().mutable_parameters()->add_elem()->add_value()->mutable_agent()->set_id( transportedId );
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void UnitTeleporter::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*perimeter*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: UnitTeleporter::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void UnitTeleporter::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void UnitTeleporter::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::SubEntitiesChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void UnitTeleporter::SubEntitiesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void UnitTeleporter::PropagationChanged( const std::string& /*rtiIdentifier*/, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
                int /*col*/, int /*lig*/, double /*xll*/, double /*yll*/, double /*dx*/, double /*dy*/ )
{
    // NOTHING
}


void UnitTeleporter::Notify( const sword::FormationCreation& /*message*/, const std::string& identifier )
{
    // do not listen to updates on formations
    T_Objects::const_iterator it = objects_.find(identifier);
    if( objects_.end() != it )
    {
        it->second->Unregister(*this);
        objects_.erase( it );
    }
}
void UnitTeleporter::Notify( const sword::AutomatCreation& /*message*/, const std::string& identifier )
{
    // do not listen to updates on automats
    T_Objects::const_iterator it = objects_.find(identifier);
    if( objects_.end() != it )
    {
        it->second->Unregister(*this);
        objects_.erase( it );
    }
}
