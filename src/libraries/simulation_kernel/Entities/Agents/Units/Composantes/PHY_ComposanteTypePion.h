// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h $
// $Author: Age $
// $Modtime: 4/05/05 14:44 $
// $Revision: 19 $
// $Workfile: PHY_ComposanteTypePion.h $
//
// *****************************************************************************

#ifndef __PHY_ComposanteType_h_
#define __PHY_ComposanteType_h_

#include "PHY_ComposanteType_ABC.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCapacities.h"
#include "ENT/ENT_Enums.h"
#include <tools/Map.h>

namespace xml
{
    class xistream;
}

class Human_ABC;
class MIL_ObjectType_ABC;
class MIL_Object_ABC;
class MIL_Time_ABC;
class MT_InterpolatedFunction;
class ObjectTypeResolver_ABC;
class PHY_ActiveProtection;
class PHY_AmmoDotationClass;
class PHY_Breakdown;
class PHY_BreakdownType;
class PHY_ComposantePion;
class PHY_ComposanteTypeObjectData;
class PHY_ConsumptionType;
class PHY_DotationConsumptions;
class PHY_DotationNature;
class PHY_MaintenanceLevel;
class PHY_MaterialCompositionType;
class PHY_RadarType;
class PHY_RolePion_Composantes;
class PHY_Sensor;
class PHY_SensorType;
class PHY_Weapon;
class PHY_WeaponType;

