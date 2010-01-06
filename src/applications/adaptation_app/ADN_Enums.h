//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Enums.h $
// $Author: Ape $
// $Modtime: 7/06/05 10:15 $
// $Revision: 5 $
// $Workfile: ADN_Enums.h $
//
//*****************************************************************************

#ifndef __ADN_Enums_h_
#define __ADN_Enums_h_

#include "ENT/ENT_Enums_Gen.h"
#include "ADN_Enums_Gen.h"

enum E_OpenMode
{
    eOpenMode_Normal,
    eOpenMode_Admin
};

enum E_Location
{
    eLocation_Unknown,

    eLocation_Forest,
    eLocation_Plantation,
    eLocation_Swamp,
    eLocation_Dune,
    eLocation_Urban,
    eLocation_Water,
    eLocation_Ice,

    eLocation_ForestBorder,
    eLocation_UrbanBorder,
    eLocation_WaterBorder,
    eLocation_PlantationBorder,
    eLocation_SwampBorder,
    eLocation_DuneBorder,
    eLocation_IceBorder,

    eLocation_SmallRoad,
    eLocation_Motorway,
    eLocation_LargeRoad,
    eLocation_MediumRoad,
    eLocation_SmallRiver,
    eLocation_MediumRiver,
    eLocation_LargeRiver,
    eLocation_Bridge,
    eLocation_Cliff,
    eLocation_Railroad,
    eLocation_Crossroad,

    eLocation_Mountain,
    eLocation_MountainBorder,
    eNbrLocation
};


enum E_ConsumptionType
{
    eEngineStopped  = 0,
    eEngineStarted  = 1,
    eMoving         = 2,  
    eWorking        = 3,
    eNbrConsumptionType
};


enum E_TimeCategory
{
    eTimeTypeDay_NoCloud = 0,
    eTimeTypeDay_LightCloud,
    eTimeTypeDay_MiddleCloud,
    eTimeTypeDay_HeavyCloud,
    eTimeTypeDay_FullCloud,
    eTimeTypeNight_FullMoon,
    eTimeTypeNight_ThreeQuarterMoon,
    eTimeTypeNight_HalfMoon,
    eTimeTypeNight_OneQuarterMoon,
    eTimeTypeNight_NewMoon,
    eTimeTypeNight_LightingRocket,
    eNbrTimeCategory
};


enum E_VisionObject
{
    eVisionGround = 0,
    eVisionEmpty,
    eVisionForest,
    eVisionUrban,
    eNbrVisionObjects
};


enum E_KeyPoint
{
    eKeyPointForest = 0,
    eKeyPointUrban,
    eKeyPointCrossroads,
    eKeyPointBridge,
    eKeyPointLima,
    eNbrKeyPoints
};

enum E_AgentTypePion
{
    eAgentTypePionINF,
    eAgentTypePionASA,
    eAgentTypePionALAT,
    eAgentTypePionRENS,
    eAgentTypePionNBC,
    eAgentTypePionABC,
    eAgentTypePionGEN,
    eAgentTypePionASS,
    eAgentTypePionTRANS,
    eAgentTypePionLOGTC2,
    eAgentTypePionLOGBLDSante,
    eAgentTypePionLOGBLDMaintenance,
    eAgentTypePionLOGBLDRavitaillement,
    eAgentTypePionLOGBLTSante,
    eAgentTypePionLOGBLTMaintenance,
    eAgentTypePionLOGBLTRavitaillement,
    eAgentTypePionLOGConvoi,
    eAgentTypePionCIRCULATION,
    eAgentTypePionREFUGIE,
    eAgentTypePionMILICE,
    eAgentTypePionJOINT,
    eAgentTypePionASY,
    eAgentTypePionEmergency,

    eNbrAgentTypePion
};

enum E_AgentTypeAutomate
{
    eAgentTypeAutomateINF,
    eAgentTypeAutomateASA,
    eAgentTypeAutomateALAT,
    eAgentTypeAutomateRENS,
    eAgentTypeAutomateNBC,
    eAgentTypeAutomateABC,
    eAgentTypeAutomateGEN,
    eAgentTypeAutomateASS,
    eAgentTypeAutomateTRANS,
    eAgentTypeAutomateLOGTC2,
    eAgentTypeAutomateLOGBLDSante,
    eAgentTypeAutomateLOGBLDMaintenance,
    eAgentTypeAutomateLOGBLDRavitaillement,
    eAgentTypeAutomateLOGBLTSante,
    eAgentTypeAutomateLOGBLTMaintenance,
    eAgentTypeAutomateLOGBLTRavitaillement,
    eAgentTypeAutomateCIRCULATION,
    eAgentTypeAutomateREFUGIE,
    eAgentTypeAutomateMILICE,
    eAgentTypeAutomateJOINT,
    eAgentTypeAutomateASY,
    eAgentTypeAutomateEmergency,
    eNbrAgentTypeAutomate
};


