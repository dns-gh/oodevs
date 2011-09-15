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
#include "Aggregate_ABC.h"
#include "AgentSubject_ABC.h"
#include "Agent_ABC.h"
#include "AggregateFactory_ABC.h"
#include "RemoteAggregateFactory_ABC.h"
#include "RemoteAgentListener_ABC.h"
#include "RemoteAgentListenerComposite.h"
#include "ClassBuilder_ABC.h"
#include <hla/Class.h>
#include <hla/ClassIdentifier.h>
#include <hla/ObjectRegistration_ABC.h>
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass constructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
AggregateEntityClass::AggregateEntityClass( Federate_ABC& federate, AgentSubject_ABC& subject,
                                            const AggregateFactory_ABC& factory, const RemoteAggregateFactory_ABC& remoteFactory,
                                            const ClassBuilder_ABC& builder )
    : id_           ( 1 )
    , subject_      ( subject )
    , factory_      ( factory )
    , remoteFactory_( remoteFactory )
    , pListeners_   ( new RemoteAgentListenerComposite() )
    , hlaClass_     ( new ::hla::Class< Aggregate_ABC >( *this, true ) )
{
    builder.Build( federate, *hlaClass_, true, true );
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
// Name: AggregateEntityClass::Created
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void AggregateEntityClass::Created( Agent_ABC& agent, const std::string& identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
{
    localEntities_[ identifier ] = T_Entity( factory_.Create( agent, name, ++id_, force, type ).release() );
    hlaClass_->Register( *localEntities_[ identifier ], identifier );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Destroyed
// Created: SLI 2011-07-25
// -----------------------------------------------------------------------------
void AggregateEntityClass::Destroyed( const std::string& identifier )
{
    if( localEntities_.find( identifier ) == localEntities_.end() )
        return;
    hlaClass_->Unregister( *localEntities_[ identifier ] );
    localEntities_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Create
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
Aggregate_ABC& AggregateEntityClass::Create( const ::hla::ObjectIdentifier& /*objectID*/, const std::string& objectName )
{
    T_Entity& entity = remoteEntities_[ objectName ];
    entity.reset( remoteFactory_.Create( objectName, *pListeners_ ).release() );
    pListeners_->Created( objectName );
    return *entity;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Destroy
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void AggregateEntityClass::Destroy( Aggregate_ABC& object )
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
