// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePion.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFirePion_h_
#define __PHY_ActionDirectFirePion_h_

#include "MIL.h"

#include "PHY_ActionDirectFirePion_ABC.h"

class PHY_DirectFireResult;

// =============================================================================
// @class  PHY_ActionDirectFirePion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePion : public PHY_ActionDirectFirePion_ABC
{

public:
             PHY_ActionDirectFirePion( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDirectFirePion();
};

#endif // __PHY_ActionDirectFirePion_h_
