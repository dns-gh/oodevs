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
#include "Database_ABC.h"
#include "QueryBuilder.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "Line.h"
#include "Point.h"
#include "WorkingSession.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "game_asn/SimulationSenders.h"

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: ObjectListener constructor
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
ObjectListener::ObjectListener( Database_ABC& database, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession& session )
    : publisher_( publisher )
    , database_  ( database )
	, session_ ( session )
{
    Clean();
    table_.reset( database_.OpenTable( "Create_TacticalObject_Point" ) );
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
	std::stringstream clause;
    clause << "session_id=" << session_.GetId();
	try
    {
		database_.Execute( DeleteQueryBuilder( database_.GetTableName( "TacticalObject_Point" ), clause.str() ) );
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
    if ( bHasUpdates )
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
    simulation::ObjectMagicAction asn;
    asn().action.t = T_MsgObjectMagicAction_action_create_object;
    ASN1T_MagicActionCreateObject*& creation = asn().action.u.create_object = new ASN1T_MagicActionCreateObject();
    creation->team = 1; // $$$$ SBO 2007-09-23: Hard coded !!
    creation->type = (ASN1VisibleString)GetType( boost::get< std::string >( row.GetField( "info" ) ) ).c_str();
    creation->name = "";
    row.GetShape().Serialize( creation->location );
    asn.Send( publisher_ );
}
