// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequest.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:23 $
// $Revision: 3 $
// $Workfile: PHY_SupplyStockRequest.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyStockRequest_h_
#define __PHY_SupplyStockRequest_h_

#include "MIL.h"

#include "PHY_SupplyRequest_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"

// =============================================================================
// @class  PHY_SupplyStockRequest
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyStockRequest : public PHY_SupplyRequest_ABC< PHY_DotationStock >
{
public:
     PHY_SupplyStockRequest();
    ~PHY_SupplyStockRequest();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void     ApplyQuota               ( MT_Float rQuota );
    void     AddStock                 ( PHY_DotationStock& stock );
    void     AddStock                 ( PHY_DotationStock& stock, MT_Float rValue );
    //@}
};

#include "PHY_SupplyStockRequest.inl"

#endif // __PHY_SupplyStockRequest_h_
