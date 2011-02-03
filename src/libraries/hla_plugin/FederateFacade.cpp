// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FederateFacade.h"
#include "AggregateEntityClass.h"
#include "RtiAmbassadorFactory_ABC.h"
#include <hla/hla_lib.h>
#include <hla/SimpleTimeFactory.h>
#include <hla/SimpleTimeIntervalFactory.h>
#include <hla/SimpleTime.h>
#include <hla/SimpleTimeInterval.h>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;
using namespace hla;

// -----------------------------------------------------------------------------
// Name: FederateFacade constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::FederateFacade( xml::xisubstream xis, AgentSubject_ABC& subject, const RtiAmbassadorFactory_ABC& factory, unsigned int lookAhead )
    : joined_         ( false )
    , timeFactory_    ( new SimpleTimeFactory() )
    , intervalFactory_( new SimpleTimeIntervalFactory() )
    , ambassador_     ( factory.CreateAmbassador( *timeFactory_, *intervalFactory_, RtiAmbassador_ABC::TimeStampOrder, xis.attribute< std::string >( "host", "localhost" ), xis.attribute< std::string >( "port", "8989" ) ) )
    , federate_       ( new Federate( *ambassador_, xis.attribute< std::string >( "name" ), SimpleTime(), SimpleTimeInterval( lookAhead ) ) )
{
    const std::string name = xis.attribute< std::string >( "federation" );
    try
    {
        joined_ = federate_->Join( name, false, true );
    }
    catch( HLAException& /*e*/ )
    {
        // NOTHING
    }
    if( joined_ )
        agentClass_.reset( new AggregateEntityClass( *federate_, subject ) );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::~FederateFacade()
{
    agentClass_.reset();
    if( joined_ )
        federate_->Resign();
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Step
// Created: AGE 2008-02-25
// -----------------------------------------------------------------------------
void FederateFacade::Step()
{
    if( joined_ )
        federate_->Step();
}
