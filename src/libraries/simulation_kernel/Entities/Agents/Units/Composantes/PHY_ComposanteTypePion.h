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

#include "MIL.h"

#include "PHY_ComposanteType_ABC.h"

#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCapacities.h"

#include "game_asn/Simulation.h"

namespace xml
{
    class xistream;
}

class PHY_WeaponType;
class PHY_Weapon;
class PHY_SensorType;
class PHY_Sensor;
class PHY_RadarType;
class PHY_ComposantePion;
class PHY_ComposanteTypeObjectData;
class PHY_ConsumptionType;
class PHY_Posture;
class PHY_Human;
class PHY_DotationConsumptions;
class PHY_DotationNature;
class MIL_ObjectType_ABC;
class MIL_Object_ABC;
class PHY_RolePion_Dotations;
class PHY_MaintenanceLevel;
class PHY_Breakdown;
class PHY_BreakdownType;
class PHY_RolePion_Composantes;
class MIL_AgentPion;
class MIL_Time_ABC;

// =============================================================================
// @class  PHY_ComposanteTypePion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposanteTypePion : public PHY_ComposanteType_ABC
{
    MT_COPYNOTALLOWED( PHY_ComposanteTypePion )

public:
             PHY_ComposanteTypePion( const MIL_Time_ABC& time, const std::string& strName, xml::xistream& xis );
    virtual ~PHY_ComposanteTypePion();

    //! @name Static initialization
    //@{
    static void Initialize( const MIL_Time_ABC& time, xml::xistream& xis );
    static void Terminate ();

    static const PHY_ComposanteTypePion* Find( const std::string& strName );
    static const PHY_ComposanteTypePion* Find( ASN1T_EquipmentType nAsnID );
    //@}

    //! @name Instanciation
    //@{
    PHY_ComposantePion& InstanciateComposante( PHY_RolePion_Composantes& role, uint nNbrHumanInCrew, bool bMajor, bool bLoadable, bool bCanBePartOfConvoy ) const;
    void                InstanciateWeapons   ( std::back_insert_iterator < std::vector< PHY_Weapon* > > inserter ) const;
    void                InstanciateSensors   ( std::back_insert_iterator < std::vector< PHY_Sensor* > > inserter ) const;
    //@}

    //! @name Dotations
    //@{
    const PHY_DotationCapacities&   GetDotationCapacities  () const;
    const PHY_DotationConsumptions* GetDotationConsumptions( const PHY_ConsumptionType& consumptionMode ) const;
    const PHY_ConsumptionType&      GetConsumptionMode     ( const MIL_ObjectType_ABC& objectType ) const;
    //@}

    //! @name Accessors
    //@{
    MT_Float GetMaxSpeed           () const;
    MT_Float GetMaxSpeed           ( const TerrainData& data ) const;
    MT_Float GetMaxSpeed           ( const MIL_Object_ABC& object ) const;
    MT_Float GetSensorRotationAngle() const;
    MT_Float GetWeight             () const;
    MT_Float GetMaxSlope           () const;
    //@}

    //! @name Load/unload - Transport
    //@{
    bool     CanTransportHumans             () const;
    MT_Float GetNbrHumansLoadedPerTimeStep  () const;
    MT_Float GetNbrHumansUnloadedPerTimeStep() const;

    bool     CanTransportPion                           () const;
    MT_Float GetPionTransporterWeightCapacity           () const;
    MT_Float GetPionTransporterWeightLoadedPerTimeStep  () const;
    MT_Float GetPionTransporterWeightUnloadedPerTimeStep() const;
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

    //! @name Operations
    //@{
    virtual MT_Float GetDangerosity                ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rDistBtwSourceAndTarget ) const;
    virtual MT_Float GetMaxRangeToFireOn           ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH               ) const;
    virtual MT_Float GetMinRangeToFireOn           ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH               ) const;
    virtual MT_Float GetMaxRangeToFireOnWithPosture( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
    virtual MT_Float GetMinRangeToFireOnWithPosture( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
            MT_Float GetMaxRangeToIndirectFire     ( const MIL_Agent_ABC& firer, const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
            MT_Float GetMinRangeToIndirectFire     ( const MIL_Agent_ABC& firer, const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
    //@}

    //! @name Sensors / radars
    //@{
    template< typename T > void ApplyOnRadars ( T& func ) const;
    //@}

    //! @name Logistic - maintenance
    //@{
    bool     CanRepair              ( const PHY_Breakdown& breakdown ) const;
    bool     CanRepair              () const;
    bool     CanHaul                () const;
    bool     CanHaul                ( const PHY_ComposanteTypePion& type ) const;
    MT_Float GetHaulerWeightCapacity() const;    
    MT_Float GetHaulerLoadingTime   () const;
    MT_Float GetHaulerUnloadingTime () const;
    
    const PHY_BreakdownType& GetRandomBreakdownType   () const;
    const PHY_BreakdownType& GetAttritionBreakdownType() const;
    //@}

    //! @name Logistic - medical
    //@{
    bool     CanEvacuateCasualties                       () const;
    bool     CanEvacuateCasualty                         ( const PHY_Human& human ) const;
    bool     CanCollectCasualties                        () const;
    bool     CanCollectCasualty                          ( const PHY_Human& human ) const;
    bool     CanHealHuman                                ( const PHY_Human& human ) const;
    bool     CanHealHumans                               () const;
    bool     CanDiagnoseHumans                           () const;
    bool     CanSortHumans                               () const;
    uint     GetAmbulanceCollectionCapacity              () const;
    MT_Float GetNbrHumansLoadedForCollectionPerTimeStep  () const;
    MT_Float GetNbrHumansUnloadedForCollectionPerTimeStep() const;
    uint     GetAmbulanceEvacuationCapacity              () const;
    MT_Float GetNbrHumansLoadedForEvacuationPerTimeStep  () const;
    MT_Float GetNbrHumansUnloadedForEvacuationPerTimeStep() const;
    uint     GetHealingTime                              ( const PHY_Human& human ) const;
    uint     Heal                                        (       PHY_Human& human ) const;
    //@}

    //! @name Logistic - supply
    //@{
          bool                 CanTransportStock               ( const PHY_DotationCategory& dotationCategory ) const;  // Indépendamment du fait que la composante peut faire partie d'un convoi
          void                 GetStockTransporterCapacity     ( MT_Float& rWeightMax, MT_Float& rVolumeMax ) const;
          uint                 GetStockTransporterLoadingTime  () const;
          uint                 GetStockTransporterUnloadingTime() const;
    const PHY_DotationNature*  GetStockTransporterNature       () const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_ComposanteTypePion& rhs ) const;
    bool operator!=( const PHY_ComposanteTypePion& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, PHY_ComposanteTypePion*, sCaseInsensitiveLess > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                                    CIT_ComposanteTypeMap;

    typedef std::map< const PHY_WeaponType*, bool > T_WeaponTypeMap;
    typedef T_WeaponTypeMap::const_iterator         CIT_WeaponTypeMap;

    typedef std::map< const PHY_SensorType*, MT_Float > T_SensorTypeMap;
    typedef T_SensorTypeMap::const_iterator             CIT_SensorTypeMap;

    typedef std::set< const PHY_RadarType* > T_RadarTypeSet;
    typedef T_RadarTypeSet::const_iterator   CIT_RadarTypeSet;

    typedef std::vector< const PHY_ComposanteTypeObjectData* >  T_ObjectDataVector;
    typedef T_ObjectDataVector::const_iterator                  CIT_ObjectDataVector;

    typedef std::vector< PHY_DotationConsumptions* >  T_ConsumptionVector;
    typedef T_ConsumptionVector::const_iterator       CIT_ConsumptionVector;

    struct sNTICapability
    {
        sNTICapability();
        sNTICapability( const PHY_MaintenanceLevel& maintenanceLevel );
        bool operator< ( const sNTICapability& rhs ) const;

        bool CanRepair( const PHY_Breakdown& breakdown ) const;

        const PHY_MaintenanceLevel* pMaintenanceLevel_;
              bool                  bMobility_;
              bool                  bElectronic_;
              uint                  nMaxTime_;
    };

    typedef std::set< sNTICapability >         T_NTICapabilitySet;
    typedef T_NTICapabilitySet::const_iterator CIT_NTICapabilitySet;

    typedef std::vector< bool > T_WoundCapabilityVector;

    friend struct sSummarizer;
    struct sBreakdownTypeProbability
    {
        sBreakdownTypeProbability( const PHY_BreakdownType& breakdown, MT_Float rProbabilityBound );

        const PHY_BreakdownType* pBreakdownType_;
              MT_Float           rProbabilityBound_;
    };
    typedef std::vector< sBreakdownTypeProbability >         T_BreakdownTypeProbabilityVector;
    typedef T_BreakdownTypeProbabilityVector::const_iterator CIT_BreakdownTypeProbabilityVector;
    //@}

private:
    //! @name Init
    //@{
    void InitializeWeapons                ( xml::xistream& xis );
    void InitializeTransport              ( xml::xistream& xis );
    void InitializeSensors                ( xml::xistream& xis );
    void InitializeRadars                 ( xml::xistream& xis );
    void InitializeObjects                ( xml::xistream& xis );
    void InitializeConsumptions           ( xml::xistream& xis );
    void InitializeLogistic               ( xml::xistream& xis );
    void InitializeLogisticMaintenance    ( xml::xistream& xis );
    void InitializeLogisticMedical        ( xml::xistream& xis ); 
    void InitializeLogisticSupply         ( xml::xistream& xis );
    void InitializeBreakdown              ( xml::xistream& xis );
    void InitializeRandomBreakdownTypes   ( xml::xistream& xis );
    void InitializeAttritionBreakdownTypes( xml::xistream& xis );
    bool ReadWoundCapabilities            ( xml::xistream& xis, T_WoundCapabilityVector& container, const std::string attributeName ) const;
    //@}
    
    //! @name Tools
    //@{
    const PHY_BreakdownType& GetBreakdownType( const T_BreakdownTypeProbabilityVector& probasVector ) const;
    //@}
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadElement      ( xml::xistream& xis, const MIL_Time_ABC& time );
    void InitializeBreakdownTypes( xml::xistream& xis );
    void ReadWeaponSystem        ( xml::xistream& xis );
    void ReadSensor              ( xml::xistream& xis );
    void ReadRadar               ( xml::xistream& xis );
    void ReadTransportCrew       ( xml::xistream& xis );
    void ReadTransportUnit       ( xml::xistream& xis );
    void ReadObject              ( xml::xistream& xis );
    void ReadConsumption         ( xml::xistream& xis );
    void ReadMaintenance         ( xml::xistream& xis );
    void ReadTowing              ( xml::xistream& xis );
    void ReadRepairing           ( xml::xistream& xis );
    void ReadLogistic            ( xml::xistream& xis );
    void ReadSupply              ( xml::xistream& xis );
    void ReadRelieving           ( xml::xistream& xis );
    void ReadCollecting          ( xml::xistream& xis );
    void ReadCaring              ( xml::xistream& xis );
    //@}

private:
    const MIL_Time_ABC&          time_;
    const PHY_Speeds             speeds_;
          MT_Float               rMaxSlope_;
    const PHY_DotationCapacities dotationCapacities_;         
          T_WeaponTypeMap        weaponTypes_;
          T_SensorTypeMap        sensorTypes_;
          T_RadarTypeSet         radarTypes_;
          MT_Float               rSensorRotationAngle_;
          T_ObjectDataVector     objectData_;
          T_ConsumptionVector    consumptions_;

    // Humans
    MT_Float rNbrHumansLoadedPerTimeStep_;
    MT_Float rNbrHumansUnloadedPerTimeStep_;

    // Transport
    MT_Float rPionTransporterWeightCapacity_;
    MT_Float rPionTransporterWeightLoadedPerTimeStep_;
    MT_Float rPionTransporterWeightUnloadedPerTimeStep_;

    MT_Float  rWeight_;

    // Logistic : Breakdowns
    T_BreakdownTypeProbabilityVector randomBreakdownTypeProbabilities_;
    T_BreakdownTypeProbabilityVector attritionBreakdownTypeProbabilities_;    

    // Logistic : maintenance
    MT_Float            rHaulerWeightCapacity_;
    MT_Float            rHaulerLoadingTime_;
    MT_Float            rHaulerUnloadingTime_;
    T_NTICapabilitySet  ntiCapabilities_;

    // Logistic : medical $$$$$A SPLITTER
    T_WoundCapabilityVector woundCollectionCapabilities_;
    bool                    bCanCollectMentalDiseases_;
    bool                    bCanCollectContaminated_;
    uint                    nAmbulanceCollectionCapacity_;
    MT_Float                rNbrHumansLoadedForCollectionPerTimeStep_;
    MT_Float                rNbrHumansUnloadedForCollectionPerTimeStep_;
    T_WoundCapabilityVector woundEvacuationCapabilities_;
    bool                    bCanEvacuateMentalDiseases_;
    bool                    bCanEvacuateContaminated_;
    uint                    nAmbulanceEvacuationCapacity_;
    MT_Float                rNbrHumansLoadedForEvacuationPerTimeStep_;
    MT_Float                rNbrHumansUnloadedForEvacuationPerTimeStep_;
    T_WoundCapabilityVector woundHealingCapabilities_;
    bool                    bCanHealWounds_;
    bool                    bCanHealMentalDiseases_;
    bool                    bCanHealContaminated_;
    bool                    bCanSortHumans_;
    bool                    bCanDiagnoseHumans_;

    // Logistic : supply
    const PHY_DotationNature* pStockTransporterNature_;
          MT_Float            rStockTransporterWeightCapacity_;
          MT_Float            rStockTransporterVolumeCapacity_;

private:
    static T_ComposanteTypeMap composantesTypes_;
    static MT_Random           randomGenerator_;
};

#include "PHY_ComposanteTypePion.inl"

#endif // __PHY_ComposanteType_h_
