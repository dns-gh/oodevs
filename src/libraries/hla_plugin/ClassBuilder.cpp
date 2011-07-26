// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ClassBuilder.h"
#include "Aggregate_ABC.h"
#include "Federate_ABC.h"
#include <hla/Class.h>
#include <hla/AttributeIdentifier.h>
#include <hla/ClassIdentifier.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ClassBuilder constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
ClassBuilder::ClassBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassBuilder destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
ClassBuilder::~ClassBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassBuilder::Build
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void ClassBuilder::Build( Federate_ABC& federate, ::hla::Class< Aggregate_ABC >& hlaClass, bool publish, bool subscribe ) const
{
    hlaClass.Register( ::hla::AttributeIdentifier( "EntityType" ) );              // static
    hlaClass.Register( ::hla::AttributeIdentifier( "EntityIdentifier" ) );        // static
    hlaClass.Register( ::hla::AttributeIdentifier( "ForceIdentifier" ) );         // static
    hlaClass.Register( ::hla::AttributeIdentifier( "AggregateMarking" ) );        // static
    hlaClass.Register( ::hla::AttributeIdentifier( "AggregateState" ) );          // static
    hlaClass.Register( ::hla::AttributeIdentifier( "Dimensions" ) );              // static
    hlaClass.Register( ::hla::AttributeIdentifier( "Spatial" ) );                 // dynamic
    hlaClass.Register( ::hla::AttributeIdentifier( "Formation" ) );               // dynamic
    hlaClass.Register( ::hla::AttributeIdentifier( "NumberOfSilentEntities" ) );  // dynamic
    hlaClass.Register( ::hla::AttributeIdentifier( "SilentEntities" ) );          // dynamic
    hlaClass.Register( ::hla::AttributeIdentifier( "SilentAggregates" ) );        // static
    hlaClass.Register( ::hla::AttributeIdentifier( "SubAggregateIdentifiers" ) ); // static
    hlaClass.Register( ::hla::AttributeIdentifier( "EntityIdentifiers" ) );       // static
    hlaClass.ActivateUpdates( true );
    federate.Register( ::hla::ClassIdentifier( "BaseEntity.AggregateEntity" ), hlaClass, publish, subscribe );
}
