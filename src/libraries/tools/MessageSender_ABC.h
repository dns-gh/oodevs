// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageSender_ABC_h_
#define __MessageSender_ABC_h_

#include "MessageIdentifierFactory.h"
#include "Message.h"
#include <google/protobuf/descriptor.h>
#include <boost/shared_array.hpp>

namespace tools
{
    class Message;

// =============================================================================
/** @class  MessageSender_ABC
    @brief  Message sender
*/
// Created: AGE 2007-09-06
// =============================================================================
class MessageSender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageSender_ABC() {}
    virtual ~MessageSender_ABC() {}
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void Send( const std::string& link, const T& t )
    {
        if( !t.IsInitialized() )
            throw std::runtime_error( "Message of type \"" + t.GetDescriptor()->full_name()
                + "\" is missing required fields: " + t.InitializationErrorString() );
        boost::shared_array< google::protobuf::uint8 > buffer( new google::protobuf::uint8[ t.ByteSize() ] );
        if( !t.SerializeWithCachedSizesToArray( buffer.get() ) )
            throw std::runtime_error( "Error serializing message of type \"" + t.GetDescriptor()->full_name() + '"' );
        Message message;
        message.Write( (const char*)buffer.get(), t.GetCachedSize() );
        static const unsigned long id = MessageIdentifierFactory::GetIdentifier< T >();
        Send( link, id, message );
    }
    template< typename T >
    void Send( const std::string& link, const T& , const Message& message )
    {
        static const unsigned long id = MessageIdentifierFactory::GetIdentifier< T >();
        Send( link, id, message );
    }

    virtual void Send( const std::string& endpoint, unsigned long tag, const Message& message ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetNbMessagesSent() const = 0;
    //@}
};

}

#endif // __MessageSender_ABC_h_
