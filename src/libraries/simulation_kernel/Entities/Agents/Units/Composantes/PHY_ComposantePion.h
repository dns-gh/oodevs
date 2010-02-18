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

class MIL_AutomateLOG;
class MIL_PopulationType;
class MIL_PopulationAttitude;
class PHY_Sensor;
class PHY_Weapon;
class PHY_HumanProtection;
class MIL_Object_ABC;
class AttritionCapacity;
class PHY_FireDamages_Agent;
class PHY_AttritionData;
class PHY_Breakdown;
class PHY_MaintenanceComposanteState;
class PHY_Human;
class DEC_Knowledge_AgentComposante;
class MIL_Time_ABC;
class PHY_RoleInterface_Composantes;
class MIL_Injury_ABC;
class MIL_Agent_ABC;

namespace firing
{
    class WeaponAvailabilityComputer_ABC;
}

// =============================================================================
// @class  PHY_ComposantePion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposantePion : public PHY_Composante_ABC
{

public:
    typedef std::vector< PHY_ComposantePion* >       T_ComposantePionVector;
    typedef T_ComposantePionVector::iterator         IT_ComposantePionVector;
    typedef T_ComposantePionVector::const_iterator   CIT_ComposantePionVector;
    typedef T_ComposantePionVector::reverse_iterator RIT_ComposantePionVector;

             PHY_ComposantePion( const MIL_Time_ABC& time, const PHY_ComposanteTypePion& type, PHY_RolePion_Composantes& role, uint nNbrHumanInCrew, bool bMajor, bool bLoadable, bool bCanBePartOfConvoy );
             PHY_ComposantePion();
    virtual ~PHY_ComposantePion();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Main
    //@{
    void Update();
    void TransferComposante( PHY_RoleInterface_Composantes& newRole );

    template< typename T > void ApplyOnWeapons( T& t ) const;
    template< typename T > void ApplyOnHumanProtection( T& t ) const;
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
    bool                   CanEvacuateCasualties           () const;
    bool                   CanEvacuateCasualty             ( const PHY_Human& human ) const;
    bool                   CanCollectCasualties            () const;
    bool                   CanCollectCasualty              ( const PHY_Human& human ) const;
    bool                   CanDiagnoseHumans               () const;
    bool                   CanSortHumans                   () const;
    bool                   CanHealHuman                    ( const PHY_Human& human ) const;
    uint                   GetHealingTime                  ( const PHY_Human& human ) const;
    uint                   Heal                            (       PHY_Human& human ) const;
    //@}

    //! @name Logistic - Supply
    //@{
    bool CouldBePartOfConvoy        () const; // Type : don't check the state $$$$ a renommer
    bool CanBePartOfConvoy          () const; // $$$$ a renommer
    bool CanTransportStock          ( const PHY_DotationCategory& dotationCategory ) const; // Ind�pendamment du fait que la composante peut faire partie d'un convoi
    void GetStockTransporterCapacity( MT_Float& rWeightMax, MT_Float& rVolumeMax ) const;
    //@}

    //! @name Humans
    //@{
    uint GetNbrUsableHumans() const;
    void HealAllHumans     ();
    uint HealHumans        ( const PHY_HumanRank& rank, uint nNbrToChange );
    uint WoundHumans       ( const PHY_HumanRank& rank, uint nNbrToChange, const PHY_HumanWound& wound );
    bool ChangeHumanRank   ( const PHY_HumanRank& oldRank, const PHY_HumanRank&  newRank , const PHY_HumanWound& wound );
    //@}

    //! @name Fire / Dangerosity
    //@{
    void     ApplyPopulationFire            ( const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireDamages_Agent& fireDamages );
    void     ApplyDirectFire                ( const PHY_DotationCategory& dotationCategory                                              , PHY_FireDamages_Agent& fireDamages );
    void     ApplyIndirectFire              ( const PHY_DotationCategory& dotationCategory                                              , PHY_FireDamages_Agent& fireDamages );
    void     ApplyExplosion                 ( const AttritionCapacity& capacity                                                         , PHY_FireDamages_Agent& fireDamages );
    void     ApplyContamination             ( const MIL_ToxicEffectManipulator& contamination );
    void     ApplyPoisonous                 ( const MIL_ToxicEffectManipulator& contamination );
    void     ApplyInjury                    ( MIL_Injury_ABC& injury );
    uint     GetNeutralizationTime          () const;
    void     ApplyHumansWounds              ( const PHY_ComposanteState& composanteNewState, PHY_FireDamages_Agent& fireDamages );
    double   GetDangerosity                 ( const DEC_Knowledge_AgentComposante& compTarget, double rDistBtwFirerAndTarget ) const;
    double GetOnlyLoadableMaxRangeToFireOn  ( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH              ) const;
    double GetMaxRangeToFireOn              ( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH              ) const;
    double GetMinRangeToFireOn              ( const DEC_Knowledge_AgentComposante& compTarget, double rWantedPH              ) const;
    double GetMaxRangeToFireOnWithPosture   ( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const;
    double GetMinRangeToFireOnWithPosture   ( const DEC_Knowledge_AgentComposante& compTarget, const MIL_Agent_ABC& target, MT_Float rWantedPH ) const;
    double GetMaxRangeToIndirectFire        ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
    double GetMinRangeToIndirectFire        ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
    //@}

    //! @name Accessors
    //@{    
      const PHY_RoleInterface_Composantes&  GetRole            () const;
            double                          GetWeight          () const;
            bool                            IsMajor            () const;
      const PHY_ComposanteState&            GetState           () const;
            double                          GetMaxSpeed        () const;
            double                          GetMaxSpeed        ( const TerrainData& data ) const;
            double                          GetMaxSpeed        ( const MIL_Object_ABC& object ) const;
            bool                            CanMove            () const;
            bool                            CanPerceive        () const;
            bool                            CanBeFired         () const;
            bool                            CanBeLent          () const;
    virtual bool                            CanFire            () const;
    virtual bool                            CanFireWhenUnloaded() const;            
            
    virtual const PHY_ComposanteTypePion&   GetType        () const;
    virtual uint                            GetMajorScore  () const;
    //@}

    //! @name Dotations
    //@{
    const PHY_ConsumptionType&      GetConsumptionMode     ( const MIL_ObjectType_ABC& objectType ) const;
    const PHY_DotationConsumptions* GetDotationConsumptions( const PHY_ConsumptionType& consumptionMode ) const;
    //@}

    //! @name Objects
    //@{
    bool CanConstruct( const MIL_ObjectType_ABC& objectType ) const;
    bool CanDestroy  ( const MIL_ObjectType_ABC& objectType ) const;
    bool CanMine     ( const MIL_ObjectType_ABC& objectType ) const;
    bool CanDemine   ( const MIL_ObjectType_ABC& objectType ) const;
    bool CanBypass   ( const MIL_ObjectType_ABC& objectType, bool bObjectIsMined ) const;

    MT_Float GetConstructionTime( const MIL_ObjectType_ABC& objectType, MT_Float rSizeCoef ) const;
    MT_Float GetDestructionTime ( const MIL_ObjectType_ABC& objectType, MT_Float rSizeCoef ) const;
    MT_Float GetMiningTime      ( const MIL_ObjectType_ABC& objectType ) const;
    MT_Float GetDeminingTime    ( const MIL_ObjectType_ABC& objectType ) const;
    MT_Float GetBypassTime      ( const MIL_ObjectType_ABC& objectType, MT_Float rSizeCoef, bool bObjectIsMined ) const;
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
    template< typename T > void ApplyOnRadars ( T& func ) const;

    void Execute( firing::WeaponAvailabilityComputer_ABC& algorithm ) const;

    MT_Float GetOperationalState() const;
    void     ReinitializeState  ( const PHY_ComposanteState& state );
    void     Repair             ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_Weapon* >     T_WeaponVector;
    typedef T_WeaponVector::const_iterator CIT_WeaponVector;

    typedef std::vector< PHY_HumanProtection* > T_HumanProtectionVector;
    typedef T_HumanProtectionVector::const_iterator CIT_HumanProtectionVector;

    typedef std::vector< PHY_Sensor* >     T_SensorVector;
    typedef T_SensorVector::const_iterator CIT_SensorVector;
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
    const MIL_Time_ABC&             time_;
          PHY_RoleInterface_Composantes* pRole_;
    const PHY_ComposanteState*      pState_;
    const PHY_ComposanteTypePion*   pType_;
    const bool                      bMajor_;
    const bool                      bLoadable_;
    const bool                      bCanBePartOfConvoy_;
          bool                      bUsedForLogistic_;
          T_WeaponVector            weapons_;
          T_HumanProtectionVector   protections_;
          T_SensorVector            sensors_;
          PHY_HumansComposante*     pHumans_;

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

BOOST_CLASS_EXPORT_KEY( PHY_ComposantePion )

#include "PHY_ComposantePion.inl"

#endif // __PHY_ComposantePion_h_
