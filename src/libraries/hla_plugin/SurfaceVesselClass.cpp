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
#include "ContextFactory_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include <hla/Class.h>
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVesselClass::SurfaceVesselClass( Federate_ABC& federate, AgentSubject_ABC& subject, LocalAgentResolver_ABC& resolver,
                                        const HlaObjectFactory_ABC& factory, const ClassBuilder_ABC& builder, const ContextFactory_ABC& identifierFactory )
    : subject_          ( subject )
    , resolver_         ( resolver )
    , factory_          ( factory )
    , identifierFactory_( identifierFactory )
    , hlaClass_         ( new ::hla::Class< HlaObject_ABC >( *this, true ) )
{
    builder.BuildSurfaceVessel( federate, *hlaClass_, true, false );
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
    T_Entity localEntity( factory_.CreateSurfaceVessel( agent, name, static_cast< unsigned short >( identifierFactory_.Create() ), force, type ).release() );
    ::hla::ObjectIdentifier objectId = hlaClass_->Register( *localEntity, boost::lexical_cast< std::string >( identifier ) );
    localEntities_[ objectId.ToString() ] = localEntity;
    resolver_.Add( identifier, objectId.ToString() );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass::Create
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
HlaObject_ABC& SurfaceVesselClass::Create( const ::hla::ObjectIdentifier& /*objectID*/, const std::string& /*objectName*/ )
{
    throw std::runtime_error( std::string( "unimplemented method " ) + __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVesselClass::Destroy
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVesselClass::Destroy( HlaObject_ABC& /*object*/ )
{
    throw std::runtime_error( std::string( "unimplemented method " ) + __FUNCTION__ );
}
