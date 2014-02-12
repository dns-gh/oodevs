// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TacticalObjectController.h"
#include "TacticalObjectListener_ABC.h"
#include "TacticalObjectProxy.h"

#include "dispatcher/Model_ABC.h"
#include "dispatcher/Object.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/ObjectAttributeContainer.h"
#include "dispatcher/PropagationAttribute.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Karma.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include "rpr/EntityTypeResolver_ABC.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace plugins::hla;

namespace
{
    const dispatcher::PropagationAttribute* getPropagationAttribute(const dispatcher::Object_ABC& object)
    {
        const dispatcher::PropagationAttribute* retval = 0;
        for( auto it = object.GetAttributes().GetAttributes().begin(); it!=object.GetAttributes().GetAttributes().end(); ++it)
        {
            retval = dynamic_cast< const dispatcher::PropagationAttribute* >( &*it );
            if( retval )
                break;
        }
        return retval;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectController::Register
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
TacticalObjectController::TacticalObjectController( dispatcher::Model_ABC& model, const kernel::CoordinateConverter_ABC& converter, 
        const rpr::EntityTypeResolver_ABC& objectResolver, const rpr::EntityTypeResolver_ABC& dotationResolver, dispatcher::Logger_ABC& logger,
        const SimulationTimeManager_ABC& timeManager )
    : model_( model )
    , converter_( converter )
    , objectResolver_( objectResolver )
    , dotationResolver_( dotationResolver )
    , logger_( logger )
    , timeManager_( timeManager )
{
    model_.RegisterFactory( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectController constructor
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
TacticalObjectController::~TacticalObjectController()
{
    model_.UnregisterFactory( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectController::Register
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectController::Register( TacticalObjectListener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectController::Unregister
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectController::Unregister( TacticalObjectListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectController::Visit
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectController::Visit( dispatcher::Model_ABC& model )
{
    for( tools::Iterator< const dispatcher::Object_ABC& > it = model.Objects().CreateIterator(); it.HasMoreElements(); )
    {
        dispatcher::Object_ABC& object = const_cast< dispatcher::Object_ABC& >( it.NextElement() ); // $$$$ _RC_ SLI 2011-09-28: erk...
        CreateObject( object );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectController::Create
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectController::Create( dispatcher::Object& object )
{
    CreateObject( object );
}

namespace
{
    rpr::ForceIdentifier GetForce( const dispatcher::Object_ABC& object )
    {
        const kernel::Karma& karma = object.GetTeam().GetKarma();
        if( karma == kernel::Karma::friend_ )
            return rpr::Friendly;
        if( karma == kernel::Karma::enemy_ )
            return rpr::Opposing;
        if( karma == kernel::Karma::neutral_ )
            return rpr::Neutral;
        return rpr::Other;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectController::Create
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectController::CreateObject( dispatcher::Object_ABC& object )
{
    std::string remoteExt;
    const bool isRemote = object.GetExtension( "RemoteEntity", remoteExt ) && remoteExt == "true";
    // TODO check if object must be created
    if( !isRemote )
    {
        T_Objects::iterator itObject( objects_.insert( T_Objects::value_type( object.GetId(), T_Object( new TacticalObjectProxy( object, dotationResolver_, timeManager_ ) ) ) ).first );
        const kernel::ObjectType& objectType = object.GetType();
        const bool isPropagation = !objectType.GetDisasterType().empty();
        const std::string typeName = objectType.GetType();
        rpr::EntityType entityType;
        if( !objectResolver_.Find( typeName, entityType ) )
            logger_.LogWarning( std::string( "Could not find EntityType for object type " ) + typeName );
        const rpr::ForceIdentifier forceIdentifier = GetForce( object );
        TacticalObjectListener_ABC::GeometryType geom = TacticalObjectListener_ABC::eGeometryType_Polygon;
        if( object.GetLocalisation().GetTypeName()  == "line" )
            geom = TacticalObjectListener_ABC::eGeometryType_Line;
        else if(object.GetLocalisation().GetTypeName() == "point" )
            geom = TacticalObjectListener_ABC::eGeometryType_Point;
        bool isBreachable = objectType.CanBeBypassed();
        for( auto it = listeners_.begin(); it != listeners_.end(); ++it )
            (*it)->ObjectCreated( *(itObject->second), object.GetId(), object.GetName().toStdString(), forceIdentifier, entityType, isBreachable, geom, isPropagation );
    }
}

