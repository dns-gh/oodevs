// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RtiAmbassadorFactory.h"
#include <hla/RtiAmbassador_ABC.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RtiAmbassadorFactory constructor
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
RtiAmbassadorFactory::RtiAmbassadorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RtiAmbassadorFactory destructor
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
RtiAmbassadorFactory::~RtiAmbassadorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RtiAmbassadorFactory::CreateAmbassador
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
std::auto_ptr< ::hla::RtiAmbassador_ABC > RtiAmbassadorFactory::CreateAmbassador( ::hla::TimeFactory_ABC& timeFactory, ::hla::TimeIntervalFactory_ABC& timeIntervalFactory,
                                                                                  ::hla::RtiAmbassador_ABC::E_MessagePolicy policy,
                                                                                  const std::string& host, const std::string& port ) const
{
    return std::auto_ptr< ::hla::RtiAmbassador_ABC >( ::hla::RtiAmbassador_ABC::CreateAmbassador( timeFactory, timeIntervalFactory, policy, host, port ) );
}
