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

#include "MIL.h"
#include "PHY_RoleInterface_Supply.h"
#include "ComponentsChangedNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkMessageSender_ABC.h"
#include <boost/scoped_ptr.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

class DEC_ResourceNetwork;
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
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePionLOG_Supply( MIL_AgentPionLOG_ABC& pion );
    virtual ~PHY_RolePionLOG_Supply();
    //@}

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
    virtual MIL_AutomateLOG* FindLogisticManager() const;
    virtual const MIL_AgentPion& GetPion() const;
    //@}

    //! @name Operations
    //@{
    virtual void Update        ( bool bIsDead );
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean         ();

    virtual void EnableSystem ();
    virtual void DisableSystem();
    virtual bool IsSystemEnabled() const;

    virtual bool                CanContainStock              ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock*  AddStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock*  AddEmptyStock                ( const PHY_DotationCategory& dotationCategory, double capacity ) const;
    virtual PHY_DotationStock*  GetStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual double              GetStockAvailablity          ( const PHY_DotationCategory& dotationCategory, double rRequestedValue ) const;
    virtual double              AddStockReservation          ( const PHY_DotationCategory& dotationCategory, double rRequestedValue );
    virtual double              RemoveStockReservation       ( const PHY_DotationCategory& dotationCategory, double rRequestedValue );
    virtual bool                CanReserveStock              ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_ComposantePion* GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_ComposantePion* GetAvailableConvoyTransporter( const PHY_ComposanteTypePion& type ) const;

    virtual void StartUsingForLogistic( PHY_ComposantePion& composante );
    virtual void StopUsingForLogistic ( PHY_ComposantePion& composante );

    virtual void NotifyComponentHasChanged();

    virtual void ComputeStockWeightAndVolume( const PHY_DotationNature&, double& rWeight, double& rVolume ) const;
    //@}

    //! @name Stock supply
    //@{
    virtual void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    virtual void Apply( std::function< void( PHY_DotationStock& ) > visitor ) const;
    virtual void ResupplyStocks    ( bool withLog );
    virtual void ResupplyStocks    ( const PHY_DotationCategory& category, double rNbr );
    virtual void ConnectToResourceNode( unsigned int objectId, const std::string& resource );
    virtual void DisconnectFromResourceNode();
    virtual void UpdateSupplyNeeded();
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
    double GetResourceNetworkConnectedStockValue( const PHY_DotationCategory& category ) const;
    double GetConvoyTransportersAvailabilityRatio() const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePionLOG_Supply )
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPionLOG_ABC&                owner_;
    bool                                 bSystemEnabled_;
    bool                                 bHasChanged_;
    bool                                 bExternalMustChangeState_;
    boost::scoped_ptr< PHY_DotationStockContainer > pStocks_;
    boost::scoped_ptr< DEC_ResourceNetwork > pResourceNetworkConnected_;
    mutable int                          resourceNetworkStockSent_;
    mutable bool                         bResourceConnectionChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePionLOG_Supply )

#endif // __PHY_RolePionLOG_Supply_h_
