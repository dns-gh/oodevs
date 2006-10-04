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
#include "NET_AS_MOSServerMgr_ABC.h"

#include "NET_AS_MOSServer.h"

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class NET_AS_MOSServerConnectionMgr : public NET_AS_MOSServerMgr_ABC
{
    MT_COPYNOTALLOWED( NET_AS_MOSServerConnectionMgr );

public:
    NET_AS_MOSServerConnectionMgr( NET_AgentServer& agentServer );
    virtual ~NET_AS_MOSServerConnectionMgr();

    //! @name Main methods
    //@{
    bool StartServer();
    bool StopServer ();
    //@}
    
    //! @name Accessors
    //@{
    const T_MosConnectionMap& GetMosConnections     () const;
    const T_MosConnectionMap& GetMosLightConnections() const;

    bool NeedsUpdating() const;
    //@}

    //! @name Connections management
    //@{
    NET_AS_MOSServer& AddConnection   ( DIN::DIN_Link& link, NET_AS_MOSServer::E_ClientType nClientType );
    void              RemoveConnection( DIN::DIN_Link& link  );
    //@}

private:
    //! @name Connection service callbacks
    //@{
    void                OnConnectionReceived   ( DIN::DIN_Server& server, DIN::DIN_Link& link );
    void                OnBadConnectionReceived( DIN::DIN_Server& server, const NEK::NEK_Address_ABC& address, const DIN::DIN_ErrorDescription& reason );
    void                OnConnectionLost       ( DIN::DIN_Server& server, DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

private:
    DIN::DIN_ConnectionServiceServerUserCbk< NET_AS_MOSServerConnectionMgr > connectionService_;

    // Server
    DIN::DIN_Server* pServer_;

    // Clients
    T_MosConnectionMap mosConnectionMap_;
    T_MosConnectionMap mosLightConnectionMap_;
};

#include "NET_AS_MOSServerConnectionMgr.inl"

#endif // __NET_AS_MOSServerConnectionMgr_h_
