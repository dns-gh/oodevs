// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Client_h_
#define __Client_h_

#include "protocol/ClientPublisher_ABC.h"


////using namespace Common;

namespace tools
{
    class MessageSender_ABC;
}

namespace dispatcher
{

// =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: NLD 2006-09-19
// =============================================================================
class Client : public ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( tools::MessageSender_ABC& sender, const std::string& endpoint );
    virtual ~Client();
    //@}

    //! @name Messages
    //@{
    virtual void Send( const MsgsAuthenticationToClient::MsgAuthenticationToClient& msg );
    virtual void Send( const MsgsSimToClient::MsgSimToClient& msg );
    virtual void Send( const MsgsReplayToClient::MsgReplayToClient& msg );
    virtual void Send( const MsgsAarToClient::MsgAarToClient& msg );
    virtual void Send( const MsgsMessengerToClient::MsgMessengerToClient& msg );
    virtual void Send( const MsgsDispatcherToClient::MsgDispatcherToClient& msg );
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetEndpoint() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Client( const Client& );            //!< Copy constructor
    Client& operator=( const Client& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& sender_;
    std::string endpoint_;
    //@}
};

}

#endif // __Client_h_
