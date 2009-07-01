// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationRequestContainer.h $
// $Author: Nld $
// $Modtime: 3/03/05 16:04 $
// $Revision: 3 $
// $Workfile: PHY_SupplyDotationRequestContainer.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyDotationRequestContainer_h_
#define __PHY_SupplyDotationRequestContainer_h_

#include "PHY_SupplyDotationRequest.h"
#include "Entities/Automates/MIL_Automate.h"

class PHY_DotationCategory;
class PHY_Dotation;
class PHY_SupplyDotationRequest;
class PHY_SupplyDotationState;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_SupplyDotationRequestContainer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyDotationRequestContainer
{
    MT_COPYNOTALLOWED( PHY_SupplyDotationRequestContainer )

public:
     PHY_SupplyDotationRequestContainer( MIL_Automate& suppliedAutomate, bool bForceRequestActivation );
    ~PHY_SupplyDotationRequestContainer();

    //! @name Operations
    //@{
    bool Execute    ( MIL_AutomateLOG* pFirstSupplyingAutomate, MIL_AutomateLOG* pSecondSupplyingAutomate, MIL_Automate::T_SupplyDotationStateMap& dotationSupplies );
    void AddDotation( PHY_Dotation& dotation );
    //@}   

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_SupplyDotationRequest > T_RequestMap;
    typedef T_RequestMap::iterator                                             IT_RequestMap;
    //@}

private:
    //! @name Tools
    //@{
    void AffectRequestsToAutomate( MIL_AutomateLOG& supplyingAutomate );
    bool IsEnoughSatisfied       () const;
    bool IsFullSatisfied         () const;
    void ActivateSupply          ( MIL_Automate::T_SupplyDotationStateMap& dotationSupplies );
    //@}

private:
    MIL_Automate& suppliedAutomate_;
    T_RequestMap  requests_;
    bool          bForceRequestActivation_;
    bool          bAtLeastOneExplicitSupplySatisfied_;
    bool          bAtLeastOneSupplySatisfied_;
    bool          bExplicitSupplyFullSatisfied_;   
};

#endif // __PHY_SupplyDotationRequestContainer_h_
