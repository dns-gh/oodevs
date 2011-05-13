// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h $
// $Author: Jvt $
// $Modtime: 12/05/05 15:16 $
// $Revision: 21 $
// $Workfile: PHY_RolePion_Composantes.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Composantes_h_
#define __PHY_RolePion_Composantes_h_

#include "PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/NBC/ToxicEffectHandler_ABC.h"
#include "SurrenderNotificationHandler_ABC.h"
#include "TransportNotificationHandler_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "simulation_kernel/HumansChangedNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"
#include "simulation_kernel/NetworkMessageSender_ABC.h"
#include "simulation_kernel/LoadingChangeNotificationHandler_ABC.h"

class MIL_Injury_ABC;

namespace xml
{
    class xostream;
    class xistream;
}

namespace firing
{
    class WeaponAvailabilityComputer_ABC;
}

namespace transport
{
    class TransportCapacityComputer_ABC;
    class TransportWeightComputer_ABC;
    class HumanLoadingTimeComputer_ABC;
    class LoadedStateConsistencyComputer_ABC;
}
namespace moving
{
    class SpeedComputer_ABC;
}

class OnComponentComputer_ABC;
class OnComponentLendedFunctorComputer_ABC;
class MIL_AgentPion_ABC;
class PHY_DotationStock;
class PHY_RoleInterface_Supply;

