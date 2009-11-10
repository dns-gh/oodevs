// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderListener.h"
#include "OrderDispatcher.h"
#include "QueryBuilder.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "QueryBuilder.h"
#include "WorkingSession.h"
#include <boost/lexical_cast.hpp>

using namespace dispatcher;
using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: OrderListener constructor
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
OrderListener::OrderListener( Database_ABC& database, const dispatcher::Model& model, const OrderTypes& types, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession& session )
    : publisher_ ( publisher )
    , dispatcher_( new OrderDispatcher( database, types, model ) )
    , database_  ( database )
    , ref_ ( 0 )
	, session_ ( session )
{
    Clean();
    table_.reset( database_.OpenTable( "Orders" ) );
}

// -----------------------------------------------------------------------------
// Name: OrderListener destructor
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
OrderListener::~OrderListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderListener::Orders
// Created: MPT 2009-07-28
// -----------------------------------------------------------------------------
void OrderListener::Clean()
{
	try
    {
        std::string clause( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) );
		database_.Execute( DeleteQueryBuilder( database_.GetTableName( "Orders" ), clause ) );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "OrderListener is not correctly loaded : " + std::string( e.what() ) );
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
// Name: OrderListener::Listen
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::Listen()
{
    try
    {
        std::stringstream ss;
        ss << "id > " << ref_ << " AND checked=-1" << " AND session_id=" << session_.GetId(); // OBJECTID
        long orderid = -1;
        const Row_ABC* row = table_->Find( ss.str() );
        while( row != 0 )
        {
            try
            {
                ref_ = row->GetID();
                orderid = GetField< long >( *row, "id" );
                dispatcher_->Dispatch( publisher_, *row );
                MarkProcessed( orderid );
            }
            catch ( std::exception& ex )
            {
                if( orderid >= 0 )
                    MarkProcessed( orderid );
                MT_LOG_ERROR_MSG( "crossbow::Listen - unable to build order correctly: " << std::endl << ex.what() );
            }
            row = table_->GetNextRow();
        }
    }
    catch ( std::exception& ex )
    {
        MT_LOG_ERROR_MSG( "crossbow::Listen : " + std::string( ex.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderListener::MarkProcessed
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::MarkProcessed( long orderid ) const
{
    std::stringstream ss;

    ss << "id = " << orderid << " AND checked=-1" << " AND session_id=" << session_.GetId(); // OBJECTID
    UpdateQueryBuilder builder( database_.GetTableName( "Orders" ) );
    builder.SetField( "checked", 0 );
    builder.SetClause( ss.str() );
    database_.Execute( builder );
}
