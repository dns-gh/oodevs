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
#include "dispatcher/Config.h"
#include <directia/brain/Brain.h>
#include <tools/Path.h>
#include <boost/make_shared.hpp>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: IndicatorConditions constructor
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
IndicatorConditions::IndicatorConditions( kernel::Controller& controller, const dispatcher::Config& config )
    : controller_( controller )
    , config_ ( config )
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

namespace directia
{
    void UsedByDIA( IndicatorConditions* ) {}
    void ReleasedByDIA( IndicatorConditions* ) {}
}

// -----------------------------------------------------------------------------
// Name: IndicatorConditions::RegisterIn
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void IndicatorConditions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "events.indicators" ] = this;
    brain.Register( "IndicatorChanged", &IndicatorConditions::IndicatorChanged );
    brain.Register( "PrependSessionPath", &IndicatorConditions::PrependSessionPath );
}

// -----------------------------------------------------------------------------
// Name: IndicatorConditions::PrependSessionPath
// Created: HBD 2010-11-26
// -----------------------------------------------------------------------------
std::string IndicatorConditions::PrependSessionPath( const std::string& filename )
{
    return config_.BuildSessionChildFile( filename.c_str() ).ToUTF8();
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
            : SimpleEventCondition( controller )
        {
            // NOTHING
        }
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
    return boost::make_shared< IndicatorChanged >( controller_ );
}
