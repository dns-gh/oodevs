// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaClass.h"
#include "HlaObject_ABC.h"
#include "Federate_ABC.h"
#include "Agent_ABC.h"
#include "HlaObjectFactory_ABC.h"
#include "RemoteHlaObjectFactory_ABC.h"
#include "ClassListenerComposite.h"
#include "LocalAgentResolver_ABC.h"
#include "ClassBuilder_ABC.h"
#include "HlaObjectNameFactory_ABC.h"
#include "OwnershipStrategy_ABC.h"
#include <hla/Class.h>
#include <hla/ClassIdentifier.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <cassert>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: HlaClass constructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
HlaClass::HlaClass( Federate_ABC& federate, LocalAgentResolver_ABC& resolver, const HlaObjectNameFactory_ABC& nameFactory,
                    std::auto_ptr< HlaObjectFactory_ABC > factory, std::auto_ptr< RemoteHlaObjectFactory_ABC > remoteFactory,
                    std::auto_ptr< ClassBuilder_ABC > builder, OwnershipStrategy_ABC& ownershipStrategy )
    : federate_         ( federate )
    , resolver_         ( resolver )
    , nameFactory_      ( nameFactory )
    , factory_          ( factory )
    , remoteFactory_    ( remoteFactory )
    , ownershipStrategy_( ownershipStrategy )
    , pListeners_       ( new ClassListenerComposite() )
    , hlaClass_         ( new ::hla::Class< HlaObject_ABC >( *this, true ) )
{
    builder->Build( federate, *hlaClass_ );
    std::vector< std::string > temp;
    builder->GetAttributes( temp );
    BOOST_FOREACH( const std::string& name, temp)
        attributes_.push_back( ::hla::AttributeIdentifier( name ) );
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
void HlaClass::Created( Agent_ABC& agent, unsigned long identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol )
{
    std::string objectName( nameFactory_.CreateName( name + boost::lexical_cast< std::string >( identifier ) ) );
    T_Entity localEntity( factory_->Create( agent, name, identifier, force, type, symbol, objectName ).release() );
    if( !localEntity.get() )
        return;
    ::hla::ObjectIdentifier objectId( hlaClass_->Register( *localEntity, objectName ) );
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
        if( entity.second.get() == &object )
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
bool HlaClass::RequestConfirmDivestiture( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    T_IdentifierSet::iterator it = divesting_.find( objectID.ToLong() );
    const std::string& objectName( object.GetIdentifier() );
    bool retval = ( divesting_.end() != it ) ||
            ( ownershipStrategy_.AcceptDivestiture( objectName, false ) );
    if( retval )
    {
        T_Entities::iterator locIt = localEntities_.find( objectName );
        assert( localEntities_.end() != locIt );
        remoteEntities_[ objectName ] = locIt->second;
        localEntities_.erase( locIt );
        divesting_.erase( it );
        pListeners_->Divested( objectName );
    }
    return retval;
}

// -----------------------------------------------------------------------------
// Name: HlaClass::OwnershipAcquisitionNotification
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void HlaClass::OwnershipAcquisitionNotification( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    T_IdentifierSet::iterator it = acquiring_.find( objectID.ToLong() );
    assert( acquiring_.end() != it );
    const std::string& objectName( object.GetIdentifier() );
    T_Entities::iterator remIt = remoteEntities_.find( objectName );
    assert( remoteEntities_.end() != remIt );
    remIt->second->ResetAttributes();
    localEntities_[ objectName ] = remIt->second;
    remoteEntities_.erase( remIt );
    acquiring_.erase( it );
    pListeners_->Acquired( objectName );
}

// -----------------------------------------------------------------------------
// Name: HlaClass::RequestOwnershipAssumption
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
bool HlaClass::RequestOwnershipAssumption( const ::hla::ObjectIdentifier& objectID, const HlaObject_ABC& object, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    T_IdentifierSet::iterator it = acquiring_.find( objectID.ToLong() );
    if( acquiring_.end() != it )
        return true;
    const std::string& objectName( object.GetIdentifier() );
    return ownershipStrategy_.AcceptAcquisition( objectName, false );
}

// -----------------------------------------------------------------------------
// Name: HlaClass::Divest
// Created: AHC 2012-03-01
// -----------------------------------------------------------------------------
void HlaClass::Divest(const std::string& objectID )
{
    T_Entities::iterator it = localEntities_.find( objectID );
    if( localEntities_.end() == it )
        return;

    assert( hlaIdentifiers_.end() != hlaIdentifiers_.find(objectID) );

    if( ownershipStrategy_.PerformAttributeOwnershipNegotiation() )
    {
        divesting_.insert( hlaIdentifiers_[objectID] );
        federate_.DivestRequest( hlaIdentifiers_[objectID], attributes_ );
    }
    else
    {
        federate_.UnconditionalDivest( hlaIdentifiers_[objectID], attributes_ );
        remoteEntities_[ objectID ] = it->second;
        localEntities_.erase( it );
        pListeners_->Divested( objectID );
    }
}

// -----------------------------------------------------------------------------
// Name: HlaClass::Acquire
// Created: AHC 2012-03-02
// -----------------------------------------------------------------------------
void HlaClass::Acquire(const std::string& objectID )
{
    T_Entities::iterator it = remoteEntities_.find( objectID );
    if( remoteEntities_.end() == it )
        return;

    if( ownershipStrategy_.PerformAttributeOwnershipNegotiation() )
    {
        acquiring_.insert( hlaIdentifiers_[objectID] );
        federate_.AcquisitionRequest( hlaIdentifiers_[objectID], attributes_ );
    }
    else
    {
        acquiring_.insert( hlaIdentifiers_[objectID] );
        federate_.UnconditionalAcquisition( hlaIdentifiers_[objectID], attributes_ );
    }
}
