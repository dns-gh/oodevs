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
#include "ClassListenerComposite.h"

#include "rpr/EntityType.h"

#include <hla/Class.h>

#include <boost/lexical_cast.hpp>
#include <algorithm>

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
    , pListeners_       ( new ClassListenerComposite() )
{
    builder->Build( federate, *hlaClass_ );
    std::vector< std::string > temp;
    builder->GetAttributes( temp );
    std::for_each( temp.begin(), temp.end(), [&](const std::string& name)
        {
            attributes_.push_back( ::hla::AttributeIdentifier( name ) );
        });
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
// Name: HlaTacticalObjectClass::Create
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
HlaObject_ABC& HlaTacticalObjectClass::Create ( const ::hla::ObjectIdentifier& /*objectID*/, const std::string& objectName )
{
    T_Entity& entity = remoteEntities_[ objectName ];
    entity.reset( remoteFactory_->Create( objectName ).release() );
    pListeners_->RemoteCreated( objectName, *this, *entity );
    return *entity;
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Destroy
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Destroy( HlaObject_ABC& /*object*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Reflected
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Reflected( HlaObject_ABC& /*object*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::RequestConfirmDivestiture
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
bool HlaTacticalObjectClass::RequestConfirmDivestiture( const ::hla::ObjectIdentifier& /*objectID*/, const HlaObject_ABC& /*object*/, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::OwnershipAcquisitionNotification
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::OwnershipAcquisitionNotification( const ::hla::ObjectIdentifier& /*objectID*/, const HlaObject_ABC& /*object*/, const ::hla::T_AttributeIdentifiers& /*attributes*/, const ::hla::VariableLengthData& /*tag*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::RequestOwnershipAssumption
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
bool HlaTacticalObjectClass::RequestOwnershipAssumption( const ::hla::ObjectIdentifier& /*objectID*/, const HlaObject_ABC& /*object*/, const ::hla::T_AttributeIdentifiers& /*attributes*/, const ::hla::VariableLengthData& /*tag*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Created
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Created( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
{
    std::string objectName( nameFactory_.CreateName( name + boost::lexical_cast< std::string >( identifier ) ) );
    T_Entity localEntity( factory_->Create( object, name, identifier, force, type, objectName ).release() );
    ::hla::ObjectIdentifier objectId( hlaClass_->Register( *localEntity, objectName ) );
    pListeners_->LocalCreated( objectId.ToString(), *this, *localEntity );
    localEntities_[ objectName ] = localEntity;
    //hlaIdentifiers_[ objectName ] = objectId.ToLong();
    //resolver_.Add( identifier, objectName );
    //pListeners_->LocalCreated( objectName, *this, *localEntity );
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Divest
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Divest(const std::string& /*objectID*/, const ::hla::T_AttributeIdentifiers&, const ::hla::VariableLengthData& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Acquire
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Acquire(const std::string& /*objectID*/, const ::hla::T_AttributeIdentifiers&, const ::hla::VariableLengthData& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Register
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Register( ClassListener_ABC& listener )
{
    pListeners_->Register( listener );
    std::for_each( remoteEntities_.begin(), remoteEntities_.end(), [&](const T_Entities::value_type& entity)
        {
            listener.RemoteCreated( entity.first, *this, *entity.second );
        });
    std::for_each( localEntities_.begin(), localEntities_.end(), [&](const T_Entities::value_type& entity)
        {
            listener.LocalCreated( entity.first, *this, *entity.second );
        });
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::Unregister
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::Unregister( ClassListener_ABC& listener )
{
    pListeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::RequestAttributeOwnershipRelease
// Created: AHC 2012-10-26
// -----------------------------------------------------------------------------
void HlaTacticalObjectClass::RequestOwnershipRelease( const ::hla::ObjectIdentifier&, const HlaObject_ABC&, const ::hla::T_AttributeIdentifiers&, const ::hla::VariableLengthData& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaTacticalObjectClass::GetAttributes
// Created: AHC 2012-10-29
// -----------------------------------------------------------------------------
const ::hla::T_AttributeIdentifiers& HlaTacticalObjectClass::GetAttributes() const
{
    return attributes_;
}
