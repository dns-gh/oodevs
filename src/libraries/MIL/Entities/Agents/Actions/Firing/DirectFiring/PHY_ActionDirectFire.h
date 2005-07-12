// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFire.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFire.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFire_h_
#define __PHY_ActionDirectFire_h_

#include "MIL.h"

#include "PHY_ActionDirectFire_ABC.h"

class PHY_DirectFireResult;

// =============================================================================
// @class  PHY_ActionDirectFire
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFire : public PHY_ActionDirectFire_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDirectFire )

public:
    PHY_ActionDirectFire( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDirectFire();
};

#include "PHY_ActionDirectFire.inl"

#endif // __PHY_ActionDirectFire_h_
