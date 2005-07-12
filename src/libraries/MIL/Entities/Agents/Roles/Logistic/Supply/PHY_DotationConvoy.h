// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_DotationConvoy.h $
// $Author: Jvt $
// $Modtime: 11/04/05 17:23 $
// $Revision: 2 $
// $Workfile: PHY_DotationConvoy.h $
//
// *****************************************************************************

#ifndef __PHY_DotationConvoy_h_
#define __PHY_DotationConvoy_h_

#include "MIL.h"

#include "PHY_Convoy_ABC.h"

class PHY_SupplyDotationConsign;

// =============================================================================
// @class  PHY_DotationConvoy
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationConvoy : public PHY_Convoy_ABC
{
    MT_COPYNOTALLOWED( PHY_DotationConvoy )

public:
    //! @name 
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();
    //@}

public:
     PHY_DotationConvoy( PHY_SupplyDotationConsign& consign );
     PHY_DotationConvoy();
    ~PHY_DotationConvoy();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint ); 
    //@}

    //! @name Operations
    //@{
    uint GetLoadingTime  () const;
    uint GetUnloadingTime() const;
    uint GetTravelTime   () const;
    //@}
};

#include "PHY_DotationConvoy.inl"

#endif // __PHY_DotationConvoy_h_
