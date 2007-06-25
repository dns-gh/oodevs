//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/network/NET_AS_MOSServerConnectionMgr.h $
// $Author: Age $
// $Modtime: 20/10/04 18:50 $
// $Revision: 2 $
// $Workfile: NET_AS_MOSServerConnectionMgr.h $
//
//*****************************************************************************

#ifndef __NET_AS_MOSServerConnectionMgr_h_
#define __NET_AS_MOSServerConnectionMgr_h_

#include "MIL.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceServerUserCbk.h"
class NET_AgentServer;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class NET_AS_MOSServerConnectionMgr
{
    MT_COPYNOTALLOWED( NET_AS_MOSServerConnectionMgr );

public:
    //! @name Types
    //@{
    typedef std::set< DIN::DIN_Link* >        T_ConnectionMap;
    typedef T_ConnectionMap::const_iterator CIT_ConnectionMap;
    //@}

public:

    //! @name Constructor/Destructor
    //@{
    explicit NET_AS_MOSServerConnectionMgr( NET_AgentServer& agentServer );
    virtual ~NET_AS_MOSServerConnectionMgr();
    //@}

    //! @name Main methods
    //@{
    bool StartServer();
    bool StopServer ();
    //@}
    
    //! @name Accessors
    //@{
    const T_ConnectionMap& GetConnections() const;

    bool NeedsUpdating() const;
    //@}

    //! @name Connections management
    //@{
    void AddConnection   ( DIN::DIN_Link& link );
    void RemoveConnection( DIN::DIN_Link& link );
    //@}

private:
    //! @name Connection service callbacks
    //@{
    void OnConnectionReceived   ( DIN::DIN_Server& server, DIN::DIN_Link& link );
    void OnBadConnectionReceived( DIN::DIN_Server& server, const NEK::NEK_Address_ABC& address, const DIN::DIN_ErrorDescription& reason );
    void OnConnectionLost       ( DIN::DIN_Server& server, DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

private:
    NET_AgentServer& agentServer_;
    DIN::DIN_ConnectionServiceServerUserCbk< NET_AS_MOSServerConnectionMgr > connectionService_;

    // Server
    DIN::DIN_Server* pServer_;

    // Clients
    T_ConnectionMap connections_;
};

#endif // __NET_AS_MOSServerConnectionMgr_h_
