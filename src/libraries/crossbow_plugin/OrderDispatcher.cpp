// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderDispatcher.h"
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "ActionSerializer_ABC.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include "actions/Action_ABC.h"
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

using namespace plugins;
using namespace plugins::crossbow;

namespace 
{
    Database_ABC& GetDatabase( Workspace_ABC& workspace )
    {
        return workspace.GetDatabase( "flat" );
    }
}


// -----------------------------------------------------------------------------
// Name: OrderDispatcher constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderDispatcher::OrderDispatcher( const dispatcher::Model_ABC& model, Workspace_ABC& workspace, ActionSerializer_ABC& serializer )
    : model_        ( model )
    , workspace_    ( workspace )
    , serializer_   ( serializer )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderListener::Orders
// Created: JCR 2009-08-14
// -----------------------------------------------------------------------------
void OrderDispatcher::Clean()
{
//    try
//    {
//        std::string clause( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) );
//        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "OrderParameters_Area" ), clause ) );
//        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "OrderParameters_Line" ), clause ) );
//        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "OrderParameters_Point" ), clause ) );
//        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "OrderParameters" ), clause ) );
//    }
//    catch ( std::exception& e )
//    {
//        MT_LOG_ERROR_MSG( "OrderListener is not correctly loaded : " + std::string( e.what() ) );
//    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher destructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderDispatcher::~OrderDispatcher()
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

    std::string BuildError( const std::string& mission, int id )
    {
        std::stringstream ssError;
        ssError << "Unable to serialize order " << mission
                << " for [" << id << "]" << std::endl;
        return ssError.str();
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::IsValidOrder
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
bool OrderDispatcher::IsValidOrder( const Row_ABC& /*row*/ ) const
{
    return true;
}

#include <xeumeuleu/xml.hpp>

namespace
{
    void DebugOrder( const actions::Action_ABC& action )
    {
        xml::xostringstream xos;
        xos << xml::start( "action" );
            action.Serialize( xos );

        MT_LOG_ERROR_MSG( xos.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::Dispatch
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::Dispatch( Publisher_ABC& publisher, const Row_ABC& row )
{
    const int id = GetField< int >( row, "unit_id" );
    std::auto_ptr< actions::Action_ABC > mission;
    
    if( const dispatcher::Agent_ABC* agent = model_.Agents().Find( id ) )
    {
        if( agent->GetSuperior().IsEngaged() ) 
            serializer_.SerializeMission( agent->GetSuperior(), row, mission );
        else
            serializer_.SerializeMission( *agent, row, mission );
    }
    else if( const dispatcher::Automat_ABC* automat = model_.Automats().Find( id ) )
        serializer_.SerializeMission( *automat, row, mission );
    if ( !mission.get() )
        throw std::runtime_error( "Unable to resolve order for the target unit: " + boost::lexical_cast< std::string >( id ) );
    if ( mission->IsValid() )
        mission->Publish( publisher );
    DebugOrder( *mission );
}


// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchFragOrder
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchFragOrder( Publisher_ABC& publisher, unsigned long /*targetId*/, const Row_ABC& /*row*/ )
{
    std::auto_ptr< actions::Action_ABC > action;

    if( action.get() )
        action->Publish( publisher );
    /*
    const kernel::OrderType* type = types_.FindFragOrder( GetField< std::string >( row, "OrderName" ) );
    if( !type )
        return; // $$$$ SBO 2007-06-07:
    simulation::FragOrder message;
    model_.SetToTasker( *message().mutable_tasker(), targetId );
    //message().set_id( targetId );
    message().mutable_frag_order()->set_id( type->GetId() );
    message().mutable_parameters(); // $$$$ SBO 2007-06-07: parameters not supported !
    message.Send( publisher, 0 );
    */
}