enum E_NatureAtlasType
{
    eNatureAtlas_PC,
    eNatureAtlas_Armored,       
    eNatureAtlas_ASS,           
    eNatureAtlas_Log,           
    eNatureAtlas_Mortar,        
    eNatureAtlas_RocketLauncher,
    eNatureAtlas_None,
    eNatureAtlas_Vehicule,
    eNatureAtlas_PersonnelAPied,
    eNatureAtlas_Helicoptere,
    eNatureAtlas_ASA,
    eNatureAtlas_Indefini,
    eNbrNatureAtlas
};


enum 
{
    ePostureNeedTimeStart = 3
};


enum E_SpeedImpact
{
    eSpeedImpact_AuPlusLent,
    eSpeedImpact_VitesseMaxObjet,
    eSpeedImpact_VitesseMaxAgent,
    eNbrSpeedImpact
};


enum E_SensorWeatherModifiers
{
    eSensorWeatherModifiers_PasDePrecipitation,
    eSensorWeatherModifiers_TempeteSable,
    eSensorWeatherModifiers_Brouillard,
    eSensorWeatherModifiers_Crachin,
    eSensorWeatherModifiers_Pluie,
    eSensorWeatherModifiers_Neige,
    eSensorWeatherModifiers_Fumigene,
    eNbrSensorWeatherModifiers
};


enum E_DoctorSkills
{
    eDoctorSkills_UE,
    eDoctorSkills_U1,
    eDoctorSkills_U2,
    eDoctorSkills_U3,
    eNbrDoctorSkills
};


enum E_ProtectionType
{
    eProtectionType_Material,
    eProtectionType_Human,
    eNbrProtectionType
};


enum E_BreakdownType
{
    eBreakdownType_EA,
    eBreakdownType_M,
    eNbrBreakdownType
};

enum E_BreakdownNTI
{
    eBreakdownNTI_NTI1,
    eBreakdownNTI_NTI2,
    eBreakdownNTI_NTI3,
    eNbrBreakdownNTI,
};


enum E_MunitionType
{
    eMunitionType_Obus,
    eMunitionType_MissileSol,
    eMunitionType_MissileAir,
    eMunitionType_Mitraille,
    eNbrMunitionType
};


enum E_CapacityMission
{
    eCivilianLawEnforcement,
    eCommandOperations,
    eCouvertureAerienne,
    eJointIntelligence,
    eMoraleWelfareRecreation,
    eReglageElectronique,
    eRemplacement,
    eServiceMortuaire,
    eSignalsIntelligence,
    eTheatreMissileDefence,
    eUtilitaire,
    eAéroporte,
    eAffairesPubliques,
    eAffairesPubliques_Communication,
    eAffairesPubliques_JointInformation,
    eAlpin,
    eAmphibie,
    eArctique,
    eArtillerieReconnaissance,
    eAttaque,
    eAttaqueAerienne,
    eC2,
    eCheminDeFer,
    eCiblage,
    eContreEspionnage,
    eEclairage,
    eEntretien,
    eEntretienMateriel,
    eEntretienMissile,
    eEntretienOptoElectronique,
    eEspionnage,
    eEspionnageMilitaireAerialExploitation,
    eEspionnageMilitaireOperation,
    eEspionnageMilitaireTacticalExploitation,
    eEvacuationSanitaire,
    eFinance,
    eGenieCombat,
    eGenieConstruction,
    eGenieConstructionNavale,
    eGuerreElectronique,
    eGuerreElectroniqueInterception,
    eGuerreElectroniqueBrouillage,
    eGuerreElectroniqueDétection,
    eInterrogation,
    eMainDOeuvre,
    eMaintienDeLaPaix,
    eMarine,
    eMineDeContreMesure,
    eNBCBiologique,
    eNBCChimique,
    eNBCChimiqueFumee,
    eNBCDecontamination,
    eNBCDecontaminationChimique,
    eNBCNucleaire,
    eNonRenseigne,
    eObservation,
    eObservationCapteur,
    eObservationLonguePortee,
    eObservationMeteo,
    eObservationModuleTerrestre,
    ePurificationDeLEau,
    eRavitaillement_ClasseI,
    eRavitaillement_ClasseII,
    eRavitaillement_ClasseIIIAviation,
    eRavitaillement_ClasseIII,
    eRavitaillement_ClasseIV,
    eRavitaillement_ClasseV,
    eRavitaillement_Eau,
    eRavitaillementBlanchisserieBain,
    eRechercheEtSauvetage,
    eReconnaissance,
    eReconnaissanceAerienneTactique,
    eRecuperation,
    eServiceDuPersonnel,
    eServiceJuridique,
    eSupportAerienRapproche,
    eTransmissions_ForwardCommunications,
    eTransmissions_NodeCentre,
    eTransmissions_NodeLargeExtension,
    eTransmissions_NodeSmallExtension,
    eTransmissions_RadioRelay,
    eTransmissions_RadioTacticalSatellite,
    eTransmissions_RadioTeletype,
    eTransmissions_Support,
    eTransmissions_Radio,
    eTransport_AllocationDesMouvements,
    eTransport_APOD_APOE,
    eTransport_SPOD_SPOE,
    eTransportMissile,
    eVerrouillageDeCible,
    eVerrouillageDeCible_Flash,
    eVerrouillageDeCible_Radar,
    eVerrouillageDeCible_Sonore,
    eNbrCapacityMission
};

