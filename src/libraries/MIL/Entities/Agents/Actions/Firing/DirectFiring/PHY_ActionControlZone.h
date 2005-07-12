// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionControlZone.h $
// $Author: Jvt $
// $Modtime: 28/10/04 14:56 $
// $Revision: 2 $
// $Workfile: PHY_ActionControlZone.h $
//
// *****************************************************************************

#ifndef __PHY_ActionControlZone_h_
#define __PHY_ActionControlZone_h_

#include "MIL.h"

#include "Entities/Agents/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_DirectFiring;
class MIL_ControlZone;
class PHY_ControlZoneFireResults;

// =============================================================================
// @class  PHY_ActionControlZone
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionControlZone : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionControlZone )

public:
    PHY_ActionControlZone( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionControlZone();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_DirectFiring& role_;
    MIL_ControlZone*             pZoneControlled_;
    PHY_ControlZoneFireResults*  pFireResult_;
    void*                        pPerceptionZoneID_;
};

#include "PHY_ActionControlZone.inl"

#endif // __PHY_ActionControlZone_h_
