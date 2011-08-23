// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyStockConsign.h $
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
    //! @name Constructors/Destructor
    //@{
             PHY_SupplyStockConsign( MIL_AutomateLOG& supplier, PHY_SupplyStockState& supplyState, MIL_Automate& stockSupplier );
             PHY_SupplyStockConsign();
    virtual ~PHY_SupplyStockConsign();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual bool Update();

    // Scripts events
    bool IsLoadingDone() const;
    bool IsUnloadingDone() const;
    bool ConvoyLoad();
    bool ConvoyUnload();
    void ConvoyEndMission();

    virtual MIL_Automate* GetNextSupplied() const;
    virtual const T_PointVector* GetWayPointsToGoBack() const;
    virtual const T_PointVector* GetWayPointsToGoToNextSupplied() const;

    virtual void GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const;
    virtual void RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations );
    virtual void AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations );
    virtual void CancelMerchandiseOverheadReservation();
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( client::LogSupplyHandlingUpdate& asn ) const;
    //@}

private:
    //! @name Operations
    //@{
    void EnterStateConvoyWaitingForTransporters();
    void EnterStateConvoyForming();
    void EnterStateConvoyGoingToLoadingPoint();
    void EnterStateConvoyLoading();
    void EnterStateConvoyGoingToUnloadingPoint();
    void EnterStateConvoyUnloading();
    void ChooseStateAfterConvoyUnloading();
    void EnterStateConvoyGoingBackToFormingPoint();
    void EnterStateFinished();
    //@}

private:
    //! @name Member data
    //@{
    PHY_SupplyStockState* pSupplyState_;
    PHY_StockConvoy* pConvoy_;
    MIL_Automate* pSupplied_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_SupplyStockConsign )

#endif // __PHY_SupplyStockConsign_h_
