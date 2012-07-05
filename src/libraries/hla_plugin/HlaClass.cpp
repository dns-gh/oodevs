// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaClass.h"
#include "HlaObject_ABC.h"
#include "Agent_ABC.h"
#include "HlaObjectFactory_ABC.h"
#include "RemoteHlaObjectFactory_ABC.h"
#include "ClassListenerComposite.h"
#include "LocalAgentResolver_ABC.h"
#include "ClassBuilder_ABC.h"
#include "HlaObjectNameFactory_ABC.h"
#include <hla/Class.h>
#include <hla/ClassIdentifier.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: HlaClass constructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
HlaClass::HlaClass( Federate_ABC& federate, LocalAgentResolver_ABC& resolver, const HlaObjectNameFactory_ABC& nameFactory,
                    std::auto_ptr< HlaObjectFactory_ABC > factory, std::auto_ptr< RemoteHlaObjectFactory_ABC > remoteFactory,
                    std::auto_ptr< ClassBuilder_ABC > builder )
    : resolver_         ( resolver )
    , nameFactory_      ( nameFactory )
    , factory_          ( factory )
    , remoteFactory_    ( remoteFactory )
    , pListeners_       ( new ClassListenerComposite() )
    , hlaClass_         ( new ::hla::Class< HlaObject_ABC >( *this, true ) )
{
    builder->Build( federate, *hlaClass_ );
}

// -----------------------------------------------------------------------------
// Name: HlaClass destructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
HlaClass::~HlaClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaClass::Created
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void HlaClass::Created( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol )
{
    T_Entity localEntity( factory_->Create( agent, name, identifier, force, type, symbol ).release() );
    std::string objectName( nameFactory_.CreateName( boost::lexical_cast< std::string >( identifier ) ) );
    ::hla::ObjectIdentifier objectId( hlaClass_->Register( *localEntity, objectName ) );
    localEntity->SetIdentifier( objectName );
    pListeners_->LocalCreated( objectId.ToString(), *this, *localEntity );
    localEntities_[ objectName ] = localEntity;
    hlaIdentifiers_[ objectName ] = objectId.ToLong();
    resolver_.Add( identifier, objectName );
    pListeners_->LocalCreated( objectName, *this, *localEntity );
}

// -----------------------------------------------------------------------------
// Name: HlaClass::Create
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
HlaObject_ABC& HlaClass::Create( const ::hla::ObjectIdentifier& objectID, const std::string& objectName )
{
    T_Entity& entity = remoteEntities_[ objectName ];
    hlaIdentifiers_[ objectName ] = objectID.ToLong();
    entity.reset( remoteFactory_->Create( objectName ).release() );
    pListeners_->RemoteCreated( objectName, *this, *entity );
    return *entity;
}

// -----------------------------------------------------------------------------
// Name: HlaClass::Destroy
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void HlaClass::Destroy( HlaObject_ABC& object )
{
    BOOST_FOREACH( const T_Entities::value_type& entity, remoteEntities_ )
        if( &*entity.second == &object )
        {
            pListeners_->RemoteDestroyed( entity.first );
            remoteEntities_.erase( entity.first );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: HlaClass::Register
// Created: SLI 2011-08-29
// -----------------------------------------------------------------------------
void HlaClass::Register( ClassListener_ABC& listener )
{
    pListeners_->Register( listener );
    BOOST_FOREACH( const T_Entities::value_type& entity, remoteEntities_ )
        listener.RemoteCreated( entity.first, *this, *entity.second );
    BOOST_FOREACH( const T_Entities::value_type& entity, localEntities_ )
        listener.LocalCreated( entity.first, *this, *entity.second );
}

// -----------------------------------------------------------------------------
// Name: HlaClass::Unregister
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void HlaClass::Unregister( ClassListener_ABC& listener )
{
    pListeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: HlaClass::Reflected
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void HlaClass::Reflected( HlaObject_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaClass::RequestConfirmDivestiture
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
bool HlaClass::RequestConfirmDivestiture( const ::hla::ObjectIdentifier& /*objectID*/, const HlaObject_ABC& /*object*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HlaClass::OwnershipAcquisitionNotification
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void HlaClass::OwnershipAcquisitionNotification( const ::hla::ObjectIdentifier& /*objectID*/, const HlaObject_ABC& /*object*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaClass::RequestOwnershipAssumption
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
bool HlaClass::RequestOwnershipAssumption( const ::hla::ObjectIdentifier& /*objectID*/, const HlaObject_ABC& /*object*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    return false;
}
