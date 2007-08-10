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
#include "DIN/DIN_Engine.h"

class NET_AS_MOSServerConnectionMgr;
class NET_AS_MOSServerMsgMgr;
class NET_Publisher_ABC;
class MIL_Config;
class MIL_Time_ABC;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class NET_AgentServer : public tools::thread::Thread
{
    MT_COPYNOTALLOWED( NET_AgentServer );

public:
             NET_AgentServer( const MIL_Config& config, const MIL_Time_ABC& time );
    virtual ~NET_AgentServer();

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
    bool                          IsThreaded   () const;
    DIN::DIN_Engine&              GetDINEngine ();
    uint16                        GetPortAS_MOS() const;

    NET_AS_MOSServerConnectionMgr& GetConnectionMgr () const;
    NET_AS_MOSServerMsgMgr&        GetConcreteMessageMgr() const;
    NET_Publisher_ABC&             GetMessageMgr() const;

    bool                           MustInitUnitVisionCones   () const;
    bool                           MustSendUnitVisionCones   () const;
    void                           SetMustSendUnitVisionCones( bool bEnable );
    MT_CriticalSection&            GetDINEngineCriticalSection();
    //@}

private:
    //! @name Thread Main Loop
    //@{
    virtual void Run();
    virtual void OnUnexpected( Win32Exception& e );
    virtual void OnUnexpected( std::exception& e );
    virtual void OnUnexpected();
    //@}

    //! @name Operations
    //@{
    void Start();
    void Stop ();
    //@}

private:
    const MIL_Time_ABC&             time_;
    DIN::DIN_Engine                 dinEngine_;
    MT_CriticalSection              dinEngineCriticalSection_;
    bool                            bThreaded_;
    uint16                          nPortAS_MOS_;   // le port final (port de base auquel + exerciceID)
    bool                            bTerminated_;

    // Managers
    NET_AS_MOSServerMsgMgr*         pMsgMgr_;
    NET_AS_MOSServerConnectionMgr*  pConnectionMgr_;

    // Debug network properties
    uint                            nUnitVisionConesChangeTimeStep_;
    bool                            bSendUnitVisionCones_;
};

#   include "NET_AgentServer.inl"

#endif // __NET_AgentServer_h_
