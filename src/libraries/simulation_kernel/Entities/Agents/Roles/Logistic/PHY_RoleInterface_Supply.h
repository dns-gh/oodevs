// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:14 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Supply.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Supply_h_
#define __PHY_RoleInterface_Supply_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>
#include "MT_Tools/MT_Vector2DTypes.h"

namespace xml
{
    class xostream;
    class xistream;
}

namespace logistic
{
    class SupplyConvoyReal_ABC;
    class SupplyRecipient_ABC;
    class SupplySupplier_ABC;
}

class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_AgentPionLOG_ABC;
class MIL_Automate;
class MIL_AutomateLOG;
class PHY_DotationStock;
class PHY_DotationCategory;
class PHY_ComposantePion;
class PHY_DotationNature;
class PHY_ComposanteTypePion;

//@TODO make multiple interface/role with specifics methods for convoy and for stockage
// =============================================================================
// @class  PHY_RoleInterface_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Supply : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Supply RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_Supply();
    virtual ~PHY_RoleInterface_Supply();
    //@}

    //! @name CheckPoints
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_AgentPion& GetPion() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead ) = 0;
    virtual void UpdateLogistic( bool bIsDead );
    virtual void Clean() = 0;
    //@}

    //! @name Init
    //@{
    virtual void ReadOverloading( xml::xistream& xis );
    //@}

    //! @name Events
    //@{
    virtual void NotifyComposanteChanged( PHY_ComposantePion& ) {}
    //@}

    //! @name Main
    //@{
    virtual void EnableSystem ();
    virtual void DisableSystem();

    virtual bool CanContainStock( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock* AddStock( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_DotationStock* AddEmptyStock( const PHY_DotationCategory& dotationCategory, double capacity ) const;
    virtual PHY_DotationStock* GetStock( const PHY_DotationCategory& dotationCategory ) const;
    virtual double AddStockReservation   ( const PHY_DotationCategory& dotationCategory, double rRequestedValue );
    virtual double RemoveStockReservation( const PHY_DotationCategory& dotationCategory, double rRequestedValue );
    virtual bool   CanReserveStock       ( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_ComposantePion* GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const;
    virtual PHY_ComposantePion* GetAvailableConvoyTransporter( const PHY_ComposanteTypePion& type ) const;

    virtual void StartUsingForLogistic( PHY_ComposantePion& composante );
    virtual void StopUsingForLogistic( PHY_ComposantePion& composante );

    virtual void ComputeStockWeightAndVolume( const PHY_DotationNature&, double& rWeight, double& rVolume ) const;
    //@}

    //! @name Stock supply
    //@{
    virtual void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const = 0;
    virtual void Apply( boost::function< void( PHY_DotationStock& ) > visitor ) const;
    virtual void ResupplyStocks( bool withLog );
    virtual void ResupplyStocks( const PHY_DotationCategory& category, double rNbr );
    virtual void ConnectToResourceNode( unsigned int objectId, const std::string& resource );
    virtual void DisconnectFromResourceNode();
    virtual void UpdateSupplyNeeded() = 0;
    //@}

    //! @name Convoy
    //@{
    virtual void AssignConvoy( boost::shared_ptr< logistic::SupplyConvoyReal_ABC > ) {}
    virtual void UnassignConvoy() {}

    virtual void ConvoyNotifyMovedToSupplier() {}
    virtual void ConvoyNotifyMovedToTransportersProvider() {}
    virtual void ConvoyNotifyMovedToSupplyRecipient() {}
    virtual void ConvoyEndMission() {}

    virtual int                            ConvoyGetCurrentAction() const { return 0; };
    virtual logistic::SupplyRecipient_ABC* ConvoyGetCurrentSupplyRecipient() const { return 0; };
    virtual const MIL_Agent_ABC*           ConvoyGetSupplier() const { return 0; };
    virtual logistic::SupplySupplier_ABC*  ConvoyGetTransportersProvider() const { return 0; };
    virtual const T_PointVector*           ConvoyGetPathToNextDestination() const { return 0; };
    virtual bool                           ConvoyIsPushedFlow() const { return false; }
    //@}

private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

#endif // __PHY_RoleInterface_Supply_h_
