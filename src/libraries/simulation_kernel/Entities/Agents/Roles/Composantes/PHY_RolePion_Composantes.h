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

#include "simulation_kernel/HumansChangedNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"
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
                               , public network::NetworkUnitMessageNotificationHandler_ABC
{


public:
    explicit PHY_RolePion_Composantes( MIL_Agent_ABC& pion, bool initialise = true );
    virtual ~PHY_RolePion_Composantes();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
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

    template< typename T > void                ApplyOnWeapons     ( T& t ) const;

    virtual void NotifyHumanHasChanged();
    virtual void NotifyTransportHasChanged();
    virtual void NotifyIsLoadedInVab();
    virtual void NotifyIsUnLoadedInVab();


    virtual void Update( bool bIsDead );
    virtual void Clean ();
    //@}

    //! @name Composantes management
    //@{
    virtual void ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, uint nNbrAvailable );
    virtual void RepairAllComposantes         ();
    virtual void DestroyRandomComposante      ();
    virtual void DestroyAllComposantes        ();
    //@}

    //! @name Pret de composantes
    //@{
    template < typename T > uint LendComposantes             ( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate );
    template < typename T > uint GetLentComposantesTravelTime( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate );
    template < typename T > uint RetrieveLentComposantes     ( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate );

    // Actions on the composante owner
    virtual void LendComposante        ( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante );
    virtual void RetrieveLentComposante( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante );

    // Notification for the beneficary
    virtual void NotifyLentComposanteReceived( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante ); 
    virtual void NotifyLentComposanteReturned( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante );
    //@}

    //$$$ Toute la partie logistique GetXXXUse() devrait �tre externalis�e

    //! @name Logistic - maintenance
    //@{
    virtual void                            PreprocessRandomBreakdowns           ( uint nEndDayTimeStep ) const;

    virtual PHY_MaintenanceComposanteState* NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante );
    virtual void                            NotifyComposanteBackFromMaintenance  ( PHY_MaintenanceComposanteState& composanteState );

    //! @name Logistic - Supply
    //@{

    //! @name Prisoners
    //@{
    virtual void NotifyCaptured();
    virtual void NotifyReleased();
    //@}

    //! @name Fire / Dangerosity
    //@{
    virtual bool     IsNeutralized                   () const;
    virtual void     GetComposantesAbleToBeFired     ( PHY_Composante_ABC::T_ComposanteVector& targets, uint nNbrFirer, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void     GetComposantesAbleToBeFired     ( PHY_Composante_ABC::T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void     Neutralize                      ();
    virtual void     ApplyPopulationFire             ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result );
    virtual void     ApplyDirectFire                 ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void     ApplyIndirectFire               ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void     ApplyExplosion                  ( const AttritionCapacity& capacity, PHY_FireResults_ABC& result );
    virtual void     ApplyContamination              ( const MIL_ToxicEffectManipulator& contamination );
    virtual void     ApplyPoisonous                  ( const MIL_ToxicEffectManipulator& contamination );

    virtual MT_Float GetOnlyLoadableMaxRangeToFireOn ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
    virtual MT_Float GetMaxRangeToFireOn             ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
    virtual MT_Float GetMinRangeToFireOn             ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
    virtual MT_Float GetMaxRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
    virtual MT_Float GetMinRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
    virtual MT_Float GetMaxRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
    virtual MT_Float GetMinRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
    virtual void     ApplyInjury                     ( MIL_Injury_ABC& injury );
            MT_Float GetDangerosity                  ( const DEC_Knowledge_Agent& target ) const;
    //@}

    //! @name Perception / Knowledge
    //@{
    virtual const PHY_Volume*          GetSignificantVolume     ( const PHY_SensorTypeAgent& sensorType  ) const;
    virtual       void                 GetVisibleVolumes        ( T_ComposanteVolumeSet& volumes         ) const;
    virtual       void                 BuildKnowledgeComposantes( T_KnowledgeComposanteVector& knowledge ) const;
    virtual const PHY_ComposantePion*  GetMajorComposante       () const;
    virtual       MT_Float             GetMajorComponentWeight  () const;
    //@}

    //! @name Load / unload / transport
    //@{
    virtual void DamageTransported( MT_Float rWeight, const PHY_ComposanteState& state, bool bTransportOnlyLoadable ) const;
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
    virtual       MT_Float       GetOperationalState     () const;
    virtual       double       GetMajorOperationalState() const;
    virtual       bool           IsUsable                () const;                    
    virtual const MIL_Agent_ABC& GetPion                 () const;    
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& asn ) const;
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& functor ) const;
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

