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
#include "dispatcher/Agent.h"
#include "ObjectClass_ABC.h"
#include <hla/hla_lib.h>
#include <hla/SimpleTimeFactory.h>
#include <hla/SimpleTimeIntervalFactory.h>
#include <hla/SimpleTimeInterval.h>
#include "MT/MT_Logger/MT_Logger_Lib.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: FederateFacade constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::FederateFacade( const std::string& name, unsigned int timeStepDuration )
    : joined_         ( false )
    , timeFactory_    ( new SimpleTimeFactory() )
    , intervalFactory_( new SimpleTimeIntervalFactory() )
    , ambassador_     ( RtiAmbassador_ABC::CreateAmbassador( *timeFactory_, *intervalFactory_ ) )
    , federate_       ( new Federate( *ambassador_, name, SimpleTimeInterval( timeStepDuration ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FederateFacade destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::~FederateFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Join
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
bool FederateFacade::Join( const std::string& name )
{
    try
    {
        if( ! federate_->Join( name, false, true ) )
            return joined_ = false;
        for( T_Classes::iterator it = classes_.begin(); it != classes_.end(); ++it )
            (*it)->RegisterTo( *federate_ );
        return joined_ = true;
    }
    catch( HLAException& e )
    {
        MT_LOG_ERROR_MSG( "Error joining federation '" << name << "' : " << e.what() );
        return joined_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::AddClass
// Created: AGE 2008-02-22
// -----------------------------------------------------------------------------
void FederateFacade::AddClass( ObjectClass_ABC& objectClass )
{
    if( joined_ )
        classes_.push_back( &objectClass );
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
