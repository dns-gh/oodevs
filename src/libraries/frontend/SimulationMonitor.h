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

namespace MsgsAarToClient            { class MsgAarToClient; }
namespace MsgsAuthenticationToClient { class MsgAuthenticationToClient; }
namespace MsgsClientToSim            { class MsgClientToSim; }
namespace MsgsDispatcherToClient     { class MsgDispatcherToClient; }
namespace MsgsMessengerToClient      { class MsgMessengerToClient; }
namespace MsgsReplayToClient         { class MsgReplayToClient; }
namespace MsgsSimToClient            { class MsgSimToClient; }

namespace frontend
{

// =============================================================================
/** @class  SimulationMonitor
    @brief  SimulationMonitor
*/
// Created: SBO 2010-10-26
// =============================================================================
class SimulationMonitor : private boost::noncopyable
                        , public tools::ClientNetworker
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
    bool TimedOut() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed( const std::string& endpoint, const std::string& reason );
    virtual void ConnectionError( const std::string& endpoint, const std::string& reason );
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
    struct TimeOut;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< TimeOut > timeOut_;
    bool connected_;
    //@}
};

}

#endif // __SimulationMonitor_h_
