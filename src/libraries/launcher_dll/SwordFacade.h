// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef LAUNCHER_SWORD_FACADE_H
#define LAUNCHER_SWORD_FACADE_H

#include "MessageHandler_ABC.h"
#include "client_proxy/SwordConnectionHandler_ABC.h"
#include "client_proxy/SwordMessageHandler_ABC.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <memory>
#include <map>

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
    class ClientToAuthentication;
}

namespace launcher
{
    class Config;

// =============================================================================
/** @class  SwordFacade
    @brief  SwordFacade
*/
// Created: AHC 2011-05-16
// =============================================================================
class SwordFacade : public SwordConnectionHandler_ABC
                  , public SwordMessageHandler_ABC
{
public:
    //! @name Constructor/destructor
    //@{
             SwordFacade();
    virtual ~SwordFacade();
    //@}

    //! @name Accessors
    //@{
    bool IsConnected() const { return isConnected_; }
    bool IsAuthenticated() const { return isAuthenticated_; }
    //@}

    //! @name Operations
    //@{
    void Start( frontend::ProcessObserver_ABC& observer,
            boost::shared_ptr< frontend::SpawnCommand > command,
            const launcher::Config& config );
           
    void Stop();
    // SwordConnectionHandler_ABC interface
    virtual void OnConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void OnConnectionFailed( const std::string& endpoint, const std::string& reason );
    virtual void OnConnectionError( const std::string& endpoint, const std::string& reason );
    virtual void OnConnectionWarning( const std::string& endpoint, const std::string& reason );
    virtual void OnAuthenticationSucceeded( const std::string& profile );
    virtual void OnAuthenticationFailed( const std::string& profile, const std::string& reason );
    // SwordMessageHandler_ABC interface
    virtual void OnReceiveMessage( const sword::SimToClient& message );
    virtual void OnReceiveMessage( const sword::MessengerToClient& message );
    virtual void OnReceiveMessage( const sword::AuthenticationToClient& message );
    virtual void OnReceiveMessage( const sword::DispatcherToClient& message );
    //
    void RegisterMessageHandler( int context, std::auto_ptr< MessageHandler_ABC > handler );
    void AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC > handler );

    void Send( const sword::ClientToSim& message ) const;
    void Send( const sword::ClientToAuthentication& message ) const;
    bool IsRunning() const;
    void Update() const;

    boost::shared_ptr< frontend::ProcessWrapper > GetProcess();
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void Update( const T& message );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< MessageHandler_ABC > T_Handler;
    typedef std::map< int, T_Handler > HandlerContainer;
    //@}

private:
    //! @name Member Data
    //@{
    bool isConnected_;
    bool isAuthenticated_;
    boost::shared_ptr< frontend::ProcessWrapper > process_;
    boost::shared_ptr< SwordProxy > client_;
    HandlerContainer messageHandlers_;
    std::vector< T_Handler > permanentHandler_;
    //@}
};

}

#endif // LAUNCHER_SWORD_FACADE_H
