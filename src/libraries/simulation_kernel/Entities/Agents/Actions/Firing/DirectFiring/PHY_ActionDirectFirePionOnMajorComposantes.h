// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnMajorComposantes.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionOnMajorComposantes.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFirePionOnMajorComposantes_h_
#define __PHY_ActionDirectFirePionOnMajorComposantes_h_

#include "MIL.h"

#include "PHY_ActionDirectFirePion_ABC.h"

// =============================================================================
// @class  PHY_ActionDirectFirePionOnMajorComposantes
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePionOnMajorComposantes : public PHY_ActionDirectFirePion_ABC
{

public:
             PHY_ActionDirectFirePionOnMajorComposantes( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDirectFirePionOnMajorComposantes();
};

#endif // __PHY_ActionDirectFirePionOnMajorComposantes_h_
