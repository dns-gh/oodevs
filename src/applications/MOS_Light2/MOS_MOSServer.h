//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MOSServer.h $
// $Author: Ape $
// $Modtime: 6/09/04 16:03 $
// $Revision: 3 $
// $Workfile: MOS_MOSServer.h $
//
//*****************************************************************************

#ifndef __MOS_MOSServer_h_
#define __MOS_MOSServer_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_Config.h"

class MOS_AgentServerController;

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class MOS_MOSServer
{
    MT_COPYNOTALLOWED( MOS_MOSServer );

public:
     MOS_MOSServer();
    ~MOS_MOSServer();

    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Connect   ( const std::string& strHostName, uint16 nPort );
    void Disconnect();
    void Update    ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    DIN::DIN_Engine&            GetDINEngine ();
    MOS_AgentServerController&  GetController() const;
    //@}

private:
    DIN::DIN_Engine dinEngine_;

    MOS_AgentServerController* pASController_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_MOSServer.inl"
#endif

#endif // __MOS_MOSServer_h_
