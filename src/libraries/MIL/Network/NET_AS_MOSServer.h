
//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServer.h $
// $Author: Nld $
// $Modtime: 16/06/04 10:13 $
// $Revision: 1 $
// $Workfile: NET_AS_MOSServer.h $
//
//*****************************************************************************

#ifndef __NET_AS_MOSServer_h_
#define __NET_AS_MOSServer_h_


#include "MIL.h"


//=============================================================================
// Created: NLD 2002-06-28
//=============================================================================
class NET_AS_MOSServer : public DIN::DIN_UserData_ABC
{
    friend class NET_AS_MOSServerConnectionMgr; // For Initialize() / Terminate()

    MT_COPYNOTALLOWED( NET_AS_MOSServer );

public:
    //! @name Types
    //@{
    enum E_ClientType
    {
        eMos      = 0,
        eMosLight = 1
    };
    //@}

public:
     NET_AS_MOSServer( DIN::DIN_Link& link, E_ClientType nClientType );
    ~NET_AS_MOSServer();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    DIN::DIN_Link&     GetLink      () const;
    DIN::DIN_StationID GetStationID () const;
    E_ClientType       GetClientType() const;
    //@}

private:
    DIN::DIN_Link& link_;
    E_ClientType   nClientType_;
};

#   include "NET_AS_MOSServer.inl"
#endif // __NET_AS_MOSServer_h_
