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
#include "DisaggregationStrategy.h"
#include "Facade.h"
#include "ForceResolver.h"
#include "Logger.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model_ABC.h"
#include "protocol/Protocol.h"
#include "rpr/EntityTypeResolver.h"
#pragma warning( push, 0 )
#include <vl/exConnInit.h>
#include <vl/exerciseConn.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace plugins::vrforces;

namespace
{
    DtVrlApplicationInitializer ConfigureInitializer( const dispatcher::Config& config, xml::xistream& xis )
    {
        DtVrlApplicationInitializer initializer( 0, 0, "VR-Forces Plugin" );
        const std::string rootDirectory = config.BuildPluginDirectory( "vrforces" );
        initializer.setFedFileName( ( rootDirectory + "/" + xis.attribute< std::string >( "fed" ) ).c_str() );
        return initializer;
    }
}

// -----------------------------------------------------------------------------
// Name: Plugin constructor
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
Plugin::Plugin( dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& simulation, const dispatcher::Config& config, dispatcher::Logger_ABC& logger, xml::xistream& xis )
    : model_        ( model )
    , simulation_   ( simulation )
    , forceResolver_( new ForceResolver( model_ ) )
    , typeResolver_ ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPhysicalChildFile( "dis.xml" ) ) ) )
    , logger_       ( new Logger( logger ) )
    , connection_   ( new DtExerciseConn( ConfigureInitializer( config, xis ) ) )
    , vrForces_     ( new Facade( *connection_, xis ) )
    , disaggregator_( new DisaggregationStrategy( *vrForces_ ) )
{
    connection_->setDestroyFedExecFlag( false );
}

// -----------------------------------------------------------------------------
// Name: Plugin destructor
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
Plugin::~Plugin()
{
    agents_.clear();
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
    else if( message.message().has_unit_pathfind() )
        Update( message.message().unit_pathfind() );
}

// -----------------------------------------------------------------------------
// Name: Plugin::Receive
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
void Plugin::Receive( const sword::MessengerToClient& message )
{
    if( !connection_.get() )
        return;
    if( message.message().has_shape_creation() )
        disaggregator_->AddArea( message.message().shape_creation() );
    else if( message.message().has_shape_destruction() )
        disaggregator_->RemoveArea( message.message().shape_destruction() );
}

// -----------------------------------------------------------------------------
// Name: Plugin::Create
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
void Plugin::Create( const sword::UnitCreation& message )
{
    agents_[ message.unit().id() ].reset( new Agent( model_.Agents().Get( message.unit().id() ), *connection_, *vrForces_, message, *forceResolver_, *disaggregator_, *typeResolver_, simulation_, *logger_ ) );
}

// -----------------------------------------------------------------------------
// Name: Plugin::Update
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
template< typename M >
void Plugin::Update( const M& message )
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
    agents_.erase( message.unit().id() );
}
