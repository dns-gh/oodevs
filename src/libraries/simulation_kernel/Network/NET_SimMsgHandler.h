// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#ifndef __NET_SimMsgHandler_h_
#define __NET_SimMsgHandler_h_

#include "NET_Publisher_ABC.h"

namespace sword
{
    class SimToClient;
    class ClientToSim;
    class DispatcherToSim;
    class MagicAction;
    class MagicActionAck;
    class MissionParameters;
}

class ActionManager;
class NET_AgentServer;
class NET_Simulation_ABC;

//=============================================================================
// Created: NLD 2002-07-12
// Last Modified : JVT 02-10-10
//=============================================================================
class NET_SimMsgHandler : public NET_Publisher_ABC
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             NET_SimMsgHandler( NET_AgentServer& agentServer, NET_Simulation_ABC& simulation,
                    ActionManager& actions, bool enableTestCommands );
    virtual ~NET_SimMsgHandler();
    //@}

    //! @name Message sending
    //@{
    virtual void Send( sword::SimToClient& asnMsg );
    bool RemoveClient( const std::string& client );
    //@}

private:
    //! @name Msg callbacks
    //@{
    void OnReceiveClient( const std::string& from, const sword::ClientToSim& message );
    void OnReceiveMiddle( const std::string& from, const sword::DispatcherToSim& message );
    void OnReceiveCtrlClientAnnouncement( const std::string& from );
    void OnReceiveMagicAction( const sword::MagicAction& msg, uint32_t ctx, uint32_t clientId );
    void OnReceiveDebugError( const sword::MissionParameters& params, sword::MagicActionAck& ack );
    void OnReceiveSegmentRequest( const sword::SegmentRequest& msg, uint32_t ctx, uint32_t clientId );
    //@}

    //! @name Types
    //@{
    typedef std::set< std::string > T_Clients;
    //@}

private:
    //! @name Member data
    //@{
    NET_AgentServer& agentServer_;
    NET_Simulation_ABC& simulation_;
    ActionManager& actions_;
    T_Clients clients_;
    bool enableTestCommands_;
    //@}
};

#endif // __NET_SimMsgHandler_h_
