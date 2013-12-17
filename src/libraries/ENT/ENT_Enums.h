// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Enums_h_
#define __ENT_Enums_h_

enum E_LayerTypes
{
    eLayerTypes_Agent,
    eLayerTypes_Objects,
    eLayerTypes_Automats,
    eLayerTypes_Formations,
    eLayerTypes_Populations,
    eLayerTypes_UrbanObjects,
    eLayerTypes_ResourceNetworks,
    eLayerTypes_TacticalLines,
    eLayerTypes_Ghosts,
    eLayerTypes_Fog,
    eLayerTypes_Parties,
    eLayerTypes_Drawers,
    eLayerTypes_Knowledges,
    eLayerTypes_Inhabitants,
    eNbrLayerTypes
};

enum E_LocationType
{
    eLocationType_Circle,
    eLocationType_Ellipse,
    eLocationType_Line,
    eLocationType_Rectangle,
    eLocationType_Polygon,
    eLocationType_Point,
    eLocationType_Sector,
    eNbrLocationType
};

enum E_DotationFamily
{
    eDotationFamily_Munition,
    eDotationFamily_Carburant,
    eDotationFamily_Explosif,
    eDotationFamily_Mine,
    eDotationFamily_Barbele,
    eDotationFamily_Piece,
    eDotationFamily_Ration,
    eDotationFamily_AgentExtincteur,
    eDotationFamily_Energy,
    eDotationFamily_Funeraire,
    eNbrDotationFamily
};

enum E_AmmunitionType
{
    eAmmunitionType_Obus,
    eAmmunitionType_MissileSol,
    eAmmunitionType_MissileAir,
    eAmmunitionType_Mitraille,
    eAmmunitionType_ALR,
    eNbrAmmunitionType
};

enum E_NatureLevel
{
    eNatureLevel_None,
    eNatureLevel_b,
    eNatureLevel_o,
    eNatureLevel_c,
    eNatureLevel_oo,
    eNatureLevel_ooo,
    eNatureLevel_i,
    eNatureLevel_ii,
    eNatureLevel_iii,
    eNatureLevel_x,
    eNatureLevel_xx,
    eNatureLevel_xxx,
    eNatureLevel_xxxx,
    eNatureLevel_xxxxx,
    eNbrNatureLevel
};

enum E_Diplomacy
{
    eDiplomacy_Inconnu,
    eDiplomacy_Ami,
    eDiplomacy_Ennemi,
    eDiplomacy_Neutre,
    eNbrDiplomacy
};

enum E_ForceRatioStatus
{
    eForceRatioStatus_None,
    eForceRatioStatus_Neutre,
    eForceRatioStatus_Favorable,
    eForceRatioStatus_Defavorable,
    eNbrForceRatioStatus
};

enum E_MeetingEngagementStatus
{
    eMeetingEngagementStatus_None,
    eMeetingEngagementStatus_EtatEsquive,
    eMeetingEngagementStatus_EtatFixe,
    eMeetingEngagementStatus_EtatPoursuiteMission,
    eNbrMeetingEngagementStatus
};

enum E_OperationalStatus
{
    eOperationalStatus_DetruitTotalement,
    eOperationalStatus_DetruitTactiquement,
    eOperationalStatus_Operationnel,
    eNbrOperationalStatus
};

enum E_Roe
{
    eRoe_TirLibre,
    eRoe_TirSurRiposte,
    eRoe_TirInterdit,
    eNbrRoe
};

enum E_UnitPosture
{
    eUnitPosture_PostureMouvement,
    eUnitPosture_PostureMouvementDiscret,
    eUnitPosture_PostureArret,
    eUnitPosture_PosturePosteReflexe,
    eUnitPosture_PosturePoste,
    eUnitPosture_PosturePosteAmenage,
    eUnitPosture_PosturePostePrepareGenie,
    eNbrUnitPosture
};

enum E_FireAvailability
{
    eFireAvailability_None,
    eFireAvailability_PretAuTir,
    eFireAvailability_Indisponible,
    eNbrFireAvailability
};

enum E_PopulationRoe
{
    ePopulationRoe_None,
    ePopulationRoe_EmploiForceInterdit,
    ePopulationRoe_MaintienADistanceParMoyensNonLetaux,
    ePopulationRoe_ArmesLetalesAutorisees,
    eNbrPopulationRoe
};

enum E_UnitTiredness
{
    eUnitTiredness_Normal,
    eUnitTiredness_Fatigue,
    eUnitTiredness_Epuise,
    eNbrUnitTiredness
};

enum E_UnitMorale
{
    eUnitMorale_Fanatique,
    eUnitMorale_Bon,
    eUnitMorale_Moyen,
    eUnitMorale_Mauvais,
    eNbrUnitMorale
};

enum E_UnitExperience
{
    eUnitExperience_Veteran,
    eUnitExperience_Experimente,
    eUnitExperience_Conscrit,
    eNbrUnitExperience
};

enum E_UnitStress
{
    eUnitStress_Calm,
    eUnitStress_Worried,
    eUnitStress_Stressed,
    eNbrUnitStress
};

