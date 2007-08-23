// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCapacity.h $
// $Author: Nld $
// $Modtime: 28/01/05 15:58 $
// $Revision: 3 $
// $Workfile: PHY_DotationCapacity.h $
//
// *****************************************************************************

#ifndef __PHY_DotationCapacity_h_
#define __PHY_DotationCapacity_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;

// =============================================================================
// @class  PHY_DotationCapacity
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCapacity
{
    MT_COPYNOTALLOWED( PHY_DotationCapacity )

public:
     PHY_DotationCapacity( const PHY_DotationCategory& category, xml::xistream& xis );
    ~PHY_DotationCapacity();

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetCategory       () const;
          MT_Float              GetCapacity       () const;
          MT_Float              GetSupplyThreshold() const;
    //@}

private:
    const PHY_DotationCategory& category_;
    MT_Float                    rCapacity_;
    MT_Float                    rSupplyThreshold_;
};

#include "PHY_DotationCapacity.inl"

#endif // __PHY_DotationCapacity_h_
