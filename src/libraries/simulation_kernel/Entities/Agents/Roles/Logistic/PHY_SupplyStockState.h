// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyStockState.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:34 $
// $Revision: 3 $
// $Workfile: PHY_SupplyStockState.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyStockState_h_
#define __PHY_SupplyStockState_h_

#include "PHY_SupplyState_ABC.h"
#include "PHY_SupplyStockRequest.h"
#include "MT_Tools/MT_Vector2DTypes.h"

class MIL_AutomateLOG;
class PHY_SupplyConsign_ABC;

// =============================================================================
// @class  PHY_SupplyStockState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyStockState : public PHY_SupplyState_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_SupplyStockState( MIL_AutomateLOG& convoyer, bool bConsumeQuota );
             PHY_SupplyStockState();
    virtual ~PHY_SupplyStockState();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
    bool IsSupplying( const MIL_Automate& supplied, const PHY_DotationCategory& dotationCategory ) const;
    MIL_AutomateLOG& GetConvoyer() const;
    MIL_Automate* GetNextSupplied() const;
    const T_PointVector* GetWayPointsToGoToSupplied( const MIL_Automate& supplied ) const;
    const T_PointVector* GetWayPointsToGoBack      () const;
    //@}

    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_SupplyStockRequest > T_Requests;
    typedef std::map< MIL_Automate*, T_Requests > T_AutomatRequests;
    typedef std::map< const PHY_DotationCategory*, double > T_MerchandiseToConvoyMap;
    //@}

    //! @name Operations
    //@{
    void AddRequest( MIL_Automate& supplied, const PHY_SupplyStockRequest& request );
    /*void SetWayPointsToGoToSupplied( const MIL_Automate& supplied, const T_PointVector& wayPoints ) const;
    void SetWayPointsToGoBacl      ( const T_PointVector& wayPoints ) const;*/
    // void AddTransporters( liste of SupplyFlowTransporter )


    void GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const;
    void RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations );
    void AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations );
    void CancelMerchandiseOverheadReservation();

    void Supply( MIL_Automate& supplied, T_MerchandiseToConvoyMap& stockSupplied );
    void CancelSupply();
    //@}

    //! @name Consign
    //@{
    void SetConsign( PHY_SupplyConsign_ABC* pConsign );
    //@}

    //! @name Network
    //@{
    void SendMsgCreation() const;
    void SendChangedState() const;
    void SendFullState() const;
    void Clean();
    //@}

private:
    //! @name Network tools
    //@{
    void SendMsgDestruction() const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_AutomateLOG*     pConvoyer_;
    const bool           bConsumeQuota_;
    PHY_SupplyConsign_ABC* pConsign_;
    bool bConsignChanged_;
    bool bRequestsChanged_;
    T_AutomatRequests requestsQueued_;
    T_AutomatRequests requestsDone_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_SupplyStockState )

#endif // __PHY_SupplyStockState_h_
