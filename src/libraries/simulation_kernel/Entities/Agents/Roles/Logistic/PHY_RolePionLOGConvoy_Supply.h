// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_RolePionLOGConvoy_Supply.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:26 $
// $Revision: 2 $
// $Workfile: PHY_RolePionLOGConvoy_Supply.h $
//
// *****************************************************************************

#ifndef __PHY_RolePionLOGConvoy_Supply_h_
#define __PHY_RolePionLOGConvoy_Supply_h_

#include "MIL.h"
#include "PHY_RoleInterface_Supply.h"

class MIL_Agent_ABC;
class MIL_AgentPion;

namespace logistic
{
    class SupplyConvoyReal_ABC;
}

namespace moving
{
    class SpeedComputer_ABC;
}

// =============================================================================
// @class  PHY_RolePionLOGConvoy_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePionLOGConvoy_Supply : public PHY_RoleInterface_Supply
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePionLOGConvoy_Supply( MIL_AgentPion& pion );
    virtual ~PHY_RolePionLOGConvoy_Supply();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_AgentPion& GetPion() const { return owner_; }
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean ();
    //@}

    //! @name Convoy
    //@{
    virtual void AssignConvoy( boost::shared_ptr< logistic::SupplyConvoyReal_ABC > );
    virtual void UnassignConvoy();

    virtual void ConvoyNotifyMovedToSupplier();
    virtual void ConvoyNotifyMovedToTransportersProvider();
    virtual void ConvoyNotifyMovedToSupplyRecipient();
    virtual void ConvoyEndMission();

    virtual int                                             ConvoyGetCurrentAction         () const;
    virtual logistic::SupplyRecipient_ABC*                  ConvoyGetCurrentSupplyRecipient() const;
    virtual const MIL_Agent_ABC*                            ConvoyGetSupplier              () const;
    virtual std::vector< boost::shared_ptr< MT_Vector2D > > ConvoyGetPathToNextDestination () const;
    virtual bool                                            ConvoyIsPushedFlow             () const;
    virtual bool                                            IsConvoy                       () const;
    virtual void                                            ToItinerary( sword::Pathfind& pathfind ) const;

    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    //@}

    //! @name Events
    //@{
    virtual void NotifyComposanteChanged( PHY_ComposantePion& composante );
    virtual void NotifySupplyNeeded     ( const PHY_DotationCategory& /*dotationCategory*/, bool /*bNewNeed*/ ) const {};
    virtual void UpdateSupplyNeeded() {}
    //@}

private:
    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePionLOGConvoy_Supply )
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion&                                      owner_;
    boost::shared_ptr< logistic::SupplyConvoyReal_ABC > convoy_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePionLOGConvoy_Supply )

#endif // __PHY_RolePionLOGConvoy_Supply_h_
