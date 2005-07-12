// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFireOnlyCarrier.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFireOnlyCarrier.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFireOnlyCarrier_h_
#define __PHY_ActionDirectFireOnlyCarrier_h_

#include "MIL.h"

#include "PHY_ActionDirectFire_ABC.h"

// =============================================================================
// @class  PHY_ActionDirectFireOnlyCarrier
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFireOnlyCarrier : public PHY_ActionDirectFire_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDirectFireOnlyCarrier )

public:
    PHY_ActionDirectFireOnlyCarrier( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDirectFireOnlyCarrier();
};

#include "PHY_ActionDirectFireOnlyCarrier.inl"

#endif // __PHY_ActionDirectFireOnlyCarrier_h_
