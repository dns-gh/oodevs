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

#include "MIL.h"

#include "PHY_Composante_ABC.h"

#include "PHY_ComposanteState.h"
#include "PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Humans/PHY_HumansComposante.h"

#include "MT_Tools/MT_Random.h"

class MIL_AgentPion;
class MIL_AutomateLOG;
class MIL_PopulationType;
class MIL_PopulationAttitude;
class PHY_Sensor;
class PHY_Radar;
class PHY_Weapon;
class PHY_DotationGroupContainer;
class MIL_RealObject_ABC;
class PHY_DirectFireData;
class PHY_IndirectFireData;
class PHY_IndirectFireDotationClass;
class PHY_SmokeData;
class PHY_FireDamages_Agent;
class PHY_AttritionData;
class PHY_Breakdown;
class PHY_MaintenanceComposanteState;
class PHY_MedicalHumanState;
class PHY_Human;
class DEC_Knowledge_AgentComposante;


// =============================================================================
// @class  PHY_ComposantePion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposantePion : public PHY_Composante_ABC
{
    MT_COPYNOTALLOWED( PHY_ComposantePion )

public:
     PHY_ComposantePion( const PHY_ComposanteTypePion& type, PHY_RolePion_Composantes& role, bool bMajor, bool bLoadable );
     PHY_ComposantePion();
    ~PHY_ComposantePion();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Main
    //@{
    void Update();
    void TransfertComposante( PHY_RolePion_Composantes& newRole );
    //@}

    //! @name Logistic - Maintenance
    //@{
    const PHY_Breakdown* GetBreakdown() const;
          bool           CanHaul     () const;
          bool           CanHaul     ( const PHY_ComposanteTypePion& composanteType ) const;
          bool           CanRepair   () const;
          bool           CanRepair   ( const PHY_Breakdown& breakdown ) const;
          
          void           StartUsingForLogistic      ();
          void           StopUsingForLogistic       ();
          void           NotifyHandledByMaintenance ();
          void           NotifyRepairedByMaintenance();

          void           PreprocessRandomBreakdowns ( uint nEndDayTimeStep );

          uint           ApproximateTravelTime     ( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const;
    //@}

    //! @name Logistic - Medical
    //@{
    bool                   HasWoundedHumansToEvacuate      () const;
    void                   EvacuateWoundedHumans           ( MIL_AutomateLOG& destinationTC2 ) const;
    PHY_MedicalHumanState* NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ) const;
    PHY_MedicalHumanState* NotifyHumanWaitingForMedical    ( PHY_Human& human ) const;
    void                   NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState ) const;
    bool                   CanEvacuateCasualties           () const;
    bool                   CanCollectCasualties            () const;
    bool                   CanDiagnoseHumans               () const;
    bool                   CanSortHumans                   () const;
    bool                   CanHealHuman                    ( const PHY_Human& human ) const;
    uint                   GetHealingTime                  ( const PHY_Human& human ) const;
    void                   Heal                            (       PHY_Human& human ) const;
    //@}

    //! @name Logistic - Supply
    //@{
    bool CanBePartOfConvoy               () const;
    bool CanTransportStock               ( const PHY_DotationCategory& dotationCategory ) const; // Indépendamment du fait que la composante peut faire partie d'un convoi
    void GetStockTransporterCapacity     ( MT_Float& rWeightMax, MT_Float& rVolumeMax ) const;
    uint GetStockTransporterLoadingTime  () const;
    uint GetStockTransporterUnloadingTime() const;
    //@}

    //! @name Humans
    //@{
    uint GetNbrUsableHumans() const;
    void HealAllHumans     ();
    uint HealHumans        ( const PHY_HumanRank& rank, uint nNbrToChange );
    uint WoundHumans       ( const PHY_HumanRank& rank, uint nNbrToChange, const PHY_HumanWound& wound );
    bool ChangeHumanRank   ( const PHY_HumanRank& oldRank, const PHY_HumanRank&  newRank , const PHY_HumanWound& wound );

    void NotifyHumanAdded  ( PHY_Human& human );
    void NotifyHumanRemoved( PHY_Human& human );
    void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState );
    //@}

    //! @name Fire / Dangerosity
    //@{
    void     FillDirectFireData             ( PHY_DirectFireData&   data );
    void     FillIndirectFireData           ( PHY_IndirectFireData& data );
    void     FillIndirectFireData           ( PHY_SmokeData&        data );

    void     ApplyPopulationFire            ( const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireDamages_Agent& fireDamages );
    void     ApplyDirectFire                ( const PHY_DotationCategory& dotationCategory                                              , PHY_FireDamages_Agent& fireDamages );
    void     ApplyIndirectFire              ( const PHY_DotationCategory& dotationCategory                                              , PHY_FireDamages_Agent& fireDamages );
    void     ApplyExplosion                 ( const MIL_RealObjectType& objectType                                                      , PHY_FireDamages_Agent& fireDamages );
    void     ApplyContamination             ( const MIL_NbcAgentType& nbcAgentType );
    uint     GetNeutralizationTime          () const;
    void     ApplyHumansWounds              ( const PHY_ComposanteState& composanteNewState, PHY_FireDamages_Agent& fireDamages );
    MT_Float GetDangerosity                 ( const DEC_Knowledge_AgentComposante& compTarget, MT_Float rDistBtwFirerAndTarget ) const;
    MT_Float GetOnlyLoadableMaxRangeToFireOn( const DEC_Knowledge_AgentComposante& compTarget, MT_Float rWantedPH              ) const;
    MT_Float GetMaxRangeToFireOn            ( const DEC_Knowledge_AgentComposante& compTarget, MT_Float rWantedPH              ) const;
    MT_Float GetMinRangeToFireOn            ( const DEC_Knowledge_AgentComposante& compTarget, MT_Float rWantedPH              ) const;
    MT_Float GetMaxRangeToFireOnWithPosture ( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const;
    MT_Float GetMinRangeToFireOnWithPosture ( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const;
    MT_Float GetMaxRangeToIndirectFire      ( const PHY_IndirectFireDotationClass&, bool bCheckDotationsAvailability           ) const;
    MT_Float GetMinRangeToIndirectFire      ( const PHY_IndirectFireDotationClass&, bool bCheckDotationsAvailability           ) const;
    //@}

    //! @name Accessors
    //@{    
      const PHY_RolePion_Composantes& GetRole            () const;
            MT_Float                  GetWeight          () const;
            bool                      IsMajor            () const;
      const PHY_ComposanteState&      GetState           () const;
            MT_Float                  GetMaxSpeed        () const;
            MT_Float                  GetMaxSpeed        ( const TerrainData& data ) const;
            MT_Float                  GetMaxSpeed        ( const MIL_RealObject_ABC& object ) const;
            bool                      CanMove            () const;
            bool                      CanPerceive        () const;
            bool                      CanBeFired         () const;
            bool                      CanBeLent          () const;
    virtual bool                      CanFire            () const;
    virtual bool                      CanFireWhenUnloaded() const;            
            
    virtual const PHY_ComposanteTypePion& GetType        () const;
    virtual uint                          GetMajorScore  () const;
    //@}

    //! @name Dotations
    //@{
    const PHY_ConsumptionType&      GetConsumptionMode     ( const MIL_RealObjectType& objectType ) const;
    const PHY_DotationConsumptions* GetDotationConsumptions( const PHY_ConsumptionType& consumptionMode ) const;
    //@}

    //! @name Objects
    //@{
    bool CanConstruct( const MIL_RealObjectType& objectType ) const;
    bool CanDestroy  ( const MIL_RealObjectType& objectType ) const;
    bool CanMine     ( const MIL_RealObjectType& objectType ) const;
    bool CanDemine   ( const MIL_RealObjectType& objectType ) const;
    bool CanBypass   ( const MIL_RealObjectType& objectType, bool bObjectIsMined ) const;

    MT_Float GetConstructionTime( const MIL_RealObjectType& objectType, MT_Float rSizeCoef ) const;
    MT_Float GetDestructionTime ( const MIL_RealObjectType& objectType, MT_Float rSizeCoef ) const;
    MT_Float GetMiningTime      ( const MIL_RealObjectType& objectType ) const;
    MT_Float GetDeminingTime    ( const MIL_RealObjectType& objectType ) const;
    MT_Float GetBypassTime      ( const MIL_RealObjectType& objectType, MT_Float rSizeCoef, bool bObjectIsMined ) const;
    //@}

    //! @name Load / unload / transport
    //@{
    bool CanBeLoaded       () const;
    bool CanTransportHumans() const;
    
    bool     CanBeTransported                           () const;
    bool     CanTransportPion                           () const;
    MT_Float GetPionTransporterWeightCapacity           () const;
    MT_Float GetPionTransporterWeightLoadedPerTimeStep  () const;
    MT_Float GetPionTransporterWeightUnloadedPerTimeStep() const;
    //@}

    //! @name Operations
    //@{
    template< typename T > void ApplyOnSensors( T& func ) const;
    template< typename T > void ApplyOnRadars ( T& func ) const;

    MT_Float GetOperationalState() const;
    void     ReinitializeState  ( const PHY_ComposanteState& state );
    void     Repair             ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_Weapon* >     T_WeaponVector;
    typedef T_WeaponVector::const_iterator CIT_WeaponVector;

    typedef std::vector< PHY_Sensor* >     T_SensorVector;
    typedef T_SensorVector::const_iterator CIT_SensorVector;

    typedef std::vector< PHY_Radar* >     T_RadarVector;
    typedef T_RadarVector::const_iterator CIT_RadarVector;
    //@}

private:
    //! @name Tools
    //@{
    void ApplyFire           ( const PHY_AttritionData& attritionData, PHY_FireDamages_Agent& fireDamages );
    bool CanBeUsed           () const;
    bool CanBeUsedForMove    () const;
    void ManageEndMaintenance();
    //@}

private:
          PHY_RolePion_Composantes* pRole_;
    const PHY_ComposanteState*      pState_;
    const PHY_ComposanteTypePion*   pType_;
    const bool                      bMajor_;
    const bool                      bLoadable_;
          bool                      bUsedForLogistic_;
          T_WeaponVector            weapons_;
          T_SensorVector            sensors_;
          PHY_HumansComposante      humans_;

    // Breakdowns
          uint                            nAutoRepairTimeStep_;
    const PHY_Breakdown*                  pBreakdown_;             
          PHY_MaintenanceComposanteState* pMaintenanceState_;

    // Random breakdowns
          uint                 nRandomBreakdownNextTimeStep_;
    const PHY_ComposanteState* pRandomBreakdownState_;

private:
    static MT_Random random_;

public:
    static MT_Float rOpStateWeightHumans_;
};

#include "PHY_ComposantePion.inl"

#endif // __PHY_ComposantePion_h_
