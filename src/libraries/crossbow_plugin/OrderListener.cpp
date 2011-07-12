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
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "ActionSerializer_ABC.h"
#include "WorkingSession_ABC.h"
#include "actions/Action_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <xeumeuleu/xml.hpp>

using namespace plugins::crossbow;

namespace
{
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
OrderListener::OrderListener( Workspace_ABC& workspace, ActionSerializer_ABC& serializer, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession_ABC& session, dispatcher::Logger_ABC& logger )
    : publisher_    ( new CrossbowPublisher( publisher ) )
    , workspace_    ( workspace )
    , session_      ( session )
    , serializer_   ( serializer )
    , logger_       ( logger )
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
        std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "flat" ).OpenTable( "create_order" ) );
        Row_ABC* row = table->Find( "session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) + " AND checked_xbow=0" );
        while( row != 0 )
        {
            row->SetField( "checked_xbow", FieldVariant( true ) );
            table->UpdateRow( *row );
            row = table->GetNextRow();
        }
    }
    catch ( std::exception& e )
    {
        logger_.LogError( "OrderListener is not correctly loaded : " + std::string( e.what() ) );
    }
}

namespace
{
    template< typename Type >
    Type GetField( const Row_ABC& row, const std::string& name )
    {
        return boost::get< Type >( row.GetField( name ) );
    }

    void DebugAction( const actions::Action_ABC& action, dispatcher::Logger_ABC& logger )
    {
        xml::xostringstream xos;
        xos << xml::start( "action" );
            action.Serialize( xos );
        logger.LogError( "(" + std::string( __FUNCTION__ ) + ")" + xos.str() );
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
        std::string query( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) + " AND checked_xbow=0" );
        std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "flat" ).OpenTable( "create_order" ) );

        Row_ABC* row = table->Find( query );
        while( row != 0 )
        {
            if( SendCreation( *row ) )
            {
                row->SetField( "checked_xbow", FieldVariant( true ) );
                table->UpdateRow( *row );
            }
            row = table->GetNextRow();
        }
    }
    catch ( std::exception& ex )
    {
        logger_.LogError( "crossbow::Listen : " + std::string( ex.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderListener::SendCreation
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
bool OrderListener::SendCreation( const Row_ABC& row )
{
    long orderid = -1;
    try
    {
        orderid = row.GetID();
        if( orderid != -1 )
        {
            std::auto_ptr< actions::Action_ABC > order;

            serializer_.SerializeCreation( row, order );
            if ( !order.get() )
                throw std::runtime_error( "Unable to resolve object creation." );
            if ( order->IsValid() )
                order->Publish( *publisher_ );
            else
                DebugAction( *order, logger_ );
        }
    }
    catch ( std::exception& ex )
    {
        logger_.LogError( "crossbow::ListenRow - unable to build order correctly: " + std::string( ex.what() ) );
    }
    return orderid >= 0;
}