// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SwordClient_h_
#define __SwordClient_h_

#include "tools/ClientNetworker.h"
#include <vector>

namespace MsgsAarToClient            { class MsgAarToClient; }
namespace MsgsAuthenticationToClient { class MsgAuthenticationToClient; }
namespace MsgsClientToSim            { class MsgClientToSim; }
namespace MsgsDispatcherToClient     { class MsgDispatcherToClient; }
namespace MsgsMessengerToClient      { class MsgMessengerToClient; }
namespace MsgsReplayToClient         { class MsgReplayToClient; }
namespace MsgsSimToClient            { class MsgSimToClient; }

class ConnectionHandlerProxy;
class MessageSender;
class SwordConnectionHandler_ABC;
class SwordMessageHandler_ABC;

// =============================================================================
/** @class  SwordClient
    @brief  SwordClient
*/
// Created: SEB 2010-10-12
// =============================================================================
class SwordClient : public tools::ClientNetworker
{

public:
    //! @name Constructors/Destructor
    //@{
             SwordClient( const std::string& host, unsigned short port, const std::string& profile );
    virtual ~SwordClient();
    //@}

    //! @name Operations
    //@{
	void Connect( SwordConnectionHandler_ABC* handler );
    void RegisterMessageHandler( SwordMessageHandler_ABC& handler );
    void UnregisterMessageHandler( SwordMessageHandler_ABC& handler );
    void SendMessage( const MsgsClientToSim::MsgClientToSim& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SwordClient( const SwordClient& );            //!< Copy constructor
    SwordClient& operator=( const SwordClient& ); //!< Assignment operator
    //@}

    //! @name Connection handlers
    //@{
	virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed( const std::string& endpoint, const std::string& error );
    virtual void ConnectionError( const std::string& endpoint, const std::string& error );
    //@}

    //! @name Message handlers
    //@{
	void HandleAarToClient( const std::string& endpoint, const MsgsAarToClient::MsgAarToClient& message );
    void HandleAuthenticationToClient( const std::string& endpoint, const MsgsAuthenticationToClient::MsgAuthenticationToClient& message );
    void HandleDispatcherToClient( const std::string& endpoint, const MsgsDispatcherToClient::MsgDispatcherToClient& message );
    void HandleMessengerToClient( const std::string& endpoint, const MsgsMessengerToClient::MsgMessengerToClient& message );
    void HandleReplayToClient( const std::string& endpoint, const MsgsReplayToClient::MsgReplayToClient& message );
    void HandleSimToClient( const std::string& endpoint, const MsgsSimToClient::MsgSimToClient& message );
    //@}

    //! @name Types
    //@{
    typedef std::vector< SwordMessageHandler_ABC* > T_Handlers;
    //@}

private:
    //! @name Member data
    //@{
    const std::string host_;
    const unsigned short port_;
	const std::string profile_;
    std::auto_ptr< MessageSender > publisher_;
    bool loggedIn_;
    std::auto_ptr< ConnectionHandlerProxy > connectionHandler_;
    T_Handlers messageHandlers_;
    //@}
};

#endif // __SwordClient_h_