enum E_RadarType
{
    eRadarType_Radar,
    eRadarType_Ecoute,
    eRadarType_EcouteRadar,
    eNbrRadarType
};

enum E_StockCategory
{
    eStockCategory_UniteFuel,
    eStockCategory_UniteFireDirect,
    eStockCategory_UniteFireNotDirect,
    eStockCategory_UniteSupply,
    eStockCategory_Piece,
    eNbrStockCategory
};

enum E_EquipmentState
{
    eEquipmentState_FixableWithEvac,
    eEquipmentState_FixableInPlace,
    eEquipmentState_Destroyed,
    eNbrEquipmentState
};

enum E_TypeMunitionTirIndirect
{
    eTypeMunitionTirIndirect_Explosif,
    eTypeMunitionTirIndirect_Grenade,
    eTypeMunitionTirIndirect_Aced,
    eTypeMunitionTirIndirect_Fumigene,
    eTypeMunitionTirIndirect_Eclairant,
    eTypeMunitionTirIndirect_Mine,
    eTypeMunitionTirIndirect_Scrambling,
    eNbrTypeMunitionTirIndirect
};

enum E_MissionParameterType
{
    eMissionParameterTypeAgent,
    eMissionParameterTypeAgentList,
    eMissionParameterTypeAutomate,
    eMissionParameterTypeAutomateList,
    eMissionParameterTypeBool,
    eMissionParameterTypeNumeric,
    eMissionParameterTypeString,
    eMissionParameterTypeDirection,
    eMissionParameterTypeGDH, // $$$$ SBO 2007-05-24: 
    eMissionParameterTypeEnumeration,
    eMissionParameterTypeNatureAtlas,
    eMissionParameterTypeAgentKnowledge,
    eMissionParameterTypeAgentKnowledgeList,
    eMissionParameterTypeObjectKnowledge,
    eMissionParameterTypeObjectKnowledgeList,
    eMissionParameterTypePopulationKnowledge,
    eMissionParameterTypeGenObject,
    eMissionParameterTypeGenObjectList,
    eMissionParameterTypeObjective,
    eMissionParameterTypeObjectiveList,
    eMissionParameterTypePath,
    eMissionParameterTypePathList,
    eMissionParameterTypePoint,
    eMissionParameterTypePointList,
    eMissionParameterTypePolygon,
    eMissionParameterTypePolygonList,
    eMissionParameterTypeLocation,
    eMissionParameterTypeLocationList,
    eMissionParameterTypeEquipmentType,
    eMissionParameterTypeDotationType,
    eMissionParameterTypeMaintenancePriorities,
    eMissionParameterTypeMedicalPriorities,
    eMissionParameterTypeIndirectFire,
    eMissionParameterTypeLimit,
    eMissionParameterTypePhaseLineList,
    eMissionParameterTypeIntelligenceList,

    eNbrMissionParameterType
};

#endif // __ADN_Enums_h_
