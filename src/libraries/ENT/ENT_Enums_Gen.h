// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Enums_Gen_h_
#define __ENT_Enums_Gen_h_

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
    eLayerTypes_Inhabitants
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
    eDotationFamily_Munition        = 0,
    eDotationFamily_Carburant       = 1,
    eDotationFamily_Explosif        = 2,
    eDotationFamily_Mine            = 3,
    eDotationFamily_Barbele         = 4,
    eDotationFamily_Piece           = 5,
    eDotationFamily_Ration          = 6,
    eDotationFamily_AgentExtincteur = 7,
    eDotationFamily_Energy          = 8,
    eDotationFamily_Funeraire       = 9,
    eNbrDotationFamily              = 10
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
    eNatureLevel_None  = 0,
    eNatureLevel_b     = 1,
    eNatureLevel_o     = 2,
    eNatureLevel_c     = 3,
    eNatureLevel_oo    = 4,
    eNatureLevel_ooo   = 5,
    eNatureLevel_i     = 6,
    eNatureLevel_ii    = 7,
    eNatureLevel_iii   = 8,
    eNatureLevel_x     = 9,
    eNatureLevel_xx    = 10,
    eNatureLevel_xxx   = 11,
    eNatureLevel_xxxx  = 12,
    eNatureLevel_xxxxx = 13,
    eNbrNatureLevel    = 14
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
    eLightingType_JourSansNuage          = 0,
    eLightingType_JourPeuNuageux         = 1,
    eLightingType_JourMoyennementNuageux = 2,
    eLightingType_JourAssezNuageux       = 3,
    eLightingType_JourTresNuageux        = 4,
    eLightingType_NuitPleineLune         = 5,
    eLightingType_NuitTroisQuartDeLune   = 6,
    eLightingType_NuitDemiLune           = 7,
    eLightingType_NuitQuartDeLune        = 8,
    eLightingType_NuitNouvelleLune       = 9,
    eLightingType_Eclairant              = 10,
    eNbrLightingType                     = 11
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
    eEquipmentState_Available                = 0,
    eEquipmentState_Destroyed                = 1,
    eEquipmentState_RepairableWithEvacuation = 2,
    eEquipmentState_OnSiteFixable            = 3,
    eEquipmentState_InMaintenance            = 4,
    eEquipmentState_Prisonner                = 5,
    eNbrEquipmentState                       = 6
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
    eHumanRank_Officier    = 0,
    eHumanRank_SousOfficer = 1,
    eHumanRank_Mdr         = 2,
    eNbrHumanRank          = 3
};

enum E_HumanState // $$$$ ABR 2011-07-19: Here to replace HumanWounds
{
    eHumanState_Healthy = 0,
    eHumanState_Injured = 1,
    eHumanState_Dead    = 2,
    eNbrHumanState      = 3
};

enum E_HumanLocation
{
    eHumanLocation_Battlefield = 0,
    eHumanLocation_Maintenance = 1,
    eHumanLocation_Medical     = 2,
    eHumanLocation_Funeral     = 3,
    eNbrHumanLocation          = 4
};

enum E_InjuriesSeriousness
{
    eInjuriesSeriousness_U1 = 0,
    eInjuriesSeriousness_U2 = 1,
    eInjuriesSeriousness_U3 = 2,
    eInjuriesSeriousness_UE = 3,
    eNbrInjuriesSeriousness = 4
};

enum E_GhostType
{
    eGhostType_Agent   = 0,
    eGhostType_Automat = 1,
    eGhostType_Invalid = 2,
    eNbrGhostType      = 3
};

enum E_NbcState
{
    eNbcState_Liquid  = 0,
    eNbcState_Gaseous = 1,
    eNbrNbcState      = 2
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

    eModes_All          = 0xFFFF        // Filter
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
    eMissionType_Pawn       = 0,
    eMissionType_Automat    = 1,
    eMissionType_Population = 2,
    eMissionType_FragOrder  = 3,
    eNbrMissionTypes        = 4
};

enum E_AviationRange
{
    eAviationRange_ShortRange   = 0,
    eAviationRange_MediumRange  = 1,
    eAviationRange_LongRange    = 2,
    eAviationRange_NoAmmunition = 3,
    eNbrAviationRanges          = 4
};

enum E_FuncLimaType
{
    eLimaFuncLD     = 0, // Ligne de d�bouch�
    eLimaFuncLCA    = 1, // Ligne de changement d'attitude
    eLimaFuncLC     = 2, // Ligne de coordination
    eLimaFuncLI     = 3, // Ligne � interdire
    eLimaFuncLO     = 4, // Ligne d'objectif
    eLimaFuncLCAR   = 5, // Ligne de coup d'arr�t
    eLimaFuncLR     = 6, // Ligne de recueil
    eLimaFuncLDM    = 7, // Ligne de d�but de mission
    eLimaFuncLFM    = 8, // Ligne de fin de mission
    eLimaFuncLIA    = 9, // Ligne d'identification acceuil
    eLimaFuncNbr         // Keep this at the end of the enum! No !
};

enum E_EventDockModes
{
    eEventDockModes_None             = 0,
    eEventDockModes_DisplayTriggered = 1,
    eEventDockModes_EditTriggered    = 2,
    eEventDockModes_EditPlanned      = 3,
    eEventDockModes_Create           = 4,
    eNbrEventDockModes               = 5
};

#endif // __ENT_Enums_Gen_h_
