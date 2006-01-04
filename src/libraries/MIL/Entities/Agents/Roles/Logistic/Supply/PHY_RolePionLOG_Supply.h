// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOG_Supply.h $
// $Author: Nld $
// $Modtime: 13/04/05 14:14 $
// $Revision: 7 $
// $Workfile: PHY_RolePionLOG_Supply.h $
//
// *****************************************************************************

#ifndef __PHY_RolePionLOG_Supply_h_
#define __PHY_RolePionLOG_Supply_h_

#include "MIL.h"

#include "PHY_RolePion_Supply.h"

class MIL_AgentPionLOG_ABC;
class PHY_DotationStockContainer;
class PHY_ComposantePion;

// =============================================================================
// @class  PHY_RolePionLOG_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePionLOG_Supply : public PHY_RolePion_Supply
{
    MT_COPYNOTALLOWED( PHY_RolePionLOG_Supply )

public:
             PHY_RolePionLOG_Supply( MT_RoleContainer& role, MIL_AgentPionLOG_ABC& pion );
             PHY_RolePionLOG_Supply();
    virtual ~PHY_RolePionLOG_Supply();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Init
    //@{
    virtual void ReadOverloading( MIL_InputArchive& archive );
    //@}

    //! @name Accessors
    //@{
    const MIL_AgentPionLOG_ABC& GetPion() const;
    //@}

    //! @name Operations
    //@{
    virtual void Update        ( bool bIsDead );
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean         ();

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
            void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    virtual void FillSupplyRequest ( PHY_SupplyStockRequestContainer& supplyRequest ) const;
    virtual void ResupplyStocks    ();
    virtual void ResupplyStocks    ( const PHY_DotationCategory& category, MT_Float rNbr );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    //@}

private:
    //! @name Tools
    //@{
    MT_Float GetStockValue( const PHY_DotationCategory& category ) const;
    //@}

private:
    MIL_AgentPionLOG_ABC*       pPion_;
    bool                        bSystemEnabled_;
    bool                        bHasChanged_;
    PHY_DotationStockContainer* pStocks_; 
};

#include "PHY_RolePionLOG_Supply.inl"

#endif // __PHY_RolePionLOG_Supply_h_
