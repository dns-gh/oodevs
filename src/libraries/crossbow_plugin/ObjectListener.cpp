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
    : publisher_ ( publisher )
    , workspace_ ( workspace )
    , session_ ( session )
{

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
        std::string query( "session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) );
        {
            std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "Create_TacticalObject_Point" ) );
            table->DeleteRows( query );
        }
        {
            std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "Create_TacticalObject_Line" ) );
            table->DeleteRows( query );
        }
        {
            std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "Create_TacticalObject_Area" ) );
            table->DeleteRows( query );
        }
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
    try
    {
        std::string query( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) );
        Database_ABC& database = workspace_.GetDatabase( "geometry" );

        ListenObjectCreationRequest( database, "Create_TacticalObject_Point", query );
        ListenObjectCreationRequest( database, "Create_TacticalObject_Line", query );
        ListenObjectCreationRequest( database, "Create_TacticalObject_Area", query );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__ + std::string( e.what() ) );
    }
}

namespace
{
    template< typename Type >
    Type GetField( const Row_ABC& row, const std::string& name )
    {
        return boost::get< Type >( row.GetField( name ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::ListenObjectCreationRequest
// Created: JCR 2010-06-07
// -----------------------------------------------------------------------------
void ObjectListener::ListenObjectCreationRequest( Database_ABC& database, const std::string& tablename, const std::string& query )
{
    std::auto_ptr< Table_ABC > table( database.OpenTable( tablename ) );

    Row_ABC* row = table->Find( query );
    bool bHasUpdates = row != 0;
    while( row )
    {
        SendCreation( *row );
        row = table->GetNextRow();
    }
    if( bHasUpdates )
        table->DeleteRows( query );
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
    // team
    message().mutable_parameters()->add_elem()->mutable_value()->mutable_party()->set_id( GetField< int >( row, "team_id" ) );
    // type
    message().mutable_parameters()->add_elem()->mutable_value()->set_acharstr( GetField< std::string >( row, "type" ) );
    // name
    message().mutable_parameters()->add_elem()->mutable_value()->set_acharstr( GetField< std::string >( row, "name" ) );

    // location
    row.GetGeometry().Serialize( *message().mutable_parameters()->add_elem()->mutable_value()->mutable_location() );
    
    // list (unused but must be created)
    message().mutable_parameters()->add_elem();

    message.Send( publisher_ );
}
