// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposantePion.h $
// $Author: Nld $
// $Modtime: 2/05/05 18:37 $
// $Revision: 23 $
// $Workfile: PHY_ComposantePion.h $
//
// *****************************************************************************

#ifndef __PHY_ComposantePion_h_
#define __PHY_ComposantePion_h_

#include "PHY_ComposanteTypePion.h"
#include "PHY_Composante_ABC.h"
#include "PHY_ComposanteState.h"

class AttritionCapacity;
class DEC_Knowledge_AgentComposante;
class Human_ABC;
class HumansComposante_ABC;
class MIL_Agent_ABC;
class MIL_AutomateLOG;
class MIL_Object_ABC;
class MIL_PopulationType;
class MIL_PopulationAttitude;
class MIL_Time_ABC;
class MIL_BurnEffectManipulator;
class MIL_DisasterEffectManipulator;
class PHY_AttritionData;
class PHY_Breakdown;
class PHY_FireDamages_Agent;
class PHY_HumanRank;
class PHY_MaintenanceComposanteState;
class PHY_RoleInterface_Composantes;
class PHY_Sensor;
class PHY_Weapon;
class PHY_HumanWound;
class WoundEffects_ABC;

namespace firing
{
    class WeaponAvailabilityComputer_ABC;
}

namespace transport
{
    class PHY_RoleAction_Loading;
}

namespace sword
{
    class MissionParameters;
}

