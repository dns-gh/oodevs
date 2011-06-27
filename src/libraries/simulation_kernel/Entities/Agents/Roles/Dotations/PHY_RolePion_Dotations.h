// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h $
// $Author: Nld $
// $Modtime: 11/05/05 18:18 $
// $Revision: 11 $
// $Workfile: PHY_RolePion_Dotations.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Dotations_h_
#define __PHY_RolePion_Dotations_h_

#include "MIL.h"
#include "PHY_RoleInterface_Dotations.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "SurrenderNotificationHandler_ABC.h"
#include "ConsumeDotationNotificationHandler_ABC.h"
#include "DotationsActionsNotificationHandler_ABC.h"
#include "simulation_kernel/ConsumptionChangeRequestHandler_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"

class MIL_AgentPion;
class PHY_DotationGroupContainer;

namespace xml
{
    class xostream;
    class xistream;
}

namespace dotation
{
    class DotationComputer_ABC;
}

namespace dotation
{
// =============================================================================
// @class  PHY_RolePion_Dotations
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Dotations : public PHY_RoleInterface_Dotations
                             , public tools::AlgorithmModifier_ABC< dotation::DotationComputer_ABC >
                             , public dotation::ConsumptionChangeRequestHandler_ABC
                             , public surrender::SurrenderNotificationHandler_ABC
                             , public ConsumeDotationNotificationHandler_ABC
                             , public DotationsActionsNotificationHandler_ABC
                             , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    explicit PHY_RolePion_Dotations( MIL_AgentPion& pion, bool fromArchive = false );
    virtual ~PHY_RolePion_Dotations();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name External operations
    //@{
    void ReadOverloading              ( xml::xistream& xis );
    //@}

    //! @name Notification
    void NotifyCaptured();
    void NotifyReleased();
    virtual void NotifyConsumeDotation( const PHY_DotationCategory& category, double rNbr );
    virtual void RegisterDotationsCapacities  ( const PHY_DotationCapacities& capacities );
    virtual void UnregisterDotationsCapacities( const PHY_DotationCapacities& capacities );
    //@}

    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    virtual void Execute( dotation::DotationComputer_ABC& algorithm ) const;
    virtual void ChangeConsumptionMode(ConsumptionModeChangeRequest_ABC& request);
    //@}

    //! @name Dotations management
    //@{
    double SupplyDotation   ( const PHY_DotationCategory& category, double rNbr );
    void   ResupplyDotations();
    void   ResupplyDotations( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor );
    void   ResupplyDotations( const PHY_DotationType&      type             , double rFactor );
    //@}

    //! @name Consumption management
    //@{
                  bool                 SetConsumptionMode      ( const PHY_ConsumptionType& mode );
    virtual const PHY_ConsumptionType& GetConsumptionMode      () const;
                  void                 RollbackConsumptionMode ();
                  double             GetMaxTimeForConsumption( const PHY_ConsumptionType& mode ) const;
    //@}

    //! @name Fire dotations
    //@{
    virtual double AddFireReservation( const PHY_DotationCategory& category, double rNbr );
    virtual double GetDotationNumber( const PHY_DotationCategory& category ) const;
    virtual const PHY_DotationCategory* GetIlluminationDotations( float range, bool permanent ) const;
    virtual float GetIlluminatingRange( ) const;
    virtual void SetForbiddenDotation( const PHY_DotationCategory& category );
    virtual void RemoveForbiddenDotation( const PHY_DotationCategory& category );
    virtual void AllowAllDotations();    
    virtual bool HasDotationForFiring( const PHY_DotationCategory& category, int iterations );
    //@}

    //! @name Network
    //@{

    void SendChangedState( client::UnitAttributes& asn ) const;
    void SendFullState   ( client::UnitAttributes& asn ) const;
    //@}

    //! @name Logistic - Supply
    //@{
    void NotifySupplyNeeded          ( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const; // Logistic
    void FillSupplyRequest           ( PHY_SupplyDotationRequestContainer& supplyRequest ) const;
    void ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_Dotation*, double /*rValueReserved*/ > T_DotationReservedMap;
    typedef T_DotationReservedMap::const_iterator                        CIT_DotationReservedMap;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    //@}

          MIL_AgentPion&              pion_;
          PHY_DotationGroupContainer* pDotations_;

    const PHY_ConsumptionType*      pCurrentConsumptionMode_;
    const PHY_ConsumptionType*      pPreviousConsumptionMode_;
          T_DotationReservedMap     reservedConsumptions_;

    std::vector< const PHY_DotationCategory* > forbiddenDotations_;
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Dotations* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Dotations* role, const unsigned int /*version*/ );
};

} // namespace dotation

BOOST_CLASS_EXPORT_KEY( dotation::PHY_RolePion_Dotations )

#endif // __PHY_RolePion_Dotations_h_
