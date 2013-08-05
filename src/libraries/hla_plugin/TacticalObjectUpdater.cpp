// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TacticalObjectUpdater.h"
#include "HlaObject_ABC.h"
#include "ContextHandler_ABC.h"
#include "PropagationManager_ABC.h"
#include "RemoteTacticalObjectSubject_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include <boost/lexical_cast.hpp>
#include <fstream>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater constructor
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
TacticalObjectUpdater::TacticalObjectUpdater( dispatcher::SimulationPublisher_ABC& publisher, ContextHandler_ABC< sword::ObjectMagicActionAck >& contextHandler,
        dispatcher::Logger_ABC& logger, PropagationManager_ABC& propMgr, RemoteTacticalObjectSubject_ABC& subject )
    : publisher_( publisher )
    , contextHandler_( contextHandler )
    , logger_( logger )
    , propagationManager_( propMgr )
    , subject_( subject )
{
    subject_.RegisterTactical( *this );
    contextHandler_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater destructor
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
TacticalObjectUpdater::~TacticalObjectUpdater()
{
    subject_.UnregisterTactical( *this );
    contextHandler_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::SideChanged
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::RemoteCreated
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::RemoteCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    object.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::RemoteDestroyed
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::RemoteDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::Moved
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::Moved( const std::string& /*identifier*/, double /*latitude*/, double /*longitude*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::NameChanged
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::TypeChanged
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::EquipmentUpdated
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
        unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::UniqueIdChanged
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::CallsignChanged
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::LocalCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::Divested( const std::string& /*identifier*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::Acquired( const std::string& /*identifier*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::EmbeddedUnitListChanged
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::GeometryChanged( const std::string& /*identifier*/, const std::vector< rpr::WorldLocation >& /*perimeter*/, ObjectListener_ABC::GeometryType /*type*/ )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::SubEntitiesChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::SubEntitiesChanged( const std::string& /*rtiIdentifier*/, const ObjectListener_ABC::T_EntityIDs& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
                int col, int lig, double xll, double yll, double dx, double dy )
{
    if( data.size() == 0 )
    {
        logger_.LogWarning("TacticalObjectUpdater empty propagation for " + rtiIdentifier);
        return;
    }
    logger_.LogInfo("TacticalObjectUpdater PropagationChanged " + rtiIdentifier);
    T_Identifiers::const_iterator idIt( identifiers_.find( rtiIdentifier ) );
    if( idIt == identifiers_.end() )
        return;
    const unsigned int objectId( idIt->second );
    propagationManager_.saveDataFile( rtiIdentifier, data, col, lig, xll, yll, std::max( dx, dy ) );
    simulation::ObjectMagicAction message;
    message().mutable_object()->set_id( objectId );
    message().set_type( static_cast< sword::ObjectMagicAction_Type >( sword::ObjectMagicAction::update ) );
    sword::MissionParameter_Value* value = message().mutable_parameters()->add_elem()->add_value();
    value->add_list()->set_identifier( sword::ObjectMagicAction::disaster );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectUpdater::Notify
// Created: AHC 2013-09-09
// -----------------------------------------------------------------------------
void TacticalObjectUpdater::Notify( const sword::ObjectMagicActionAck& message, const std::string& identifier )
{
    if( message.error_code() == sword::UnitActionAck::no_error )
        identifiers_[ identifier ] = message.object().id();
}

