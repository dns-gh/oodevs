// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyDotationState.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:30 $
// $Revision: 3 $
// $Workfile: PHY_SupplyDotationState.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyDotationState_h_
#define __PHY_SupplyDotationState_h_

#include "PHY_SupplyState_ABC.h"
#include "PHY_SupplyDotationRequest.h"

class MIL_Automate;
class PHY_SupplyConsign_ABC;
class PHY_DotationCategory;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_SupplyDotationState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyDotationState : public PHY_SupplyState_ABC
{
public:
    explicit PHY_SupplyDotationState( MIL_Automate& suppliedAutomate, MIL_AutomateLOG& convoyer  );
             PHY_SupplyDotationState();
    virtual ~PHY_SupplyDotationState();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, double > T_MerchandiseToConvoyMap;
    //@}

    //! @name Operations
    //@{
    void AddRequest( const PHY_SupplyDotationRequest& request );
    MIL_Automate& GetSupplied() const;
    MIL_AutomateLOG& GetConvoyer() const;
    bool IsSupplying( const PHY_DotationCategory& dotationCategory ) const;

    void GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const;
    void RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations );
    void AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations );
    void CancelMerchandiseOverheadReservation();

    void Supply() const;
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

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_SupplyDotationRequest > T_RequestMap;
    typedef T_RequestMap::iterator                                            IT_RequestMap;
    typedef T_RequestMap::const_iterator                                     CIT_RequestMap;
    //@}

private:
    MIL_Automate* pSupplied_;
    MIL_AutomateLOG*     pConvoyer_;
    PHY_SupplyConsign_ABC* pConsign_;
    bool bConsignChanged_;
    bool bRequestsChanged_;
    T_RequestMap requests_;
};

BOOST_CLASS_EXPORT_KEY( PHY_SupplyDotationState )

#endif // __PHY_SupplyDotationState_h_
