// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnClassBuilder.h"
#include "Federate_ABC.h"
#include "Aggregate_ABC.h"
#include <hla/Class.h>
#include <hla/ClassIdentifier.h>
#include <hla/AttributeIdentifier.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnClassBuilder constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnClassBuilder::NetnClassBuilder( const ClassBuilder_ABC& builder )
    : builder_( builder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnClassBuilder destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnClassBuilder::~NetnClassBuilder()
{
    // NOTHING
}

namespace
{
    class EmptyFederate : public Federate_ABC
    {
        virtual bool Connect() { return true; }
        virtual void Disconnect() {}
        virtual bool Create( const std::string&, const std::string& ) { return true; }
        virtual bool Destroy( const std::string& ) { return true; }
        virtual bool Join( const std::string&, bool, bool ) { return true; }
        virtual void Resign() {}
        virtual void Step() {}
        virtual void Register( const ::hla::ClassIdentifier&, ::hla::Class_ABC&, bool, bool ) {}
        virtual void Register( const ::hla::InteractionIdentifier&, ::hla::Interaction_ABC&, bool, bool ) {}
        virtual void Register( ::hla::FederateAmbassador_ABC& ) {}
    };
}

// -----------------------------------------------------------------------------
// Name: NetnClassBuilder::Build
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnClassBuilder::Build( Federate_ABC& federate, ::hla::Class< Aggregate_ABC >& hlaClass ) const
{
    EmptyFederate empty;
    builder_.Build( empty, hlaClass );
    hlaClass.Register( ::hla::AttributeIdentifier( "Mounted" ) );            // dynamic
    hlaClass.Register( ::hla::AttributeIdentifier( "Echelon" ) );            // static
    hlaClass.Register( ::hla::AttributeIdentifier( "UniqueID" ) );           // static
    hlaClass.Register( ::hla::AttributeIdentifier( "HigherHeadquarters" ) ); // static
    hlaClass.Register( ::hla::AttributeIdentifier( "Callsign" ) );           // static
    hlaClass.ActivateUpdates( true );
    federate.Register( ::hla::ClassIdentifier( "BaseEntity.AggregateEntity.NETN_Aggregate" ), hlaClass, true, false );
}
