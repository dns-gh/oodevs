// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnlyCarrier.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionOnlyCarrier.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFirePionOnlyCarrier_h_
#define __PHY_ActionDirectFirePionOnlyCarrier_h_

#include "MIL.h"

#include "PHY_ActionDirectFirePion_ABC.h"

// =============================================================================
// @class  PHY_ActionDirectFirePionOnlyCarrier
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePionOnlyCarrier : public PHY_ActionDirectFirePion_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDirectFirePionOnlyCarrier )

public:
    PHY_ActionDirectFirePionOnlyCarrier( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionDirectFirePionOnlyCarrier();
};
#endif // __PHY_ActionDirectFirePionOnlyCarrier_h_
