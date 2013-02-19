// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ActionScheduler.h"

#include "MiscEvents.h"

#include "clients_kernel/XmlAdapter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Simulation.h"
#include "protocol/XmlReaders.h"

#include <boost/bind.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/date_time.hpp>
#include <xeumeuleu/xml.hpp>
#include <algorithm>

using namespace plugins::script;

typedef ActionScheduler::T_Action  T_Action;
typedef ActionScheduler::T_Actions T_Actions;

struct ActionScheduler::T_Action
{
    boost::posix_time::ptime time;
    sword::ClientToSim       msg;

    bool operator<( const T_Action& other ) const
    {
        return time < other.time;
    }
};

namespace
{
    boost::posix_time::ptime MakeTime( std::string value )
    {
        boost::algorithm::erase_all( value, "-" );
        boost::algorithm::erase_all( value, ":" );
        return boost::posix_time::from_iso_string( value );
    }

    void ReadAction( T_Actions& dst, const kernel::XmlAdapter& adapter, xml::xistream& xis )
    {
        try
        {
            std::string time;
            xis >> xml::attribute( "time", time );
            T_Action action;
            action.time = MakeTime( time );
            action.msg.set_context( 0 );
            protocol::Read( adapter, *action.msg.mutable_message(), xis );
            dst.push_back( action );
        }
        catch( const std::exception& err )
        {
            MT_LOG_ERROR_MSG( "Unable to process action: " << tools::GetExceptionMsg( err ) );
        }
    }

    T_Actions ReadActions( xml::xistream& xis, const kernel::XmlAdapter& adapter )
    {
        T_Actions rpy;
        xis >> xml::start( "actions" )
            >> xml::list( "action", boost::bind( &ReadAction, boost::ref( rpy ), boost::cref( adapter ), _1 ) );
        // Assume the input file defines a valid order in case of equality.
        // This is a bit silly as either it should define a total order and
        // ensure timestamps are consistent, or provide another valid order.
        std::stable_sort( rpy.begin(), rpy.end() );
        return rpy;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionScheduler constructor
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
ActionScheduler::ActionScheduler( xml::xistream& xis,
                                  const kernel::XmlAdapter& adapter,
                                  dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_ ( publisher )
    , actions_   ( ReadActions( xis, adapter ) )
    , cursor_    ( actions_.begin() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionScheduler destructor
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
ActionScheduler::~ActionScheduler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionScheduler::Tick
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
void ActionScheduler::Tick( const boost::posix_time::ptime& time )
{
    for( ; cursor_ != actions_.end() && cursor_->time <= time; ++cursor_ )
        publisher_.Send( cursor_->msg );
}

// -----------------------------------------------------------------------------
// Name: ActionScheduler::NotifyUpdated
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
void ActionScheduler::NotifyUpdated( const events::SimulationTimeChanged& event )
{
    Tick( MakeTime( event.time ) );
}
