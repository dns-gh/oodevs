// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/PHY_ObjectExplosionFireResult.h $
// $Author: Jvt $
// $Modtime: 2/02/05 14:25 $
// $Revision: 2 $
// $Workfile: PHY_ObjectExplosionFireResult.h $
//
// *****************************************************************************

#ifndef __PHY_ObjectExplosionFireResult_h_
#define __PHY_ObjectExplosionFireResult_h_

#include "MIL.h"

#include "Tools/MIL_MOSIDManager.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h"

class MIL_RealObject_ABC;

// =============================================================================
// @class  PHY_ObjectExplosionFireResult
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ObjectExplosionFireResult : public PHY_FireResults_ABC
{
    MT_COPYNOTALLOWED( PHY_ObjectExplosionFireResult )

public:
    PHY_ObjectExplosionFireResult( const MIL_RealObject_ABC& object );
    virtual ~PHY_ObjectExplosionFireResult();

    //! @name Debug
    //@{
//    virtual void Dump() const;
    //@}

private:
    const MIL_RealObject_ABC& object_;
};

#include "PHY_ObjectExplosionFireResult.inl"

#endif // __PHY_ObjectExplosionFireResult_h_
