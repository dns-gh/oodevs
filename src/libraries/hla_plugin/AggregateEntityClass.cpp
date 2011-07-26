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
#include "ClassBuilder_ABC.h"
#include <hla/Class.h>
#include <hla/ClassIdentifier.h>
#include <hla/ObjectRegistration_ABC.h>

using namespace plugins::hla;

struct AggregateEntityClass::UnitRegistration : public ::hla::ObjectRegistration_ABC< Aggregate_ABC >
{
    virtual Aggregate_ABC& Create( const ::hla::ObjectIdentifier&, const std::string& )
    {
        throw std::runtime_error( __FUNCTION__ );
    }
    virtual void Destroy( Aggregate_ABC& )
    {
        throw std::runtime_error( __FUNCTION__ );
    }
};

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass constructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
AggregateEntityClass::AggregateEntityClass( Federate_ABC& federate, AgentSubject_ABC& subject,
                                            const AggregateFactory_ABC& factory, const ClassBuilder_ABC& builder )
    : id_          ( 1 )
    , subject_     ( subject )
    , factory_     ( factory )
    , registration_( new UnitRegistration() )
    , hlaClass_    ( new ::hla::Class< Aggregate_ABC >( *registration_, true ) )
{
    builder.Build( federate, *hlaClass_, true, false );
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
    entities_[ identifier ] = T_Entity( factory_.Create( agent, name, ++id_, force, type ).release() );
    hlaClass_->Register( *entities_[ identifier ], identifier );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Destroyed
// Created: SLI 2011-07-25
// -----------------------------------------------------------------------------
void AggregateEntityClass::Destroyed( const std::string& identifier )
{
    if( entities_.find( identifier ) == entities_.end() )
        return;
    hlaClass_->Unregister( *entities_[ identifier ] );
    entities_.erase( identifier );
}
