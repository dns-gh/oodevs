// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-09 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_ENT_Enums_Skeleton.h $
// $Author: Ape $
// $Modtime: 21/02/05 11:27 $
// $Revision: 2 $
// $Workfile: AGR_ENT_Enums_Skeleton.h $
//
// *****************************************************************************

#ifndef __ENT_Emuns_Gen_h_
#define __ENT_Emuns_Gen_h_


enum E_TypeLocalisation
{
    eTypeLocalisation_Circle,
    eTypeLocalisation_Ellipse,
    eTypeLocalisation_Line,
    eTypeLocalisation_Polygon,
    eTypeLocalisation_Point,
    eTypeLocalisation_Sector,
    eNbrTypeLocalisation
};


enum E_FamilleDotation
{
    eFamilleDotation_Munition,
    eFamilleDotation_Carburant,
    eFamilleDotation_Explosif,
    eFamilleDotation_Mine,
    eFamilleDotation_Barbele,
    eFamilleDotation_Piece,
    eFamilleDotation_Ration,
    eNbrFamilleDotation
};


enum E_FamilleMunition
{
    eFamilleMunition_Obus,
    eFamilleMunition_MissileAir,
    eFamilleMunition_MissileSol,
    eFamilleMunition_Mitraille,
    eNbrFamilleMunition
};


