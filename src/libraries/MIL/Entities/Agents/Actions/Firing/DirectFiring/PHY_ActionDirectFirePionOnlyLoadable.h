// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnlyLoadable.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionOnlyLoadable.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFirePionOnlyLoadable_h_
#define __PHY_ActionDirectFirePionOnlyLoadable_h_

#include "MIL.h"

#include "PHY_ActionDirectFirePion_ABC.h"

// =============================================================================
// @class  PHY_ActionDirectFirePionOnlyLoadable
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePionOnlyLoadable : public PHY_ActionDirectFirePion_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDirectFirePionOnlyLoadable )

public:
    PHY_ActionDirectFirePionOnlyLoadable( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDirectFirePionOnlyLoadable();
};

#endif // __PHY_ActionDirectFirePionOnlyLoadable_h_
