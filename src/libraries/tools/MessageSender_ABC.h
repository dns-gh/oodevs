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

namespace google
{
namespace protobuf
{

class Message;

}
}

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
    template< typename Message >
    void Send( const std::string& link, const Message& message )
    {
        static const unsigned long tag = MessageIdentifierFactory::GetIdentifier< Message >();
        Send( link, tag, message );
    }
    virtual void Send( const std::string& link, unsigned long tag, const google::protobuf::Message& message );
    void Serialize( const google::protobuf::Message& m, Message& message ) const;
    virtual void Send( const std::string& link, unsigned long tag, Message& message ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetNbMessagesSent() const = 0;
    virtual bool HasAnsweredSinceLastTick( const std::string& endpoint ) = 0;
    //@}
};

}

#endif // __MessageSender_ABC_h_
