// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaTacticalObjectClass.h"

#include "RemoteHlaObjectFactory_ABC.h"
#include "HlaObjectFactory_ABC.h"
#include "HlaObjectNameFactory_ABC.h"

#include "ClassBuilder_ABC.h"
#include "HlaObject_ABC.h"

#include "rpr/EntityType.h"

#include <hla/Class.h>

#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass constructor
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
HlaTacticalObjectClass::HlaTacticalObjectClass( Federate_ABC& federate, const HlaObjectNameFactory_ABC& nameFactory, std::auto_ptr< HlaTacticalObjectFactory_ABC > factory,
        std::auto_ptr< RemoteHlaObjectFactory_ABC > remoteFactory, std::auto_ptr< ClassBuilder_ABC > builder )
    : nameFactory_( nameFactory )
    , factory_( factory )
    , remoteFactory_    ( remoteFactory )
    , hlaClass_         ( new ::hla::Class< HlaObject_ABC >( *this, true ) )
{
    builder->Build( federate, *hlaClass_ );
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass destructor
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
HlaTacticalObjectClass::~HlaTacticalObjectClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass destructor
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
HlaObject_ABC& HlaTacticalObjectClass::Create ( const ::hla::ObjectIdentifier& objectID, const std::string& objectName )
{
    T_Entity& entity = remoteEntities_[ objectName ];
    entity.reset( remoteFactory_->Create( objectName ).release() );
    return *entity;
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Destroy
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Destroy( HlaObject_ABC& object )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Reflected
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Reflected( HlaObject_ABC& object )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::RequestConfirmDivestiture
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
bool HlaTacticalObjectClass::RequestConfirmDivestiture( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const ::hla::T_AttributeIdentifiers& attributes )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::OwnershipAcquisitionNotification
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::OwnershipAcquisitionNotification( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const ::hla::T_AttributeIdentifiers& attributes )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::RequestOwnershipAssumption
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
bool HlaTacticalObjectClass::RequestOwnershipAssumption( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const ::hla::T_AttributeIdentifiers& attributes )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Created
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Created( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
{
    T_Entity localEntity( factory_->Create( object, name, identifier, force, type ).release() );
    std::string objectName( nameFactory_.CreateName( name + boost::lexical_cast< std::string >( identifier ) ) );
    ::hla::ObjectIdentifier objectId( hlaClass_->Register( *localEntity, objectName ) );
    localEntity->SetIdentifier( objectName );
    //pListeners_->LocalCreated( objectId.ToString(), *this, *localEntity );
    localEntities_[ objectName ] = localEntity;
    //hlaIdentifiers_[ objectName ] = objectId.ToLong();
    //resolver_.Add( identifier, objectName );
    //pListeners_->LocalCreated( objectName, *this, *localEntity );
}