enum E_LightingType
{
    eLightingType_JourSansNuage,
    eLightingType_JourPeuNuageux,
    eLightingType_JourMoyennementNuageux,
    eLightingType_JourAssezNuageux,
    eLightingType_JourTresNuageux,
    eLightingType_NuitPleineLune,
    eLightingType_NuitTroisQuartDeLune,
    eLightingType_NuitDemiLune,
    eLightingType_NuitQuartDeLune,
    eLightingType_NuitNouvelleLune,
    eLightingType_Eclairant,
    eNbrLightingType
};

enum E_WeatherType
{
    eWeatherType_None,
    eWeatherType_SandStorm,
    eWeatherType_Fog,
    eWeatherType_Drizzle,
    eWeatherType_Rain,
    eWeatherType_Snow,
    eWeatherType_Smoke,
    eNbrWeatherType
};

enum E_LogFuneralHandlingStatus
{
    eLogFuneralHandlingStatus_WaitingForHandling,
    eLogFuneralHandlingStatus_TransportingUnpackaged,
    eLogFuneralHandlingStatus_WaitingForPackaging,
    eLogFuneralHandlingStatus_Packaging,
    eLogFuneralHandlingStatus_WaitingForTransporter,
    eLogFuneralHandlingStatus_TransportingPackaged,
    eLogFuneralHandlingStatus_Finished,
    eNbrLogFuneralHandlingStatus
};

enum E_LogMaintenanceHandlingStatus
{
    eLogMaintenanceHandlingStatus_DeplacementVersChaine,
    eLogMaintenanceHandlingStatus_AttenteDisponibiliteRemorqueur,
    eLogMaintenanceHandlingStatus_RemorqueurDeplacementAller,
    eLogMaintenanceHandlingStatus_RemorqueurChargement,
    eLogMaintenanceHandlingStatus_RemorqueurDeplacementRetour,
    eLogMaintenanceHandlingStatus_RemorqueurDechargement,
    eLogMaintenanceHandlingStatus_Diagnostique,
    eLogMaintenanceHandlingStatus_AttentePriseEnChargeParNiveauSuperieur,
    eLogMaintenanceHandlingStatus_AttenteDisponibilitePieces,
    eLogMaintenanceHandlingStatus_AttenteDisponibiliteReparateur,
    eLogMaintenanceHandlingStatus_Reparation,
    eLogMaintenanceHandlingStatus_RetourPion,
    eLogMaintenanceHandlingStatus_Termine,
    eNbrLogMaintenanceHandlingStatus
};

enum E_LogSupplyHandlingStatus
{
    eLogSupplyHandlingStatus_ConvoiEnAttenteCamions,
    eLogSupplyHandlingStatus_ConvoiConstitution,
    eLogSupplyHandlingStatus_ConvoiDeplacementVersPointChargement,
    eLogSupplyHandlingStatus_ConvoiChargement,
    eLogSupplyHandlingStatus_ConvoiDeplacementVersPointDechargement,
    eLogSupplyHandlingStatus_ConvoiDechargement,
    eLogSupplyHandlingStatus_ConvoiDeplacementRetour,
    eLogSupplyHandlingStatus_Termine,
    eNbrLogSupplyHandlingStatus
};

enum E_LogMedicalHandlingStatus
{
    eLogMedicalHandlingStatus_AttenteDisponibiliteAmbulanceReleve,
    eLogMedicalHandlingStatus_AmbulanceReleveDeplacementAller,
    eLogMedicalHandlingStatus_AmbulanceReleveChargement,
    eLogMedicalHandlingStatus_AttenteChargementCompletAmbulanceReleve,
    eLogMedicalHandlingStatus_AmbulanceReleveDeplacementRetour,
    eLogMedicalHandlingStatus_AmbulanceReleveDechargement,
    eLogMedicalHandlingStatus_AttenteDisponibiliteMedecinPourDiagnostique,
    eLogMedicalHandlingStatus_Diagnostique,
    eLogMedicalHandlingStatus_RechercheSecteurTri,
    eLogMedicalHandlingStatus_AttenteDisponibiliteMedecinPourTri,
    eLogMedicalHandlingStatus_Tri,
    eLogMedicalHandlingStatus_RechercheSecteurSoin,
    eLogMedicalHandlingStatus_AttenteDisponibiliteMedecinPourSoin,
    eLogMedicalHandlingStatus_Soin,
    eLogMedicalHandlingStatus_Hospitalisation,
    eLogMedicalHandlingStatus_AttenteDisponibiliteAmbulanceRamassage,
    eLogMedicalHandlingStatus_AmbulanceRamassageChargement,
    eLogMedicalHandlingStatus_AttenteChargementCompletAmbulanceRamassage,
    eLogMedicalHandlingStatus_AmbulanceRamassageDeplacementAller,
    eLogMedicalHandlingStatus_AmbulanceRamassageDechargement,
    eLogMedicalHandlingStatus_Termine,
    eNbrLogMedicalHandlingStatus
};

