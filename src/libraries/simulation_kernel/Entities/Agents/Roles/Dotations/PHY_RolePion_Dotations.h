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
             PHY_RolePion_Dotations();
    explicit PHY_RolePion_Dotations( MIL_AgentPion& pion );
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
    void ReadOverloading( xml::xistream& xis );
    //@}

    //! @name Notification
    void NotifyCaptured();
    void NotifyReleased();
    virtual void NotifySurrendered();
    virtual void NotifySurrenderCanceled();
    virtual void NotifyConsumeDotation( const PHY_DotationCategory& category, double& rNbr );
    virtual void RegisterDotationsCapacities( const PHY_DotationCapacities& capacities, T_Dotations*& dotations );
    virtual void UnregisterDotationsCapacities( const PHY_DotationCapacities& capacities, T_Dotations*& dotations );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean();
    virtual void Execute( dotation::DotationComputer_ABC& algorithm ) const;
    virtual void ChangeConsumptionMode(ConsumptionModeChangeRequest_ABC& request);
    //@}

    //! @name Accessors
     //@{
    const MIL_AgentPion* GetPion() const;
    //@}

    //! @name Dotations management
    //@{
    double SupplyDotation( const PHY_DotationCategory& category, double rNbr );
    void ResupplyDotations( bool withLog );
    void ResupplyDotations( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor );
    void ResupplyDotations( const PHY_DotationType& type, double rFactor );
    void ChangeDotation( const PHY_DotationCategory& category, unsigned int number, float threshold );
    void ChangeDotation( const PHY_DotationCategory& category, double fraction );
    void ChangeDotation( const PHY_DotationType& type, double fraction );
    //@}

    //! @name Consumption management
    //@{
    bool SetConsumptionMode( const PHY_ConsumptionType& mode );
    virtual const PHY_ConsumptionType& GetConsumptionMode() const;
    void RollbackConsumptionMode();
    double GetMaxTimeForConsumption( const PHY_ConsumptionType& mode ) const;
    //@}

    //! @name Fire dotations
    //@{
    virtual double AddFireReservation( const PHY_DotationCategory& category, double rNbr );
    virtual double GetDotationNumber( const PHY_DotationCategory& category ) const;
    virtual double GetFuelDotationNumber() const;
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
    virtual void Apply( boost::function< void( PHY_Dotation& ) > visitor ) const;
    void NotifySupplyNeeded          ( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const; // Logistic
    void ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor ) const;
    virtual void EnforceAviationResources( E_AviationRange aviationRange);
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion*                             owner_;
    PHY_DotationGroupContainer*                pDotations_;
    const PHY_ConsumptionType*                 pCurrentConsumptionMode_;
    const PHY_ConsumptionType*                 pPreviousConsumptionMode_;
    std::vector< const PHY_DotationCategory* > forbiddenDotations_;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( dotation::PHY_RolePion_Dotations )

#endif // __PHY_RolePion_Dotations_h_
