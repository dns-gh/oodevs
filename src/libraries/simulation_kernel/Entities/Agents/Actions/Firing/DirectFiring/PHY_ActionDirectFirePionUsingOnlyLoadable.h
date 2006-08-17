// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyLoadable.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionUsingOnlyLoadable.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFirePionUsingOnlyLoadable_h_
#define __PHY_ActionDirectFirePionUsingOnlyLoadable_h_

#include "MIL.h"

#include "PHY_ActionDirectFirePion_ABC.h"

// =============================================================================
// @class  PHY_ActionDirectFirePionUsingOnlyLoadable
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePionUsingOnlyLoadable : public PHY_ActionDirectFirePion_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDirectFirePionUsingOnlyLoadable )

public:
    PHY_ActionDirectFirePionUsingOnlyLoadable( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDirectFirePionUsingOnlyLoadable();
};

#endif // __PHY_ActionDirectFirePionUsingOnlyLoadable_h_
