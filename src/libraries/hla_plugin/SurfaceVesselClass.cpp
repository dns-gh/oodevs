// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SurfaceVesselClass.h"
#include "ClassBuilder_ABC.h"
#include "AgentSubject_ABC.h"
#include "HlaObject_ABC.h"
#include "HlaObjectFactory_ABC.h"
#include "RemoteHlaObjectFactory_ABC.h"
#include "ContextFactory_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include "RemoteAgentListenerComposite.h"
#include <hla/Class.h>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVesselClass::SurfaceVesselClass( Federate_ABC& federate, AgentSubject_ABC& subject, LocalAgentResolver_ABC& resolver,
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
    builder.BuildSurfaceVessel( federate, *hlaClass_, true, true );
    subject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass destructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVesselClass::~SurfaceVesselClass()
{
    subject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass::AggregateCreated
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVesselClass::AggregateCreated( Agent_ABC& /*agent*/, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass::SurfaceVesselCreated
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVesselClass::SurfaceVesselCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type )
{
    T_Entity localEntity( factory_->Create( agent, name, static_cast< unsigned short >( identifierFactory_.Create() ), force, type ).release() );
    ::hla::ObjectIdentifier objectId = hlaClass_->Register( *localEntity, boost::lexical_cast< std::string >( identifier ) );
    localEntities_[ objectId.ToString() ] = localEntity;
    resolver_.Add( identifier, objectId.ToString() );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass::Create
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
HlaObject_ABC& SurfaceVesselClass::Create( const ::hla::ObjectIdentifier& objectID, const std::string& /*objectName*/ )
{
    T_Entity& entity = remoteEntities_[ objectID.ToString() ];
    entity.reset( remoteFactory_->Create( objectID.ToString(), *pListeners_ ).release() );
    pListeners_->Created( objectID.ToString() );
    return *entity;
}

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass::Destroy
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVesselClass::Destroy( HlaObject_ABC& object )
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
// Name: SurfaceVesselClass::Register
// Created: SLI 2011-10-05
// -----------------------------------------------------------------------------
void SurfaceVesselClass::Register( RemoteAgentListener_ABC& listener )
{
    pListeners_->Register( listener );
    BOOST_FOREACH( const T_Entities::value_type& entity, remoteEntities_ )
        listener.Created( entity.first );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass::Unregister
// Created: SLI 2011-10-05
// -----------------------------------------------------------------------------
void SurfaceVesselClass::Unregister( RemoteAgentListener_ABC& listener )
{
    pListeners_->Unregister( listener );
}
