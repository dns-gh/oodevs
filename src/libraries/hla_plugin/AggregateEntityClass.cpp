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
#include "AggregateEntity.h"
#include "AgentSubject_ABC.h"
#include "Agent_ABC.h"
#include "Federate_ABC.h"
#include <hla/Class.h>
#include <hla/ClassIdentifier.h>
#include <hla/ObjectRegistration_ABC.h>

using namespace plugins::hla;

struct AggregateEntityClass::UnitRegistration : public ::hla::ObjectRegistration_ABC< AggregateEntity >
{
    virtual AggregateEntity& Create( const ::hla::ObjectIdentifier&, const std::string& )
    {
        throw std::runtime_error( __FUNCTION__ );
    }
    virtual void Destroy( AggregateEntity& )
    {
        throw std::runtime_error( __FUNCTION__ );
    }
};

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass constructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
AggregateEntityClass::AggregateEntityClass( Federate_ABC& federate, AgentSubject_ABC& subject )
    : id_          ( 1 )
    , subject_     ( subject )
    , registration_( new UnitRegistration() )
    , hlaClass_    ( new ::hla::Class< AggregateEntity >( *registration_, true ) )
{
    // BaseEntity.AggregateEntity
    hlaClass_->Register( ::hla::AttributeIdentifier( "EntityType" ) );              // static
    hlaClass_->Register( ::hla::AttributeIdentifier( "EntityIdentifier" ) );        // static
    hlaClass_->Register( ::hla::AttributeIdentifier( "Spatial" ) );                 // dynamic
    hlaClass_->Register( ::hla::AttributeIdentifier( "AggregateMarking" ) );        // static
    hlaClass_->Register( ::hla::AttributeIdentifier( "AggregateState" ) );          // static
    hlaClass_->Register( ::hla::AttributeIdentifier( "Dimensions" ) );              // static
    hlaClass_->Register( ::hla::AttributeIdentifier( "EntityIdentifiers" ) );       // static
    hlaClass_->Register( ::hla::AttributeIdentifier( "ForceIdentifier" ) );         // static
    hlaClass_->Register( ::hla::AttributeIdentifier( "Formation" ) );               // dynamic
    hlaClass_->Register( ::hla::AttributeIdentifier( "NumberOfSilentEntities" ) );  // dynamic
    hlaClass_->Register( ::hla::AttributeIdentifier( "SilentEntities" ) );          // dynamic
    hlaClass_->Register( ::hla::AttributeIdentifier( "SilentAggregates" ) );        // static
    hlaClass_->Register( ::hla::AttributeIdentifier( "SubAggregateIdentifiers" ) ); // static
    // BaseEntity.AggregateEntity.NETN_Aggregate
    hlaClass_->Register( ::hla::AttributeIdentifier( "Mounted" ) );                 // dynamic
    hlaClass_->Register( ::hla::AttributeIdentifier( "Echelon" ) );                 // static
    federate.Register( ::hla::ClassIdentifier( "BaseEntity.AggregateEntity.NETN_Aggregate" ), *hlaClass_, true, false );
    subject_.Register( *this );
    hlaClass_->ActivateUpdates( true );
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
    entities_.push_back( T_Entity( new AggregateEntity( agent, ++id_, name, force, type ) ) );
    hlaClass_->Register( *entities_.back(), identifier );
}
