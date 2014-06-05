// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SimulationMonitor_h_
#define __SimulationMonitor_h_

#include "tools/ClientNetworker.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class AarToClient;
    class AuthenticationToClient;
    class ClientToSim;
    class DispatcherToClient;
    class MessengerToClient;
    class ReplayToClient;
    class SimToClient;
}

namespace frontend
{
// =============================================================================
/** @class  SimulationMonitor
    @brief  Simulation monitor
*/
// Created: SBO 2010-10-26
// =============================================================================
class SimulationMonitor : public tools::ClientNetworker
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SimulationMonitor( const std::string& host );
    virtual ~SimulationMonitor();
    //@}

    //! @name Accessors
    //@{
    bool Connected() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void ConnectionFailed( const std::string& endpoint, const std::string& reason );
    virtual void ConnectionError( const std::string& endpoint, const std::string& reason );
    virtual void ConnectionWarning( const std::string& endpoint, const std::string& reason );
    //@}

    //! @name Message handlers
    //@{
    void HandleAarToClient( const std::string& endpoint, const sword::AarToClient& message );
    void HandleAuthenticationToClient( const std::string& endpoint, const sword::AuthenticationToClient& message );
    void HandleDispatcherToClient( const std::string& endpoint, const sword::DispatcherToClient& message );
    void HandleMessengerToClient( const std::string& endpoint, const sword::MessengerToClient& message );
    void HandleReplayToClient( const std::string& endpoint, const sword::ReplayToClient& message );
    void HandleSimToClient( const std::string& endpoint, const sword::SimToClient& message );
    //@}

    //! @name Types
    //@{
    struct TimeOut;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< TimeOut > timeOut_;
    bool connected_;
    //@}
};

}

#endif // __SimulationMonitor_h_