private:
    //! @name Types
    //@{
    struct T_ComposanteTypeProperties
    {
        T_ComposanteTypeProperties();

        bool HasUsableComposantes() const;
        template< typename Archive > void serialize( Archive&, const uint );

        std::vector< uint > nbrsPerState_;
        bool                bHasChanged_;
    };
    //@}

public:
    //! @name Types
    //@{   
    typedef std::set< PHY_MaintenanceComposanteState* >     T_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::iterator       IT_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::const_iterator CIT_MaintenanceComposanteStateSet;
    
    typedef std::map< const PHY_RoleInterface_Composantes*, PHY_ComposantePion::T_ComposantePionVector > T_LoanMap;
    typedef T_LoanMap::iterator                                                 IT_LoanMap;
    typedef T_LoanMap::const_iterator                                           CIT_LoanMap;

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteTypeProperties > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::iterator                                         IT_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                                   CIT_ComposanteTypeMap;    
    //@}

private:
    //! @name Init
    //@{
    void DistributeCommanders      ();
    void DistributeHumanWounds     ( const PHY_HumanRank& rank, const PHY_HumanWound& wound, uint nNbr, PHY_ComposantePion::CIT_ComposantePionVector& itCurrentComp );
    void ReadHumansOverloading     ( xml::xistream& xis );
    void ReadComposantesOverloading( xml::xistream& xis );
    //@}

    //! @name Tools
    //@{
    virtual bool HasChanged() const;
    void UpdateOperationalStates();
    void UpdateMajorComposante  ();

    void UpdateDataWhenComposanteRemoved( const PHY_ComposanteState& state, T_ComposanteTypeProperties& properties );
    void UpdateDataWhenComposanteAdded  ( const PHY_ComposanteState& state, T_ComposanteTypeProperties& properties );

    void SendLoans( NET_ASN_MsgUnitAttributes& asn ) const;

    void SendLogisticChangedState() const;
    void SendLogisticFullState   () const;
    //@}
    //! @name Helpers
    //@{
    void ReadEquipements( xml::xistream& xis );
    void ReadEquipement ( xml::xistream& xis );
    void ReadPersonnels ( xml::xistream& xis );
    void ReadPersonnel  ( xml::xistream& xis );

    //@}

private:
    MIL_Agent_ABC&         pion_;
    PHY_ComposantePion::T_ComposantePionVector composantes_;
    T_ComposanteTypeMap    composanteTypes_;
    uint                   nNbrComposanteChanged_;
    double               rMajorOperationalState_;
    double               rOperationalState_;
    bool                   bOperationalStateChanged_;    
    PHY_ComposantePion*    pMajorComposante_;
    uint                   nNeutralizationEndTimeStep_;
    uint                   nNbrUsableComposantes_;

    T_LoanMap              lentComposantes_;
    T_LoanMap              borrowedComposantes_;

    bool                   bLoansChanged_;
    bool                   bExternalMustChange_;
    bool                   bTransportHasChanged_;
    bool                   bIsLoaded_;
    bool                   bIsSurrender_;

    // Maintenance
    T_MaintenanceComposanteStateSet maintenanceComposanteStates_;
    uint                            nTickRcMaintenanceQuerySent_;


private:
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Composantes* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Composantes* role, const unsigned int /*version*/ );
};

#include "PHY_RolePion_Composantes.inl"

#endif // __PHY_RolePion_Composantes_h_
