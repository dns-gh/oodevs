// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "StatusListener.h"
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "WorkingSession_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "protocol/SimulationSenders.h"
#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: StatusListener constructor
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
StatusListener::StatusListener( Workspace_ABC& workspace, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession_ABC& session, dispatcher::Logger_ABC& logger )
    : publisher_      ( publisher )
    , workspace_      ( workspace )
    , paused_         ( false )
    , session_        ( session )
    , logger_         ( logger )
{
    Clean( logger );
}

// -----------------------------------------------------------------------------
// Name: StatusListener destructor
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
StatusListener::~StatusListener()
{
    // NOTHING
}

namespace
{
    template< typename Type >
    Type GetField( const Row_ABC& row, const std::string& name )
    {
        return boost::get< Type >( row.GetField( name ) );
    }

    Database_ABC& GetDatabase( Workspace_ABC& workspace )
    {
        return workspace.GetDatabase( "flat" );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::Clean
// Created: MPT 2009-12-15
// -----------------------------------------------------------------------------
void StatusListener::Clean( dispatcher::Logger_ABC& logger )
{
    try
    {
        std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( "SimulationProperties" ) );
        table->DeleteRows( "session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) );
    }
    catch ( std::exception& e )
    {
        logger.LogError( "StatusListener is not correctly loaded : " + std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::Listen
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::Listen()
{
    try
    {
        std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( "SimulationProperties" ) );

        ListenStatusUpdate( *table );
        ListenTimefactorUpdate( *table );
    }
    catch ( std::exception& e )
    {
        logger_.LogError( __FUNCTION__ + std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::ListenStatus
// Created: JCR 2010-06-07
// -----------------------------------------------------------------------------
void StatusListener::ListenStatusUpdate( Table_ABC& table )
{
    std::stringstream ss;
    ss << "property='status' AND checked=-1 AND session_id=" << session_.GetId();

    Row_ABC* results = table.Find( ss.str() );
    while( results != 0 )
    {
        ChangeStatus( GetField< std::string >( *results, "value" ) );
        MarkProcessed( *results );
        results = table.GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::ListenTimefactorUpdate
// Created: JCR 2010-06-07
// -----------------------------------------------------------------------------
void StatusListener::ListenTimefactorUpdate( Table_ABC& table )
{
    std::stringstream ss;

    ss << "property = 'timefactor' AND checked=-1 AND session_id=" << session_.GetId();
    Row_ABC* results = table.Find( ss.str() );
    while ( results != 0 )
    {
        int factor = boost::lexical_cast< int >( GetField< std::string >( *results, "value" ) );

        ChangeTimeFactor( factor );
        MarkProcessed( *results );
        results = table.GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: OrderListener::MarkProcessed
// Created: MPT 2009-12-15
// -----------------------------------------------------------------------------
void StatusListener::MarkProcessed( Row_ABC& row ) const
{
    row.SetField( "checked", FieldVariant( true ) );
}

// -----------------------------------------------------------------------------
// Name: StatusListener::ChangeStatus
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::ChangeStatus( const std::string& status )
{
    if( paused_ && status == "resumed" )
    {
        simulation::ControlResume message;
        message.Send( publisher_ );
        paused_ = false;
    }
    else if( !paused_ && status == "paused" )
    {
        simulation::ControlPause message;
        message.Send( publisher_ );
        paused_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::ChangeTimeFactor
// Created: MPT 2009-12-15
// -----------------------------------------------------------------------------
void StatusListener::ChangeTimeFactor( int speed )
{
    simulation::ControlChangeTimeFactor message;

    message().set_time_factor( speed );
    message.Send( publisher_ );
}
