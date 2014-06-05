// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "ClientObjectsModel.h"
#include "ClientObject.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include "tools/IdManager.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;
using namespace sword;

// -----------------------------------------------------------------------------
// Name: ClientObjectsModel constructor
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
ClientObjectsModel::ClientObjectsModel( dispatcher::ClientPublisher_ABC& clients, tools::IdManager& idManager )
    : clients_  ( clients )
    , idManager_( idManager )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientObjectsModel destructor
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
ClientObjectsModel::~ClientObjectsModel()
{
    DeleteAll();
}

namespace
{
    ClientObjectAck::ErrorCode CheckProperties( const ::google::protobuf::RepeatedPtrField< ::sword::ClientObjectProperty >& properties )
    {
        std::set< std::string > attributeNames;
        for( int i = 0; i < properties.size(); ++i )
        {
            const std::string& attributeName = properties.Get( i ).name();
            if( attributeName.empty() )
                return ClientObjectAck::invalid_attribute_name;
            if( !attributeNames.insert( attributeName ).second )
                return ClientObjectAck::duplicate_attribute_name;
        }
        return ClientObjectAck::success;
    }
}
// -----------------------------------------------------------------------------
// Name: ClientObjectsModel::HandleRequest
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
void ClientObjectsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ClientObjectCreationRequest& message )
{
    ClientObjectCreationRequestAck ack;
    ClientObjectAck::ErrorCode error = CheckProperties( message.properties() );
    if( error == ClientObjectAck::success )
    {
        try
        {
            std::unique_ptr< ClientObject > clientObject( new ClientObject( idManager_.GetNextId(), message ) );
            Register( clientObject->GetID(), *clientObject );
            clientObject->SendCreation( clients_ );
            clientObject.release();
        }
        catch( ... )
        {
            error = ClientObjectAck::duplicate_attribute_name;
        }
    }
    std::string name;
    if( message.has_name() )
        name = message.name();
    ack().set_name( name );
    ack().set_error_code( error );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectsModel::HandleRequest
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
void ClientObjectsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ClientObjectDestructionRequest& message )
{
    ClientObjectDestructionRequestAck ack ;
    ClientObjectAck::ErrorCode error = ClientObjectAck::success;
    ClientObject* clientObject = Find( message.object().id() );
    if( clientObject )
    {
        clientObject->SendDestruction( clients_ );
        delete clientObject;
        Remove( message.object().id() );
    }
    else
        error = ClientObjectAck::invalid_object_id;
    ack().mutable_object()->set_id( message.object().id() );
    ack().set_error_code( error );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectsModel::HandleRequest
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
void ClientObjectsModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ClientObjectUpdateRequest& message )
{
    ClientObjectUpdateRequestAck ack ;
    ClientObjectAck::ErrorCode error = CheckProperties( message.properties() );
    if( error == ClientObjectAck::success )
    {
        ClientObject* clientObject = Find( message.object().id() );
        if( clientObject )
        {
            clientObject->Update( message );
            clientObject->SendUpdate( clients_ );
        }
        else
            error = ClientObjectAck::invalid_object_id;
    }
    ack().mutable_object()->set_id( message.object().id() );
    ack().set_error_code( error );
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectsModel::SendStateToNewClient
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
void ClientObjectsModel::SendStateToNewClient( dispatcher::ClientPublisher_ABC& client ) const
{
    tools::Iterator< const ClientObject& > it( CreateIterator() );
    while( it.HasMoreElements() )
        it.NextElement().SendFullState( client );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectsModel::Write
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
void ClientObjectsModel::Write( xml::xostream& xos ) const
{
    if( Count() )
    {
        xos << xml::start( "client-objects" );
            tools::Iterator< const ClientObject& > it( CreateIterator() );
            while( it.HasMoreElements() )
                it.NextElement().Write( xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ClientObjectsModel::ReadClientObject
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
void ClientObjectsModel::ReadClientObject( xml::xistream& xis )
{
    unsigned int id = xis.attribute< unsigned int >( "id" );
    idManager_.Lock( id );
    std::unique_ptr< ClientObject > clientObject( new ClientObject( id, xis ) );
    Register( clientObject->GetID(), *clientObject );
    clientObject.release();
}
