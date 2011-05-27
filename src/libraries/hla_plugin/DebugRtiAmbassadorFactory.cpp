// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DebugRtiAmbassadorFactory.h"
#include "DebugRtiAmbassador.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassadorFactory constructor
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
DebugRtiAmbassadorFactory::DebugRtiAmbassadorFactory( const RtiAmbassadorFactory_ABC& factory, dispatcher::Logger_ABC& logger )
    : factory_( factory )
    , logger_ ( logger )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassadorFactory destructor
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
DebugRtiAmbassadorFactory::~DebugRtiAmbassadorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassadorFactory::CreateAmbassador
// Created: SLI 2011-05-27
// -----------------------------------------------------------------------------
std::auto_ptr< ::hla::RtiAmbassador_ABC > DebugRtiAmbassadorFactory::CreateAmbassador( ::hla::TimeFactory_ABC& timeFactory, ::hla::TimeIntervalFactory_ABC& timeIntervalFactory,
                                                                                       ::hla::RtiAmbassador_ABC::E_MessagePolicy policy,
                                                                                       const std::string& host, const std::string& port ) const
{
    std::auto_ptr< ::hla::RtiAmbassador_ABC > ambassador = factory_.CreateAmbassador( timeFactory, timeIntervalFactory, policy, host, port );
    return std::auto_ptr< ::hla::RtiAmbassador_ABC >( new DebugRtiAmbassador( ambassador, logger_ ) );
}
