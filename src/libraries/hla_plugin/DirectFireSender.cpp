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
#include "protocol/Simulation.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DirectFireSender constructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
DirectFireSender::DirectFireSender( InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender,
                                    const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                                    RemoteAgentSubject_ABC& remoteAgentSubject, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                    const std::string& federateName )
    : interactionSender_ ( interactionSender )
    , remoteResolver_    ( remoteResolver )
    , localResolver_     ( localResolver )
    , remoteAgentSubject_( remoteAgentSubject )
    , federateName_      ( federateName )
{
    CONNECT( controller, *this, start_unit_fire );
    CONNECT( controller, *this, stop_unit_fire );
    remoteAgentSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender destructor
// Created: SLI 2011-09-23
// -----------------------------------------------------------------------------
DirectFireSender::~DirectFireSender()
{
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
    const std::string targetTdentifier = remoteResolver_.Resolve( startMessage.target().unit().id() );
    if( targetTdentifier.empty() )
        return;
    interactions::MunitionDetonation parameters;
    parameters.articulatedPartData.clear();  // empty array
    parameters.detonationLocation = positions_[ targetTdentifier ];
    parameters.detonationResultCode = 1; // EntityImpact
    parameters.eventIdentifier.eventCount = static_cast< uint16 >( fireIdentifier );
    parameters.eventIdentifier.issuingObjectIdentifier = Omt13String( federateName_ );
    parameters.firingObjectIdentifier = Omt13String( localResolver_.Resolve( startMessage.firing_unit().id() ) );
    parameters.finalVelocityVector = rpr::VelocityVector( 0., 0., 700. ); // $$$$ _RC_ SLI 2011-09-23: Hardcoded
    parameters.fuseType = 0; // Other
    parameters.munitionObjectIdentifier = Omt13String();
    parameters.munitionType = rpr::EntityType( "2 8 71 2 10" ); // 12.7mm, Hardcoded
    parameters.quantityFired = 10; // Hardcoded
    parameters.rateOfFire = 40; // Hardcoded
    parameters.relativeDetonationLocation = rpr::VelocityVector(); // Entity location
    parameters.targetObjectIdentifier = Omt13String( targetTdentifier );
    parameters.warheadType = 0; // Other
    fires_.erase( fireIdentifier );
    interactionSender_.Send( parameters );
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
void DirectFireSender::UniqueIdChanged( const std::string& /*identifier*/, const std::string& /*uniqueId*/ )
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
void DirectFireSender::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< std::string >& /*units*/ )
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
// Name: DirectFireSender::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireSender::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void DirectFireSender::SubEntitiesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}
