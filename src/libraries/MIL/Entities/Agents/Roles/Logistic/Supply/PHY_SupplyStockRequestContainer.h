// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequestContainer.h $
// $Author: Nld $
// $Modtime: 4/02/05 16:02 $
// $Revision: 1 $
// $Workfile: PHY_SupplyStockRequestContainer.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyStockRequestContainer_h_
#define __PHY_SupplyStockRequestContainer_h_

#include "MIL.h"

#include "PHY_SupplyStockRequest.h"

class PHY_DotationCategory;
class PHY_DotationStock;
class PHY_SupplyStockRequest;
class PHY_SupplyStockState;
class MIL_AutomateLOG;

struct ASN1T__SeqOfDotationStock;

// =============================================================================
// @class  PHY_SupplyStockRequestContainer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyStockRequestContainer
{
    MT_COPYNOTALLOWED( PHY_SupplyStockRequestContainer )

public:
     PHY_SupplyStockRequestContainer( MIL_AutomateLOG& suppliedAutomate );
     PHY_SupplyStockRequestContainer( MIL_AutomateLOG& suppliedAutomate, ASN1T__SeqOfDotationStock& asnStocks );
    ~PHY_SupplyStockRequestContainer();

    //! @name Operations
    //@{
    bool Execute ( MIL_AutomateLOG& supplyAutomate, PHY_SupplyStockState*& pStockSupplyState );
    void AddStock( PHY_DotationStock& stock );
    //@}   

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_SupplyStockRequest > T_RequestMap;
    typedef T_RequestMap::iterator                                          IT_RequestMap;
    //@}

private:
    //! @name Tools
    //@{
    void AffectRequestsToAutomate( MIL_AutomateLOG& supplyingAutomate );
    bool ApplyQuotas             ();
    void ActivateSupply          ( PHY_SupplyStockState*& pStockSupplyState );
    //@}

private:
    const bool             bPushedFlow_;
          MIL_AutomateLOG& suppliedAutomate_;
          T_RequestMap     requests_;
          bool             bAtLeastOneExplicitSupplySatisfied_;
          bool             bAtLeastOneSupplySatisfied_;
          bool             bExplicitSupplyFullSatisfied_;   
};

#include "PHY_SupplyStockRequestContainer.inl"

#endif // __PHY_SupplyStockRequestContainer_h_
