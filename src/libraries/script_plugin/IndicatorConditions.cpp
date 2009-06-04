// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "IndicatorConditions.h"
#include "EventCondition.h"
#include "MiscEvents.h"
#include "directia/Brain.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: IndicatorConditions constructor
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
IndicatorConditions::IndicatorConditions( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorConditions destructor
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
IndicatorConditions::~IndicatorConditions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorConditions::RegisterIn
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void IndicatorConditions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "events.indicators", this );
    brain.RegisterFunction( "IndicatorChanged", &IndicatorConditions::IndicatorChanged );
}

// -----------------------------------------------------------------------------
// Name: IndicatorConditions::IndicatorChanged
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > IndicatorConditions::IndicatorChanged()
{
    struct IndicatorChanged : public SimpleEventCondition< events::IndicatorChanged >
    {
        IndicatorChanged( kernel::Controller& controller )
            : SimpleEventCondition( controller ) {}

        virtual void NotifyUpdated( const events::IndicatorChanged& message )
        {
            double& value = indicators_[message.name];
            if( value != message.value )
            {
                value = message.value;
                Trigger( message.name, value );
            }
        };
        std::map< std::string, double > indicators_;
    };
    return boost::shared_ptr< Condition_ABC >( new IndicatorChanged( controller_ ) );
}
