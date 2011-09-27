// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Enums_Gen_h_
#define __ENT_Enums_Gen_h_

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
    eNbrDotationFamily
};

enum E_AmmunitionFamily
{
    eAmmunitionFamily_Obus,
    eAmmunitionFamily_MissileAir,
    eAmmunitionFamily_MissileSol,
    eAmmunitionFamily_Mitraille,
    eNbrAmmunitionFamily
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
    eRoe_None,
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

enum E_DemolitionTargetType
{
    eDemolitionTargetType_Preliminary,
    eDemolitionTargetType_Reserved,
    eNbrDemolitionTargetType
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

enum E_ActionParameter
{
    eActionParameter_DangerDirection,
    eActionParameter_Limit1,
    eActionParameter_Limit2,
    eNbrActionParameter
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
    eNbrHumanLocation          = 3
};

enum E_InjuriesSeriousness
{
    eInjuriesSeriousness_U1 = 0,
    eInjuriesSeriousness_U2 = 1,
    eInjuriesSeriousness_U3 = 2,
    eInjuriesSeriousness_UE = 3,
    eNbrInjuriesSeriousness = 4
};

#endif // __ENT_Enums_Gen_h_
