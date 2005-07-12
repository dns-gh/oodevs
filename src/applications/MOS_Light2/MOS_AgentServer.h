//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentServer.h $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: MOS_AgentServer.h $
//
//*****************************************************************************

#ifndef __MOS_AgentServer_h_
#define __MOS_AgentServer_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

//=============================================================================
// Created: NLD 2002-06-28
//=============================================================================
class MOS_AgentServer : public DIN::DIN_UserData_ABC
{
    friend class MOS_AgentServerConnectionMgr; // For Initialize() / Terminate()

    MT_COPYNOTALLOWED( MOS_AgentServer );

public:
     MOS_AgentServer();
    ~MOS_AgentServer();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    DIN::DIN_Link&  GetSession  () const;
    DIN::DIN_StationID GetStationID() const;
    //@}

private:    
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Initialize( DIN::DIN_Link& link );
    void Terminate ();
    //@}

private:
    DIN::DIN_Link* pSession_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_AgentServer.inl"
#endif

#endif // __MOS_AgentServer_h_
