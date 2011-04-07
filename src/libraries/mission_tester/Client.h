// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef mission_tester_Client_h
#define mission_tester_Client_h

#include "client_proxy/SwordConnectionHandler_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include <boost/shared_ptr.hpp>

class SwordMessageHandler_ABC;
class SwordProxy;

namespace mission_tester
{
    class Model;
    class Listener_ABC;
// =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: PHC 2011-03-28
// =============================================================================
class Client : public Publisher_ABC
             , private SwordConnectionHandler_ABC
{
public:
    //! @name Constructors / Destructors
    //@{
             Client( SwordMessageHandler_ABC& handler, const std::string& host, unsigned short port, const std::string& profile, const std::string& password );
    virtual ~Client();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const sword::ClientToSim& message );
    virtual void Send( const sword::ClientToAuthentication& message );
    virtual void Send( const sword::ClientToReplay& message );
    virtual void Send( const sword::ClientToAar& message );
    virtual void Send( const sword::ClientToMessenger& message );

    void Update();
    void Disconnect();
    bool IsConnected() const;
    bool IsAuthentified() const;
    void Register( const Listener_ABC& listener );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnConnectionSucceeded( const std::string& endpoint );
    virtual void OnConnectionFailed( const std::string& endpoint, const std::string& reason );
    virtual void OnConnectionError( const std::string& endpoint, const std::string& reason );
    virtual void OnAuthenticationSucceeded( const std::string& profile );
    virtual void OnAuthenticationFailed( const std::string& profile, const std::string& reason );
    //@}

private:
    //! @name mermbers data
    //@{
    std::auto_ptr< SwordProxy > proxy_;
    std::vector< const Listener_ABC* > listeners_;
    bool connected_;
    bool authentified_;
    //@}
};

}

#endif // mission_tester_Client_h