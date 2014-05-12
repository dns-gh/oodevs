// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FederateAmbassadorFactory.h"
#include "Federate.h"
#include <hla/SimpleTime.h>
#include <hla/SimpleTimeInterval.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: FederateAmbassadorFactory constructor
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
FederateAmbassadorFactory::FederateAmbassadorFactory( unsigned int lookAhead )
    : lookAhead_( lookAhead )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FederateAmbassadorFactory destructor
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
FederateAmbassadorFactory::~FederateAmbassadorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FederateAmbassadorFactory::Create
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
std::unique_ptr< plugins::hla::Federate_ABC > FederateAmbassadorFactory::Create( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, int lookAhead ) const
{
    return std::unique_ptr< plugins::hla::Federate_ABC >( new plugins::hla::Federate( ambassador, name, ::hla::SimpleTime(), ::hla::SimpleTimeInterval( lookAhead > 0 ? lookAhead : lookAhead_ ) ) );
}
