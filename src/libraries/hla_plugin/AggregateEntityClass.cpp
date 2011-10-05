// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AggregateEntityClass.h"
#include "HlaObject_ABC.h"
#include "AgentSubject_ABC.h"
#include "Agent_ABC.h"
#include "HlaObjectFactory_ABC.h"
#include "RemoteHlaObjectFactory_ABC.h"
#include "RemoteAgentListener_ABC.h"
#include "RemoteAgentListenerComposite.h"
#include "LocalAgentResolver_ABC.h"
#include "ClassBuilder_ABC.h"
#include "ContextFactory_ABC.h"
#include <hla/Class.h>
#include <hla/ClassIdentifier.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass constructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
AggregateEntityClass::AggregateEntityClass( Federate_ABC& federate, AgentSubject_ABC& subject, LocalAgentResolver_ABC& resolver,
                                            std::auto_ptr< HlaObjectFactory_ABC > factory, std::auto_ptr< RemoteHlaObjectFactory_ABC > remoteFactory,
                                            const ClassBuilder_ABC& builder, const ContextFactory_ABC& identifierFactory )
    : subject_          ( subject )
    , resolver_         ( resolver )
    , factory_          ( factory )
    , remoteFactory_    ( remoteFactory )
    , identifierFactory_( identifierFactory )
    , pListeners_       ( new RemoteAgentListenerComposite() )
    , hlaClass_         ( new ::hla::Class< HlaObject_ABC >( *this, true ) )
{
    builder.BuildAggregate( federate, *hlaClass_, true, true );
    subject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass destructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
AggregateEntityClass::~AggregateEntityClass()
{
    subject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::AggregateCreated
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void AggregateEntityClass::AggregateCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
{
    T_Entity localEntity( factory_->Create( agent, name, static_cast< unsigned short >( identifierFactory_.Create() ), force, type ).release() );
    ::hla::ObjectIdentifier objectId = hlaClass_->Register( *localEntity, boost::lexical_cast< std::string >( identifier ) );
    localEntities_[ objectId.ToString() ] = localEntity;
    resolver_.Add( identifier, objectId.ToString() );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::SurfaceVesselCreated
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void AggregateEntityClass::SurfaceVesselCreated( Agent_ABC& /*agent*/, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Create
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
HlaObject_ABC& AggregateEntityClass::Create( const ::hla::ObjectIdentifier& objectID, const std::string& /*objectName*/ )
{
    T_Entity& entity = remoteEntities_[ objectID.ToString() ];
    entity.reset( remoteFactory_->Create( objectID.ToString(), *pListeners_ ).release() );
    pListeners_->Created( objectID.ToString() );
    return *entity;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Destroy
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void AggregateEntityClass::Destroy( HlaObject_ABC& object )
{
    BOOST_FOREACH( const T_Entities::value_type& entity, remoteEntities_ )
        if( &*entity.second == &object )
        {
            pListeners_->Destroyed( entity.first );
            remoteEntities_.erase( entity.first );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Register
// Created: SLI 2011-08-29
// -----------------------------------------------------------------------------
void AggregateEntityClass::Register( RemoteAgentListener_ABC& listener )
{
    pListeners_->Register( listener );
    BOOST_FOREACH( const T_Entities::value_type& entity, remoteEntities_ )
        listener.Created( entity.first );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Unregister
// Created: SLI 2011-08-29
// -----------------------------------------------------------------------------
void AggregateEntityClass::Unregister( RemoteAgentListener_ABC& listener )
{
    pListeners_->Unregister( listener );
}
