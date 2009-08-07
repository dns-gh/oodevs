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

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_DirectFiring;
class PHY_RolePion_Perceiver;
class PHY_FireResults_Default;
class MIL_AgentPion;
class MIL_Object_ABC;
class TER_Localisation;

// =============================================================================
// @class  PHY_ActionControlZone
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionControlZone : public PHY_Action_ABC
{

public:
    typedef MIL_AgentPion ActorType;        

public:
             PHY_ActionControlZone( MIL_AgentPion& pion, const TER_Localisation* pLocalisation, MT_Float rRadius, bool bFire );
    virtual ~PHY_ActionControlZone();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RolePion_Perceiver&      rolePerceiver_;
    PHY_RoleAction_DirectFiring& roleDirectFiring_;
    MIL_Object_ABC*              pZoneControlled_;
    PHY_FireResults_Default*     pFireResult_;
    int                          perceptionZoneID_;
};

#endif // __PHY_ActionControlZone_h_
