// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_Fire_ABC.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 4 $
// $Workfile: MIL_Effect_Fire_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_Fire_ABC_h_
#define __MIL_Effect_Fire_ABC_h_

#include "MIL_Effect_ABC.h"
#include "MT_Tools/MT_Ellipse.h"
#include "Tools/MIL_IDManager.h"

namespace sword
{
    enum StartFireEffect_EnumFireEffectType;
}

class PHY_IndirectFireDotationClass;

// =============================================================================
// @class  MIL_Effect_Fire_ABC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_Fire_ABC : public MIL_Effect_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Effect_Fire_ABC( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory );
    virtual ~MIL_Effect_Fire_ABC();
    //@}

public:
    //! @name Accessors
    //@{
    unsigned int GetFireEffectId() const;
    const MT_Ellipse& GetSurface() const;
    //@}

protected:
    //! @name Operations
    //@{
    void SendMsgStartEffect( sword::StartFireEffect_EnumFireEffectType fireEffectType ) const;
    void SendMsgStopEffect() const;
    //@}

protected:
    //! @name Member data
    //@{
    const unsigned int                   nID_;
    const MT_Ellipse                     surface_;
    const PHY_IndirectFireDotationClass& ammoCategory_;
    static MIL_IDManager idManager_;
    //@}
};

#endif // __MIL_Effect_Fire_ABC_h_
