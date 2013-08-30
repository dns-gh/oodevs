// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "MessageSender_ABC.h"
#include "Message.h"
#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#pragma warning( pop )
#include <tools/Exception.h>
#include <boost/shared_array.hpp>

using namespace tools;

void MessageSender_ABC::Send( const std::string& link, unsigned long tag,
        const google::protobuf::Message& message )
{
    Message toolsMessage;
    Serialize( message, toolsMessage );
    Send( link, tag, toolsMessage );
}

void MessageSender_ABC::Serialize( const google::protobuf::Message& m, Message& message ) const
{
    if( !m.IsInitialized() )
        throw MASA_EXCEPTION(  "Message \"" + m.ShortDebugString()
            + "\" of type \"" + m.GetDescriptor()->full_name()
            + "\" is missing required fields: " + m.InitializationErrorString() );

    boost::shared_array< google::protobuf::uint8 > buffer( new google::protobuf::uint8[ m.ByteSize() ] );
    if( !m.SerializeWithCachedSizesToArray( buffer.get() ) )
        throw MASA_EXCEPTION( "Error serializing message of type \"" + m.GetDescriptor()->full_name() + '"' );
    message.Write( (const char*)buffer.get(), m.GetCachedSize() );
}


