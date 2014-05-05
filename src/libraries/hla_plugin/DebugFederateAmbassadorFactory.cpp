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
#include "Federate_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassadorFactory constructor
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
DebugFederateAmbassadorFactory::DebugFederateAmbassadorFactory( const FederateAmbassadorFactory_ABC& factory, dispatcher::Logger_ABC& logger, ObjectResolver_ABC& resolver )
    : factory_   ( factory )
    , ambassador_( new DebugFederateAmbassador( logger, resolver ) )
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
std::unique_ptr< plugins::hla::Federate_ABC > DebugFederateAmbassadorFactory::Create( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, int lookAhead ) const
{
    std::unique_ptr< plugins::hla::Federate_ABC > result = factory_.Create( ambassador, name, lookAhead );
    result->Register( *ambassador_ );
    return result;
}