// =============================================================================
// @class  PHY_RolePion_Composantes
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Composantes : public PHY_RoleInterface_Composantes
                               , public tools::AlgorithmModifier_ABC< firing::WeaponAvailabilityComputer_ABC >
                               , public tools::AlgorithmModifier_ABC< transport::TransportCapacityComputer_ABC >
                               , public tools::AlgorithmModifier_ABC< transport::TransportWeightComputer_ABC>
                               , public tools::AlgorithmModifier_ABC< OnComponentComputer_ABC >
                               , public tools::AlgorithmModifier_ABC< OnComponentLendedFunctorComputer_ABC >
                               , public tools::AlgorithmModifier_ABC< transport::HumanLoadingTimeComputer_ABC>
                               , public tools::AlgorithmModifier_ABC< transport::LoadedStateConsistencyComputer_ABC>
                               , public tools::AlgorithmModifier_ABC< moving::SpeedComputer_ABC >
                               , public nbc::ToxicEffectHandler_ABC
                               , public transport::TransportNotificationHandler_ABC
                               , public transport::LoadingChangeNotificationHandler_ABC
                               , public surrender::SurrenderNotificationHandler_ABC
                               , public human::HumansChangedNotificationHandler_ABC
                               , public network::NetworkUnitAttributesMessageSender_ABC
                               , public network::NetworkMessageSender_ABC
{
public:
    explicit PHY_RolePion_Composantes( MIL_Agent_ABC& pion, bool initialise = true );
    virtual ~PHY_RolePion_Composantes();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Init
    //@{
    void ReadOverloading( xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    virtual void Execute( firing::WeaponAvailabilityComputer_ABC& algorithm ) const;
    virtual void Execute( transport::TransportCapacityComputer_ABC& algorithm ) const;
    virtual void Execute( transport::TransportWeightComputer_ABC& algorithm ) const;
    virtual void Execute( transport::HumanLoadingTimeComputer_ABC& algorithm ) const;
    virtual void Execute( transport::LoadedStateConsistencyComputer_ABC& algorithm ) const;
    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const; // TODO replace with Execute( OnComponentComputer_ABC
    virtual void Execute( OnComponentComputer_ABC& algorithm ) const;
    virtual void Execute( OnComponentLendedFunctorComputer_ABC& algorithm ) const;

    template< typename T > void ApplyOnWeapons( T& t ) const;

    virtual void NotifyHumanHasChanged();
    virtual void NotifyTransportHasChanged();
    virtual void NotifyIsLoadedInVab();
    virtual void NotifyIsUnLoadedInVab();

    virtual void Update( bool bIsDead );
    virtual void Clean ();
    //@}

    //! @name Composantes management
    //@{
    virtual void ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, unsigned int nNbrAvailable );
    virtual void RepairAllComposantes();
    virtual void DestroyRandomComposante();
    virtual void DestroyAllComposantes();
    unsigned int GetNbrUsableHumans() const;
    //@}

    //! @name Pret de composantes
    //@{
    template < typename T > unsigned int LendComposantes             ( PHY_RolePion_Composantes& borrower, unsigned int nNbr, T funcPredicate );
    template < typename T > unsigned int GetLentComposantesTravelTime( PHY_RolePion_Composantes& borrower, unsigned int nNbr, T funcPredicate );
    template < typename T > unsigned int RetrieveLentComposantes     ( PHY_RolePion_Composantes& borrower, unsigned int nNbr, T funcPredicate );

    // Actions on the composante owner
    virtual void LendComposante        ( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante );
    virtual void RetrieveLentComposante( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante );

    // Notification for the beneficary
    virtual void NotifyLentComposanteReceived( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante );
    virtual void NotifyLentComposanteReturned( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante );
    //@}

    //$$$ Toute la partie logistique GetXXXUse() devrait être externalisée
    void GetConvoyTransportersUse( T_ComposanteUseMap& composanteUse ) const;
    //! @name Logistic - maintenance
    //@{
    virtual void PreprocessRandomBreakdowns ( unsigned int nEndDayTimeStep ) const;

    virtual PHY_MaintenanceComposanteState* NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante );
    virtual void                            NotifyComposanteBackFromMaintenance  ( PHY_MaintenanceComposanteState& composanteState );
    //@}

    //! @name Prisoners
    //@{
    virtual void NotifyCaptured();
    virtual void NotifyReleased();
    //@}

    //! @name Fire / Dangerosity
    //@{
    virtual bool IsNeutralized                    () const;
    virtual void GetComposantesAbleToBeFired      ( PHY_Composante_ABC::T_ComposanteVector& targets, unsigned int nNbrFirer, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void GetComposantesAbleToBeFired      ( PHY_Composante_ABC::T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void Neutralize                       ();
    virtual void ApplyPopulationFire              ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result, double armedIndividuals );
    virtual void ApplyDirectFire                  ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void ApplyDirectFireOnMajorComposantes( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult );
    virtual void ApplyIndirectFire                ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result, double ratio );
    virtual void ApplyExplosion                   ( const AttritionCapacity& capacity, PHY_FireResults_ABC& result );
    virtual void ApplyContamination               ( const MIL_ToxicEffectManipulator& contamination );
    virtual void ApplyPoisonous                   ( const MIL_ToxicEffectManipulator& contamination );
    virtual void ApplyUrbanObjectCrumbling        ( const MIL_Object_ABC& object );
    virtual void ApplyBurn                        ( const MIL_BurnEffectManipulator& burn );
    virtual void ApplyFlood                       ( const MIL_FloodEffectManipulator& flood );

    virtual double GetDangerosity                  ( const DEC_Knowledge_AgentComposante& compTarget, float rDistBtwSourceAndTarget ) const;
    virtual double GetOnlyLoadableMaxRangeToFireOn ( const DEC_Knowledge_Agent& target, double rWantedPH ) const;
    virtual double GetMaxRangeToFireOn             ( const DEC_Knowledge_Agent& target, double rWantedPH ) const;
    virtual double GetMinRangeToFireOn             ( const DEC_Knowledge_Agent& target, double rWantedPH ) const;
    virtual double GetMaxRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, double rWantedPH ) const;
    virtual double GetMinRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, double rWantedPH ) const;
    virtual double GetMaxRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
    virtual double GetMinRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
    virtual double GetMaxRangeToFire               ( const MIL_Agent_ABC&  pion, double rWantedPH ) const;
    virtual void ApplyInjury( MIL_Injury_ABC& injury );
    //@}

    //! @name Perception / Knowledge
    //@{
    virtual const PHY_Volume* GetSignificantVolume( const PHY_SensorTypeAgent& sensorType ) const;
    virtual void GetVisibleVolumes( T_ComposanteVolumeSet& volumes ) const;
    virtual void BuildKnowledgeComposantes( T_KnowledgeComposanteVector& knowledge ) const;
    virtual const PHY_ComposantePion* GetMajorComposante() const;
    virtual double GetMajorComponentWeight() const;
    virtual double GetAttritionIndexComposante( int idMaterial ) const;
    //@}

    //! @name Load / unload / transport
    //@{
    virtual void DamageTransported( double rWeight, const PHY_ComposanteState& state, bool bTransportOnlyLoadable ) const;
    //@}

    //! @name Notifications (internal)
    //@{
    virtual void NotifyComposanteAdded   ( PHY_ComposantePion& composante );
    virtual void NotifyComposanteRemoved ( PHY_ComposantePion& composante );
    virtual void NotifyComposanteChanged ( PHY_ComposantePion& composante, const PHY_ComposanteState& oldState );
    virtual void NotifyComposanteRepaired();
    //@}

    //! @name Accessors
    //@{
    virtual double GetOperationalState() const;
    virtual double GetMajorOperationalState() const;
    virtual bool IsUsable() const;
    virtual const MIL_Agent_ABC& GetPion() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& message ) const;
    virtual void SendFullState   ( client::UnitAttributes& message ) const;

    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    //@}

    //! @name PHY_RoleInterface_ActiveProtection
    //@{
    virtual void UseAmmunition( const PHY_DotationCategory& );
    virtual double GetPHModifier( const PHY_DotationCategory& ) const;
    virtual bool CounterIndirectFire( const PHY_DotationCategory& ) const;
    virtual bool DestroyIndirectFire( const PHY_DotationCategory& ) const;
    //@}

    //! @name Composante reserved
    //@{
    virtual void WoundLoadedHumans( const PHY_ComposantePion& composanteChanged, const PHY_ComposanteState& newState, PHY_FireDamages_Agent& fireDamages );
    //@}

    //! @name Stock
    //@{
    PHY_DotationStock& GetOrAddStock( PHY_RoleInterface_Supply& supplyRole, const PHY_DotationCategory& dotation );
    bool CanStockMoreOf( PHY_RoleInterface_Supply& supplyRole, const PHY_DotationCategory& dotation ) const;
    //@}
