// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Emuns_Gen_h_
#define __ENT_Emuns_Gen_h_


enum E_ChangeHierarchyErrorCode
{
    eChangeHierarchyErrorCode_NoError,
    eChangeHierarchyErrorCode_ErrorInvalidPion,
    eChangeHierarchyErrorCode_ErrorInvalidAutomate,
    eChangeHierarchyErrorCode_ErrorInvalidAutomateTc2,
    eChangeHierarchyErrorCode_ErrorInvalidAutomateMaintenance,
    eChangeHierarchyErrorCode_ErrorInvalidAutomateSante,
    eChangeHierarchyErrorCode_ErrorInvalidAutomateRavitaillement,
    eChangeHierarchyErrorCode_ErrorUnitSurrendered,
    eChangeHierarchyErrorCode_ErrorInvalidCamp,
    eChangeHierarchyErrorCode_ErrorInvalidGroupeConnaissance,
    eChangeHierarchyErrorCode_ErrorCampsIncompatibles,
    eNbrChangeHierarchyErrorCode
};


enum E_LocationType
{
    eLocationType_Circle,
    eLocationType_Ellipse,
    eLocationType_Line,
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


enum E_UnitIdentificationLevel
{
    eUnitIdentificationLevel_Identifiee,
    eUnitIdentificationLevel_Reconnue,
    eUnitIdentificationLevel_Detectee,
    eUnitIdentificationLevel_Signale,
    eNbrUnitIdentificationLevel
};


enum E_NatureLevel
{
    eNatureLevel_None,
    eNatureLevel_o,
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


enum E_Diplomacy
{
    eDiplomacy_Inconnu,
    eDiplomacy_Ami,
    eDiplomacy_Ennemi,
    eDiplomacy_Neutre,
    eNbrDiplomacy
};


enum E_ChangeDiplomacyErrorCode
{
    eChangeDiplomacyErrorCode_NoError,
    eChangeDiplomacyErrorCode_ErrorInvalidCamp,
    eNbrChangeDiplomacyErrorCode
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


enum E_UnitErrorCode
{
    eUnitErrorCode_NoError,
    eUnitErrorCode_ErrorInvalidUnit,
    eUnitErrorCode_ErrorAutomateEmbraye,
    eUnitErrorCode_ErrorInvalidAttribute,
    eUnitErrorCode_ErrorUnitSurrendered,
    eNbrUnitErrorCode
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
    ePopulationRoe_DispersionParMoyensDeDefenseActifs,
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


enum E_AutomatMode
{
    eAutomatMode_Embraye,
    eAutomatMode_Debraye,
    eNbrAutomatMode
};


enum E_SetAutomatModeErrorCode
{
    eSetAutomatModeErrorCode_NoError,
    eSetAutomatModeErrorCode_ErrorInvalidUnit,
    eSetAutomatModeErrorCode_ErrorNotAllowed,
    eNbrSetAutomatModeErrorCode
};


enum E_ControlErrorCode
{
    eControlErrorCode_NoError,
    eControlErrorCode_ErrorAlreadyStarted,
    eControlErrorCode_ErrorNotStarted,
    eControlErrorCode_ErrorNotPaused,
    eControlErrorCode_ErrorAlreadyPaused,
    eControlErrorCode_ErrorInvalidTimeFactor,
    eNbrControlErrorCode
};


enum E_SimulationState
{
    eSimulationState_Running,
    eSimulationState_Paused,
    eSimulationState_Stopped,
    eNbrSimulationState
};


enum E_PrecipitationType
{
    ePrecipitationType_PasDePrecipitation,
    ePrecipitationType_TempeteSable,
    ePrecipitationType_Brouillard,
    ePrecipitationType_Crachin,
    ePrecipitationType_Pluie,
    ePrecipitationType_Neige,
    eNbrPrecipitationType
};


enum E_FireEffectType
{
    eFireEffectType_Fumigene,
    eFireEffectType_Eclairant,
    eNbrFireEffectType
};


enum E_InfoContextErrorCode
{
    eInfoContextErrorCode_NoError,
    eInfoContextErrorCode_ErrorInvalidId,
    eInfoContextErrorCode_ErrorInvalidGeometry,
    eInfoContextErrorCode_ErrorInvalidLimaFunction,
    eInfoContextErrorCode_ErrorInvalidDiffusion,
    eNbrInfoContextErrorCode
};


enum E_LimaType
{
    eLimaType_LigneDebouche,
    eLimaType_LigneChangementAttitude,
    eLimaType_LigneCoordination,
    eLimaType_LigneInterdire,
    eLimaType_LigneObjectif,
    eLimaType_LigneCoupArret,
    eLimaType_LigneRecueil,
    eLimaType_LigneDebutMission,
    eLimaType_LigneFinMission,
    eLimaType_LigneIdentificationAccueil,
    eNbrLimaType
};


enum E_LogMaintenanceRegimeTravail
{
    eLogMaintenanceRegimeTravail_Regime1,
    eLogMaintenanceRegimeTravail_Regime2,
    eLogMaintenanceRegimeTravail_Regime3,
    eLogMaintenanceRegimeTravail_Regime4,
    eNbrLogMaintenanceRegimeTravail
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


enum E_MsgLogSupplyChangeQuotasAck
{
    eMsgLogSupplyChangeQuotasAck_NoError,
    eMsgLogSupplyChangeQuotasAck_ErrorInvalidDonneur,
    eMsgLogSupplyChangeQuotasAck_ErrorInvalidReceveur,
    eNbrMsgLogSupplyChangeQuotasAck
};


enum E_MsgLogSupplyPushFlowAck
{
    eMsgLogSupplyPushFlowAck_NoError,
    eMsgLogSupplyPushFlowAck_ErrorInvalidDonneur,
    eMsgLogSupplyPushFlowAck_ErrorInvalidReceveur,
    eNbrMsgLogSupplyPushFlowAck
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


enum E_ObjectErrorCode
{
    eObjectErrorCode_NoError,
    eObjectErrorCode_ErrorInvalidObject,
    eObjectErrorCode_ErrorInvalidId,
    eObjectErrorCode_ErrorInvalidCamp,
    eObjectErrorCode_ErrorInvalidLocalisation,
    eObjectErrorCode_ErrorMissingSpecificAttributes,
    eObjectErrorCode_ErrorInvalidSpecificAttributes,
    eNbrObjectErrorCode
};


enum E_ObjectType
{
    eObjectType_BouchonMines,
    eObjectType_ZoneMineeLineaire,
    eObjectType_ZoneMineeParDispersion,
    eObjectType_FosseAntiChar,
    eObjectType_Abattis,
    eObjectType_Barricade,
    eObjectType_Eboulement,
    eObjectType_DestructionRoute,
    eObjectType_DestructionPont,
    eObjectType_PontFlottantContinu,
    eObjectType_PosteTir,
    eObjectType_ZoneProtegee,
    eObjectType_ZoneImplantationCanon,
    eObjectType_ZoneImplantationCobra,
    eObjectType_ZoneImplantationLrm,
    eObjectType_SiteFranchissement,
    eObjectType_NuageNbc,
    eObjectType_PlotRavitaillement,
    eObjectType_SiteDecontamination,
    eObjectType_ZoneBrouillageBrod,
    eObjectType_Rota,
    eObjectType_ZoneNbc,
    eObjectType_ZoneBrouillageBromure,
    eObjectType_AirePoser,
    eObjectType_Piste,
    eObjectType_Plateforme,
    eObjectType_ZoneMobiliteAmelioree,
    eObjectType_ZonePoserHelicoptere,
    eObjectType_AireLogistique,
    eObjectType_ItineraireLogistique,
    eObjectType_CampPrisonniers,
    eObjectType_CampRefugies,
    eObjectType_PosteControle,
    eObjectType_TerrainLargage,
    eObjectType_ZoneInterditeMouvement,
    eObjectType_ZoneInterditeTir,
    eObjectType_ZoneImplantationMortier,
    eObjectType_PontFlottantDiscontinu,
    eObjectType_Installation,
    eNbrObjectType
};


enum E_ObstacleType
{
    eObstacleType_Initial,
    eObstacleType_Reserved,
    eNbrObstacleType
};


enum E_OrderErrorCode
{
    eOrderErrorCode_NoError,
    eOrderErrorCode_ErrorInvalidUnit,
    eOrderErrorCode_ErrorInvalidLimit,
    eOrderErrorCode_ErrorInvalidLima,
    eOrderErrorCode_ErrorInvalidMission,
    eOrderErrorCode_ErrorInvalidMissionParameters,
    eOrderErrorCode_ErrorUnitCannotReceiveOrder,
    eOrderErrorCode_ErrorInvalidOrderConduite,
    eOrderErrorCode_ErrorInvalidOrderMission,
    eOrderErrorCode_ErrorInvalidOrderInitial,
    eOrderErrorCode_ErrorInvalidOrderConduiteParameters,
    eOrderErrorCode_ErrorUnitSurrendered,
    eOrderErrorCode_ErrorInvalidLimaFunction,
    eNbrOrderErrorCode
};


enum E_ReportType
{
    eReportType_Message,
    eReportType_Operationel,
    eReportType_EvenementExceptionnel,
    eReportType_Warning,
    eNbrReportType
};


enum E_AutomatOrderFormation
{
    eAutomatOrderFormation_UnEchelon,
    eAutomatOrderFormation_DeuxEchelons,
    eNbrAutomatOrderFormation
};


enum E_PopulationErrorCode
{
    ePopulationErrorCode_NoError,
    ePopulationErrorCode_ErrorInvalidUnit,
    ePopulationErrorCode_ErrorInvalidAttribute,
    eNbrPopulationErrorCode
};


enum E_PopulationAttitude
{
    ePopulationAttitude_Calme,
    ePopulationAttitude_Agitee,
    ePopulationAttitude_Excitee,
    ePopulationAttitude_Agressive,
    eNbrPopulationAttitude
};




#endif // __ENT_Emuns_Gen_h_
