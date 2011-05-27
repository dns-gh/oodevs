// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DebugFederateAmbassadorFactory.h"
#include "DebugFederateAmbassador.h"
#include <hla/Federate.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassadorFactory constructor
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
DebugFederateAmbassadorFactory::DebugFederateAmbassadorFactory( const FederateAmbassadorFactory_ABC& factory, dispatcher::Logger_ABC& logger )
    : factory_   ( factory )
    , ambassador_( new DebugFederateAmbassador( logger ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassadorFactory destructor
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
DebugFederateAmbassadorFactory::~DebugFederateAmbassadorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassadorFactory::Create
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
std::auto_ptr< ::hla::Federate > DebugFederateAmbassadorFactory::Create( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, int lookAhead ) const
{
    std::auto_ptr< ::hla::Federate > result = factory_.Create( ambassador, name, lookAhead );
    result->Register( *ambassador_ );
    return result;
}
