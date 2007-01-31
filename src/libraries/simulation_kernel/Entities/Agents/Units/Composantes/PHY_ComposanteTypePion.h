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

#include "game_asn/Asn.h"

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
class MIL_RealObjectType;
class MIL_RealObject_ABC;
class PHY_RolePion_Dotations;
class PHY_MaintenanceLevel;
class PHY_Breakdown;
class PHY_BreakdownType;
class PHY_RolePion_Composantes;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ComposanteTypePion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposanteTypePion : public PHY_ComposanteType_ABC
{
    MT_COPYNOTALLOWED( PHY_ComposanteTypePion )

public:
             PHY_ComposanteTypePion( const std::string& strName, MIL_InputArchive& archive );
    virtual ~PHY_ComposanteTypePion();

    //! @name Static initialization
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();

    static const PHY_ComposanteTypePion* Find( const std::string& strName );
    static const PHY_ComposanteTypePion* Find( ASN1T_TypeEquipement nAsnID );
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
    const PHY_ConsumptionType&      GetConsumptionMode     ( const MIL_RealObjectType& objectType ) const;
    //@}

    //! @name Accessors
    //@{
    MT_Float GetMaxSpeed           () const;
    MT_Float GetMaxSpeed           ( const TerrainData& data ) const;
    MT_Float GetMaxSpeed           ( const MIL_RealObject_ABC& object ) const;
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
    bool     CanCollectCasualties                        () const;
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

    typedef std::vector< const PHY_ComposanteTypeObjectData* > T_ObjectDataVector;
    typedef T_ObjectDataVector::const_iterator                 CIT_ObjectDataVector;

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

    typedef std::vector< bool > T_WoundHealingCapabilityVector;

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
    void InitializeWeapons                ( MIL_InputArchive& archive );
    void InitializeTransport              ( MIL_InputArchive& archive );
    void InitializeSensors                ( MIL_InputArchive& archive );
    void InitializeRadars                 ( MIL_InputArchive& archive );
    void InitializeObjects                ( MIL_InputArchive& archive );
    void InitializeConsumptions           ( MIL_InputArchive& archive );
    void InitializeLogistic               ( MIL_InputArchive& archive );
    void InitializeLogisticMaintenance    ( MIL_InputArchive& archive );
    void InitializeLogisticMedical        ( MIL_InputArchive& archive ); 
    void InitializeLogisticSupply         ( MIL_InputArchive& archive );
    void InitializeBreakdownTypes         ( MIL_InputArchive& archive );
    void InitializeRandomBreakdownTypes   ( MIL_InputArchive& archive );
    void InitializeAttritionBreakdownTypes( MIL_InputArchive& archive );
    //@}
    
    //! @name Tools
    //@{
    const PHY_BreakdownType& GetBreakdownType( const T_BreakdownTypeProbabilityVector& probasVector ) const;
    //@}

private:
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

    // Logistic : medical
    uint                            nAmbulanceCollectionCapacity_;
    MT_Float                        rNbrHumansLoadedForCollectionPerTimeStep_;
    MT_Float                        rNbrHumansUnloadedForCollectionPerTimeStep_;
    uint                            nAmbulanceEvacuationCapacity_;
    MT_Float                        rNbrHumansLoadedForEvacuationPerTimeStep_;
    MT_Float                        rNbrHumansUnloadedForEvacuationPerTimeStep_;
    T_WoundHealingCapabilityVector  woundHealingCapabilities_;
    bool                            bCanHealWounds_;
    bool                            bCanHealMentalDiseases_;
    bool                            bCanHealContaminated_;
    bool                            bCanSortHumans_;
    bool                            bCanDiagnoseHumans_;

    // Logistic : supply
    const PHY_DotationNature*       pStockTransporterNature_;
          MT_Float                  rStockTransporterWeightCapacity_;
          MT_Float                  rStockTransporterVolumeCapacity_;

private:
    static T_ComposanteTypeMap composantesTypes_;
    static MT_Random           randomGenerator_;
};

#include "PHY_ComposanteTypePion.inl"

#endif // __PHY_ComposanteType_h_
