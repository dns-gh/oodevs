// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Enums_h_
#define __ENT_Enums_h_

enum E_LayerTypes
{
    eLayerTypes_Actions,
    eLayerTypes_AgentKnowledges,
    eLayerTypes_Agents,
    eLayerTypes_Automats,
    eLayerTypes_ContourLines,
    eLayerTypes_Creations,
    eLayerTypes_CrowdKnowledges,
    eLayerTypes_Crowds,
    eLayerTypes_CrowdsComposite,
    eLayerTypes_Default,
    eLayerTypes_Drawings,
    eLayerTypes_Elevation2d,
    eLayerTypes_Elevation3d,
    eLayerTypes_EventCreation,
    eLayerTypes_Fog,
    eLayerTypes_Formations,
    eLayerTypes_Ghosts,
    eLayerTypes_Grid,
    eLayerTypes_IndicatorCreation,
    eLayerTypes_InhabitantCreation,
    eLayerTypes_Inhabitants,
    eLayerTypes_InhabitantsComposite,
    eLayerTypes_Locations,
    eLayerTypes_Mapnik,
    eLayerTypes_Metric,
    eLayerTypes_ObjectCreation,
    eLayerTypes_ObjectKnowledges,
    eLayerTypes_Objects,
    eLayerTypes_ObjectsComposite,
    eLayerTypes_Parameters,
    eLayerTypes_Parties,
    eLayerTypes_Pathfinds,
    eLayerTypes_Raster,
    eLayerTypes_RasterDynamic,
    eLayerTypes_ResourceNetworks,
    eLayerTypes_Selection,
    eLayerTypes_TacticalLines,
    eLayerTypes_TacticalLinesComposite,
    eLayerTypes_Terrain,
    eLayerTypes_TerrainProfiler,
    eLayerTypes_Tooltips,
    eLayerTypes_UnitsComposite,
    eLayerTypes_Urban,
    eLayerTypes_Watershed,
    eLayerTypes_WeaponRanges,
    eLayerTypes_Weather,
    eLayerTypes_WeatherComposite,
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

    eModes_Prepare      = 0x0001 << 4,
    eModes_Terrain      = 0x0002 << 4,
    eModes_LivingArea   = 0x0004 << 4,
    eModes_AllPrepare   = 0x000F << 4,

    eModes_Gaming       = 0x0001 << 8,
    eModes_Planning     = 0x0002 << 8,
    eModes_Replay       = 0x0004 << 8,
    eModes_Itinerary    = 0x0008 << 8,
    eModes_AllGaming    = 0x000F << 8,

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
    eEventTypes_Magic,
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
    eLimaFuncLD,    // Ligne de débouché
    eLimaFuncLCA,   // Ligne de changement d'attitude
    eLimaFuncLC,    // Ligne de coordination
    eLimaFuncLI,    // Ligne à interdire
    eLimaFuncLO,    // Ligne d'objectif
    eLimaFuncLCAR,  // Ligne de coup d'arrêt
    eLimaFuncLR,    // Ligne de recueil
    eLimaFuncLDM,   // Ligne de début de mission
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

enum E_CoordinateSystem
{
    eCoordinateSystem_Mgrs,
    eCoordinateSystem_SanC,
    eCoordinateSystem_Wgs84Dd,
    eCoordinateSystem_Wgs84Dms,
    eCoordinateSystem_Local,
    eNbrCoordinateSystem,
};

enum E_VisualisationScale
{
    eVisualisationScale_LargeTexts,
    eVisualisationScale_SmallTexts,
    eVisualisationScale_Edges,
    eVisualisationScale_Cliffs,
    eVisualisationScale_Highways,
    eVisualisationScale_MainRoads,
    eVisualisationScale_SecondaryRoads,
    eVisualisationScale_CountryRoads,
    eVisualisationScale_Bridges,
    eVisualisationScale_Railroads,
    eVisualisationScale_MainRivers,
    eVisualisationScale_Rivers,
    eVisualisationScale_Streams,
    eVisualisationScale_UrbanBlocks,
    eNbrVisualisationScale
};

enum E_PerceptionLevel
{
    ePerceptionLevel_Detection,
    ePerceptionLevel_Recognition,
    ePerceptionLevel_Identification,
    eNbrPerceptionLevel
};

enum E_PerceptionType
{
    ePerceptionType_Heading,
    ePerceptionType_Speed,
    ePerceptionType_OpState,
    ePerceptionType_Side,
    ePerceptionType_Level,
    ePerceptionType_NaturePartial,
    ePerceptionType_NatureFull,
    ePerceptionType_Surrendered,
    ePerceptionType_Prisoner,
    ePerceptionType_Refugees,
    ePerceptionType_CommandPost,
    eNbrPerceptionType
};

enum E_LocationCategory
{
    eLocationCategory_Hydrography,
    eLocationCategory_Network,
    eLocationCategory_Physiography,
    eLocationCategory_Urban,
    eLocationCategory_Vegetation,
    eNbrLocationCategory
};

#endif __ENT_Enums_h_
