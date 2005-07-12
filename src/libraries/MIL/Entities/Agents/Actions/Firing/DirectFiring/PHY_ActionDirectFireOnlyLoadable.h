// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFireOnlyLoadable.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFireOnlyLoadable.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFireOnlyLoadable_h_
#define __PHY_ActionDirectFireOnlyLoadable_h_

#include "MIL.h"

#include "PHY_ActionDirectFire_ABC.h"

// =============================================================================
// @class  PHY_ActionDirectFireOnlyLoadable
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFireOnlyLoadable : public PHY_ActionDirectFire_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDirectFireOnlyLoadable )

public:
    PHY_ActionDirectFireOnlyLoadable( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDirectFireOnlyLoadable();
};

#include "PHY_ActionDirectFireOnlyLoadable.inl"

#endif // __PHY_ActionDirectFireOnlyLoadable_h_
