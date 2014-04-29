// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAgentController.h"
#include "RemoteAgentSubject_ABC.h"
#include "ContextHandler_ABC.h"
#include "UnitTypeResolver_ABC.h"
#include "ExtentResolver_ABC.h"
#include "HlaObject_ABC.h"
#include "AgentSubject_ABC.h"
#include "SideResolver_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/Karma.h"
#include "rpr/EntityType.h"
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <sstream>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteAgentController constructor
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
RemoteAgentController::RemoteAgentController( RemoteAgentSubject_ABC& agentSubject,
                                              ContextHandler_ABC< sword::AutomatCreation >& automatHandler,
                                              ContextHandler_ABC< sword::UnitCreation >& unitHandler,
                                              const SideResolver_ABC& sideResolver,
                                              const UnitTypeResolver_ABC& typeResolver, dispatcher::Logger_ABC& logger,
                                              const ExtentResolver_ABC& extent, AgentSubject_ABC& subject )
    : agentSubject_  ( agentSubject )
    , automatHandler_( automatHandler )
    , unitHandler_   ( unitHandler )
    , sideResolver_  ( sideResolver )
    , typeResolver_  ( typeResolver )
    , logger_        ( logger )
    , extent_        ( extent )
    , simSubject_       ( subject )
{
    automatHandler_.Register( *this );
    unitHandler_.Register( *this );
    agentSubject_.Register( *this );
    simSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController destructor
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
RemoteAgentController::~RemoteAgentController()
{
    automatHandler_.Unregister( *this );
    unitHandler_.Unregister( *this );
    agentSubject_.Unregister( *this );
    simSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::AutomatCreation& message, const std::string& identifier )
{
    if( identifier != "default_remote_automat" )
        return;

    logger_.LogInfo( "parties[ " + boost::lexical_cast< std::string >( message.party().id() ) + " ] = " + boost::lexical_cast< std::string >( message.automat().id() ) );
    parties_[ message.party().id() ] = message.automat().id();
    std::for_each( waitingAutomats_.begin(), waitingAutomats_.end(), [&](const T_WaitingAutomats::value_type& waiting )
            {
                SideChanged( waiting.first, waiting.second );
            });
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::RemoteCreated
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::RemoteCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    object.Register( *this );
    remoteObjects_[ identifier ] = &object;
    unitCreations_[ identifier ] = T_UnitCreation( new simulation::UnitMagicAction() );
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    message().mutable_parameters()->add_elem(); // type
    message().mutable_parameters()->add_elem(); // position
    message().mutable_parameters()->add_elem(); // name
    message().mutable_parameters()->add_elem()->add_value()->set_booleanvalue( false ); // isPC
    sword::Extension* ext = message().mutable_parameters()->add_elem()->add_value()->mutable_extensionlist();
    sword::Extension_Entry* entry = ext->add_entries(); // extension
    entry->set_name( "RemoteEntity" );
    entry->set_value( "true" );
    entry = ext->add_entries(); // extension
    entry->set_name( "HLA_ObjectID" );
    entry->set_value( identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::RemoteDestroyed
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::RemoteDestroyed( const std::string& identifier )
{
    if( remoteObjects_.erase( identifier ) )
    {
        unitCreations_.erase( identifier );
        remoteIds_.erase( identifier );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Moved
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::Moved( const std::string& identifier, double latitude, double longitude )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    if( !extent_.IsInBoundaries( geometry::Point2d( longitude, latitude ) ) ) // $$$$ _RC_ SLI 2011-11-30: geocoord invert latitude/longitude???
        return;
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    sword::CoordLatLong* coordLatLong = 0;
    if( message().mutable_parameters()->mutable_elem( 1 )->value_size() != 0 )
    {
        sword::Location* location = message().mutable_parameters()->mutable_elem( 1 )->mutable_value( 0 )->mutable_point()->mutable_location();
        coordLatLong = location->mutable_coordinates()->mutable_elem( 0 );
    }
    else
    {
        sword::Location* location = message().mutable_parameters()->mutable_elem( 1 )->add_value()->mutable_point()->mutable_location();
        location->set_type( sword::Location::point );
        coordLatLong = location->mutable_coordinates()->add_elem();
    }
    coordLatLong->set_latitude( latitude );
    coordLatLong->set_longitude( longitude );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::SideChanged
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    {
        std::stringstream ss;
        ss << "Received side for " << identifier << " " << (int)side;
        logger_.LogInfo( ss.str() );
    }

    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    const unsigned long automat = FindAutomat( side );
    if( automat == 0 )
        waitingAutomats_[ identifier ] = side;
    else
    {
        message().mutable_tasker()->mutable_automat()->set_id( automat );
        Send( message, identifier );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::NameChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void RemoteAgentController::NameChanged( const std::string& identifier, const std::string& name )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    {
        std::stringstream ss;
        ss << "Received name for " << identifier << " " << name;
        logger_.LogInfo( ss.str() );
    }
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    if( message().parameters().elem( 2 ).value_size() == 1)
        message().mutable_parameters()->mutable_elem( 2 )->mutable_value( 0 )->set_acharstr( name );
    else
        message().mutable_parameters()->mutable_elem( 2 )->add_value()->set_acharstr( name );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::TypeChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void RemoteAgentController::TypeChanged( const std::string& identifier, const rpr::EntityType& type )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    {
        std::stringstream ss;
        ss << "Received type for " << identifier << " " << type.str();
        logger_.LogInfo( ss.str() );
    }
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    if( message().parameters().elem( 0 ).value_size() == 1)
        message().mutable_parameters()->mutable_elem( 0 )->mutable_value( 0 )->set_identifier( typeResolver_.Resolve( type ) );
    else
        message().mutable_parameters()->mutable_elem( 0 )->add_value()->set_identifier( typeResolver_.Resolve( type ) );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::EquipmentUpdated
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void RemoteAgentController::EquipmentUpdated( const std::string& identifier, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
                    unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    T_UnitCreations::const_iterator it( unitCreations_.find( identifier ) );
    if( unitCreations_.end() == it )
        return;
    simulation::UnitMagicAction& message = *(it->second);
    message().set_type( sword::UnitMagicAction::unit_creation );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void RemoteAgentController::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void RemoteAgentController::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Send
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void RemoteAgentController::Send( simulation::UnitMagicAction& message, const std::string& identifier )
{
    {
        std::stringstream ss;
        ss << "Attempt creation unit " << identifier << std::boolalpha << " " << message().has_type() << " " << 
            ( message().has_tasker() ) << " " << 
            ( message().parameters().elem( 0 ).value_size() > 0 ) << " " << // type 
            ( message().parameters().elem( 1 ).value_size() > 0 ) << " " << // position
            ( message().parameters().elem( 2 ).value_size() > 0 ); // name
        logger_.LogInfo( ss.str() );
    }

    if( message().has_type() && 
        message().has_tasker() &&
        message().parameters().elem( 0 ).value_size() > 0 &&
        message().parameters().elem( 1 ).value_size() > 0 &&
        message().parameters().elem( 2 ).value_size() > 0 )
    {
        unitHandler_.Send( message, identifier );
        unitCreations_.erase( identifier );
        remoteObjects_[ identifier ]->Unregister( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::FindAutomat
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
unsigned long RemoteAgentController::FindAutomat( rpr::ForceIdentifier force ) const
{
    unsigned long teamId = sideResolver_.ResolveTeam( force );
    T_Parties::const_iterator itParty = parties_.find( teamId );
    if( itParty == parties_.end() )
    {
        logger_.LogWarning( "Party '" + boost::lexical_cast< std::string >( teamId ) + "' not existing yet" );
        return 0;
    }
    return itParty->second;
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void RemoteAgentController::LocalCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void RemoteAgentController::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void RemoteAgentController::Divested( const std::string& /*identifier*/, const T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void RemoteAgentController::Acquired( const std::string& /*identifier*/, const T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: AHC 2010-03-09
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    remoteIds_[ identifier ] = message.unit().id();
    Attach( message.unit().id() );
}

namespace
{
    struct CheckId
    {
        CheckId( unsigned long p ) : p_( p ) {}
        bool operator()( const std::map< std::string, unsigned long >::value_type& v)
        {
            return v.second == p_;
        }
    private:
        unsigned long p_;
    };
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Attach
// Created: AHC 2010-03-09
// -----------------------------------------------------------------------------
void RemoteAgentController::Attach( unsigned long simId )
{
    T_Hla2SimIds::iterator itId( std::find_if( remoteIds_.begin(), remoteIds_.end(),
        CheckId( simId ) ) );
    if( remoteIds_.end() == itId )
        return;
    T_Agents::iterator itAg( remoteAgents_.find( simId ) );
    if( remoteAgents_.end() == itAg )
        return;
    T_RemoteObjects::iterator itR( remoteObjects_.find( itId->first ) );
    if( remoteObjects_.end() != itR )
    {
        itR->second->Attach( itAg->second, simId );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::AggregateCreated
// Created: AHC 2010-03-09
// -----------------------------------------------------------------------------
void RemoteAgentController::AggregateCreated( Agent_ABC& agent, unsigned long identifier, const std::string& /*name*/,
        rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/, const std::string& /*symbol*/, bool isLocal, const std::vector< char >& /*uniqueId*/ )
{
    if( !isLocal )
        remoteAgents_[ identifier ] = &agent;
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void RemoteAgentController::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::PlatformCreated
// Created: AHC 2010-03-09
// -----------------------------------------------------------------------------
void RemoteAgentController::PlatformCreated( Agent_ABC& /*agent*/, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/, const std::string& /*symbol*/, const std::vector< char >& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::PlatformCreated
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*perimeter*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::ParentChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteAgentController::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    //  NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::SubAgregatesChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteAgentController::SubAgregatesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children )
{
    T_UnitCreations::const_iterator it( unitCreations_.find( rtiIdentifier ) );
    if( unitCreations_.end() == it )
        return;
    if( children.empty() ) // FIXME heuristic to determine if remote is a Sword Unit
    {
        simulation::UnitMagicAction& message = *(it->second);
        message().set_type( sword::UnitMagicAction::unit_creation );
        Send( message, rtiIdentifier );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::SubEntitiesChanged
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteAgentController::SubEntitiesChanged(const std::string& rtiIdentifier, const std::set< std::string >& children )
{
    T_UnitCreations::const_iterator it( unitCreations_.find( rtiIdentifier ) );
    if( unitCreations_.end() == it )
        return;
    if( !children.empty() ) // FIXME heuristic to determine if remote is a Sword Unit
    {
        simulation::UnitMagicAction& message = *(it->second);
        message().set_type( sword::UnitMagicAction::unit_creation );
        Send( message, rtiIdentifier );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void RemoteAgentController::PropagationChanged( const std::string& /*rtiIdentifier*/, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
                int /*col*/, int /*lig*/, double /*xll*/, double /*yll*/, double /*dx*/, double /*dy*/ )
{
    // NOTHING
}
