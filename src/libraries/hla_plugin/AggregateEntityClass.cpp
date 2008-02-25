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
#include "HlaExtension_ABC.h"
#include "hla/ObjectRegistration_ABC.h"
#include "hla/Class.h"
#include "hla/Federate.h"
#include "hla/ClassIdentifier.h"
#include "hla/Deserializer.h"

using namespace hla;

struct AggregateEntityClass::UnitRegistration : public ObjectRegistration_ABC< HlaExtension_ABC >
{
    virtual HlaExtension_ABC& Create( const ObjectIdentifier& )
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
AggregateEntityClass::AggregateEntityClass()
    : registration_( new UnitRegistration() )
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
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass destructor
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
AggregateEntityClass::~AggregateEntityClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::RegisterTo
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
void AggregateEntityClass::RegisterTo( Federate& federate )
{
    federate.Register( ClassIdentifier( "BaseEntity.AggregateEntity" ), *hlaClass_, true, false );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntityClass::Register
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
void AggregateEntityClass::Register( HlaExtension_ABC& localObject )
{
    hlaClass_->Register( localObject );
}
