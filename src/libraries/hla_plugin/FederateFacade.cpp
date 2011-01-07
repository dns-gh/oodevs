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
#include "AgentExtension.h"
#include "AggregateEntityClass.h"
#include "ExtensionFactory.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Model_ABC.h"
#include "MT_Tools/MT_Logger.h"
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
FederateFacade::FederateFacade( xml::xisubstream xis, dispatcher::Model_ABC& model, unsigned int lookAhead )
    : joined_         ( false )
    , model_          ( model )
    , agentClass_     ( new AggregateEntityClass() )
    , factory_        ( new ExtensionFactory( *agentClass_ ) )
    , timeFactory_    ( new SimpleTimeFactory() )
    , intervalFactory_( new SimpleTimeIntervalFactory() )
    , ambassador_     ( RtiAmbassador_ABC::CreateAmbassador( *timeFactory_, *intervalFactory_, RtiAmbassador_ABC::TimeStampOrder, xis.attribute< std::string >( "host" ), xis.attribute< std::string >( "port", "8989" ) ) )
    , federate_       ( new Federate( *ambassador_, xis.attribute< std::string >( "name" ), SimpleTime(), SimpleTimeInterval( lookAhead ) ) )
{
    model_.RegisterFactory( *factory_ );
    const std::string name = xis.attribute< std::string >( "federation" );
    try
    {
        joined_ = federate_->Join( name, false, true );
    }
    catch( HLAException& e )
    {
        MT_LOG_ERROR_MSG( "Error joining federation '" << name << "' : " << e.what() );
    } 
    agentClass_->RegisterTo( *federate_ );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::~FederateFacade()
{
    model_.UnregisterFactory( *factory_ );
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
