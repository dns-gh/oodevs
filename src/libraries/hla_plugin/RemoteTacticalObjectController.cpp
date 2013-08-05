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
#include "PropagationManager_ABC.h"
#include "ContextHandler_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/ObjectType.h"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include <sstream>
#include <limits>
#include <fstream>


using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController constructor
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
RemoteTacticalObjectController::RemoteTacticalObjectController( const ExtentResolver_ABC& extent, const SideResolver_ABC& sideResolver,
    const rpr::EntityTypeResolver_ABC& objectEntityTypeResolver, ContextHandler_ABC< sword::ObjectMagicActionAck >& contextHandler,
    RemoteTacticalObjectSubject_ABC& subject, dispatcher::Logger_ABC& logger, PropagationManager_ABC& propMgr )
    : extent_( extent )
    , sideResolver_( sideResolver )
    , objectEntityTypeResolver_( objectEntityTypeResolver )
    , contextHandler_( contextHandler )
    , subject_( subject )
    , logger_( logger )
    , propagationManager_( propMgr )
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
    message().mutable_parameters()->add_elem()->add_value()->set_acharstr("remote"+boost::lexical_cast<std::string>(objId)); // name FIXME
    message().mutable_parameters()->add_elem(); // army
    message().mutable_parameters()->add_elem(); // attributes
    sword::Extension* ext = message().mutable_parameters()->add_elem()->add_value()->mutable_extensionlist();
    sword::Extension_Entry* entry = ext->add_entries(); // extension
    entry->set_name( "RemoteEntity" );
    entry->set_value( "true" );
    entry = ext->add_entries(); // extension
    entry->set_name( "HLA_ObjectID" );
    entry->set_value( identifier );
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
void RemoteTacticalObjectController::Divested( const std::string& /*identifier*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::Acquired
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::Acquired( const std::string& /*identifier*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
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
    simulation::ObjectMagicAction& message = *it->second;
    sword::Location* loc = message().mutable_parameters()->mutable_elem( 1 )->mutable_value()->size() == 0 ?
            message().mutable_parameters()->mutable_elem( 1 )->add_value()->mutable_location() :
            message().mutable_parameters()->mutable_elem( 1 )->mutable_value( 0 )->mutable_location();

    loc->set_type( sword::Location_Geometry_point );
    loc->mutable_coordinates()->clear_elem();
    sword::CoordLatLong *elem = loc->mutable_coordinates()->add_elem();
    elem->set_latitude( latitude );
    elem->set_longitude( longitude );

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
    if( message().mutable_parameters()->mutable_elem( 3 )->mutable_value()->size() == 1  )
        return;
    message().mutable_parameters()->mutable_elem( 3 )->add_value()->mutable_party()->set_id( sideResolver_.ResolveTeam( side ) );
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
    if( message().mutable_parameters()->mutable_elem( 2 )->mutable_value()->size() == 1  )
        message().mutable_parameters()->mutable_elem( 2 )->mutable_value()->Mutable( 0 )->set_acharstr( name.c_str() );
    else
        message().mutable_parameters()->mutable_elem( 2 )->add_value()->set_acharstr( name.c_str() );
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

    if( message().mutable_parameters()->mutable_elem( 0 )->mutable_value()->size() == 1 )
        message().mutable_parameters()->mutable_elem( 0 )->mutable_value()->Mutable( 0 )->set_acharstr( objTypeName.c_str() );
    else
        message().mutable_parameters()->mutable_elem( 0 )->add_value()->set_acharstr( objTypeName.c_str() );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::EquipmentUpdated
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*available*/,
            unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::UniqueIdChanged
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::UniqueIdChanged( const std::string& /*identifier*/, const T_UniqueId& /*uniqueId*/ )
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
void RemoteTacticalObjectController::EmbeddedUnitListChanged( const std::string& /*identifier*/, const std::vector< T_UniqueId >& /*units*/ )
{
    // NOTHING
}

namespace
{
    void fillCoord( const rpr::WorldLocation& p, sword::CoordLatLongList& list )
    {
        sword::CoordLatLong *elem = list.add_elem();
        elem->set_latitude( p.Latitude() );
        elem->set_longitude( p.Longitude() );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::GeometryChanged( const std::string& identifier, const std::vector< rpr::WorldLocation >& perimeter, ObjectListener_ABC::GeometryType type )
{
    T_ObjectCreations::iterator it( objectCreations_.find( identifier ) );
    if( objectCreations_.end() == it)
        return;
    perimeters_[ identifier ] = perimeter;
    simulation::ObjectMagicAction& message = *it->second;

    sword::Location* loc = message().mutable_parameters()->mutable_elem( 1 )->mutable_value()->size() == 0 ?
            message().mutable_parameters()->mutable_elem( 1 )->add_value()->mutable_location() :
            message().mutable_parameters()->mutable_elem( 1 )->mutable_value( 0 )->mutable_location();
    loc->mutable_coordinates()->clear_elem();
    if( perimeter.size() == 1 )
    {
        loc->set_type( sword::Location_Geometry_point );
        sword::CoordLatLong *elem = loc->mutable_coordinates()->add_elem();
        elem->set_latitude( perimeter[0].Latitude() );
        elem->set_longitude( perimeter[0].Longitude() );
    }
    else
    {
        loc->set_type( type == ObjectListener_ABC::eGeometryType_Line ? sword::Location_Geometry_line : sword::Location_Geometry_polygon );
        std::for_each( perimeter.begin(), perimeter.end(), boost::bind( &fillCoord, _1, boost::ref( *loc->mutable_coordinates() ) ) );
    }
    Send( message, identifier );
}


// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::Send
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::Send( simulation::ObjectMagicAction& message, const std::string& identifier )
{
    {
        std::stringstream ss;
        ss << "Attempt creation object " << identifier << std::boolalpha << " " << message().has_type() << " " <<
            ( message().has_object() ) << " " <<
            ( message().parameters().elem( 0 ).value_size() > 0 ) << " " << // type
            ( message().parameters().elem( 1 ).value_size() > 0 ) << " " << // position
            ( message().parameters().elem( 3 ).value_size() > 0 ); // army
        logger_.LogInfo( ss.str() );
    }

    if( message().parameters().elem( 0 ).value_size() > 0 &&
        message().parameters().elem( 1 ).value_size() > 0 &&
        message().parameters().elem( 3 ).value_size() > 0 )
    {
        contextHandler_.Send( message, identifier );
        objectCreations_.erase( identifier );
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

// -----------------------------------------------------------------------------
// Name: RemoteTacticalObjectController::PropagationChanged
// Created: AHC 2013-07-08
// -----------------------------------------------------------------------------
void RemoteTacticalObjectController::PropagationChanged( const std::string& identifier, const std::vector< ObjectListener_ABC::PropagationData >&  data,
    int col, int lig, double xll, double yll, double dx, double dy)
{
    if( data.empty() )
    {
        logger_.LogWarning("RemoteTacticalObjectController empty propagation for " + identifier);
        return;
    }
    // type, location, name, team, attributes, extension
    T_ObjectCreations::iterator it( objectCreations_.find( identifier ) );
    if( objectCreations_.end() == it)
        return;
    simulation::ObjectMagicAction& message = *it->second;

    logger_.LogInfo("Received propagation data " + identifier);
    // Location
    sword::Location* loc = message().mutable_parameters()->mutable_elem( 1 )->mutable_value()->size() == 0 ?
            message().mutable_parameters()->mutable_elem( 1 )->mutable_value()->Add()->mutable_location() :
            message().mutable_parameters()->mutable_elem( 1 )->mutable_value( 0 )->mutable_location();
    loc->set_type( sword::Location_Geometry_point );
    loc->mutable_coordinates()->clear_elem();
    sword::CoordLatLong *elem = loc->mutable_coordinates()->add_elem();
    elem->set_latitude( data[0].latitude );
    elem->set_longitude( data[0].longitude );
    // side
    message().mutable_parameters()->mutable_elem( 3 )->mutable_value()->Add()->mutable_party()->set_id( 1 );
    // attributes
    sword::MissionParameter_Value* value = message().mutable_parameters()->mutable_elem( 4 )->add_value();
    value->add_list()->set_identifier( sword::ObjectMagicAction::disaster );
    value->add_list()->set_acharstr( identifier );

    // FIXME save data locally
    propagationManager_.saveDataFile( identifier, data, col, lig, xll, yll, std::max( dx, dy ) );

    Send( message, identifier );
}

