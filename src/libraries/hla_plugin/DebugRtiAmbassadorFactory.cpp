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
DebugRtiAmbassadorFactory::DebugRtiAmbassadorFactory( const RtiAmbassadorFactory_ABC& factory, dispatcher::Logger_ABC& logger, ObjectResolver_ABC& resolver )
    : factory_ ( factory )
    , logger_  ( logger )
    , resolver_( resolver )
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
::hla::RtiAmbassador_ABC* DebugRtiAmbassadorFactory::CreateAmbassador( ::hla::TimeFactory_ABC& timeFactory, ::hla::TimeIntervalFactory_ABC& timeIntervalFactory,
                                                                                       ::hla::RtiAmbassador_ABC::E_MessagePolicy policy,
                                                                                       const std::string& lrcSettings ) const
{
    ::hla::RtiAmbassador_ABC* ambassador = factory_.CreateAmbassador( timeFactory, timeIntervalFactory, policy, lrcSettings );
    ::hla::RtiAmbassador_ABC* retval = new DebugRtiAmbassador( ambassador, logger_, resolver_ );
    ambassadors_[ retval ] = ambassador;
    return retval;
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassadorFactory::DeleteAmbassador
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
void DebugRtiAmbassadorFactory::DeleteAmbassador( ::hla::RtiAmbassador_ABC* ambassador ) const
{
    T_AmbassadorMap::iterator it( ambassadors_.find( ambassador ) );
    assert( ambassadors_.end() != it );
    factory_.DeleteAmbassador( it->second );
    ambassadors_.erase( it );
    delete ambassador;
}
