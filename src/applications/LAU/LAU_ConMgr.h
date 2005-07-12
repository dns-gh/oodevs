//*****************************************************************************
//
// Name     : LAU_ConMgr.h
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 10 $
// $Workfile: LAU_ConMgr.h $
//
//*****************************************************************************

#ifndef __LAU_ConMgr_h_
#define __LAU_ConMgr_h_

#include "LAU.h"

#include "LAU_SrvMgr_ABC.h"
#include "LAU_MOS.h"
#include "LAU_SIM.h"

namespace DIN
{
    class DIN_Server;
    class DIN_ErrorDescription;
} 

namespace NEK
{
    class NEK_Address_ABC;
} 


class LAU_NetMgr;

//*****************************************************************************
// Object : PUT THE COMMENTS ON THE CLASS HERE
// Created: AML 03-03-26
//*****************************************************************************
class LAU_ConMgr : public LAU_SrvMgr_ABC
{
    MT_COPYNOTALLOWED( LAU_ConMgr )

public:
    explicit LAU_ConMgr( LAU_NetMgr& netMgr );
    virtual ~LAU_ConMgr();
    
    void Reset();

    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    bool StartServer();
    bool StopServer ();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Connection service callbacks */
    //-------------------------------------------------------------------------
    //@{
    void OnConnectionReceivedMos    ( DIN::DIN_Server& server, DIN::DIN_Link& link );
    void OnConnectionReceivedSim    ( DIN::DIN_Server& server, DIN::DIN_Link& link );
    void OnBadConnectionReceived    ( DIN::DIN_Server& server, const NEK::NEK_Address_ABC& address, const DIN::DIN_ErrorDescription& reason );
    void OnConnectionLost           ( DIN::DIN_Server& server, DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

public:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{

private:
    LAU_MOS&            CreateMOS                       ( DIN::DIN_Link& link );
    void                DestroyMOS                      ( DIN::DIN_Link& link );
    LAU_MOS*            FindLinkedMOS                   ( DIN::DIN_Link& link );

    bool32              IsLinkToMOS                     ( DIN::DIN_Link& link );

public:
    LAU_SIM&            AssociateSIM_WithLink           ( LAU_SIM& sim, DIN::DIN_Link& link );

private:
    void                DestroySIM                      ( DIN::DIN_Link& link );
    LAU_SIM*            FindLinkedSIM                   ( DIN::DIN_Link& link );

    bool32              IsLinkToSIM                     ( DIN::DIN_Link& link );
    bool32              IsSIM_Connected                 ( LAU_SIM& simToFind ); // cette sim est t'elle connecté?


    //@}


private:


    DIN::DIN_ConnectionServiceServerUserCbk< LAU_ConMgr >* pConnServiceMos_; // (owned)
    DIN::DIN_ConnectionServiceServerUserCbk< LAU_ConMgr >* pConnServiceSim_; // (owned)

    // Clients Mos
    T_ID_MOS_PtrMap mapMos_;   // (owned)    

    // Clients Sim
    T_ID_SIM_PtrMap mapSim_;   // (not owned)

};

#ifdef USE_INLINE
#   include "LAU_ConMgr.inl"
#endif 

#endif // __LAU_ConMgr_h_
