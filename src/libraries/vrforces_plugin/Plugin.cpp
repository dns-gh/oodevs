// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "Plugin.h"
#include "Agent.h"
#include "Facade.h"
#include "ForceResolver.h"
#include "dispatcher/Config.h"
#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include <vl/exConnInit.h>
#include <vl/exerciseConn.h>
#include <vlutil/vlprint.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: Plugin constructor
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
Plugin::Plugin( dispatcher::Model_ABC& model, const dispatcher::Config& config, xml::xistream& /*xis*/ )
    : model_        ( model )
    , connection_   ( new DtExerciseConn( DtVrlApplicationInitializer( 0, 0, "VR-Link Plugin" ) ) )
    , logger_       ( new DtFilePrinter( config.BuildSessionChildFile( "vrforces.log" ).c_str() ) )
    , vrForces_     ( new Facade( *connection_ ) )
    , forceResolver_( new ForceResolver( model_ ) )
{
    DtWarn.attachPrinter( logger_.get() );
    DtInfo.attachPrinter( logger_.get() );
    DtVerbose.attachPrinter( logger_.get() );
    DtFatal.attachPrinter( logger_.get() );
    DtDebug.attachPrinter( logger_.get() );
}

// -----------------------------------------------------------------------------
// Name: Plugin destructor
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
Plugin::~Plugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Plugin::Receive
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
void Plugin::Receive( const sword::SimToClient& message )
{
    if( !connection_.get() )
        return;
    if( message.message().has_control_begin_tick() )
        connection_->drainInput();
    else if( message.message().has_control_end_tick() )
        connection_->clock()->setSimTime( message.message().control_end_tick().current_tick() * 10 ); // $$$$ SBO 2011-01-20: hard coded time step
    else if( message.message().has_unit_creation() )
        Create( message.message().unit_creation() );
    else if( message.message().has_unit_attributes() )
        Update( message.message().unit_attributes() );
    else if( message.message().has_unit_destruction() )
        Destroy( message.message().unit_destruction() );
    else if( message.message().has_automat_attributes() )
    {
        if( message.message().automat_attributes().has_mode() )
        {
            const bool aggregated = message.message().automat_attributes().mode() == sword::engaged;
            const unsigned long id = message.message().automat_attributes().automat().id();
            T_AutomatAgents::iterator it = automatAgents_.find( id );
            for( std::set< boost::shared_ptr< Agent > >::const_iterator itAgents = it->second.begin(); itAgents != it->second.end(); ++itAgents )
                (*itAgents)->SetAggregated( aggregated );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Plugin::NotifyClientAuthenticated
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
void Plugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Plugin::NotifyClientLeft
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
void Plugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Plugin::Create
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
void Plugin::Create( const sword::UnitCreation& message )
{
    agents_[ message.unit().id() ].reset( new Agent( *connection_, *vrForces_, message, *forceResolver_ ) );
    automatAgents_[ message.automat().id() ].insert( agents_[ message.unit().id() ] );
    agentAutomat_[ message.unit().id() ] = message.automat().id();
}

// -----------------------------------------------------------------------------
// Name: Plugin::Update
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
void Plugin::Update( const sword::UnitAttributes& message )
{
    T_Agents::iterator it = agents_.find( message.unit().id() );
    if( it != agents_.end() )
        it->second->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Plugin::Destroy
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
void Plugin::Destroy( const sword::UnitDestruction& message )
{
    const unsigned long id = message.unit().id();
    T_AgentAutomat::iterator itAutomat = agentAutomat_.find( id );
    if( itAutomat != agentAutomat_.end() )
    {
        T_AutomatAgents::iterator itAgents = automatAgents_.find( itAutomat->second );
        if( itAgents != automatAgents_.end() )
            for( std::set< boost::shared_ptr< Agent > >::iterator itAgent = itAgents->second.begin(); itAgent != itAgents->second.end(); ++itAgent )
                if( (*itAgent)->GetId() == id )
                {
                    itAgents->second.erase( itAgent );
                    break;
                }
        agentAutomat_.erase( id );
    }
    agents_.erase( id );
}
