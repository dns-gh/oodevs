// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Supply.h $
// $Author: Nld $
// $Modtime: 13/04/05 14:14 $
// $Revision: 7 $
// $Workfile: PHY_RolePionLOG_Supply.h $
//
// *****************************************************************************

#ifndef __PHY_RolePionLOG_Supply_h_
#define __PHY_RolePionLOG_Supply_h_

#include "PHY_RoleInterface_Supply.h"
#include "ComponentsChangedNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkMessageSender_ABC.h"

namespace xml
{
    class xistream;
    class xostream;
}

class MIL_AgentPionLOG_ABC;
class PHY_DotationStockContainer;
class PHY_ComposantePion;

// =============================================================================
// @class  PHY_RolePionLOG_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePionLOG_Supply : public PHY_RoleInterface_Supply
                             , public component::ComponentsChangedNotificationHandler_ABC
                             , public network::NetworkMessageSender_ABC
{
public:
    explicit PHY_RolePionLOG_Supply( MIL_AgentPionLOG_ABC& pion, bool fromArchive = false );
    virtual ~PHY_RolePionLOG_Supply();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Init
    //@{
    virtual void ReadOverloading( xml::xistream& xis );
    //@}

    //! @name Accessors
    //@{
    virtual       MIL_AutomateLOG*      FindLogisticManager() const;
    virtual const MIL_AgentPionLOG_ABC& GetPion() const;
    //@}

    //! @name Operations
    //@{
    virtual void Update        ( bool bIsDead );
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean         ();

    virtual void EnableSystem ();
    virtual void DisableSystem();

    virtual bool                CanContainStock              ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock*  AddStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock*  AddEmptyStock                     ( const PHY_DotationCategory& dotationCategory, double capacity ) const;
    virtual PHY_DotationStock*  GetStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual double            GetStockAvailablity          ( const PHY_DotationCategory& dotationCategory, double rRequestedValue ) const;
    virtual double            AddStockReservation          ( const PHY_DotationCategory& dotationCategory, double rRequestedValue );
    virtual void                RemoveStockReservation       ( const PHY_DotationCategory& dotationCategory, double rRequestedValue );
    virtual PHY_ComposantePion* GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const;

    virtual void StartUsingForLogistic( PHY_ComposantePion& composante );
    virtual void StopUsingForLogistic ( PHY_ComposantePion& composante );

    virtual void NotifyComponentHasChanged();

    virtual void ComputeStockWeightAndVolume( const PHY_DotationNature&, double& rWeight, double& rVolume ) const;
    //@}

    //! @name Stock supply
    //@{
    virtual void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    virtual void FillSupplyRequest ( PHY_SupplyStockRequestContainer& supplyRequest ) const;
    virtual void ResupplyStocks    ();
    virtual void ResupplyStocks    ( const PHY_DotationCategory& category, double rNbr );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   ( unsigned int context ) const;
    //@}

private:
    //! @name Tools
    //@{
    double GetStockValue( const PHY_DotationCategory& category ) const;
    double GetConvoyTransportersAvailabilityRatio() const;
    //@}

private:
    MIL_AgentPionLOG_ABC&       pion_;
    bool                        bSystemEnabled_;
    bool                        bHasChanged_;
    bool                        bExternalMustChangeState_;
    PHY_DotationStockContainer* pStocks_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePionLOG_Supply* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePionLOG_Supply* role, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( PHY_RolePionLOG_Supply )

#endif // __PHY_RolePionLOG_Supply_h_
