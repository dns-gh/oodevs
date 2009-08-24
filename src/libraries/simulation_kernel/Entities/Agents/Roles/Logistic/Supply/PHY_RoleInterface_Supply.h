// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RoleInterface_Supply.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:14 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Supply.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Supply_h_
#define __PHY_RoleInterface_Supply_h_

#include "MT_Tools/Role_ABC.h"

class PHY_DotationStock;
class MIL_Automate;
class PHY_DotationCategory;
class PHY_ComposantePion;
class PHY_SupplyStockRequestContainer;
class PHY_StockConvoy;
class MIL_AutomateLOG;
class MIL_AgentPionLOG_ABC;
//@TODO make multiple interface/role with specifics methods for convoy and for stockage
// =============================================================================
// @class  PHY_RoleInterface_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Supply : public tools::Role_ABC
                               , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Supply RoleInterface;
    //@}

public:
             PHY_RoleInterface_Supply();
    virtual ~PHY_RoleInterface_Supply();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint ) = 0;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_AgentPionLOG_ABC& GetPion() const { return *pPion_; };
    //@}

    //! @name Operations
    //@{
    virtual void Update        ( bool bIsDead ) = 0;
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean         () = 0;
    //@}

    //! @name Init
    //@{
    virtual void ReadOverloading( xml::xistream& xis );
    //@}

    //! @name Main
    //@{
    virtual void EnableSystem ();
    virtual void DisableSystem();

    virtual bool                CanContainStock              ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock*  AddStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock*  GetStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual MT_Float            GetStockAvailablity          ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) const;
    virtual MT_Float            AddStockReservation          ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue );
    virtual void                RemoveStockReservation       ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) ;
    virtual PHY_ComposantePion* GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const;

    virtual void StartUsingForLogistic( PHY_ComposantePion& composante );
    virtual void StopUsingForLogistic ( PHY_ComposantePion& composante );
    //@}

    //! @name Stock supply
    //@{
    virtual void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const =0;
    virtual void FillSupplyRequest( PHY_SupplyStockRequestContainer& supplyRequest ) const;
    virtual void ResupplyStocks   ();
    virtual void ResupplyStocks   ( const PHY_DotationCategory& category, MT_Float rNbr );
    //@}

    //! @name Events
    //@{
    virtual void NotifyComposanteChanged( PHY_ComposantePion& composante );
    //@}

    //! @name Convoy
    //@{
    virtual       void             AssignConvoy              ( PHY_StockConvoy& convoy );
    virtual       void             UnassignConvoy            ( PHY_StockConvoy& convoy );

    virtual       bool             ConvoyLoad                () const;
    virtual       bool             ConvoyUnload              () const;
    virtual       bool             ConvoyIsLoadingDone       () const;
    virtual       bool             ConvoyIsUnloadingDone     () const;
    virtual const MIL_AutomateLOG* ConvoyGetSupplyingAutomate() const;
    virtual const MIL_AutomateLOG* ConvoyGetConvoyingAutomate() const;
    virtual const MIL_Automate*    ConvoyGetSuppliedAutomate () const;
    virtual       void             ConvoyEndMission          () ;

    virtual       MT_Float         ModifySpeed               ( MT_Float rSpeed ) const;
    //@}
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    //@}

private:
    MIL_AgentPionLOG_ABC*       pPion_; //$$$Hack

};

#endif // __PHY_RoleInterface_Supply_h_
