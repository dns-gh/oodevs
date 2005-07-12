// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectMineFire.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 3 $
// $Workfile: PHY_DotationCategory_IndirectMineFire.h $
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_IndirectMineFire_h_
#define __PHY_DotationCategory_IndirectMineFire_h_

#include "MIL.h"

#include "PHY_DotationCategory_IndirectFire_ABC.h"

// =============================================================================
// @class  PHY_DotationCategory_IndirectMineFire
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory_IndirectMineFire : public PHY_DotationCategory_IndirectFire_ABC
{
    MT_COPYNOTALLOWED( PHY_DotationCategory_IndirectMineFire )

public:
    PHY_DotationCategory_IndirectMineFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive );
    virtual ~PHY_DotationCategory_IndirectMineFire();

    //! @name 
    //@{
    static PHY_DotationCategory_IndirectFire_ABC& Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive );
    //@}

    //! @name Operations
    //@{
    virtual void Fire( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    //@}

private:
    uint nNbrObjects_;
};

#include "PHY_DotationCategory_IndirectMineFire.inl"

#endif // __PHY_DotationCategory_IndirectMineFire_h_