enum E_GDH_Qualificatif
{
    eGDH_Qualificatif_At,
    eGDH_Qualificatif_After,
    eGDH_Qualificatif_AsOf,
    eGDH_Qualificatif_Before,
    eGDH_Qualificatif_Cleared,
    eGDH_Qualificatif_Due,
    eGDH_Qualificatif_End,
    eGDH_Qualificatif_Off,
    eGDH_Qualificatif_On,
    eGDH_Qualificatif_Since,
    eGDH_Qualificatif_Start,
    eGDH_Qualificatif_Until,
    eGDH_Qualificatif_Wef,
    eNbrGDH_Qualificatif
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


enum E_Diplomatie
{
    eDiplomatie_Inconnu,
    eDiplomatie_Ami,
    eDiplomatie_Ennemi,
    eDiplomatie_Neutre,
    eNbrDiplomatie
};


enum E_ChangeDiplomatieErrorCode
{
    eChangeDiplomatieErrorCode_NoError,
    eChangeDiplomatieErrorCode_ErrorInvalidCamp,
    eNbrChangeDiplomatieErrorCode
};


enum E_EtatRapFor
{
    eEtatRapFor_None,
    eEtatRapFor_Neutre,
    eEtatRapFor_Favorable,
    eEtatRapFor_Defavorable,
    eNbrEtatRapFor
};

enum E_TypeObstacle
{
    eTypeObstacle_Preliminaire,
    eTypeObstacle_DeManoeuvre,
    eNbrTypeObstacle
};

enum E_EtatCombatRencontre
{
    eEtatCombatRencontre_None,
    eEtatCombatRencontre_EtatEsquive,
    eEtatCombatRencontre_EtatFixe,
    eEtatCombatRencontre_EtatPoursuiteMission,
    eNbrEtatCombatRencontre
};


enum E_EtatOperationnel
{
    eEtatOperationnel_DetruitTotalement,
    eEtatOperationnel_DetruitTactiquement,
    eEtatOperationnel_Operationnel,
    eNbrEtatOperationnel
};


enum E_Roe
{
    eRoe_None,
    eRoe_TirLibre,
    eRoe_TirSurRiposte,
    eRoe_TirInterdit,
    eNbrRoe
};


enum E_UnitAttrErrorCode
{
    eUnitAttrErrorCode_NoError,
    eUnitAttrErrorCode_ErrorInvalidUnit,
    eUnitAttrErrorCode_ErrorAutomateEmbraye,
    eUnitAttrErrorCode_ErrorInvalidAttribute,
    eUnitAttrErrorCode_ErrorUnitSurrendered,
    eNbrUnitAttrErrorCode
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


enum E_DisponibiliteAuTir
{
    eDisponibiliteAuTir_None,
    eDisponibiliteAuTir_PretAuTir,
    eDisponibiliteAuTir_Indisponible,
    eNbrDisponibiliteAuTir
};


enum E_RoePopulation
{
    eRoePopulation_None,
    eRoePopulation_EmploiForceInterdit,
    eRoePopulation_MaintienADistanceParMoyensNonLetaux,
    eRoePopulation_DispersionParMoyensDeDefenseActifs,
    eRoePopulation_ArmesLetalesAutorisees,
    eNbrRoePopulation
};


enum E_UnitFatigue
{
    eUnitFatigue_Normal,
    eUnitFatigue_Fatigue,
    eUnitFatigue_Epuise,
    eNbrUnitFatigue
};


enum E_UnitMoral
{
    eUnitMoral_Fanatique,
    eUnitMoral_Bon,
    eUnitMoral_Moyen,
    eUnitMoral_Mauvais,
    eNbrUnitMoral
};


enum E_UnitExperience
{
    eUnitExperience_Veteran,
    eUnitExperience_Experimente,
    eUnitExperience_Conscrit,
    eNbrUnitExperience
};


enum E_AutomateState
{
    eAutomateState_Embraye,
    eAutomateState_Debraye,
    eNbrAutomateState
};


enum E_SetAutomateModeErrorCode
{
    eSetAutomateModeErrorCode_NoError,
    eSetAutomateModeErrorCode_ErrorInvalidUnit,
    eSetAutomateModeErrorCode_ErrorUnitNotAutomate,
    eSetAutomateModeErrorCode_ErrorAlreadyEmbraye,
    eSetAutomateModeErrorCode_ErrorAlreadyDebraye,
    eSetAutomateModeErrorCode_ErrorUnitSurrendered,
    eNbrSetAutomateModeErrorCode
};

enum E_CtrlErrorCode
{
    eCtrlErrorCode_NoError,
    eCtrlErrorCode_ErrorAlreadyStarted,
    eCtrlErrorCode_ErrorNotStarted,
    eCtrlErrorCode_ErrorNotPaused,
    eCtrlErrorCode_ErrorAlreadyPaused,
    eCtrlErrorCode_ErrorInvalidTimeFactor,
    eNbrCtrlErrorCode
};


enum E_EtatSim
{
    eEtatSim_Running,
    eEtatSim_Paused,
    eEtatSim_Stopped,
    eNbrEtatSim
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


enum E_TypeLima
{
    eTypeLima_LigneDebouche,
    eTypeLima_LigneChangementAttitude,
    eTypeLima_LigneCoordination,
    eTypeLima_LigneInterdire,
    eTypeLima_LigneObjectif,
    eTypeLima_LigneCoupArret,
    eTypeLima_LigneRecueil,
    eTypeLima_LigneDebutMission,
    eTypeLima_LigneFinMission,
    eTypeLima_LigneIdentificationAccueil,
    eNbrTypeLima
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


enum E_PopulationAttitude
{
    ePopulationAttitude_Calme,
    ePopulationAttitude_Agitee,
    ePopulationAttitude_Excitee,
    ePopulationAttitude_Agressive,
    eNbrPopulationAttitude
};


enum E_PopulationAttrErrorCode
{
    ePopulationAttrErrorCode_NoError,
    ePopulationAttrErrorCode_ErrorInvalidUnit,
    ePopulationAttrErrorCode_ErrorInvalidAttribute,
    eNbrPopulationAttrErrorCode
};


enum E_TypeCR
{
    eTypeCR_Message,
    eTypeCR_Operationel,
    eTypeCR_EvenementExceptionnel,
    eTypeCR_Warning,
    eNbrTypeCR
};


enum E_FireEffectType
{
    eFireEffectType_Fumigene,
    eFireEffectType_Eclairant,
    eNbrFireEffectType
};


enum E_LogMaintenanceRegimeTravail
{
    eLogMaintenanceRegimeTravail_Regime1,
    eLogMaintenanceRegimeTravail_Regime2,
    eLogMaintenanceRegimeTravail_Regime3,
    eLogMaintenanceRegimeTravail_Regime4,
    eNbrLogMaintenanceRegimeTravail
};


enum E_LogMaintenanceTraitementEtat
{
    eLogMaintenanceTraitementEtat_DeplacementVersChaine,
    eLogMaintenanceTraitementEtat_AttenteDisponibiliteRemorqueur,
    eLogMaintenanceTraitementEtat_RemorqueurDeplacementAller,
    eLogMaintenanceTraitementEtat_RemorqueurChargement,
    eLogMaintenanceTraitementEtat_RemorqueurDeplacementRetour,
    eLogMaintenanceTraitementEtat_RemorqueurDechargement,
    eLogMaintenanceTraitementEtat_Diagnostique,
    eLogMaintenanceTraitementEtat_AttentePriseEnChargeParNiveauSuperieur,
    eLogMaintenanceTraitementEtat_AttenteDisponibilitePieces,
    eLogMaintenanceTraitementEtat_AttenteDisponibiliteReparateur,
    eLogMaintenanceTraitementEtat_Reparation,
    eLogMaintenanceTraitementEtat_RetourPion,
    eLogMaintenanceTraitementEtat_Termine,
    eNbrLogMaintenanceTraitementEtat
};


enum E_LogRavitaillementTraitementEtat
{
    eLogRavitaillementTraitementEtat_ConvoiEnAttenteCamions,
    eLogRavitaillementTraitementEtat_ConvoiConstitution,
    eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointChargement,
    eLogRavitaillementTraitementEtat_ConvoiChargement,
    eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointDechargement,
    eLogRavitaillementTraitementEtat_ConvoiDechargement,
    eLogRavitaillementTraitementEtat_ConvoiDeplacementRetour,
    eLogRavitaillementTraitementEtat_Termine,
    eNbrLogRavitaillementTraitementEtat
};


enum E_MsgLogRavitaillementChangeQuotasAck
{
    eMsgLogRavitaillementChangeQuotasAck_NoError,
    eMsgLogRavitaillementChangeQuotasAck_ErrorInvalidDonneur,
    eMsgLogRavitaillementChangeQuotasAck_ErrorInvalidReceveur,
    eNbrMsgLogRavitaillementChangeQuotasAck
};


enum E_MsgLogRavitaillementPousserFluxAck
{
    eMsgLogRavitaillementPousserFluxAck_NoError,
    eMsgLogRavitaillementPousserFluxAck_ErrorInvalidDonneur,
    eMsgLogRavitaillementPousserFluxAck_ErrorInvalidReceveur,
    eNbrMsgLogRavitaillementPousserFluxAck
};


enum E_LogSanteTraitementEtat
{
    eLogSanteTraitementEtat_AttenteDisponibiliteAmbulanceReleve,
    eLogSanteTraitementEtat_AmbulanceReleveDeplacementAller,
    eLogSanteTraitementEtat_AmbulanceReleveChargement,
    eLogSanteTraitementEtat_AttenteChargementCompletAmbulanceReleve,
    eLogSanteTraitementEtat_AmbulanceReleveDeplacementRetour,
    eLogSanteTraitementEtat_AmbulanceReleveDechargement,
    eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourDiagnostique,
    eLogSanteTraitementEtat_Diagnostique,
    eLogSanteTraitementEtat_RechercheSecteurTri,
    eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourTri,
    eLogSanteTraitementEtat_Tri,
    eLogSanteTraitementEtat_RechercheSecteurSoin,
    eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourSoin,
    eLogSanteTraitementEtat_Soin,
    eLogSanteTraitementEtat_Hospitalisation,
    eLogSanteTraitementEtat_AttenteDisponibiliteAmbulanceRamassage,
    eLogSanteTraitementEtat_AmbulanceRamassageChargement,
    eLogSanteTraitementEtat_AttenteChargementCompletAmbulanceRamassage,
    eLogSanteTraitementEtat_AmbulanceRamassageDeplacementAller,
    eLogSanteTraitementEtat_AmbulanceRamassageDechargement,
    eLogSanteTraitementEtat_Termine,
    eNbrLogSanteTraitementEtat
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


enum E_MissionGenSousTypeObstacle
{
    eMissionGenSousTypeObstacle_Preliminaire,
    eMissionGenSousTypeObstacle_DeManoeuvre,
    eNbrMissionGenSousTypeObstacle
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


enum E_OrderState
{
    eOrderState_Started,
    eOrderState_Stopped,
    eNbrOrderState
};


enum E_AutomateOrderFormation
{
    eAutomateOrderFormation_UnEchelon,
    eAutomateOrderFormation_DeuxEchelons,
    eNbrAutomateOrderFormation
};


#endif // __ENT_Emuns_Gen_h_
