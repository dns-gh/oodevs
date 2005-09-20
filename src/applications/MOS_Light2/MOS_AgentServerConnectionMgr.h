//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentServerConnectionMgr.h $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: MOS_AgentServerConnectionMgr.h $
//
//*****************************************************************************

#ifndef __MOS_AgentServerConnectionMgr_h_
#define __MOS_AgentServerConnectionMgr_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Light2_Pch.h"

#include "MOS_Types.h"

#include "MOS_AgentServerMgr_ABC.h"
#include "MOS_AgentServer.h"


//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class MOS_AgentServerConnectionMgr : public MOS_AgentServerMgr_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentServerConnectionMgr );

public:
    MOS_AgentServerConnectionMgr( MOS_AgentServerController& controller );
    virtual ~MOS_AgentServerConnectionMgr();

    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    bool ConnectToServer     ( const std::string& strHostName, uint16 nPort );
    bool DisconnectFromServer();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    bool             IsConnectedToAgentServer() const;
    MOS_AgentServer& GetAgentServer          () const;
    //@}


private:
    //-------------------------------------------------------------------------
    /** @name Connection service callbacks */
    //-------------------------------------------------------------------------
    //@{
    void OnConnected     ( DIN::DIN_Link& link );
    void OnNotConnected  ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    void OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    MOS_AgentServer& AssociateAgentServerToSession   ( DIN::DIN_Link& link );
    void             DissociateAgentServerFromSession( DIN::DIN_Link& link );
    //@}

private:
    DIN::DIN_ConnectionServiceClientUserCbk< MOS_AgentServerConnectionMgr >* pConnService_;

    NEK::NEK_Address_ABC* pServerAddress_;

    MOS_AgentServer* pAgentServer_;

    MT_ObjectRecycler< MOS_AgentServer > agentServerRecycler_;
};

#   include "MOS_AgentServerConnectionMgr.inl"

#endif // __MOS_AgentServerConnectionMgr_h_