enum E_ObstacleActivation
{
    eObstacleActivation_Activated,
    eObstacleActivation_Deactivated,
    eNbrObstacleActivation
};

enum E_PopulationAttitude
{
    ePopulationAttitude_Calme,
    ePopulationAttitude_Agitee,
    ePopulationAttitude_Excitee,
    ePopulationAttitude_Agressive,
    eNbrPopulationAttitude
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

enum E_CrossingType
{
    eBridge,
    eHighway,
    eMainRoad,
    eSecondaryRoad,
    eCountryRoad,
    eNothing,
    eNbrCrossingType,
};

enum E_EquipmentState
{
    eEquipmentState_Available,
    eEquipmentState_Destroyed,
    eEquipmentState_RepairableWithEvacuation,
    eEquipmentState_OnSiteFixable,
    eEquipmentState_InMaintenance,
    eEquipmentState_Prisonner,
    eNbrEquipmentState
};

enum E_HumanWound
{
    eHumanWound_NonBlesse,
    eHumanWound_Mort,
    eHumanWound_BlesseUrgence1,
    eHumanWound_BlesseUrgence2,
    eHumanWound_BlesseUrgence3,
    eHumanWound_BlesseUrgenceExtreme,
    eNbrHumanWound
};

enum E_HumanRank
{
    eHumanRank_Officier,
    eHumanRank_SousOfficer,
    eHumanRank_Mdr,
    eNbrHumanRank
};

enum E_HumanState // $$$$ ABR 2011-07-19: Here to replace HumanWounds
{
    eHumanState_Healthy,
    eHumanState_Injured,
    eHumanState_Dead,
    eNbrHumanState
};

enum E_HumanLocation
{
    eHumanLocation_Battlefield,
    eHumanLocation_Maintenance,
    eHumanLocation_Medical,
    eHumanLocation_Funeral,
    eNbrHumanLocation
};

enum E_InjuriesSeriousness
{
    eInjuriesSeriousness_U1,
    eInjuriesSeriousness_U2,
    eInjuriesSeriousness_U3,
    eInjuriesSeriousness_UE,
    eNbrInjuriesSeriousness
};

enum E_GhostType
{
    eGhostType_Agent,
    eGhostType_Automat,
    eGhostType_Invalid,
    eNbrGhostType
};

enum E_NbcState
{
    eNbcState_Liquid,
    eNbcState_Gaseous,
    eNbrNbcState
};

enum E_Modes
{
    eModes_None         = 0x0000,       // Filter

    eModes_Default      = 0x0001,       // Not connected

    eModes_Prepare      = 0x0002,
    eModes_Terrain      = 0x0004,
    eModes_LivingArea   = 0x0008,
    eModes_Preparation  = 0x000F,

    eModes_Gaming       = 0x0001 << 4,
    eModes_Planning     = 0x0002 << 4,
    eModes_Replay       = 0x0004 << 4,
    eModes_AllGaming    = 0x000F << 4,

    eModes_All          = 0xFFFF,       // Filter

    eNbrModes = 11 // hardcoded
};

enum E_AgentNbcSuit
{
    eAgentNone,
    eAgentNbcSuitLevel1,
    eAgentNbcSuitLevel2,
    eAgentNbcSuitLevel3,
    eAgentNbcSuitLevel4,
    eAgentNbcSuitLevel5,
    eNbrAgentNbcSuit
};

enum E_EventTypes
{
    eEventTypes_Order,
    eEventTypes_SupervisorAction,
    eEventTypes_Report,
    eEventTypes_Task,
    eEventTypes_Multimedia,
    eNbrEventTypes
};

enum E_MissionType
{
    eMissionType_Pawn,
    eMissionType_Automat,
    eMissionType_Population,
    eMissionType_FragOrder,
    eNbrMissionType
};

enum E_AviationRange
{
    eAviationRange_ShortRange,
    eAviationRange_MediumRange,
    eAviationRange_LongRange,
    eAviationRange_NoAmmunition,
    eNbrAviationRange
};

enum E_FuncLimaType
{
    eLimaFuncLD,    // Ligne de d�bouch�
    eLimaFuncLCA,   // Ligne de changement d'attitude
    eLimaFuncLC,    // Ligne de coordination
    eLimaFuncLI,    // Ligne � interdire
    eLimaFuncLO,    // Ligne d'objectif
    eLimaFuncLCAR,  // Ligne de coup d'arr�t
    eLimaFuncLR,    // Ligne de recueil
    eLimaFuncLDM,   // Ligne de d�but de mission
    eLimaFuncLFM,   // Ligne de fin de mission
    eLimaFuncLIA,   // Ligne d'identification acceuil
    eLimaFuncNbr    // Keep this at the end of the enum! No !
};

enum E_EventDockModes
{
    eEventDockModes_None,
    eEventDockModes_DisplayTriggered,
    eEventDockModes_EditTriggered,
    eEventDockModes_EditPlanned,
    eEventDockModes_Create,
    eNbrEventDockModes
};

#endif __ENT_Enums_h_
