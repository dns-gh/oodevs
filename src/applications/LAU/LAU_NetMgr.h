//*****************************************************************************
//
// Name     : LAU_NetMgr.h
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 11 $
// $Workfile: LAU_NetMgr.h $
//
//*****************************************************************************

#ifndef __LAU_NetMgr_h_
#define __LAU_NetMgr_h_

#include "LAU.h"

namespace DIN
{
    class DIN_Link;
    class DIN_ConnectionProtocols;
} 

class LAU_ConMgr;
class LAU_MsgMgr;
class LAU_Workspace;


//*****************************************************************************
// Object : PUT THE COMMENTS ON THE CLASS HERE
// Created: AML 03-03-26
//*****************************************************************************
class LAU_NetMgr
{
    MT_COPYNOTALLOWED( LAU_NetMgr )

public:
    explicit LAU_NetMgr( LAU_Workspace& workspace, MT_InputArchive_ABC& archive );
    virtual ~LAU_NetMgr();
    
    //-----------------------------------------------------------------------------
    /** @name Main methods */
    //-----------------------------------------------------------------------------
    //@{
    void Update();
    //@}
    
    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{

    const DIN::DIN_Engine&              GetDIN_Engine() const;
    DIN::DIN_Engine&                    GetDIN_Engine();

    uint16                              GetPortMos() const;
    uint16                              GetPortSim() const;
    uint                                GetMagic() const;
    DIN::DIN_ConnectionProtocols&       GetConnectionProtocols();

    LAU_ConMgr&                         GetConMgr() const;
    LAU_MsgMgr&                         GetMsgMgr() const;

    const LAU_Workspace& GetWorkspace() const;
          LAU_Workspace& GetWorkspace();


    //@}

private:

    DIN::DIN_Engine                 dinEngine_;

    LAU_Workspace&                  workspace_; // (not owned)

    // Managers
    LAU_ConMgr*                     pConMgr_;   // (owned)
    LAU_MsgMgr*                     pMsgMgr_;   // (owned)

    uint16                          nPortMos_;
    uint16                          nPortSim_;
    uint                            nMagic_;


    DIN::DIN_ConnectionProtocols    connectionProtocols_;
};

#ifdef USE_INLINE
#   include "LAU_NetMgr.inl"
#endif 

#endif // __LAU_NetMgr_h_
