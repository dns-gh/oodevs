//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServerMgr_ABC.h $
// $Author: Nld $
// $Modtime: 16/06/04 10:13 $
// $Revision: 1 $
// $Workfile: NET_AS_MOSServerMgr_ABC.h $
//
//*****************************************************************************

#ifndef __NET_AS_MOSServerMgr_ABC_h_
#define __NET_AS_MOSServerMgr_ABC_h_

#include "MIL.h"

class NET_AS_MOSServer;
class NET_AgentServer;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class NET_AS_MOSServerMgr_ABC
{
    MT_COPYNOTALLOWED( NET_AS_MOSServerMgr_ABC );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< DIN::DIN_StationID, NET_AS_MOSServer* > T_MosConnectionMap;
    typedef T_MosConnectionMap::iterator                      IT_MosConnectionMap;
    typedef T_MosConnectionMap::const_iterator                CIT_MosConnectionMap;
    //@}

public:
    NET_AS_MOSServerMgr_ABC( NET_AgentServer& agentServer );
    virtual ~NET_AS_MOSServerMgr_ABC();

protected:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    NET_AS_MOSServer* GetMosConnectionFromLink( DIN::DIN_Link& link );
    //@}

protected:
    NET_AgentServer& agentServer_;
};

#   include "NET_AS_MOSServerMgr_ABC.inl"
#endif // __NET_AS_MOSServerMgr_ABC_h_
