// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:17 $
// $Revision: 6 $
// $Workfile: PHY_RolePion_Supply.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Supply_h_
#define __PHY_RolePion_Supply_h_

#include "MIL.h"

#include "PHY_RoleInterface_Supply.h"

class PHY_DotationStock;
class MIL_Automate;
class PHY_DotationCategory;
class PHY_ComposantePion;
class PHY_SupplyStockRequestContainer;
class PHY_StockConvoy;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_RolePion_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Supply : public PHY_RoleInterface_Supply
{
    MT_COPYNOTALLOWED( PHY_RolePion_Supply )

public:
             PHY_RolePion_Supply( MT_RoleContainer& role );
             PHY_RolePion_Supply();
    virtual ~PHY_RolePion_Supply();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean ();
    //@}

    //! @name Init
    //@{
    virtual void ReadOverloading( MIL_InputArchive& archive );
    //@}

    //! @name Main
    //@{
    virtual void EnableSystem ();
    virtual void DisableSystem();

    virtual bool                CanContainStock              ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock*  GetStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual MT_Float            GetStockAvailablity          ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) const;
    virtual MT_Float            AddStockReservation          ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue );
    virtual void                RemoveStockReservation       ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue );
    virtual PHY_ComposantePion* GetAvailableConvoyTransporter() const;
    virtual PHY_ComposantePion* GetAvailableConvoyCommander  () const;

    virtual void StartUsingForLogistic( PHY_ComposantePion& composante );
    virtual void StopUsingForLogistic ( PHY_ComposantePion& composante );
    //@}

    //! @name Stock supply
    //@{
    virtual void FillSupplyRequest( PHY_SupplyStockRequestContainer& supplyRequest ) const;
    virtual void ResupplyStocks   ();
    //@}

    //! @name Events
    //@{
    virtual void NotifyComposanteChanged( PHY_ComposantePion& composante );
    //@}

    //! @name Convoy
    //@{
    virtual       void          AssignConvoy             ( PHY_StockConvoy& convoy );
    virtual       void          UnassignConvoy           ( PHY_StockConvoy& convoy );
    virtual       bool          ConvoySupply             () const;
    virtual       bool          ConvoyIsSupplyDone       () const;
    virtual const MIL_Automate* ConvoyGetSuppliedAutomate() const;
    virtual       void          ConvoyEndMission         ();
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    //@}
};

#include "PHY_RolePion_Supply.inl"

#endif // __PHY_RolePion_Supply_h_