// =============================================================================
// @class  PHY_ComposantePion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposantePion : public PHY_Composante_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< PHY_ComposantePion* > T_ComposantePionVector;
    //@}

    //! @name Constructors/Destructor
    //@{
             PHY_ComposantePion();
             PHY_ComposantePion( const MIL_Time_ABC& time, const PHY_ComposanteTypePion& type, PHY_RolePion_Composantes& role, unsigned int nNbrHumanInCrew, bool bMajor, bool bLoadable, bool bCanBePartOfConvoy );
    virtual ~PHY_ComposantePion();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Main
    //@{
    void Update();
    void TransferComposante( PHY_RoleInterface_Composantes& newRole );

    template< typename T > void ApplyOnWeapons( T& functor ) const;
    template< typename T > void ApplyOnWeapons( const T& functor ) const;
    template< typename T > void ApplyOnHumanProtection( T& functor ) const;
    //@}

    //! @name Logistic - Maintenance
    //@{
    const PHY_Breakdown* GetBreakdown() const;
    bool CanHaul() const;
    bool CanHaul1( const PHY_ComposanteTypePion& composanteType ) const;
    bool CanRepair() const;
    bool CanRepair( const PHY_Breakdown& breakdown ) const;

    void StartUsingForLogistic();
    void StopUsingForLogistic();
    void NotifyHandledByMaintenance();
    void NotifyRepairedByMaintenance();

    void PreprocessRandomBreakdowns( unsigned int nEndDayTimeStep );

    unsigned int ApproximateTravelTime( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const;
    void DeleteMaintenanceState();
    void CancelLogisticRequests();
    //@}

    //! @name Logistic - Medical
    //@{
    bool HasWoundedHumansToEvacuate() const;
    void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const;
    bool CanEvacuateCasualties() const;
    bool CanEvacuateCasualty( const Human_ABC& human ) const;
    bool CanCollectCasualties() const;
    bool CanCollectCasualty( const Human_ABC& human ) const;
    bool CanDiagnoseHumans() const;
    bool CanSortHumans() const;
    bool CanHealHuman( const Human_ABC& human ) const;
    unsigned int GetHealingTime( const Human_ABC& human ) const;
    unsigned int Heal( Human_ABC& human ) const;
    void ChangeHumanState( sword::MissionParameters& msg, std::set< boost::shared_ptr< Human_ABC > >& done );
    void UpdateHumanState( sword::MissionParameters& msg, std::set< boost::shared_ptr< Human_ABC > >& done );
    //@}

    //! @name Logistic - Supply
    //@{
    bool CouldBePartOfConvoy() const; // Type : don't check the state $$$$ a renommer
    bool CanBePartOfConvoy() const; // $$$$ a renommer
    bool CanTransportStock( const PHY_DotationCategory& dotationCategory ) const; // Indépendamment du fait que la composante peut faire partie d'un convoi
    void GetStockTransporterCapacity( double& rWeightMax, double& rVolumeMax ) const;
    //@}

    //! @name Humans
    //@{
    unsigned int GetNbrUsableHumans() const;
    unsigned int GetNbrHealthyHumans( const PHY_HumanRank& rank ) const;
    void HealAllHumans( bool withLog );
    void KillAllHumans();
    unsigned int HealHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange );
    unsigned int OverloadHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& wound, bool psyop = false, bool contaminated = false );
    unsigned int WoundHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& wound );
    bool ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank&  newRank , const PHY_HumanWound& wound );
    void RemoveHealthyHumans( const PHY_HumanRank& rank, unsigned int humansToRemove );
    void OnNoAvailableHuman();
    //@}

    //! @name Fire / Dangerosity
    //@{
    void ApplyPopulationFire( const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireDamages_Agent& fireDamages, double armedIndividuals, bool& bCanBePlundered );
    void ApplyDirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireDamages_Agent& fireDamages );
    void ApplyIndirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireDamages_Agent& fireDamages, double ratio );
    void ApplyExplosion( const AttritionCapacity& capacity, PHY_FireDamages_Agent& fireDamages );
    void ApplyContamination();
    void ApplyEffect( const WoundEffects_ABC& effects );
    void ApplyUrbanObjectCrumbling( const MIL_Object_ABC& object );

    unsigned int GetNeutralizationTime() const;
    void ApplyHumansWounds( const PHY_ComposanteState& composanteNewState, PHY_FireDamages_Agent& fireDamages );
    double GetDangerosity( const DEC_Knowledge_AgentComposante& compTarget, double rDistBtwFirerAndTarget, bool bUseAmmo ) const;
    double GetOnlyLoadableMaxRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH ) const;
    double GetMaxRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH, const PHY_DotationCategory* dotation ) const;
    double GetMinRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH ) const;
    double GetMaxRangeToFireOnWithPosture( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, double rWantedPH ) const;
    double GetMinRangeToFireOnWithPosture( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, double rWantedPH ) const;
    double GetMaxRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability, bool theoric ) const;
    double GetMinRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability, bool theoric ) const;
    double GetMaxRangeToIndirectFire( ) const;
    double GetMaxRangeToDirectFire() const;
    double GetMaxRangeToFire( const MIL_Agent_ABC& pion, double rWantedPH ) const;
    double GetProtectionHumanDeadRatio( const PHY_ComposanteState& state ) const;
    double GetProtectionHumanWoundedRatio( const PHY_ComposanteState& state ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_RoleInterface_Composantes& GetRole() const;
    double GetWeight() const;
    bool IsMajor() const;
    bool IsAway() const;
    const PHY_ComposanteState& GetState() const;
    double GetMaxSpeed() const;
    double GetMaxSpeed( const TerrainData& data ) const;
    double GetMaxSpeed( const MIL_Object_ABC& object ) const;
    bool CanMove() const;
    bool IsUsable() const;
    bool IsLoadable() const;
    bool CanPerceive(  const transport::PHY_RoleAction_Loading* roleLoading  ) const;
    bool CanBeFired() const;
    bool CanBeLent() const;
    virtual bool CanFire() const;
    virtual bool CanFireWhenUnloaded() const;

    virtual const PHY_ComposanteTypePion& GetType() const;
    virtual float GetIdentificationMaxRange() const;
    virtual float GetReconnoissanceMaxRange() const;
    virtual unsigned int GetMajorScore() const;

    virtual bool CanPerceive( const MIL_ObjectType_ABC& objectType ) const;
    //@}

    //! @name Dotations
    //@{
    const PHY_ConsumptionType& GetConsumptionMode( const MIL_ObjectType_ABC& objectType ) const;
    const PHY_DotationConsumptions* GetDotationConsumptions( const PHY_ConsumptionType& consumptionMode ) const;
    //@}

    //! @name Objects
    //@{
    double GetConstructionSpeed( const TerrainData& data ) const;
    bool HasConstructionSpeeds() const;

    bool CanConstruct( const MIL_ObjectType_ABC& objectType, bool bWithLoaded ) const;
    bool CanDestroy( const MIL_ObjectType_ABC& objectType ) const;
    bool CanMine( const MIL_ObjectType_ABC& objectType ) const;
    bool CanDemine( const MIL_ObjectType_ABC& objectType, bool bWithLoaded ) const;
    bool CanBypass( const MIL_ObjectType_ABC& objectType, bool bObjectIsMined ) const;
    bool CanExtinguish( const MIL_ObjectType_ABC& objectType ) const;
    bool CanRemoveFromPath( const MIL_ObjectType_ABC& objectType, bool isObjectMined ) const;

    double GetConstructionTime( const MIL_ObjectType_ABC& objectType, double rSizeCoef ) const;
    double GetDestructionTime( const MIL_ObjectType_ABC& objectType, double rSizeCoef ) const;
    double GetMiningTime( const MIL_ObjectType_ABC& objectType ) const;
    double GetDeminingTime( const MIL_ObjectType_ABC& objectType ) const;
    double GetBypassTime( const MIL_ObjectType_ABC& objectType, double rSizeCoef, bool bObjectIsMined ) const;
    double GetExtinguishingTime( const MIL_ObjectType_ABC& objectType ) const;
    //@}

    //! @name Load / unload / transport
    //@{
    bool IsLoadableAndUsable() const;
    bool CanTransportHumans() const;

    bool CanBeTransported() const;
    bool CanTransportPion() const;
    bool CanTransportCrowd() const;
    unsigned int GetCrowdTransporterCapacity() const;
    double GetCrowdTransporterLoadedPerTimeStep() const;
    double GetCrowdTransporterUnloadedPerTimeStep() const;
    double GetPionTransporterWeightCapacity() const;
    double GetPionTransporterWeightLoadedPerTimeStep() const;
    double GetPionTransporterWeightUnloadedPerTimeStep() const;
    //@}

    //! @name PHY_RoleInterface_ActiveProtection
    //@{
    virtual void UseAmmunition( const PHY_DotationCategory& category, MIL_Agent_ABC& pion );
    virtual double GetPHModifier( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const;
    virtual bool CounterIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const;
    virtual bool DestroyIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const;
    //@}

    //! @name Operations
    //@{
    template< typename T > void ApplyOnSensors( T& func ) const;
    template< typename T > void ApplyOnSensors( const T& func ) const;
    template< typename T > void ApplyOnRadars ( T& func ) const;
    template< typename T > void ApplyOnRadars ( const T& func ) const;

    void Execute( firing::WeaponAvailabilityComputer_ABC& algorithm ) const;

    double GetOperationalState() const;
    void ReinitializeState( const PHY_ComposanteState& state, const PHY_BreakdownType* breakdownType = 0 );
    void Repair( bool withLog );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_Weapon* >          T_WeaponVector;
    typedef std::vector< PHY_Sensor* >          T_SensorVector;
    //@}

private:
    //! @name Tools
    //@{
    void ApplyFire( const PHY_AttritionData& attritionData, double urbanProtection, PHY_FireDamages_Agent& fireDamages );
    void ApplyNewComposanteState( const PHY_ComposanteState& pNewState, const PHY_ComposanteState& oldState );
    bool CanBeUsed( bool bWithLoaded = false ) const;
    bool CanBeUsedForMove() const;
    bool CanComponentBeUsed( const transport::PHY_RoleAction_Loading* role, bool bWithLoaded ) const;
    void ManageEndMaintenance();
    bool HasRepairEvacuationMeans() const;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Time_ABC& time_;
    PHY_RoleInterface_Composantes* pRole_;
    const PHY_ComposanteState* pState_;
    const PHY_ComposanteTypePion* pType_;
    const bool bMajor_;
    const bool bLoadable_;
    const bool bCanBePartOfConvoy_;
    bool bUsedForLogistic_;
    bool bRepairEvacuationNoMeansChecked_;
    T_WeaponVector weapons_;
    T_SensorVector sensors_;
    HumansComposante_ABC* pHumans_;
    // Breakdowns
    unsigned int nAutoRepairTimeStep_;
    std::unique_ptr< const PHY_Breakdown > pBreakdown_;
    boost::shared_ptr< PHY_MaintenanceComposanteState > pMaintenanceState_;
    // Random breakdowns
    unsigned int nRandomBreakdownNextTimeStep_;
    const PHY_ComposanteState* pRandomBreakdownState_;
    //@}

public:
    //! @name Member data
    //@{
    static double rOpStateWeightHumans_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_ComposantePion )

#include "PHY_ComposantePion.inl"

#endif // __PHY_ComposantePion_h_
