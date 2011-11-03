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

class NET_AS_MOSServerMsgMgr;
class NET_Publisher_ABC;
class MIL_Config;
class MIL_Time_ABC;
class NET_Simulation_ABC;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class NET_AgentServer : public tools::ServerNetworker
{
public:
             NET_AgentServer( const MIL_Config& config, const MIL_Time_ABC& time, NET_Simulation_ABC& simulation );
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
    bool                           MustInitUnitVisionCones   () const;
    bool                           MustSendUnitVisionCones   () const;
    void                           SetMustSendUnitVisionCones( bool bEnable );
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    virtual void ConnectionWarning  ( const std::string& address, const std::string& warning );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Time_ABC&             time_;
    NET_AS_MOSServerMsgMgr*         pMsgMgr_;
    unsigned int                            nUnitVisionConesChangeTimeStep_;
    bool                            bSendUnitVisionCones_;
    //@}
};

#endif // __NET_AgentServer_h_
