// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockConsign.h $
// $Author: Nld $
// $Modtime: 13/04/05 12:10 $
// $Revision: 4 $
// $Workfile: PHY_SupplyStockConsign.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyStockConsign_h_
#define __PHY_SupplyStockConsign_h_

#include "PHY_SupplyConsign_ABC.h"
#include "PHY_StockConvoy.h"

class PHY_SupplyStockState;

// =============================================================================
// @class  PHY_SupplyStockConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyStockConsign : public PHY_SupplyConsign_ABC
{

public:
             PHY_SupplyStockConsign( MIL_AutomateLOG& supplyAutomate, PHY_SupplyStockState& supplyState );
             PHY_SupplyStockConsign();
    virtual ~PHY_SupplyStockConsign();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual bool Update(); 

    // Scripts events
    bool IsLoadingDone   () const;
    bool IsUnloadingDone () const;
    bool ConvoyLoad      ();
    bool ConvoyUnload    ();
    void ConvoyEndMission();

    virtual void GetMerchandiseToConvoy              ( T_MerchandiseToConvoyMap& container ) const;
    virtual void RemoveConvoyedMerchandise           ( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations );
    virtual void AddConvoyedMerchandise              ( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations );
    virtual void CancelMerchandiseOverheadReservation();
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( NET_ASN_MsgLogSupplyHandlingUpdate& asn ) const;
    //@}

private:
    //! @name Operations
    //@{
    void EnterStateConvoyWaitingForTransporters ();
    void EnterStateConvoyForming                ();
    void EnterStateConvoyGoingToLoadingPoint    ();
    void EnterStateConvoyLoading                ();
    void EnterStateConvoyGoingToUnloadingPoint  ();
    void EnterStateConvoyUnloading              ();
    void EnterStateConvoyGoingBackToFormingPoint();
    void EnterStateFinished                     ();

    void DoSupply();
    //@}

private:
    PHY_SupplyStockState* pSupplyState_;
    PHY_StockConvoy*      pConvoy_;
};

#endif // __PHY_SupplyStockConsign_h_
