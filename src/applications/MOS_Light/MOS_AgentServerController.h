//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerController.h $
// $Author: Nld $
// $Modtime: 2/03/03 2:40 $
// $Revision: 5 $
// $Workfile: MOS_AgentServerController.h $
//
//*****************************************************************************

#ifndef __MOS_AgentServerController_h_
#define __MOS_AgentServerController_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

#include "MOS_Config.h"

class MOS_MOSServer;
class MOS_Config;
class MOS_AgentServerConnectionMgr;
class MOS_AgentServerMsgMgr;


//=============================================================================
// Created: NLD 2002-06-28
//=============================================================================
class MOS_AgentServerController
{
    MT_COPYNOTALLOWED( MOS_AgentServerController );

public:
    MOS_AgentServerController( MOS_MOSServer& mosServer );
    virtual ~MOS_AgentServerController();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    DIN::DIN_Engine&               GetDINEngine     ();
    MOS_MOSServer&                 GetMOSServer     () const;
    MOS_AgentServerConnectionMgr&  GetConnectionMgr () const;
    MOS_AgentServerMsgMgr&         GetMessageMgr    () const;
    //@}


private:
    MOS_MOSServer&          mosServer_;

    // Managers
    MOS_AgentServerConnectionMgr* pConnectionMgr_;
    MOS_AgentServerMsgMgr*        pMsgMgr_;
   
};

#ifdef MOS_USE_INLINE
#   include "MOS_AgentServerController.inl"
#endif

#endif // __MOS_AgentServerController_h_
