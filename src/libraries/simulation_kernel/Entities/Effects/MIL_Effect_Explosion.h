// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_Explosion.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 4 $
// $Workfile: MIL_Effect_Explosion.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_Explosion_h_
#define __MIL_Effect_Explosion_h_

#include "MIL_Effect_Fire_ABC.h"

// =============================================================================
// @class  MIL_Effect_Explosion
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_Explosion : public MIL_Effect_Fire_ABC
{
public:
             MIL_Effect_Explosion( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory, double rLifeDuration, bool neutralization, unsigned int dotation );
    virtual ~MIL_Effect_Explosion();

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    bool neutralization_;
    bool started_;

    const unsigned int nLifeLastTimeStep_;
    const unsigned int dotation_;
};

#endif // __MIL_Effect_Explosion_h_
