// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h $
// $Author: Jvt $
// $Modtime: 2/05/05 14:37 $
// $Revision: 5 $
// $Workfile: PHY_DotationCategory_IndirectFire_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_IndirectFire_ABC_h_
#define __PHY_DotationCategory_IndirectFire_ABC_h_

#include "MIL.h"

class PHY_DotationCategory;
class PHY_IndirectFireDotationClass;
class PHY_FireResults_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_DotationCategory_IndirectFire_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory_IndirectFire_ABC
{
    MT_COPYNOTALLOWED( PHY_DotationCategory_IndirectFire_ABC )

public:
    PHY_DotationCategory_IndirectFire_ABC( const PHY_IndirectFireDotationClass& category, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive );
    virtual ~PHY_DotationCategory_IndirectFire_ABC();

    //! @name Accessors
    //@{
    const PHY_IndirectFireDotationClass& GetIndirectFireDotationCategory() const;
    //@}

    //! @name Operations
    //@{
            MT_Float ConvertToInterventionType( uint nNbr ) const;
            uint     ConvertToNbrAmmo         ( MT_Float rNbrIT ) const;
    virtual void     Fire                     ( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const = 0;
    virtual void     Fire                     ( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired ) const;
    //@}

protected:
    const PHY_IndirectFireDotationClass& category_;
    const PHY_DotationCategory&          dotationCategory_;
          uint                           nInterventionType_;
          MT_Float                       rDispersionX_;
          MT_Float                       rDispersionY_;
};

#include "PHY_DotationCategory_IndirectFire_ABC.inl"

#endif // __PHY_DotationCategory_IndirectFire_ABC_h_
