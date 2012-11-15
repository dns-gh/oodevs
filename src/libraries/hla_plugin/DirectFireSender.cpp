// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DirectFireSender.h"
#include "Omt13String.h"
#include "Interactions.h"
#include "InteractionSender_ABC.h"
#include "HlaObject_ABC.h"
#include "RemoteAgentResolver_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include "DotationTypeResolver_ABC.h"
#include "AgentSubject_ABC.h"
#include "ChildListener.h"
#include "Agent_ABC.h"
#include "protocol/Simulation.h"
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DirectFireSender constructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
DirectFireSender::DirectFireSender( InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender,
                                    InteractionSender_ABC< interactions::WeaponFire >& weaponFireSender,
                                    const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                                    RemoteAgentSubject_ABC& remoteAgentSubject, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                    const std::string& federateName, const DotationTypeResolver_ABC& munitionTypeResolver, AgentSubject_ABC& agentSubject )
    : interactionSender_ ( interactionSender )
    , weaponFireSender_  ( weaponFireSender )
    , remoteResolver_    ( remoteResolver )
    , localResolver_     ( localResolver )
    , remoteAgentSubject_( remoteAgentSubject )
    , federateName_      ( federateName )
    , munitionTypeResolver_( munitionTypeResolver )
    , localAgentSubject_ ( agentSubject )
{
    CONNECT( controller, *this, start_unit_fire );
    CONNECT( controller, *this, stop_unit_fire );
    remoteAgentSubject_.Register( *this );
    localAgentSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender destructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
DirectFireSender::~DirectFireSender()
{
    localAgentSubject_.Unregister( *this );
    remoteAgentSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Notify
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Notify( const sword::StartUnitFire& message, int /*context*/ )
{
    if( message.type() == sword::StartUnitFire::direct )
        fires_[ message.fire().id() ] = message;
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Notify
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Notify( const sword::StopUnitFire& message, int /*context*/ )
{
    const unsigned int fireIdentifier = message.fire().id();
    if( fires_.find( fireIdentifier ) == fires_.end() )
        return;
    const sword::StartUnitFire& startMessage = fires_[ fireIdentifier ];
    std::string targetIdentifier = remoteResolver_.Resolve( startMessage.target().unit().id() );
    if( targetIdentifier.empty() )
    {
        targetIdentifier = localResolver_.Resolve( startMessage.target().unit().id() );
        if( targetIdentifier.empty() )
            return;
    }
    std::string firingRtiId( localResolver_.Resolve( startMessage.firing_unit().id() ) );
    interactions::MunitionDetonation parameters;
    parameters.articulatedPartData.clear();  // empty array
    parameters.detonationLocation = positions_[ targetIdentifier ];
    parameters.detonationResultCode = 1; // EntityImpact
    parameters.eventIdentifier.eventCount = static_cast< uint16 >( fireIdentifier );
    parameters.eventIdentifier.issuingObjectIdentifier = Omt13String( firingRtiId );
    parameters.firingObjectIdentifier = Omt13String( firingRtiId );
    parameters.finalVelocityVector = rpr::VelocityVector( 0., 0., 700. ); // $$$$ _RC_ SLI 2011-09-23: Hardcoded
    parameters.fuseType = 0; // Other
    parameters.munitionObjectIdentifier = Omt13String();
    parameters.munitionType = startMessage.has_ammunition() ? 
                                munitionTypeResolver_.Resolve( startMessage.ammunition().id() ) : 
                                rpr::EntityType( "2 8 71 2 10" ); // 12.7mm, Hardcoded
    parameters.quantityFired = 10; // Hardcoded
    parameters.rateOfFire = 40; // Hardcoded
    parameters.relativeDetonationLocation = rpr::VelocityVector(); // Entity location
    parameters.targetObjectIdentifier = Omt13String( targetIdentifier );
    parameters.warheadType = 0; // Other
    interactionSender_.Send( parameters );
    // process platforms
    T_LocalListeners::const_iterator itL( listeners_.find( startMessage.firing_unit().id() ) );
    if( itL != listeners_.end() && !itL->second->GetPlatforms().empty() )
    {
        interactions::WeaponFire fire;
        fire.eventIdentifier.eventCount = static_cast< uint16 >( fireIdentifier );
        fire.fireControlSolutionRange = 0.f;
        fire.fireMissionIndex = fireIdentifier;
        fire.firingLocation = positions_[ firingRtiId ];
        fire.fuseType = 0; // Other
        fire.initialVelocityVector = rpr::VelocityVector( 0., 0., 700. ); // $$$$ _RC_ SLI 2011-09-23: Hardcoded
        fire.munitionObjectIdentifier = Omt13String();
        fire.munitionType = parameters.munitionType;
        fire.quantityFired = 10; // Hardcoded
        fire.rateOfFire = 40; // Hardcoded
        fire.targetObjectIdentifier = Omt13String( targetIdentifier );
        fire.warheadType = 0; // Other
        
        BOOST_FOREACH( unsigned int chId, itL->second->GetPlatforms() )
        {
            std::string chRtiId( localResolver_.Resolve( chId ) );
            if( !chRtiId.empty() )
            {
                // WeaponFire
                fire.firingObjectIdentifier = Omt13String( chRtiId );
                fire.eventIdentifier.issuingObjectIdentifier = Omt13String( chRtiId );
                fire.targetObjectIdentifier = Omt13String( targetIdentifier );
                weaponFireSender_.Send( fire );
                // Munition Detonation
                parameters.eventIdentifier.issuingObjectIdentifier = Omt13String( chRtiId );
                parameters.firingObjectIdentifier = Omt13String( chRtiId );
                parameters.targetObjectIdentifier = Omt13String( targetIdentifier );
                interactionSender_.Send( parameters );
            }
        }
    }
    fires_.erase( fireIdentifier );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::RemoteCreated
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::RemoteCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object)
{
    object.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::RemoteDestroyed
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::RemoteDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Moved
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::Moved( const std::string& identifier, double latitude, double longitude )
{
    positions_[ identifier ] = rpr::WorldLocation( latitude, longitude, 0. );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::SideChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::NameChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::TypeChanged
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
void DirectFireSender::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::EquipmentUpdated
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void DirectFireSender::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*number*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void DirectFireSender::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void DirectFireSender::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void DirectFireSender::LocalCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void DirectFireSender::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void DirectFireSender::Divested( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void DirectFireSender::Acquired( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void DirectFireSender::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::PerimeterChanged( const std::string& /*identifier*/, const std::vector< rpr::PerimeterPoint >& /*points*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::ParentChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::SubAgregatesChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::SubEntitiesChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::SubEntitiesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

namespace
{
    void UpdateLocation( std::map< std::string, rpr::WorldLocation >& positions, unsigned long id, const LocalAgentResolver_ABC& localResolver, const ChildListener& child )
    {
        const ChildListener::LocationStruct& loc = child.GetLocation();
        std::string name = localResolver.Resolve( id );
        if( !name.empty() )
            positions[ name ]  = rpr::WorldLocation( loc.latitude, loc.longitude, loc.altitude );
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PlatformCreated
// Created: AHC 2012-11-14
// -----------------------------------------------------------------------------
void DirectFireSender::AggregateCreated( Agent_ABC& agent, unsigned long identifier, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/, const std::string& /*symbol*/, bool /*isLocal*/, const std::vector< char >& /*uniqueId*/ )
{
    boost::shared_ptr< ChildListener > child( new ChildListener( boost::bind( &UpdateLocation, boost::ref( positions_ ), identifier, boost::cref( localResolver_ ), _1 ) ) );
    listeners_[ identifier ] = child;
    agent.Register( *child );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PlatformCreated
// Created: AHC 2012-11-14
// -----------------------------------------------------------------------------
void DirectFireSender::PlatformCreated( Agent_ABC& /*agent*/, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/, const std::string& /*symbol*/, const std::vector< char >& /*uniqueId*/ )
{
    // NOTHING
}
