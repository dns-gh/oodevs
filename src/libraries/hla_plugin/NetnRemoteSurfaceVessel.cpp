// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRemoteSurfaceVessel.h"
#include <hla/Deserializer.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRemoteSurfaceVessel constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteSurfaceVessel::NetnRemoteSurfaceVessel( std::auto_ptr< HlaObject_ABC > vessel )
    : vessel_( vessel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteSurfaceVessel destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteSurfaceVessel::~NetnRemoteSurfaceVessel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteSurfaceVessel::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnRemoteSurfaceVessel::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "NETN remote surface vessel can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteSurfaceVessel::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void NetnRemoteSurfaceVessel::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer )
{
    vessel_->Deserialize( identifier, deserializer );
}
