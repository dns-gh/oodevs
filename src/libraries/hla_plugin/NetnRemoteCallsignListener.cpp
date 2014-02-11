// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRemoteCallsignListener.h"
#include "RemoteAgentSubject_ABC.h"
#include "CallsignResolver_ABC.h"
#include "ContextHandler_ABC.h"
#include "HlaObject_ABC.h"
#include "protocol/Simulation.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
NetnRemoteCallsignListener::NetnRemoteCallsignListener( CallsignResolver_ABC& resolver, RemoteAgentSubject_ABC& subject,
                                                        ContextHandler_ABC< sword::UnitCreation >& handler )
    : resolver_( resolver )
    , subject_ ( subject )
    , handler_ ( handler )
{
    subject_.Register( *this );
    handler_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener destructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
NetnRemoteCallsignListener::~NetnRemoteCallsignListener()
{
    handler_.Unregister( *this );
    subject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Add
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Add( const std::string& identifier )
{
    T_Add& add = adds_[ identifier ];
    if( add.simulationIdentifier != 0 && !add.callsign.empty() && !add.uniqueId.empty() )
    {
        resolver_.Add( add.simulationIdentifier, add.callsign, add.uniqueId );
        adds_.erase( identifier );
    }
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::UniqueIdChanged( const std::string& identifier, const T_UniqueId& uniqueId )
{
    adds_[ identifier ].uniqueId = uniqueId;
    Add( identifier );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::CallsignChanged( const std::string& identifier, const std::string& callsign )
{
    adds_[ identifier ].callsign = callsign;
    Add( identifier );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Notify
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    adds_[ identifier ].simulationIdentifier = message.unit().id();
    Add( identifier );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::RemoteCreated
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::RemoteCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    object.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::RemoteDestroyed
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::RemoteDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Moved
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Moved( const std::string& /*identifier*/, double /*latitude*/, double /*longitude*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::SideChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::NameChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::TypeChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::EquipmentUpdated
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
        unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::LocalCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Divested( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Acquired( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*perimeter*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::SubEntitiesChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::SubEntitiesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::PropagationChanged( const std::string& /*rtiIdentifier*/, const std::vector< ObjectListener_ABC::PropagationData >& /*data*/,
                int /*col*/, int /*lig*/, double /*xll*/, double /*yll*/, double /*dx*/, double /*dy*/ )
{
    // NOTHING
}
