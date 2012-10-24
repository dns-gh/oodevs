// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteTacticalObjectController.h"
#include "HlaObject_ABC.h"
#include "ExtentResolver_ABC.h"
#include "SideResolver_ABC.h"
#include "RemoteTacticalObjectSubject_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/ObjectType.h"

#include <boost/bind.hpp>

#include <limits>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController constructor
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
RemoteTacticalObjectController::RemoteTacticalObjectController( const ExtentResolver_ABC& extent, const SideResolver_ABC& sideResolver,
    const rpr::EntityTypeResolver_ABC& objectEntityTypeResolver, dispatcher::SimulationPublisher_ABC& publisher,
    RemoteTacticalObjectSubject_ABC& subject, dispatcher::Logger_ABC& logger )
    : extent_( extent )
    , sideResolver_( sideResolver )
    , objectEntityTypeResolver_( objectEntityTypeResolver )
    , publisher_( publisher )
    , subject_( subject )
    , logger_( logger )
{
    subject_.RegisterTactical( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController destructor
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
RemoteTacticalObjectController::~RemoteTacticalObjectController()
{
    subject_.UnregisterTactical( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::RemoteCreated
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::RemoteCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& object )
{
    static unsigned int objId = std::numeric_limits< unsigned int>::max();
    object.Register( *this );
    objectCreations_[ identifier ] = T_ObjectCreation( new simulation::ObjectMagicAction() );
    simulation::ObjectMagicAction& message = *objectCreations_[ identifier ];
    message().set_type( sword::ObjectMagicAction::create );
    message().mutable_object()->set_id( objId ); // FIXME
    --objId;
    message().mutable_parameters()->add_elem(); // type
    message().mutable_parameters()->add_elem(); // position
    message().mutable_parameters()->add_elem(); // name
    message().mutable_parameters()->add_elem(); // army
    /* Uncomment when simulation_kernel is modified
    sword::Extension* ext = message().mutable_parameters()->add_elem()->add_value()->mutable_extensionlist();
    sword::Extension_Entry* entry = ext->add_entries(); // extension
    entry->set_name( "RemoteEntity" );
    entry->set_value( "true" );
    entry = ext->add_entries(); // extension
    entry->set_name( "HLA_ObjectID" );
    entry->set_value( identifier );*/
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::RemoteDestroyed
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::RemoteDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::LocalCreated
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::LocalCreated( const std::string& /*identifier*/, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::LocalDestroyed
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::LocalDestroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::Divested
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::Divested( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::Acquired
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::Acquired( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::Moved
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::Moved( const std::string& identifier, double latitude, double longitude )
{
    T_ObjectCreations::iterator it( objectCreations_.find( identifier ) );
    if( objectCreations_.end() == it)
        return;
    centers_[ identifier ] = std::make_pair( latitude, longitude );
    simulation::ObjectMagicAction& message = *it->second;
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::SideChanged
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    T_ObjectCreations::iterator it( objectCreations_.find( identifier ) );
    if( objectCreations_.end() == it)
        return;
    simulation::ObjectMagicAction& message = *it->second;
    message().mutable_parameters()->mutable_elem( 3 )->mutable_value()->Add()->mutable_party()->set_id( sideResolver_.ResolveTeam( side ) );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::NameChanged
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::NameChanged( const std::string& identifier, const std::string& name )
{
    T_ObjectCreations::iterator it( objectCreations_.find( identifier ) );
    if( objectCreations_.end() == it)
        return;
    simulation::ObjectMagicAction& message = *it->second;
    message().mutable_parameters()->mutable_elem( 2 )->mutable_value()->Add()->set_acharstr( std::string( "HLA_" ) +name.c_str() );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::TypeChanged
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::TypeChanged( const std::string& identifier, const rpr::EntityType& type )
{
    T_ObjectCreations::iterator it( objectCreations_.find( identifier ) );
    if( objectCreations_.end() == it)
        return;
    simulation::ObjectMagicAction& message = *it->second;
    std::string objTypeName;
    if( !objectEntityTypeResolver_.Resolve( type, objTypeName ) )
        logger_.LogWarning( std::string( "Could not find object for EntityType: ") + type.str() );
    message().mutable_parameters()->mutable_elem( 0 )->mutable_value()->Add()->set_acharstr( objTypeName.c_str() );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::EquipmentUpdated
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*number*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::UniqueIdChanged
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::UniqueIdChanged( const std::string& /*identifier*/, const std::string& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::CallsignChanged
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::EmbeddedUnitListChanged
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< std::string >& /*units*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::PerimeterChanged( const std::string& identifier, const std::vector< rpr::PerimeterPoint >& perimeter )
{
    T_ObjectCreations::iterator it( objectCreations_.find( identifier ) );
    if( objectCreations_.end() == it)
        return;
    perimeters_[ identifier ] = perimeter;
    simulation::ObjectMagicAction& message = *it->second;
    Send( message, identifier );
}

namespace
{
    void fillCoord( const rpr::PerimeterPoint& p, const rpr::WorldLocation& center, sword::CoordLatLongList& list )
    {
        sword::CoordLatLong *elem = list.add_elem();
        rpr::WorldLocation v;
        v.Reset( center.X() + p.X(), center.Y() + p.Y(), center.Z() );
        elem->set_latitude( v.Latitude() );
        elem->set_longitude( v.Longitude() );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::Send
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::Send( simulation::ObjectMagicAction& message, const std::string& identifier )
{
    T_Centers::const_iterator itC( centers_.find( identifier ) );
    T_Perimeters::const_iterator itP( perimeters_.find( identifier ) );

    if( message().parameters().elem( 0 ).value_size() > 0 &&        
        message().parameters().elem( 2 ).value_size() > 0 &&
        message().parameters().elem( 3 ).value_size() > 0 && 
        centers_.end() != itC &&
        perimeters_.end() != itP )
    {
        sword::Location* loc = message().mutable_parameters()->mutable_elem( 1 )->mutable_value()->Add()->mutable_location();
        if( itP->second.size() == 1 )
        {
            loc->set_type( sword::Location_Geometry_point );
            sword::CoordLatLong *elem = loc->mutable_coordinates()->add_elem();
            elem->set_latitude( itC->second.first );
            elem->set_longitude( itC->second.second );
        }
        else
        {
            loc->set_type( sword::Location_Geometry_polygon );
            rpr::WorldLocation center( itC->second.first, itC->second.second, 0);
            std::for_each( itP->second.begin(), itP->second.end(), boost::bind( &fillCoord, _1, boost::cref( center ), boost::ref( *loc->mutable_coordinates() ) ) );
        }
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::ParentChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::ParentChanged( const std::string& /*rtiIdentifier*/, const std::string& /*parentRtiId*/ )
{
    //  NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::SubAgregatesChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::SubAgregatesChanged( const std::string& /*rtiIdentifier*/, const std::set< std::string >& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::SubEntitiesChanged
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::SubEntitiesChanged(const std::string& /*rtiIdentifier*/, const std::set< std::string >& /*children*/ )
{
    // NOTHING
}
