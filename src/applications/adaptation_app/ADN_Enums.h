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
    eAgentTypePionOrganization,
    eAgentTypePionNotable,
    eAgentTypePionJournalist,
    eAgentTypePionCivilian,

    eNbrAgentTypePion
};

enum E_AgentTypeAutomate
{
    eAgentTypeAutomateSGTIA,
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
    eAgentTypeAutomateOrganization,
    eAgentTypeAutomateNotable,
    eAgentTypeAutomateJournalist,
    eAgentTypeAutomateCivilian,
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
    eSpeedImpact_VitesseBlocUrbain,
    eNbrSpeedImpact
};

enum E_CrossingHeight
{
    eCrossingHeight_Never,
    eCrossingHeight_LowAreas,
    eCrossingHeight_Always,
    eNbrCrossingHeight
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
    eA�roporte,
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
    eGuerreElectroniqueD�tection,
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

enum E_EquipmentState_ADN
{
    eEquipmentState_ADN_FixableWithEvac,
    eEquipmentState_ADN_FixableInPlace,
    eEquipmentState_ADN_Destroyed,
    eNbrEquipmentStateADN
};

enum E_TypeMunitionTirIndirect
{
    eTypeMunitionTirIndirect_Explosif,
    eTypeMunitionTirIndirect_Grenade,
    eTypeMunitionTirIndirect_Aced,
    eTypeMunitionTirIndirect_Fumigene,
    eTypeMunitionTirIndirect_Eclairant,
    eTypeMunitionTirIndirect_Mine,
    eTypeMunitionTirIndirect_Effect,
    eNbrTypeMunitionTirIndirect
};

enum E_MissionParameterType
{
    eMissionParameterTypeAgent,
    eMissionParameterTypeAutomate,
    eMissionParameterTypeBool,
    eMissionParameterTypeNumeric,
    eMissionParameterTypeString,
    eMissionParameterTypeDirection,
    eMissionParameterTypeGDH,
    eMissionParameterTypeEnumeration,
    eMissionParameterTypeNatureAtlas,
    eMissionParameterTypeAgentKnowledge,
    eMissionParameterTypeObjectKnowledge,
    eMissionParameterTypePopulationKnowledge,
    eMissionParameterTypeGenObject,
    eMissionParameterTypePath,
    eMissionParameterTypePoint,
    eMissionParameterTypePolygon,
    eMissionParameterTypeLocation,
    eMissionParameterTypeEquipmentType,
    eMissionParameterTypeDotationType,
    eMissionParameterTypeMaintenancePriorities,
    eMissionParameterTypeMedicalPriorities,
    eMissionParameterTypeIndirectFire,
    eMissionParameterTypeLimit,
    eMissionParameterTypePhaseLine,
    eMissionParameterTypeUrbanBlock,
    eMissionParameterTypeLocationComposite,
    eMissionParameterTypeResourceNetwork,

    eNbrMissionParameterType
};

enum E_AnchorType
{
    eNoAnchor,
    eStart,
    eEnd,
    eNbrAnchorType
};

enum E_ConstructorType
{
    eRaw,
    eDensity,
    eNbrConstructorType
};

enum E_PropagationModel
{
    eInput,
    eFire,
    eCloud,
    eNbrPropagationModel
};

#endif // __ADN_Enums_h_
