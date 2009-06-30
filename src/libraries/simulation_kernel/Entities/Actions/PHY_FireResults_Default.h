// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_FireResults_Default.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_FireResults_Default.h $
//
// *****************************************************************************

#ifndef __PHY_FireResults_Default_h_
#define __PHY_FireResults_Default_h_

#include "PHY_FireResults_ABC.h"

// =============================================================================
// @class  PHY_FireResults_Default
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireResults_Default : public PHY_FireResults_ABC
{
    MT_COPYNOTALLOWED( PHY_FireResults_Default )

public:
             PHY_FireResults_Default();
    virtual ~PHY_FireResults_Default();
};

#endif // __PHY_FireResults_Default_h_
