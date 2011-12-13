//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AgentServer.h $
// $Author: Age $
// $Modtime: 2/03/05 15:59 $
// $Revision: 5 $
// $Workfile: NET_AgentServer.h $
//
//*****************************************************************************

#ifndef __NET_AgentServer_h_
#define __NET_AgentServer_h_

#include "tools/ServerNetworker.h"
#include "NET_Publisher_ABC.h"

namespace sword
{
    class SimToClient;
    class ClientToSim;
    class DispatcherToSim;
}

class MIL_Config;
class MIL_Time_ABC;
class NET_Simulation_ABC;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class NET_AgentServer : public tools::ServerNetworker
                      , public NET_Publisher_ABC
{
public:
             NET_AgentServer( const MIL_Config& config, const MIL_Time_ABC& time, NET_Simulation_ABC& simulation );
    virtual ~NET_AgentServer();

    //!  @name Main methods
    //@{
    void Update();
    //@}

    //! @name NET_Publisher_ABC
    //@{
    virtual void Send( sword::SimToClient& asnMsg );
    //@}

    //! @name Ugly accessors
    //@{
    bool MustInitUnitVisionCones   () const;
    bool MustSendUnitVisionCones   () const;
    void SetMustSendUnitVisionCones( bool bEnable );
    //@}

private:
    //! @name Low level callbacks
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    virtual void ConnectionWarning  ( const std::string& address, const std::string& warning );
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveClient( const std::string& from, const sword::ClientToSim& message );
    void OnReceiveMiddle( const std::string& from, const sword::DispatcherToSim& message );
    void OnReceiveCtrlClientAnnouncement( const std::string& from );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Clients;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Time_ABC& time_;
    NET_Simulation_ABC& simulation_;
    T_Clients           clients_;
    unsigned int        nUnitVisionConesChangeTimeStep_;
    bool                bSendUnitVisionCones_;
    //@}
};

#endif // __NET_AgentServer_h_
