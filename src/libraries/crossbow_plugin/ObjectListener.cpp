// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ObjectListener.h"
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "QueryBuilder.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "Line.h"
#include "Point.h"
#include "WorkingSession.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"

#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: ObjectListener constructor
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
ObjectListener::ObjectListener( Workspace_ABC& workspace, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession& session )
    : publisher_( publisher )
    , database_  ( workspace.GetDatabase( "geometry" ) )
    , session_ ( session )
{
    table_.reset( database_.OpenTable( "Create_TacticalObject_Point" ) );
    Clean();
}

// -----------------------------------------------------------------------------
// Name: ObjectListener destructor
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
ObjectListener::~ObjectListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::CleanSession
// Created: MPT 2009-07-28
// -----------------------------------------------------------------------------
void ObjectListener::Clean()
{
    try
    {
        table_->DeleteRows( "session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "ObjectListener is not correctly loaded : " + std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::Listen
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
void ObjectListener::Listen()
{
    std::stringstream query;
    query << "info <> '' AND session_id=" << session_.GetId();

    Row_ABC* row = table_->Find( query.str() );
    bool bHasUpdates = row != 0;
    while( row )
    {
        SendCreation( *row );
        row = table_->GetNextRow();
    }
    if( bHasUpdates )
        table_->DeleteRows( query.str() );
}

namespace
{
    // $$$$ SBO 2007-09-23: hard coded object list
    std::string GetType( const std::string& type )
    {
        if( type == "fire" )
            return "fire";
        if( type == "emergency shelter" )
            return "emergency shelter";
        throw std::runtime_error( "unsupported object type" );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::SendCreation
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
void ObjectListener::SendCreation( const Row_ABC& row )
{
    simulation::ObjectMagicAction message;
    /*MsgsClientToSim::MsgMagicActionCreateObject* creation = message().mutable_action()->mutable_create_object();
    creation->set_team( 1 ); // $$$$ SBO 2007-09-23: Hard coded !!
    creation->set_type( GetType( boost::get< std::string >( row.GetField( "Info" ) ) ).c_str() );
    creation->set_name( "" );
    row.GetGeometry().Serialize( *creation->mutable_location() );*/
    message().mutable_object()->set_id( 0 );
    message().set_type( MsgsClientToSim::MsgObjectMagicAction_Type_create );

    // type
    message().mutable_parameters()->add_elem()->mutable_value()->set_acharstr( GetType( boost::get< std::string >( row.GetField( "Info" ) ) ).c_str() );

    // location
    row.GetGeometry().Serialize( *message().mutable_parameters()->add_elem()->mutable_value()->mutable_location() );

    // name
    message().mutable_parameters()->add_elem()->mutable_value()->set_acharstr( "" );

    // team
    message().mutable_parameters()->add_elem()->mutable_value()->mutable_party()->set_id( 1 ); // $$$$ SBO 2007-09-23: Hard coded !!

    // list (unused but must be created)
    message().mutable_parameters()->add_elem();

    message.Send( publisher_ );
}