// =============================================================================
// @class  PHY_ComposanteTypePion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposanteTypePion : public PHY_ComposanteType_ABC
{
public:
             PHY_ComposanteTypePion( const MIL_Time_ABC& time, const std::string& strName, xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    virtual ~PHY_ComposanteTypePion();

    //! @name Static initialization
    //@{
    static void Initialize( const MIL_Time_ABC& time, xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    static void Terminate();

    static const PHY_ComposanteTypePion* Find( const std::string& strName );
    static const PHY_ComposanteTypePion* Find( uint32_t id );
    //@}

    //! @name Instanciation
    //@{
    PHY_ComposantePion& InstanciateComposante( PHY_RolePion_Composantes& role, unsigned int nNbrHumanInCrew, bool bMajor, bool bLoadable, bool bCanBePartOfConvoy ) const;
    void InstanciateWeapons( std::back_insert_iterator< std::vector< PHY_Weapon* > > inserter ) const;
    void InstanciateSensors( std::back_insert_iterator< std::vector< PHY_Sensor* > > inserter ) const;
    //@}

    //! @name Dotations
    //@{
    const PHY_DotationCapacities&   GetDotationCapacities  () const;
    const PHY_DotationConsumptions* GetDotationConsumptions( const PHY_ConsumptionType& consumptionMode ) const;
    const PHY_ConsumptionType&      GetConsumptionMode     ( const MIL_ObjectType_ABC& objectType ) const;
    //@}

    //! @name Accessors
    //@{
    double GetMaxSpeed           () const;
    double GetMaxSpeed           ( const TerrainData& data ) const;
    double GetMaxSpeed           ( const MIL_Object_ABC& object ) const;
    double GetSensorRotationAngle() const;
    double GetWeight             () const;
    double GetMaxSlope           () const;
    double GetSlopeDeceleration  () const;
    bool   HasWeaponTypes        () const;
    //@}

    //! @name Load/unload - Transport
    //@{
    bool   CanTransportHumans             () const;
    double GetNbrHumansLoadedPerTimeStep  () const;
    double GetNbrHumansUnloadedPerTimeStep() const;

    bool   CanTransportPion                           () const;
    double GetPionTransporterWeightCapacity           () const;
    double GetPionTransporterWeightLoadedPerTimeStep  () const;
    double GetPionTransporterWeightUnloadedPerTimeStep() const;
    bool CanTransportDestroyed() const;

    bool   CanTransportCrowd                     () const;
    unsigned int GetCrowdTransporterCapacity     () const;
    double GetCrowdTransporterLoadedPerTimeStep  () const;
    double GetCrowdTransporterUnloadedPerTimeStep() const;
    //@}

    //! @name Objects
    //@{
    bool HasConstructionSpeeds() const;
    double GetConstructionSpeed( const TerrainData& data ) const;

    bool CanConstruct( const MIL_ObjectType_ABC& objectType ) const;
    bool CanDestroy  ( const MIL_ObjectType_ABC& objectType ) const;
    bool CanMine     ( const MIL_ObjectType_ABC& objectType ) const;
    bool CanDemine   ( const MIL_ObjectType_ABC& objectType ) const;
    bool CanBypass   ( const MIL_ObjectType_ABC& objectType, bool bObjectIsMined ) const;
    bool CanExtinguish( const MIL_ObjectType_ABC& objectType ) const;
    bool CanRemoveFromPath( const MIL_ObjectType_ABC& objectType, bool isObjectMined ) const;

    double GetConstructionTime( const MIL_ObjectType_ABC& objectType, double rSizeCoef ) const;
    double GetDestructionTime ( const MIL_ObjectType_ABC& objectType, double rSizeCoef ) const;
    double GetMiningTime      ( const MIL_ObjectType_ABC& objectType ) const;
    double GetDeminingTime    ( const MIL_ObjectType_ABC& objectType ) const;
    double GetExtinguishingTime  ( const MIL_ObjectType_ABC& objectType ) const;
    double GetBypassTime      ( const MIL_ObjectType_ABC& objectType, double rSizeCoef, bool bObjectIsMined ) const;
    //@}

    //! @name Operations
    //@{
    virtual double GetDangerosity                ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rDistBtwSourceAndTarget, bool bUseAmmo, const PHY_MaterialCompositionType* material ) const;
    virtual double GetMaxRangeToFireOn           ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH, const PHY_DotationCategory* dotation, bool useAmmo ) const;
    virtual double GetMinRangeToFireOn           ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH               ) const;
    virtual double GetMaxRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const;
    virtual double GetMinRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const;
            double GetMaxRangeToIndirectFire     ( const MIL_Agent_ABC& firer, const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
            double GetMinRangeToIndirectFire     ( const MIL_Agent_ABC& firer, const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
            double GetMaxRangeToIndirectFire     ( const MIL_Agent_ABC& firer ) const;
            double GetMaxRangeToDirectFire       () const;
    virtual double GetAttritionIndexComposante   ( const PHY_MaterialCompositionType& material ) const;
    virtual double GetMaxRangeToFire             ( const MIL_Agent_ABC&  pion, double rWantedPH ) const;
    //@}

    //! @name Sensors / radars
    //@{
    template< typename T >
    void ApplyOnRadars ( T& func ) const
    {
        for( auto it = radarTypes_.begin(); it != radarTypes_.end(); ++it )
            func( *it->first, it->second );
    }
    //@}

    //! @name Logistic - maintenance
    //@{
    bool   CanRepair              ( const PHY_Breakdown& breakdown ) const;
    bool   CanRepair              () const;
    bool   CanHaul                () const;
    bool   CanHaul1               ( const PHY_ComposanteTypePion& type ) const;
    double GetHaulerWeightCapacity() const;
    double GetHaulerLoadingTime   () const;
    double GetHaulerUnloadingTime () const;

    const PHY_BreakdownType* GetRandomBreakdownType   () const;
    const PHY_BreakdownType* GetAttritionBreakdownType() const;
    bool CanHaveBreakdown() const;
    bool CanHaveBreakdown( const PHY_BreakdownType* ) const;
    //@}

    //! @name Logistic - medical
    //@{
    bool         CanEvacuateCasualties                       () const;
    bool         CanEvacuateCasualty                         ( const Human_ABC& human ) const;
    bool         CanCollectCasualties                        () const;
    bool         CanCollectCasualty                          ( const Human_ABC& human ) const;
    bool         CanHealHuman                                ( const Human_ABC& human ) const;
    bool         CanHealHumans                               () const;
    bool         CanDiagnoseHumans                           () const;
    bool         CanSortHumans                               () const;
    unsigned int GetAmbulanceCollectionCapacity              () const;
    double       GetNbrHumansLoadedForCollectionPerTimeStep  () const;
    double       GetNbrHumansUnloadedForCollectionPerTimeStep() const;
    unsigned int GetAmbulanceEvacuationCapacity              () const;
    double       GetNbrHumansLoadedForEvacuationPerTimeStep  () const;
    double       GetNbrHumansUnloadedForEvacuationPerTimeStep() const;
    unsigned int GetHealingTime                              ( const Human_ABC& human ) const;
    unsigned int Heal                                        (       Human_ABC& human ) const;
    //@}

    //! @name Logistic - supply
    //@{
          bool                CanTransportStock               ( const PHY_DotationCategory& dotationCategory ) const;  // Indépendamment du fait que la composante peut faire partie d'un convoi
          void                GetStockTransporterCapacity     ( double& rWeightMax, double& rVolumeMax ) const;
    std::pair< double, double > GetStockTransporterCapacity() const;
          unsigned int        GetStockTransporterLoadingTime  () const;
          unsigned int        GetStockTransporterUnloadingTime() const;
    const PHY_DotationNature* GetStockTransporterNature       () const;
    //@}

    //! @name Logistic - aviation resource quota
    //@{
    double GetAviationResourceQuota( E_AviationRange aviationRange, const PHY_DotationType* dotationType, const PHY_AmmoDotationClass* ammoClass ) const;
    //@}

    //! @name Active Protection
    //@{
    void   UseAmmunition( const PHY_DotationCategory&, MIL_Agent_ABC& pion ) const;
    double GetPHModifier( const PHY_DotationCategory&, MIL_Agent_ABC& pion ) const;
    bool   CounterIndirectFire( const PHY_DotationCategory&, MIL_Agent_ABC& pion ) const;
    bool   DestroyIndirectFire( const PHY_DotationCategory&, MIL_Agent_ABC& pion ) const;
    //@}

    //! @name Disasters
    //@{
    double GetDisasterImpact( double value ) const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_ComposanteTypePion& rhs ) const;
    bool operator!=( const PHY_ComposanteTypePion& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, PHY_ComposanteTypePion* >   T_ComposanteTypeMap;
    typedef tools::Map< const PHY_WeaponType*, bool >          T_WeaponTypeMap;
    typedef tools::Map< const PHY_SensorType*, double >        T_SensorTypeMap;
    typedef tools::Map< const PHY_RadarType*, double >         T_RadarTypeMap;
    typedef std::vector< const PHY_ComposanteTypeObjectData* > T_ObjectDataVector;
    typedef std::vector< PHY_DotationConsumptions* >           T_ConsumptionVector;

    struct sNTICapability
    {
        sNTICapability();
        sNTICapability( const PHY_MaintenanceLevel& maintenanceLevel );
        bool operator< ( const sNTICapability& rhs ) const;

        bool CanRepair( const PHY_Breakdown& breakdown ) const;

        const PHY_MaintenanceLevel* pMaintenanceLevel_;
              bool                  bMobility_;
              bool                  bElectronic_;
              unsigned int          nMaxTime_;
    };

    typedef std::set< sNTICapability > T_NTICapabilitySet;

    typedef std::vector< bool > T_WoundCapabilityVector;

    friend struct sSummarizer;
    struct sBreakdownTypeProbability
    {
        sBreakdownTypeProbability( const PHY_BreakdownType& breakdown, double rProbabilityBound );

        const PHY_BreakdownType* pBreakdownType_;
              double             rProbabilityBound_;
    };
    typedef std::vector< sBreakdownTypeProbability >   T_BreakdownTypeProbabilityVector;
    typedef std::vector< const PHY_ActiveProtection* > T_ActiveProtectionVector;

    struct sAviationResourceQuota
    {
        sAviationResourceQuota() : dotationType_( 0 )
                                 , ammoClass_( 0 )
                                 , quota_( 0 )
        {
            // NOTHING
        }
        const PHY_DotationType* dotationType_;
        const PHY_AmmoDotationClass* ammoClass_;
        double quota_;
    };
    typedef std::vector< sAviationResourceQuota > T_AviationResourceQuota;
    typedef std::map< E_AviationRange, T_AviationResourceQuota > T_AviationResourceQuotas;
    //@}

private:
    //! @name Init
    //@{
    void InitializeAttritionBreakdownTypes( xml::xistream& xis );
    void InitializeBreakdown              ( xml::xistream& xis );
    void InitializeConsumptions           ( xml::xistream& xis );
    void InitializeAviationQuotas         ( xml::xistream& xis );
    void InitializeLogistic               ( xml::xistream& xis );
    void InitializeLogisticMaintenance    ( xml::xistream& xis );
    void InitializeLogisticMedical        ( xml::xistream& xis );
    void InitializeLogisticSupply         ( xml::xistream& xis );
    void InitializeObjects                ( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    void InitializeProtections            ( xml::xistream& xis );
    void InitializeRadars                 ( xml::xistream& xis );
    void InitializeRandomBreakdownTypes   ( xml::xistream& xis );
    void InitializeSensors                ( xml::xistream& xis );
    void InitializeTransport              ( xml::xistream& xis );
    void InitializeWeapons                ( xml::xistream& xis );
    void InitializeDisasterImpacts        ( xml::xistream& xis );
    bool ReadWoundCapabilities            ( xml::xistream& xis, T_WoundCapabilityVector& container, const std::string attributeName ) const;
    //@}

    //! @name Tools
    //@{
    const PHY_BreakdownType* GetBreakdownType( const T_BreakdownTypeProbabilityVector& probasVector ) const;
    //@}
    //! @name Helpers
    //@{
    static void ReadElement      ( xml::xistream& xis, const MIL_Time_ABC& time, const ObjectTypeResolver_ABC& resolver );
    void InitializeBreakdownTypes( xml::xistream& xis );
    void ReadWeaponSystem        ( xml::xistream& xis );
    void ReadSensor              ( xml::xistream& xis );
    void ReadRadar               ( xml::xistream& xis );
    void ReadTransportCrew       ( xml::xistream& xis );
    void ReadTransportUnit       ( xml::xistream& xis );
    void ReadTransportCrowd      ( xml::xistream& xis );
    void ReadObject              ( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    void ReadConsumption         ( xml::xistream& xis );
    void ReadAviationQuotas      ( xml::xistream& xis, T_AviationResourceQuota& resourceQuota );
    void ReadMaintenance         ( xml::xistream& xis );
    void ReadTowing              ( xml::xistream& xis );
    void ReadRepairing           ( xml::xistream& xis );
    void ReadLogistic            ( xml::xistream& xis );
    void ReadSupply              ( xml::xistream& xis );
    void ReadRelieving           ( xml::xistream& xis );
    void ReadCollecting          ( xml::xistream& xis );
    void ReadCaring              ( xml::xistream& xis );
    void ReadActiveProtection    ( xml::xistream& xis );
    //@}

private:
    const MIL_Time_ABC&          time_;
    const PHY_Speeds             speeds_;
          double                 rMaxSlope_;
          double                 rSlopeDeceleration_;
    const PHY_DotationCapacities dotationCapacities_;
          T_WeaponTypeMap        weaponTypes_;
          T_SensorTypeMap        sensorTypes_;
          T_RadarTypeMap         radarTypes_;
          double                 rSensorRotationAngle_;
          T_ObjectDataVector     objectData_;
          T_ConsumptionVector    consumptions_;

    // Humans
    double rNbrHumansLoadedPerTimeStep_;
    double rNbrHumansUnloadedPerTimeStep_;

    // Transport
    double rPionTransporterWeightCapacity_;
    double rPionTransporterWeightLoadedPerTimeStep_;
    double rPionTransporterWeightUnloadedPerTimeStep_;
    bool bCanTransportDestroyed_;

    // Crowd
    unsigned int nCrowdTransporterCapacity_;
    double rCrowdTransporterLoadedPerTimeStep_;
    double rCrowdTransporterUnloadedPerTimeStep_;

    double  rWeight_;

    // Logistic : Breakdowns
    T_BreakdownTypeProbabilityVector randomBreakdownTypeProbabilities_;
    T_BreakdownTypeProbabilityVector attritionBreakdownTypeProbabilities_;

    // Logistic : maintenance
    double            rHaulerWeightCapacity_;
    double            rHaulerLoadingTime_;
    double            rHaulerUnloadingTime_;
    T_NTICapabilitySet  ntiCapabilities_;

    // Logistic : medical $$$$$A SPLITTER
    T_WoundCapabilityVector woundCollectionCapabilities_;
    bool                    bCanCollectMentalDiseases_;
    bool                    bCanCollectContaminated_;
    bool                    bCanEvacuateMentalDiseases_;
    bool                    bCanEvacuateContaminated_;
    bool                    bCanHealWounds_;
    bool                    bCanHealMentalDiseases_;
    bool                    bCanHealContaminated_;
    bool                    bCanSortHumans_;
    bool                    bCanDiagnoseHumans_;
    unsigned int            nAmbulanceCollectionCapacity_;
    double                  rNbrHumansLoadedForCollectionPerTimeStep_;
    double                  rNbrHumansUnloadedForCollectionPerTimeStep_;
    T_WoundCapabilityVector woundEvacuationCapabilities_;
    unsigned int            nAmbulanceEvacuationCapacity_;
    double                  rNbrHumansLoadedForEvacuationPerTimeStep_;
    double                  rNbrHumansUnloadedForEvacuationPerTimeStep_;
    T_WoundCapabilityVector woundHealingCapabilities_;

    // Logistic : supply
    const PHY_DotationNature* pStockTransporterNature_;
    double rStockTransporterMinWeightCapacity_;
    double rStockTransporterMaxWeightCapacity_;
    double rStockTransporterMinVolumeCapacity_;
    double rStockTransporterMaxVolumeCapacity_;

    // Logistic : aviation resources quotas
    T_AviationResourceQuotas aviationResourceQuotas_;

    // Active Protections
    T_ActiveProtectionVector activeProtections_;

    // Disasters
    std::unique_ptr< MT_InterpolatedFunction > disasterImpacts_;

private:
    static T_ComposanteTypeMap composantesTypes_;
};

namespace boost
{
namespace archive
{
    void save( MIL_CheckPointOutArchive& a, const PHY_ComposanteTypePion* t );
    void load( MIL_CheckPointInArchive& a, const PHY_ComposanteTypePion*& t );
}
}

#endif // __PHY_ComposanteType_h_
