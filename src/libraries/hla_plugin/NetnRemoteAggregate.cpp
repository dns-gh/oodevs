// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRemoteAggregate.h"
#include <hla/Deserializer.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAggregate::NetnRemoteAggregate( std::auto_ptr< Aggregate_ABC > aggregate )
    : aggregate_( aggregate )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAggregate::~NetnRemoteAggregate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnRemoteAggregate::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "NETN remote aggregate can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregate::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnRemoteAggregate::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer )
{
    aggregate_->Deserialize( identifier, deserializer );
}