private:
    //! @name Types
    //@{
    struct T_ComposanteTypeProperties
    {
        T_ComposanteTypeProperties();

        bool HasUsableComposantes() const;
        template< typename Archive > void serialize( Archive&, const unsigned int );

        std::vector< unsigned int > nbrsPerState_;
        bool bHasChanged_;
    };
    //@}

public:
    //! @name Types
    //@{
    typedef std::set< PHY_MaintenanceComposanteState* >       T_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::iterator        IT_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::const_iterator CIT_MaintenanceComposanteStateSet;

    typedef std::map< const PHY_RoleInterface_Composantes*, PHY_ComposantePion::T_ComposantePionVector > T_LoanMap;
    typedef T_LoanMap::iterator                                                                         IT_LoanMap;
    typedef T_LoanMap::const_iterator                                                                  CIT_LoanMap;

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteTypeProperties > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::iterator                                        IT_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                                 CIT_ComposanteTypeMap;
    //@}

private:
    //! @name Init
    //@{
    void DistributeCommanders      ();
    void ReadComposantesOverloading( xml::xistream& xis );
    void ReadHumansOverloading     ( xml::xistream& xis );
    void WriteComposantesODB       ( xml::xostream& xos ) const;
    void WriteHumansODB            ( xml::xostream& xos ) const;
    //@}

    //! @name Tools
    //@{
    virtual bool HasChanged() const;
    void UpdateOperationalStates();
    void UpdateMajorComposante  ();

    void UpdateDataWhenComposanteRemoved( const PHY_ComposanteState& state, T_ComposanteTypeProperties& properties );
    void UpdateDataWhenComposanteAdded  ( const PHY_ComposanteState& state, T_ComposanteTypeProperties& properties );

    void SendLoans( client::UnitAttributes& message ) const;
    //@}
    //! @name Helpers
    //@{
    void ReadEquipment( xml::xistream& xis );
    void ReadHuman    ( xml::xistream& xis );
    void GetStockTransporterCapacity( const PHY_DotationNature& nature, double& rWeightMax, double& rVolumeMax ) const;
    //@}

private:
    MIL_Agent_ABC& pion_;
    PHY_ComposantePion::T_ComposantePionVector composantes_;
    T_ComposanteTypeMap composanteTypes_;
    unsigned int nNbrComposanteChanged_;
    double rMajorOperationalState_;
    double rOperationalState_;
    bool bOperationalStateChanged_;
    PHY_ComposantePion* pMajorComposante_;
    unsigned int nNeutralizationEndTimeStep_;
    unsigned int nNbrUsableComposantes_;

    T_LoanMap lentComposantes_;
    T_LoanMap borrowedComposantes_;

    bool bLoansChanged_;
    bool bExternalMustChange_;
    bool bTransportHasChanged_;
    bool bIsLoaded_;
    bool bIsSurrender_;

    // Maintenance
    T_MaintenanceComposanteStateSet maintenanceComposanteStates_;
    unsigned int nTickRcMaintenanceQuerySent_;


private:
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Composantes* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Composantes* role, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_Composantes )

#include "PHY_RolePion_Composantes.inl"

#endif // __PHY_RolePion_Composantes_h_
