// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "ClientObject.h"
#include "ClientObjectProperty.h"
#include "protocol/MessengerSenders.h"
#include "protocol/ClientPublisher_ABC.h"
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

#define REGISTER_PROPERTY( MSG, TYPE ) \
    if( msg.value().has_##MSG##_value() ) \
    { \
        Register( msg.name(), *new ClientObjectProperty< TYPE >( msg.value().MSG##_value(), msg.name() ) ); \
        continue; \
    }

// -----------------------------------------------------------------------------
// Name: ClientObject constructor
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
ClientObject::ClientObject( unsigned int id, const sword::ClientObjectCreationRequest& message )
    : id_        ( id )
    , persistent_( false )
{
    if( message.has_name() )
        name_ = message.name();
    if( message.has_persistent() )
        persistent_ = message.persistent();
    for( int i = 0; i < message.properties_size(); ++i )
    {
        const sword::ClientObjectProperty& msg = message.properties( i );
        REGISTER_PROPERTY( string, std::string )
        REGISTER_PROPERTY( integer, int )
        REGISTER_PROPERTY( unsigned_integer, unsigned int )
        REGISTER_PROPERTY( float, float )
        REGISTER_PROPERTY( bool, bool )
    }
}

// -----------------------------------------------------------------------------
// Name: ClientObject constructor
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
ClientObject::ClientObject( unsigned int id, xml::xistream& xis )
    : id_        ( id )
    , persistent_( true )
{
    xis >> xml::optional >> xml::attribute( "name", name_ )
        >> xml::list( "property", *this, &ClientObject::ReadProperty );
}

// -----------------------------------------------------------------------------
// Name: ClientObject destructor
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
ClientObject::~ClientObject()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ClientObject::GetID
// Created: JSR 2010-10-15
// -----------------------------------------------------------------------------
unsigned int ClientObject::GetID() const
{
    return id_;
}

#define UPDATE_PROPERTY( MSG, TYPE ) \
    if( msg.value().has_##MSG##_value() ) \
    { \
        ClientObjectProperty< TYPE >* dyn = dynamic_cast< ClientObjectProperty< TYPE >* >( prop ); \
        if( dyn ) \
            dyn->Update( msg.value().MSG##_value() ); \
        continue; \
    }

// -----------------------------------------------------------------------------
// Name: ClientObject::Update
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObject::Update( const sword::ClientObjectUpdateRequest& message )
{
    if( message.has_name() )
        name_ = message.name();
    for( int i = 0; i < message.properties_size(); ++i )
    {
        const sword::ClientObjectProperty& msg = message.properties( i );
        ClientObjectProperty_ABC* prop = Find( msg.name() );
        if( prop )
        {
            UPDATE_PROPERTY( string, std::string )
            UPDATE_PROPERTY( integer, int )
            UPDATE_PROPERTY( unsigned_integer, unsigned int )
            UPDATE_PROPERTY( float, float )
            UPDATE_PROPERTY( bool, bool )
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ClientObject::SendCreation
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObject::SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const
{
    ClientObjectCreation message;
    message().mutable_object()->set_id( id_ );
    message().set_name( name_ );
    tools::Iterator< const ClientObjectProperty_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
        it.NextElement().Serialize( message().add_properties() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObject::SendUpdate
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObject::SendUpdate( dispatcher::ClientPublisher_ABC& publisher ) const
{
    ClientObjectUpdate message;
    message().mutable_object()->set_id( id_ );
    message().set_name( name_ );
    tools::Iterator< const ClientObjectProperty_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
        it.NextElement().Serialize( message().add_properties() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObject::SendFullState
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObject::SendFullState( dispatcher::ClientPublisher_ABC& publisher ) const
{
    SendCreation( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObject::SendDestruction
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObject::SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const
{
    ClientObjectDestruction message;
    message().mutable_object()->set_id( id_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObject::Write
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObject::Write( xml::xostream& xos ) const
{
    if( persistent_ )
    {
        xos << xml::start( "client-object" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "name", name_ );
        tools::Iterator< const ClientObjectProperty_ABC& > it = CreateIterator();
        while( it.HasMoreElements() )
            it.NextElement().Write( xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ClientObject::ReadProperty
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
void ClientObject::ReadProperty( xml::xistream& xis )
{
    std::string key = xis.attribute< std::string >( "key" );
    std::string type = xis.attribute< std::string >( "type" );
    std::string value = xis.attribute< std::string >( "value" );
    if( type == "string" )
        Register( key, *new ClientObjectProperty< std::string >( value, key ) );
    else if( type == "integer" )
        Register( key, *new ClientObjectProperty< int >( boost::lexical_cast< int >( value ), key ) );
    else if( type == "unsigned-integer" )
        Register( key, *new ClientObjectProperty< unsigned int >( boost::lexical_cast< unsigned int >( value ), key ) );
    else if( type == "float" )
        Register( key, *new ClientObjectProperty< float >( boost::lexical_cast< float >( value ), key ) );
    else if( type == "boolean" )
        Register( key, *new ClientObjectProperty< bool >( boost::lexical_cast< bool >( value ), key ) );
}
