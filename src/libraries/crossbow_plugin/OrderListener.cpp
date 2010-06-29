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
#include "Workspace_ABC.h"
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
OrderListener::OrderListener( Workspace_ABC& workspace, const dispatcher::Model& model, const OrderTypes& types, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession& session )
    : publisher_ ( publisher )
    , dispatcher_( new OrderDispatcher( workspace, types, model ) )
    , database_  ( workspace.GetDatabase( "flat" ) )
    , ref_ ( 0 )
    , session_ ( session )
{
    table_.reset( database_.OpenTable( "Orders" ) );
    Clean();
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
        table_->DeleteRows( "session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) );
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
        const Row_ABC* row = table_->Find( ss.str() );
        while( row != 0 )
        {
            ListenRow( *row );
            row = table_->GetNextRow();
        }
    }
    catch ( std::exception& ex )
    {
        MT_LOG_ERROR_MSG( "crossbow::Listen : " + std::string( ex.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderListener::ListenRow
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
void OrderListener::ListenRow( const Row_ABC& row )
{
    long orderid = -1;
    try
    {
        ref_ = row.GetID();
        orderid = GetField< long >( row, "id" );
        dispatcher_->Dispatch( publisher_, row );
        MarkProcessed( orderid );
    }
    catch ( std::exception& ex )
    {
        if( orderid >= 0 )
            MarkProcessed( orderid );
        MT_LOG_ERROR_MSG( "crossbow::ListenRow - unable to build order correctly: " << std::endl << ex.what() );
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

    std::auto_ptr< Table_ABC > table( database_.OpenTable( "Orders" ) );
    Row_ABC* row = table->Find( ss.str() );
    if( row == 0 )
        throw std::runtime_error( "unable to process requested order: " + ss.str() );
    row->SetField( "checked", FieldVariant( true ) );
}
