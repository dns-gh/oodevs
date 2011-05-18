// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#ifndef __LAUNCHER_SWORDFACADE_H__
#define __LAUNCHER_SWORDFACADE_H__

#include <map>
#include <boost/shared_ptr.hpp>
#include "client_proxy/SwordConnectionHandler_ABC.h"
#include "client_proxy/SwordMessageHandler_ABC.h"


class SwordProxy;
class SwordMessageHandler_ABC;

namespace frontend
{
    class ProcessWrapper;
    class ProcessObserver_ABC;
    class SpawnCommand;
}

namespace sword
{
    class ClientToSim;
    class SimToClient;
    class MessengerToClient;
}

namespace launcher
{
// =============================================================================
/** @class  SwordFacade
    @brief  SwordFacade
*/
// Created: AHC 2011-05-16
// =============================================================================
class SwordFacade : public SwordConnectionHandler_ABC, public SwordMessageHandler_ABC
{
public:
    struct MessageHandler
    {
        //! @name Constructor/destructor
        //@{
        virtual ~MessageHandler() {}
        //@}

        //! @name Operations
        //@{
        virtual bool OnReceiveMessage( const sword::SimToClient& message ) = 0; // return true if handler can be deleted
        virtual bool OnReceiveMessage( const sword::MessengerToClient& message ) = 0; // return true if handler can be deleted
        //@}
    };

    //! @name Constructor/destructor
    //@{
    SwordFacade( frontend::ProcessObserver_ABC& observer, boost::shared_ptr< frontend::SpawnCommand > process, bool isDispatcher = false );
    virtual ~SwordFacade();
    //@}

    //! @name Accessors
    //@{
    bool IsConnected() const { return isConnected_; }
    bool IsAuthenticated() const { return isAuthenticated_; }
    //@}

    //! @name Operations
    //@{
    void Start(const std::string& supervisorProfile, const std::string& supervisorPassword);
    void Stop();
    // SwordConnectionHandler_ABC interface
    void OnConnectionSucceeded( const std::string& endpoint );
    void OnConnectionFailed( const std::string& endpoint, const std::string& reason );
    void OnConnectionError( const std::string& endpoint, const std::string& reason );
    void OnAuthenticationSucceeded( const std::string& profile );
    void OnAuthenticationFailed( const std::string& profile, const std::string& reason );
    // SwordMessageHandler_ABC interface
    void OnReceiveMessage( const sword::SimToClient& message );
    void OnReceiveMessage( const sword::MessengerToClient& message );
    //
    void RegisterMessageHandler( int context, std::auto_ptr<MessageHandler> handler );
    void SetPermanentMessageHandler( std::auto_ptr<MessageHandler> handler );
    void Send( const sword::ClientToSim& message ) const;
    //@}

private:
    bool isDispatcher_;
    bool isConnected_;
    bool isAuthenticated_;
    boost::shared_ptr<frontend::ProcessWrapper> process_;
    boost::shared_ptr<SwordProxy> client_;
    boost::shared_ptr<SwordConnectionHandler_ABC> connectionHandler_;
    typedef std::map<int, boost::shared_ptr<MessageHandler> > HandlerContainer;
    HandlerContainer messageHandlers_;
    std::auto_ptr<MessageHandler> permanentHandler_;
};

}

#endif // __LAUNCHER_SWORDFACADE_H__
