//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Config.h $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: MOS_Config.h $
//
//*****************************************************************************

#ifndef __MOS_Config_h_
#define __MOS_Config_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

#include "DIN/ConnectionService/DIN_ConnectionProtocols.h"

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class MOS_Config
{
    MT_COPYNOTALLOWED( MOS_Config );

public:
     MOS_Config();
    ~MOS_Config();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    uint16                        GetPortAgentServerMOSServer  () const;
    DIN::DIN_ConnectionProtocols& GetConnectionProtocols();
    //@}

private:
    uint16 nPortAgentServerMOSServer_;

    DIN::DIN_ConnectionProtocols connectionProtocols_;
};

#   include "MOS_Config.inl"

#endif // __MOS_Config_h_
