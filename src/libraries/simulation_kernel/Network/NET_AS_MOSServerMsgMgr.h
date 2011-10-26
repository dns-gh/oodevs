//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServerMsgMgr.h $
// $Author: Nld $
// $Modtime: 22/03/05 16:53 $
// $Revision: 7 $
// $Workfile: NET_AS_MOSServerMsgMgr.h $
//
//*****************************************************************************

#ifndef __NET_AS_MOSServerMsgMgr_h_
#define __NET_AS_MOSServerMsgMgr_h_

#include "NET_Publisher_ABC.h"

namespace sword
{
    class SimToClient;
    class ClientToSim;
    class DispatcherToSim;
}

class NET_AgentServer;
class NET_Simulation_ABC;

//=============================================================================
// Created: NLD 2002-07-12
// Last Modified : JVT 02-10-10
//=============================================================================
class NET_AS_MOSServerMsgMgr : public NET_Publisher_ABC
                             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             NET_AS_MOSServerMsgMgr( NET_AgentServer& agentServer, NET_Simulation_ABC& simulation );
    virtual ~NET_AS_MOSServerMsgMgr();
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
    //@}

    //! @name Types
    //@{
    typedef std::set< std::string >     T_Clients;
    typedef T_Clients::const_iterator CIT_Clients;
    //@}

private:
    //! @name Member data
    //@{
    NET_AgentServer& agentServer_;
    NET_Simulation_ABC& simulation_;
    T_Clients clients_;
    //@}
};

#endif // __NET_AS_MOSServerMsgMgr_h_
