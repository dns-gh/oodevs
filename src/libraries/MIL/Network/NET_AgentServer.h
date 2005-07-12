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

#include "MIL.h"
#undef Yield // $$$$ AGE 2005-03-02: ....
#include "tools/thread/Thread.h"

class NET_AS_MOSServerConnectionMgr;
class NET_AS_MOSServerMsgMgr;
class NET_LAU_Mgr;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class NET_AgentServer : public tools::thread::Thread
{
    MT_COPYNOTALLOWED( NET_AgentServer );

public:
     NET_AgentServer( MIL_InputArchive& archive );
    ~NET_AgentServer();

    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Update();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    bool                          IsThreaded            () const;
    DIN::DIN_Engine&              GetDINEngine          ();
    uint16                        GetPortAS_MOS         () const;
    uint                          GetMagicAS_MOS        () const;
    DIN::DIN_ConnectionProtocols& GetConnectionProtocols();

    NET_AS_MOSServerConnectionMgr& GetConnectionMgr () const;
    NET_AS_MOSServerMsgMgr&        GetMessageMgr    () const;

    bool                           MustInitUnitVisionCones   () const;
    bool                           MustSendUnitVisionCones   () const;
    void                           SetMustSendUnitVisionCones( bool bEnable );


    MT_CriticalSection&            GetDINEngineCriticalSection();
    //@}

private:
    //! @name Thread Main Loop
    //@{
    virtual void Run();
    //@}

    //! @name Operations
    //@{
    void Start ();
    void Stop  ();
    //@}

private:

    DIN::DIN_Engine                 dinEngine_;
    MT_CriticalSection              dinEngineCriticalSection_;
    bool                            bThreaded_;
    bool                            bTerminated_;

    // Managers
    NET_AS_MOSServerConnectionMgr*  pConnectionMgr_;
    NET_AS_MOSServerMsgMgr*         pMsgMgr_;

    NET_LAU_Mgr*                    pLauMgr_;    // petit bout reseaux qui gere les discussions avec le Launcher

    uint16                          nPortAS_MOS_;   // le port final (port de base auquel + exerciceID)
    uint                            nMagicAS_MOS_;

    DIN::DIN_ConnectionProtocols    connectionProtocols_;

    // Debug network properties
    uint                            nUnitVisionConesChangeTimeStep_;
    bool                            bSendUnitVisionCones_;
};

#   include "NET_AgentServer.inl"

#endif // __NET_AgentServer_h_
