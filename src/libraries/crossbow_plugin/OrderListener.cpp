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
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "WorkingSession_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

using namespace plugins::crossbow;

namespace 
{
    Database_ABC& GetDatabase( Workspace_ABC& workspace )
    {
        return workspace.GetDatabase( "flat" );
    }

    
    class CrossbowPublisher : public Publisher_ABC
                            , private boost::noncopyable
    {
    public:
        explicit CrossbowPublisher( dispatcher::SimulationPublisher_ABC& sim ) : sim_ ( sim ) {}

        virtual void Send( const sword::ClientToSim& message )
        {
            sim_.Send( message );
        }
        virtual void Send( const sword::ClientToAuthentication&  ) {}
        virtual void Send( const sword::ClientToReplay& ){}
        virtual void Send( const sword::ClientToAar& ) {}
        virtual void Send( const sword::ClientToMessenger& ) {}
    
    private:
        dispatcher::SimulationPublisher_ABC& sim_;
    };
}

// -----------------------------------------------------------------------------
// Name: OrderListener constructor
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
OrderListener::OrderListener( const dispatcher::Model_ABC& model, Workspace_ABC& workspace, ActionSerializer_ABC& serializer, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession_ABC& session )
    : publisher_    ( new CrossbowPublisher( publisher ) )
    , workspace_    ( workspace )
    , session_      ( session )
    , dispatcher_   ( new OrderDispatcher( model, workspace, serializer ) )
    , ref_ ( 0 )
{
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
        /* TODO : CASCADING Delete for every parameters
        {
            std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( "OrderParameters" ) );
            table->DeleteRows( 
                "EXISTS ( SELECT 1 FROM sword.orders WHERE" 
                          "order_id=sword.orders.id AND sword.orders.session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) + ")" );
        }
        {
            std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( "Orders" ) );
            table->DeleteRows( "session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) );
        }
        */
        std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( "create_orders" ) );
        Row_ABC* row = table->Find( "session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) );
        while( row != 0 )
        {
            row->SetField( "checked", FieldVariant( true ) );
            table->UpdateRow( *row );
            row = table->GetNextRow();
        }
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
    bool doUpdate = false;
    try
    {
        std::stringstream ss;
        ss << "checked=-1 AND session_id=" << session_.GetId(); // OBJECTID
        std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( "create_orders" ) );
        const Row_ABC* row = table->Find( ss.str() );
        while( row != 0 )
        {
            doUpdate |= ListenRow( *row );
            row = table->GetNextRow();
        }
    }
    catch ( std::exception& ex )
    {
        MT_LOG_ERROR_MSG( "crossbow::Listen : " + std::string( ex.what() ) );
    }
    if ( doUpdate )
        GetDatabase( workspace_ ).Flush();
}

// -----------------------------------------------------------------------------
// Name: OrderListener::ListenRow
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
bool OrderListener::ListenRow( const Row_ABC& row )
{
    long orderid = -1;
    try
    {
        orderid = row.GetID();
        if( orderid != -1 )
        {
            if ( dispatcher_->IsValidOrder( row ) )
            {
                dispatcher_->Dispatch( *publisher_, row );
                MarkProcessed( orderid );
            }
        }
    }
    catch ( std::exception& ex )
    {
        if( orderid >= 0 )
            MarkProcessed( orderid );
        MT_LOG_ERROR_MSG( "crossbow::ListenRow - unable to build order correctly: " << ex.what() );
    }
    return orderid >= 0;
}

// -----------------------------------------------------------------------------
// Name: OrderListener::MarkProcessed
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::MarkProcessed( long orderid ) const
{
    std::stringstream ss;
    ss << "id=" << orderid << " AND checked=-1" << " AND session_id=" << session_.GetId(); // OBJECTID

    std::auto_ptr< Table_ABC > table( GetDatabase( workspace_ ).OpenTable( "create_orders" ) );
    Row_ABC* row = table->Find( ss.str() );
    if( row == NULL )
        throw std::runtime_error( "unable to process requested order: " + ss.str() );
    while ( row != NULL )
    {
        row->SetField( "checked", FieldVariant( true ) );
        table->UpdateRow( *row );
        row = table->GetNextRow();
    }
}
