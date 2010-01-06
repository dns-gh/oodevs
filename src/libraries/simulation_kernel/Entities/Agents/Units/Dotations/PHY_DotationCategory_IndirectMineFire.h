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

#include "PHY_DotationCategory_IndirectObjectCreationFire.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_DotationCategory_IndirectMineFire
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory_IndirectMineFire : public PHY_DotationCategory_IndirectObjectCreationFire
{

public:
             PHY_DotationCategory_IndirectMineFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis );
    virtual ~PHY_DotationCategory_IndirectMineFire();

    //! @name 
    //@{
    static PHY_DotationCategory_IndirectFire_ABC& Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis );
    //@}

private:
    uint nNbrObjects_;
};

#endif // __PHY_DotationCategory_IndirectMineFire_h_
