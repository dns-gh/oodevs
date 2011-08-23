// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyDotationConsign.h $
// $Author: Nld $
// $Modtime: 13/04/05 12:12 $
// $Revision: 5 $
// $Workfile: PHY_SupplyDotationConsign.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyDotationConsign_h_
#define __PHY_SupplyDotationConsign_h_

#include "PHY_SupplyConsign_ABC.h"
#include "PHY_DotationConvoy.h"

class PHY_SupplyDotationState;

// =============================================================================
// @class  PHY_SupplyDotationConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyDotationConsign : public PHY_SupplyConsign_ABC
{
public:
             PHY_SupplyDotationConsign( MIL_AutomateLOG& supplyingAutomate, PHY_SupplyDotationState& supplyState, MIL_Automate& stockSupplier );
             PHY_SupplyDotationConsign();
    virtual ~PHY_SupplyDotationConsign();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual bool Update();
    virtual void GetMerchandiseToConvoy              ( T_MerchandiseToConvoyMap& container ) const;
    virtual void RemoveConvoyedMerchandise           ( const PHY_DotationCategory& dotationCategory, double rNbrDotations );
    virtual void AddConvoyedMerchandise              ( const PHY_DotationCategory& dotationCategory, double rNbrDotations );
    virtual void CancelMerchandiseOverheadReservation();
    virtual MIL_Automate* GetNextSupplied() const;
    virtual const T_PointVector* GetWayPointsToGoBack() const;
    virtual const T_PointVector* GetWayPointsToGoToNextSupplied() const;
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
    PHY_SupplyDotationState* pSupplyState_;
    PHY_DotationConvoy*      pConvoy_;
};

BOOST_CLASS_EXPORT_KEY( PHY_SupplyDotationConsign )

#endif // __PHY_SupplyDotationConsign_h_
