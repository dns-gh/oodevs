// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationConsign.h $
// $Author: Nld $
// $Modtime: 13/04/05 12:12 $
// $Revision: 5 $
// $Workfile: PHY_SupplyDotationConsign.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyDotationConsign_h_
#define __PHY_SupplyDotationConsign_h_

#include "MIL.h"

#include "PHY_SupplyConsign_ABC.h"
#include "PHY_DotationConvoy.h"

class PHY_SupplyDotationState;

// =============================================================================
// @class  PHY_SupplyDotationConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyDotationConsign : public PHY_SupplyConsign_ABC
{
    MT_COPYNOTALLOWED( PHY_SupplyDotationConsign )

public:
             PHY_SupplyDotationConsign( MIL_AutomateLOG& supplyingAutomate, PHY_SupplyDotationState& supplyState );
             PHY_SupplyDotationConsign();
    virtual ~PHY_SupplyDotationConsign();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual bool Update(); 
    //@}

    //! @name Accessors
    //@{
    virtual       MT_Float      GetUVolumeRequested() const;
    virtual const MIL_Automate* GetSuppliedAutomate() const;
    //@}

private:
    //! @name Operations
    //@{
    void EnterStateConvoyForming  ();
    bool DoConvoyForming          ();
    void EnterStateConvoyLoading  ();
    void EnterStateConvoyGoingTo  ();
    void EnterStateConvoyUnloading();
    void EnterStateConvoyGoingFrom();
    void EnterStateFinished       ();

    void DoSupply();
    //@}

private:
    PHY_SupplyDotationState* pSupplyState_;
    PHY_DotationConvoy*      pConvoy_;
};

#include "PHY_SupplyDotationConsign.inl"

#endif // __PHY_SupplyDotationConsign_h_
