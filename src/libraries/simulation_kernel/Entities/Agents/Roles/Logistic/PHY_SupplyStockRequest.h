// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyStockRequest.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:23 $
// $Revision: 3 $
// $Workfile: PHY_SupplyStockRequest.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyStockRequest_h_
#define __PHY_SupplyStockRequest_h_

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
    virtual ~PHY_SupplyStockRequest();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void     ApplyQuota( double rQuota );
    void     AddStock  ( PHY_DotationStock& stock );
    void     AddStock  ( PHY_DotationStock& stock, double rValue );
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_SupplyStockRequest )

// -----------------------------------------------------------------------------
// Name: PHY_SupplyStockRequest::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyStockRequest::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_SupplyRequest_ABC< PHY_DotationStock > >( *this );
}

#endif // __PHY_SupplyStockRequest_h_
