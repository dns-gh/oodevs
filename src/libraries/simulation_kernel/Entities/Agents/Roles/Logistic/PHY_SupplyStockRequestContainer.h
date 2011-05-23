// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/PHY_SupplyStockRequestContainer.h $
// $Author: Nld $
// $Modtime: 4/02/05 16:02 $
// $Revision: 1 $
// $Workfile: PHY_SupplyStockRequestContainer.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyStockRequestContainer_h_
#define __PHY_SupplyStockRequestContainer_h_

#include "PHY_SupplyStockRequest.h"

namespace sword
{
    class MissionParameter;
}

namespace logistic 
{
    class LogisticHierarchy_ABC;
}

class PHY_DotationCategory;
class PHY_DotationStock;
class PHY_SupplyStockRequest;
class PHY_SupplyStockState;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_SupplyStockRequestContainer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyStockRequestContainer : private boost::noncopyable
{
public:
    //! @name Operations
    //@{
    enum E_RequestDirection
    {
        eUpward,
        eDownward
    };
    //@}

public:
    explicit PHY_SupplyStockRequestContainer( MIL_Automate& suppliedAutomate );
             PHY_SupplyStockRequestContainer( MIL_Automate& suppliedAutomate, const sword::MissionParameter& asnStocks, E_RequestDirection  requestDirection );
    virtual  ~PHY_SupplyStockRequestContainer();

    //! @name Accessors
    //@{
    bool HasRequests () const;
    //@}

    //! @name Operations
    //@{
    bool Execute ( MIL_AutomateLOG& supplier, PHY_SupplyStockState*& pStockSupplyState );
    bool Execute ( const logistic::LogisticHierarchy_ABC& logisticHierarchy, PHY_SupplyStockState*& pStockSupplyState );   
    void AddStock( PHY_DotationStock& stock );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_SupplyStockRequest > T_RequestMap;
    typedef T_RequestMap::iterator                                          IT_RequestMap;
    typedef std::map< PHY_SupplyStockRequest* , MIL_AutomateLOG* >          T_RequestAffectationMap;
    typedef T_RequestAffectationMap::iterator                               IT_RequestAffectationMap;
    //@}

private:
    //! @name Tools
    //@{
    void AffectRequestsToAutomate( );
    bool ApplyQuotas             ( MIL_AutomateLOG& supplier );
    bool ApplyQuotas             ( const logistic::LogisticHierarchy_ABC& logisticHierarchy );
    void ActivateSupply          ( PHY_SupplyStockState*& pStockSupplyState );
    //@}

private:
    const bool               bManual_;
    const E_RequestDirection requestDirection_;
          MIL_Automate&      suppliedAutomate_;
          T_RequestMap       requests_;
          bool               bAtLeastOneExplicitSupplySatisfied_;
          bool               bAtLeastOneSupplySatisfied_;
          bool               bExplicitSupplyFullSatisfied_;
          T_RequestAffectationMap requestAffectations_;
};

#endif // __PHY_SupplyStockRequestContainer_h_
