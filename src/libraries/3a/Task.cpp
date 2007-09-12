// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Task.h"
#include "Connectors.h"
#include "Result_ABC.h"
#include "xeumeuleu/xml.h"
#include "dispatcher/ClientPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: Task constructor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
Task::Task( dispatcher::ClientPublisher_ABC& publisher )
    : publisher_( publisher )
    , result_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Task destructor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
Task::~Task()
{
    for( CIT_Connectors it = connectors_.begin(); it != connectors_.end(); ++it )
        delete it->second;
    for( CIT_Slots it = slots_.begin(); it != slots_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: Task::AddExtractor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void Task::AddExtractor( std::auto_ptr< ModelFunction_ABC > function )
{
    composite_.Add( function );
}

// -----------------------------------------------------------------------------
// Name: Task::AddFunction
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::AddFunction( const std::string& name, std::auto_ptr< Slot_ABC > function )
{
    Slot_ABC*& s = slots_[ name ];
    if( s )
        throw std::runtime_error( "A function '" + name + "' already exists" );
    s = function.release();
}

// -----------------------------------------------------------------------------
// Name: Task::AddConnector
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void Task::AddConnector( const std::string& name, std::auto_ptr< Connector_ABC > connector )
{
    Connector_ABC*& c = connectors_[ name ];
    if( c )
        throw std::runtime_error( "A function '" + name + "' already exists" );
    c = connector.release();
}

// -----------------------------------------------------------------------------
// Name: Task::Connect
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::Connect( xml::xistream& xis )
{
    xis >> xml::start( "indicator" )
            >> xml::list( *this, &Task::Connect )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Task::Connect
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::Connect( const std::string& , xml::xistream& xis )
{
    const std::string name  = xml::attribute< std::string >( xis, "name", "plot" );
    const std::string input = xml::attribute< std::string >( xis, "input", "" );

    if( !name.empty() && !input.empty() )
    {
        CIT_Connectors itC = connectors_.find( input );
        CIT_Slots itS = slots_.find( name );
        if( itC != connectors_.end() && itS != slots_.end() )
            itC->second->Connect( *(itS->second) );
        else 
            throw std::runtime_error( "Could not connect '" + input + "' to '" + name + "'" );
    }
}

// -----------------------------------------------------------------------------
// Name: Task::SetResult
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::SetResult( std::auto_ptr< Result_ABC > output )
{
    result_ = output.get();
    AddFunction( "plot", output );
}

// -----------------------------------------------------------------------------
// Name: Task::Receive
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_begin_tick )
        composite_.BeginTick();
    else if( message.msg.t == T_MsgsSimToClient_msg_msg_control_end_tick )
        composite_.EndTick();
    else
        composite_.Receive( message );
}

// -----------------------------------------------------------------------------
// Name: Task::Commit
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Task::Commit()
{
    if( result_ )
        result_->Send( publisher_ );
}
