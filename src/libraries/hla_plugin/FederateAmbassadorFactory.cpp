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
#include <hla/hla_lib.h>
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
std::auto_ptr< ::hla::Federate > FederateAmbassadorFactory::Create( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, int lookAhead ) const
{
    return std::auto_ptr< ::hla::Federate >( new ::hla::Federate( ambassador, name, ::hla::SimpleTime(), ::hla::SimpleTimeInterval( lookAhead > 0 ? lookAhead : lookAhead_ ) ) );
}
