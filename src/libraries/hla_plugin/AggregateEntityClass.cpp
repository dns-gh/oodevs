// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AggregateEntityClass.h"
#include "AgentExtension.h"
#include "AgentSubject_ABC.h"
#include "Agent_ABC.h"
#include "rpr/EntityIdentifier.h"
#include <hla/Class.h>
#include <hla/ClassIdentifier.h>
#include <hla/Deserializer.h>
#include <hla/Federate.h>
#include <hla/ObjectRegistration_ABC.h>

using namespace plugins::hla;
using namespace hla;

struct AggregateEntityClass::UnitRegistration : public ObjectRegistration_ABC< HlaExtension_ABC >
{
    virtual HlaExtension_ABC& Create( const ObjectIdentifier&, const std::string& )
    {
        throw std::runtime_error( __FUNCTION__ );
    }
    virtual void Destroy( HlaExtension_ABC& )
    {
        throw std::runtime_error( __FUNCTION__ );
    }
};

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass constructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
AggregateEntityClass::AggregateEntityClass( Federate& federate, AgentSubject_ABC& subject )
    : id_          ( 1 )
    , subject_     ( subject )
    , registration_( new UnitRegistration() )
    , hlaClass_    ( new Class< HlaExtension_ABC >( *registration_ ) )
{
    hlaClass_->Register( AttributeIdentifier( "EntityType" ) );             // static
    hlaClass_->Register( AttributeIdentifier( "EntityIdentifier" ) );       // static
    hlaClass_->Register( AttributeIdentifier( "Spatial" ) );                // dynamic
    hlaClass_->Register( AttributeIdentifier( "AggregateMarking" ) );       // static
    hlaClass_->Register( AttributeIdentifier( "AggregateState" ) );         // static
    hlaClass_->Register( AttributeIdentifier( "Dimensions" ) );             // static
    hlaClass_->Register( AttributeIdentifier( "ForceIdentifier" ) );        // static
    hlaClass_->Register( AttributeIdentifier( "Formation" ) );              // dynamic
    hlaClass_->Register( AttributeIdentifier( "NumberOfSilentEntities" ) ); // dynamic
    hlaClass_->Register( AttributeIdentifier( "SilentEntities" ) );         // dynamic
    federate.Register( ClassIdentifier( "BaseEntity.AggregateEntity" ), *hlaClass_, true, false );
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
void AggregateEntityClass::Created( Agent_ABC& agent, const std::string& identifier )
{
    rpr::EntityIdentifier id( 1, 1, id_ ); // site, application, id
    boost::shared_ptr< AgentExtension > extension( new AgentExtension( agent, agent, agent, id ) );
    hlaClass_->Register( *extension, identifier );
    extensions_.push_back( extension );
    ++id_;
}
