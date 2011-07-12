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
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "WorkingSession_ABC.h"
#include "ActionSerializer_ABC.h"
#include "actions/Action_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
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
// Name: ObjectListener constructor
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
ObjectListener::ObjectListener( Workspace_ABC& workspace, ActionSerializer_ABC& serializer, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession_ABC& session, dispatcher::Logger_ABC& logger )
    : publisher_ ( new CrossbowPublisher( publisher ) )
    , workspace_ ( workspace )
    , serializer_ ( serializer )
    , session_ ( session )
    , logger_ ( logger )
{
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
        std::string query( "session_id=" + boost::lexical_cast<std::string>( session_.GetId() ) );
        std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "flat" ).OpenTable( "create_object" ) );

        table->DeleteRows( query );
    }
    catch ( std::exception& e )
    {
        logger_.LogError( "ObjectListener is not correctly loaded : " + std::string( e.what() ) );
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
        std::string query( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) + " AND checked_xbow=0" );
        std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "flat" ).OpenTable( "create_object" ) );

        Row_ABC* row = table->Find( query );
        while( row )
        {
            if( SendCreation( *row ) )
            {
                row->SetField( "checked_xbow", FieldVariant( true ) );
                table->UpdateRow( *row );
            }
            row = table->GetNextRow();
        }
    }
    catch ( std::exception& e )
    {
        logger_.LogError( "(" + std::string( __FUNCTION__ ) + ") " + std::string( e.what() ) );
    }
}

namespace
{
    void DebugAction( const actions::Action_ABC& action, dispatcher::Logger_ABC& logger )
    {
        xml::xostringstream xos;
        xos << xml::start( "action" );
            action.Serialize( xos );
        logger.LogInfo( "(" + std::string( __FUNCTION__ ) + ")" + xos.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::SendCreation
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
bool ObjectListener::SendCreation( const Row_ABC& row )
{
    long orderid = -1;
    try
    {
        orderid = row.GetID();
        if( orderid != -1 )
        {
            std::auto_ptr< actions::Action_ABC > magic;

            serializer_.SerializeCreation( row, magic );
            if ( !magic.get() )
                throw std::runtime_error( "Unable to resolve object creation." );
            if ( magic->IsValid() )
                magic->Publish( *publisher_ );
            else
                DebugAction( *magic, logger_ );
        }
    }
    catch ( std::exception& ex )
    {
        logger_.LogError( "crossbow::ObjectListener - unable to build object correctly: " + std::string( ex.what() ) );
    }
    return orderid >= 0;
}
