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
#include "DIN/DIN_UserData_ABC.h"
namespace DIN {
    class DIN_Link;
}

//=============================================================================
// Created: NLD 2002-06-28
//=============================================================================
class NET_AS_MOSServer : public DIN::DIN_UserData_ABC
{
    MT_COPYNOTALLOWED( NET_AS_MOSServer );

public:
    explicit NET_AS_MOSServer( DIN::DIN_Link& link );
    virtual ~NET_AS_MOSServer();

    //! @name Accessors
    //@{
    DIN::DIN_Link&     GetLink     () const;
    DIN::DIN_StationID GetStationID() const;
    //@}

private:
    DIN::DIN_Link& link_;
};

#endif // __NET_AS_MOSServer_h_
