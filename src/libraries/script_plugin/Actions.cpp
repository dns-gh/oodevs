// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "Actions.h"
#include "ActionScheduler.h"

#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/XmlAdapter.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Simulation.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"

#include <directia/brain/Brain.h>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <xeumeuleu/xml.h>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: Actions constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
Actions::Actions( kernel::Controller& controller,
                  const tools::ExerciseConfig& config,
                  const dispatcher::Model_ABC& model,
                  dispatcher::SimulationPublisher_ABC& publisher )
    : config_    ( config )
    , controller_( controller )
    , publisher_ ( publisher )
    , entities_  ( new dispatcher::ModelAdapter( model ) )
    , converter_ ( new kernel::CoordinateConverter( config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Actions destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
Actions::~Actions()
{
    StopScheduler();
    assert( schedulers_.empty() );
}

namespace directia
{
    // Why does directia compile but break without this ?!?
    void UsedByDIA( Actions* ) {}
    void ReleasedByDIA( Actions* ) {}
}

// -----------------------------------------------------------------------------
// Name: Actions::RegisterIn
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void Actions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "actions" ] = this;
    brain.Register( "IssueOrder", &Actions::IssueOrder );
    brain.Register( "IssueOrderFromFile", &Actions::IssueOrderFromFile );
    brain.Register( "IssueXmlOrder", &Actions::IssueXmlOrder );
    brain.Register( "StartScheduler", &Actions::StartScheduler );
    brain.Register( "StopScheduler", &Actions::StopScheduler );
}

namespace
{
    typedef void( Actions::*T_Read)( xml::xistream& );

    void ReadWith( const std::string& name, Actions& actions, T_Read read, xml::xistream& xis )
    {
        std::string check;
        xis >> xml::optional
            >> xml::attribute( "name", check );
        if( name == check )
            ( actions.*read )( xis );
    }
}

#include <tools/Path.h>
// -----------------------------------------------------------------------------
// Name: Actions::IssueOrderFromFile
// Created: PHC 2010-09-16
// -----------------------------------------------------------------------------
void Actions::IssueOrderFromFile( const std::string& name, const std::string& filename )
{
    try
    {
        const auto& loader = config_.GetLoader();
        const auto file = loader.LoadFile( config_.BuildExerciseChildFile( tools::Path( "scripts/resources" ) / filename.c_str() + ".ord" ) );
        *file >> xml::start( "actions" )
                >> xml::list( "action", boost::bind( &ReadWith, boost::cref( name ), boost::ref( *this ), &Actions::Send, _1 ) );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in script: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Actions::IssueOrder
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void Actions::IssueOrder( const std::string& name )
{
    IssueOrderFromFile( name, "orders" );
}

// -----------------------------------------------------------------------------
// Name: Actions::IssueXmlOrder
// Created: AGE 2008-07-17
// -----------------------------------------------------------------------------
void Actions::IssueXmlOrder( const std::string& content )
{
    try
    {
        xml::xistringstream xis( content );
        xis >> xml::list( "action", boost::bind( &Actions::Send, this, _1 ) );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in script: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Actions::Read
// Created: BAX 2013-02-08
// -----------------------------------------------------------------------------
void Actions::Send( xml::xistream& xis )
{
    try
    {
        sword::ClientToSim msg;
        msg.set_context( 0 );
        protocol::Read( kernel::XmlAdapter( *converter_, *entities_ ), *msg.mutable_message(), xis );
        publisher_.Send( msg );
    }
    catch( const std::exception& err )
    {
        MT_LOG_ERROR_MSG( "Unable to process action: " << tools::GetExceptionMsg( err ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Actions::Read
// Created: LGY 2012-08-03
// -----------------------------------------------------------------------------
void Actions::Read( xml::xistream& xis )
{
    Send( xml::xisubstream( xis ) >> xml::start( "action" ) );
}

// -----------------------------------------------------------------------------
// Name: Actions::StartScheduler
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
void Actions::StartScheduler( const std::string& filename )
{
    try
    {
        const auto file = config_.GetLoader().LoadFile( config_.BuildExerciseChildFile( tools::Path( "scripts/resources" ) / tools::Path::FromUTF8( filename + ".ord" ) ) );
        const kernel::XmlAdapter xml( *converter_, *entities_ );
        const auto ptr = boost::make_shared< ActionScheduler >( *file, xml, publisher_ );
        controller_.Register( *ptr );
        schedulers_.push_back( ptr );

    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in script: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Actions::StopScheduler
// Created: SBO 2011-03-29
// -----------------------------------------------------------------------------
void Actions::StopScheduler()
{
    for( auto it = schedulers_.begin(); it != schedulers_.end(); ++it )
        controller_.Unregister( **it );
    schedulers_.clear();
}
