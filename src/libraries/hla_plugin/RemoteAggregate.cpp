// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAggregate.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteAggregate constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAggregate::RemoteAggregate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAggregate::~RemoteAggregate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteAggregate::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "NETN remote aggregate can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteAggregate::Deserialize( const ::hla::AttributeIdentifier& identifier, const ::hla::Deserializer& deserializer )
{

}
