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

#include "Entities/Actions/PHY_FireResults_ABC.h"

class MIL_Object_ABC;

// =============================================================================
// @class  PHY_ObjectExplosionFireResult
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ObjectExplosionFireResult : public PHY_FireResults_ABC
{
public:
    explicit PHY_ObjectExplosionFireResult( const MIL_Object_ABC& object );
    virtual ~PHY_ObjectExplosionFireResult();

    virtual void Hit( unsigned int hits );
    unsigned int GetHits() const;
    virtual const std::string& GetSourceName() const;

private:
    const MIL_Object_ABC& object_;
    unsigned int  hits_;
};

#endif // __PHY_ObjectExplosionFireResult_h_
