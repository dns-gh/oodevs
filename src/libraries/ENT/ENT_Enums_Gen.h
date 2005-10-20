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


enum E_UnitNatureWeapon
{
    eUnitNatureWeapon_None,
    eUnitNatureWeapon_Antichar,
    eUnitNatureWeapon_Administration,
    eUnitNatureWeapon_DefenseAerienne,
    eUnitNatureWeapon_CentreSoutienOperationsAeriennes,
    eUnitNatureWeapon_Blinde,
    eUnitNatureWeapon_Aviation,
    eUnitNatureWeapon_Cavalerie,
    eUnitNatureWeapon_Genie,
    eUnitNatureWeapon_Deminage,
    eUnitNatureWeapon_SoutienArtillerie,
    eUnitNatureWeapon_Artillerie,
    eUnitNatureWeapon_Infanterie,
    eUnitNatureWeapon_GuerreInformation,
    eUnitNatureWeapon_ForcesSecuriteInterne,
    eUnitNatureWeapon_ExecutionLoi,
    eUnitNatureWeapon_SoutienAtterrissage,
    eUnitNatureWeapon_Logistique,
    eUnitNatureWeapon_Reparation,
    eUnitNatureWeapon_Manoeuvre,
    eUnitNatureWeapon_Medical,
    eUnitNatureWeapon_RenseignementMilitaire,
    eUnitNatureWeapon_Missile,
    eUnitNatureWeapon_Nbc,
    eUnitNatureWeapon_Inconnue,
    eUnitNatureWeapon_NonSpecifiee,
    eUnitNatureWeapon_Reconnaissance,
    eUnitNatureWeapon_Transmissions,
    eUnitNatureWeapon_Ravitaillement,
    eUnitNatureWeapon_TacticalAirControlParty,
    eUnitNatureWeapon_Transport,
    eNbrUnitNatureWeapon
};


enum E_UnitNatureSpecialization
{
    eUnitNatureSpecialization_None,
    eUnitNatureSpecialization_Aerien,
    eUnitNatureSpecialization_AttaqueAerienne,
    eUnitNatureSpecialization_Aeroporte,
    eUnitNatureSpecialization_Blinde,
    eUnitNatureSpecialization_Compose,
    eUnitNatureSpecialization_Construction,
    eUnitNatureSpecialization_Dentiste,
    eUnitNatureSpecialization_GuerreElectronique,
    eUnitNatureSpecialization_Finance,
    eUnitNatureSpecialization_VoilureFixe,
    eUnitNatureSpecialization_Terrestre,
    eUnitNatureSpecialization_Helicoptere,
    eUnitNatureSpecialization_Jag,
    eUnitNatureSpecialization_Mecanise,
    eUnitNatureSpecialization_PoliceMilitaire,
    eUnitNatureSpecialization_Motorise,
    eUnitNatureSpecialization_Naval,
    eUnitNatureSpecialization_ServiceDuPersonnel,
    eUnitNatureSpecialization_Courrier,
    eUnitNatureSpecialization_Psychologique,
    eUnitNatureSpecialization_CheminDeFer,
    eUnitNatureSpecialization_Religieux,
    eUnitNatureSpecialization_Riverine,
    eUnitNatureSpecialization_SecurityPolice,
    eUnitNatureSpecialization_PatrouilleCotiere,
    eUnitNatureSpecialization_Chirurgie,
    eUnitNatureSpecialization_Veterinaire,
    eUnitNatureSpecialization_DecollageAtterissageVerticalCourt,
    eNbrUnitNatureSpecialization
};


enum E_UnitNatureQualifier
{
    eUnitNatureQualifier_None,
    eUnitNatureQualifier_Lourd,
    eUnitNatureQualifier_Leger,
    eUnitNatureQualifier_Moyen,
    eNbrUnitNatureQualifier
};


enum E_UnitNatureCategory
{
    eUnitNatureCategory_None,
    eUnitNatureCategory_Combat,
    eUnitNatureCategory_SoutienLogistique,
    eUnitNatureCategory_CombatSupport,
    eUnitNatureCategory_Inconnu,
    eNbrUnitNatureCategory
};


enum E_UnitNatureMobility
{
    eUnitNatureMobility_None,
    eUnitNatureMobility_Aeroporte,
    eUnitNatureMobility_AerienCompose,
    eUnitNatureMobility_AerienVoilureFixe,
    eUnitNatureMobility_AerienPlusLegerQueAir,
    eUnitNatureMobility_AerienAilesRotatives,
    eUnitNatureMobility_AerienAmphibie,
    eUnitNatureMobility_AnimalMonte,
    eUnitNatureMobility_Demonte,
    eUnitNatureMobility_TerrestreCheminDeFer,
    eUnitNatureMobility_TerrestreAutoPropulse,
    eUnitNatureMobility_TerrestreChenille,
    eUnitNatureMobility_TerrestreRemorque,
    eUnitNatureMobility_TerrestreRoues,
    eUnitNatureMobility_MaritimeSousMarin,
    eUnitNatureMobility_MaritimeSurface,
    eUnitNatureMobility_Inconnu,
    eNbrUnitNatureMobility
};


enum E_UnitCapaciteMission
{
    eUnitCapaciteMission_CivilianLawEnforcement,
    eUnitCapaciteMission_CommandOperations,
    eUnitCapaciteMission_CouvertureAerienne,
    eUnitCapaciteMission_JointIntelligence,
    eUnitCapaciteMission_MoraleWelfareRecreation,
    eUnitCapaciteMission_ReglageElectronique,
    eUnitCapaciteMission_Remplacement,
    eUnitCapaciteMission_ServiceMortuaire,
    eUnitCapaciteMission_SignalsIntelligence,
    eUnitCapaciteMission_TheatreMissileDefence,
    eUnitCapaciteMission_Utilitaire,
    eUnitCapaciteMission_Aeroporte,
    eUnitCapaciteMission_AffairesPubliques,
    eUnitCapaciteMission_AffairesPubliquesCommunication,
    eUnitCapaciteMission_AffairesPubliquesJointInformation,
    eUnitCapaciteMission_Alpin,
    eUnitCapaciteMission_Amphibie,
    eUnitCapaciteMission_Arctique,
    eUnitCapaciteMission_ArtillerieReconnaissance,
    eUnitCapaciteMission_Attaque,
    eUnitCapaciteMission_AttaqueAerienne,
    eUnitCapaciteMission_C2,
    eUnitCapaciteMission_CheminDeFer,
    eUnitCapaciteMission_Ciblage,
    eUnitCapaciteMission_ContreEspionnage,
    eUnitCapaciteMission_Eclairage,
    eUnitCapaciteMission_Entretien,
    eUnitCapaciteMission_EntretienMateriel,
    eUnitCapaciteMission_EntretienMissile,
    eUnitCapaciteMission_EntretienOptoElectronique,
    eUnitCapaciteMission_Espionnage,
    eUnitCapaciteMission_EspionnageMilitaireAerialExploitation,
    eUnitCapaciteMission_EspionnageMilitaireOperation,
    eUnitCapaciteMission_EspionnageMilitaireTacticalExploitation,
    eUnitCapaciteMission_EvacuationSanitaire,
    eUnitCapaciteMission_Finance,
    eUnitCapaciteMission_GenieCombat,
    eUnitCapaciteMission_GenieConstruction,
    eUnitCapaciteMission_GenieConstructionNavale,
    eUnitCapaciteMission_GuerreElectronique,
    eUnitCapaciteMission_GuerreElectroniqueInterception,
    eUnitCapaciteMission_GuerreElectroniqueBrouillage,
    eUnitCapaciteMission_GuerreElectroniqueDetection,
    eUnitCapaciteMission_Interrogation,
    eUnitCapaciteMission_MainDOeuvre,
    eUnitCapaciteMission_MaintienDeLaPaix,
    eUnitCapaciteMission_Marine,
    eUnitCapaciteMission_MineDeContreMesure,
    eUnitCapaciteMission_NbcBiologique,
    eUnitCapaciteMission_NbcChimique,
    eUnitCapaciteMission_NbcChimiqueFumee,
    eUnitCapaciteMission_NbcDecontamination,
    eUnitCapaciteMission_NbcDecontaminationChimique,
    eUnitCapaciteMission_NbcNucleaire,
    eUnitCapaciteMission_NonRenseigne,
    eUnitCapaciteMission_Observation,
    eUnitCapaciteMission_ObservationCapteur,
    eUnitCapaciteMission_ObservationLonguePortee,
    eUnitCapaciteMission_ObservationMeteo,
    eUnitCapaciteMission_ObservationModuleTerrestre,
    eUnitCapaciteMission_PurificationDeLEau,
    eUnitCapaciteMission_RavitaillementClassi,
    eUnitCapaciteMission_RavitaillementClassii,
    eUnitCapaciteMission_RavitaillementClassIiiAviation,
    eUnitCapaciteMission_RavitaillementClassiii,
    eUnitCapaciteMission_RavitaillementClassIv,
    eUnitCapaciteMission_RavitaillementClassv,
    eUnitCapaciteMission_RavitaillementEau,
    eUnitCapaciteMission_RavitaillementBlanchisserieBain,
    eUnitCapaciteMission_RechercheEtSauvetage,
    eUnitCapaciteMission_Reconnaissance,
    eUnitCapaciteMission_ReconnaissanceAerienneTactique,
    eUnitCapaciteMission_Recuperation,
    eUnitCapaciteMission_ServiceDuPersonnel,
    eUnitCapaciteMission_ServiceJuridique,
    eUnitCapaciteMission_SupportAerienRapproche,
    eUnitCapaciteMission_TransmissionsForwardCommunications,
    eUnitCapaciteMission_TransmissionsNodeCentre,
    eUnitCapaciteMission_TransmissionsNodeLargeExtension,
    eUnitCapaciteMission_TransmissionsNodeSmallExtension,
    eUnitCapaciteMission_TransmissionsRadioRelay,
    eUnitCapaciteMission_TransmissionsRadioTacticalSatellite,
    eUnitCapaciteMission_TransmissionsRadioTeletype,
    eUnitCapaciteMission_TransmissionsSupport,
    eUnitCapaciteMission_TransmissionsRadio,
    eUnitCapaciteMission_TransportAllocationDesMouvements,
    eUnitCapaciteMission_TransportApodApoe,
    eUnitCapaciteMission_TransportSpodSpoe,
    eUnitCapaciteMission_TransportMissile,
    eUnitCapaciteMission_VerrouillageDeCible,
    eUnitCapaciteMission_VerrouillageDeCibleFlash,
    eUnitCapaciteMission_VerrouillageDeCibleRadar,
    eUnitCapaciteMission_VerrouillageDeCibleSonore,
    eNbrUnitCapaciteMission
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


enum E_TempsBordee
{
    eTempsBordee_Temps8Heures,
    eTempsBordee_Temps12Heures,
    eTempsBordee_Temps16Heures,
    eTempsBordee_Temps20Heures,
    eTempsBordee_Temps24Heures,
    eNbrTempsBordee
};


enum E_MissionALAT_PorteeAction
{
    eMissionALAT_PorteeAction_CourtePortee,
    eMissionALAT_PorteeAction_MoyennePortee,
    eMissionALAT_PorteeAction_LonguePortee,
    eMissionALAT_PorteeAction_SansMunitions,
    eNbrMissionALAT_PorteeAction
};


enum E_MissionALAT_AmbianceMission
{
    eMissionALAT_AmbianceMission_aa,
    eMissionALAT_AmbianceMission_As,
    eMissionALAT_AmbianceMission_Polyvalent,
    eNbrMissionALAT_AmbianceMission
};


enum E_ModeDeploiement
{
    eModeDeploiement_EnCarre,
    eModeDeploiement_EnTriangle,
    eModeDeploiement_NasseTroisSections,
    eModeDeploiement_NasseQuatreSections,
    eModeDeploiement_DoubleRideau,
    eModeDeploiement_SimpleRideau,
    eNbrModeDeploiement
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
    eNbrOrderErrorCode
};


enum E_OrderState
{
    eOrderState_Started,
    eOrderState_Cancelled,
    eOrderState_Stopped,
    eNbrOrderState
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
    eNbrObjectType
};


enum E_MissionGenUrgence
{
    eMissionGenUrgence_SerieMinimum,
    eMissionGenUrgence_SerieComplementaire,
    eNbrMissionGenUrgence
};


enum E_MissionGenSousTypeObstacle
{
    eMissionGenSousTypeObstacle_Preliminaire,
    eMissionGenSousTypeObstacle_DeManoeuvre,
    eNbrMissionGenSousTypeObstacle
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


enum E_MissionGenPriorite
{
    eMissionGenPriorite_Prioritaire,
    eMissionGenPriorite_Normal,
    eMissionGenPriorite_PeuPrioritaire,
    eNbrMissionGenPriorite
};


enum E_ModeDeploiementSectionMistral
{
    eModeDeploiementSectionMistral_Omnidirectionnel,
    eModeDeploiementSectionMistral_Directionnel,
    eNbrModeDeploiementSectionMistral
};


enum E_TypeFosseAC
{
    eTypeFosseAC_Simple,
    eTypeFosseAC_Compose,
    eNbrTypeFosseAC
};


enum E_GenTypePontage
{
    eGenTypePontage_Continu,
    eGenTypePontage_Discontinu,
    eNbrGenTypePontage
};


enum E_MissionInfNatureObjectif
{
    eMissionInfNatureObjectif_ElementsTerrain,
    eMissionInfNatureObjectif_Objets,
    eMissionInfNatureObjectif_ElementsMobiles,
    eNbrMissionInfNatureObjectif
};


enum E_AutomateOrderFormation
{
    eAutomateOrderFormation_UnEchelon,
    eAutomateOrderFormation_DeuxEchelons,
    eNbrAutomateOrderFormation
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


enum E_MsgCtrlClientAnnouncement
{
    eMsgCtrlClientAnnouncement_Mos,
    eMsgCtrlClientAnnouncement_MosLight,
    eNbrMsgCtrlClientAnnouncement
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
    eInfoContextErrorCode_ErrorInvalidLimitOrLimaUsed,
    eNbrInfoContextErrorCode
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


enum E_ChangeGroupeConnaissanceErrorCode
{
    eChangeGroupeConnaissanceErrorCode_NoError,
    eChangeGroupeConnaissanceErrorCode_ErrorInvalidAutomate,
    eChangeGroupeConnaissanceErrorCode_ErrorInvalidCamp,
    eChangeGroupeConnaissanceErrorCode_ErrorInvalidGroupeConnaissance,
    eNbrChangeGroupeConnaissanceErrorCode
};


enum E_ChangeLiensLogistiquesErrorCode
{
    eChangeLiensLogistiquesErrorCode_NoError,
    eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomate,
    eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomateTc2,
    eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomateMaintenance,
    eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomateSante,
    eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomateRavitaillement,
    eChangeLiensLogistiquesErrorCode_ErrorUnitSurrendered,
    eNbrChangeLiensLogistiquesErrorCode
};


enum E_ChangeAutomateErrorCode
{
    eChangeAutomateErrorCode_NoError,
    eChangeAutomateErrorCode_ErrorInvalidPion,
    eChangeAutomateErrorCode_ErrorInvalidAutomate,
    eChangeAutomateErrorCode_ErrorCampsIncompatibles,
    eNbrChangeAutomateErrorCode
};


enum E_CtrlErrorCodeLauncher
{
    eCtrlErrorCodeLauncher_NoError,
    eCtrlErrorCodeLauncher_ErrorAlreadyStarted,
    eCtrlErrorCodeLauncher_ErrorNotStarted,
    eCtrlErrorCodeLauncher_ErrorNotPaused,
    eCtrlErrorCodeLauncher_ErrorAlreadyPaused,
    eCtrlErrorCodeLauncher_ErrorInvalidTimeFactor,
    eCtrlErrorCodeLauncher_ErrorInvalidScenarioIdentifier,
    eCtrlErrorCodeLauncher_ErrorErrorScenarioInit,
    eCtrlErrorCodeLauncher_ErrorInvalidDataPath,
    eCtrlErrorCodeLauncher_ErrorInvalidData,
    eCtrlErrorCodeLauncher_ErrorInvalidCheckpointIdentifier,
    eCtrlErrorCodeLauncher_ErrorErrorCheckpointInit,
    eCtrlErrorCodeLauncher_ErrorSimKilledByRemote,
    eNbrCtrlErrorCodeLauncher
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
    eLogMaintenanceTraitementEtat_AttenteRetourPion,
    eLogMaintenanceTraitementEtat_RetourPion,
    eLogMaintenanceTraitementEtat_Termine,
    eNbrLogMaintenanceTraitementEtat
};


enum E_LogRavitaillementTraitementEtat
{
    eLogRavitaillementTraitementEtat_ConvoiConsitution,
    eLogRavitaillementTraitementEtat_ConvoiChargement,
    eLogRavitaillementTraitementEtat_ConvoiDeplacementAller,
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
    eMsgLogRavitaillementPousserFluxAck_ErrorRavitaillementEnCours,
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
    eLogSanteTraitementEtat_AttenteDisponibiliteAmbulanceRamassage,
    eLogSanteTraitementEtat_AmbulanceRamassageChargement,
    eLogSanteTraitementEtat_AttenteChargementCompletAmbulanceRamassage,
    eLogSanteTraitementEtat_AmbulanceRamassageDeplacementAller,
    eLogSanteTraitementEtat_AmbulanceRamassageDechargement,
    eLogSanteTraitementEtat_Termine,
    eNbrLogSanteTraitementEtat
};


enum E_AutomateState
{
    eAutomateState_Embraye,
    eAutomateState_Debraye,
    eNbrAutomateState
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


enum E_EtatRapFor
{
    eEtatRapFor_None,
    eEtatRapFor_Neutre,
    eEtatRapFor_Favorable,
    eEtatRapFor_Defavorable,
    eNbrEtatRapFor
};


enum E_ReglesEngagement
{
    eReglesEngagement_None,
    eReglesEngagement_TirLibre,
    eReglesEngagement_TirSurRiposte,
    eReglesEngagement_TirInterdit,
    eNbrReglesEngagement
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


enum E_DisponibiliteAuTir
{
    eDisponibiliteAuTir_None,
    eDisponibiliteAuTir_PretAuTir,
    eDisponibiliteAuTir_Indisponible,
    eNbrDisponibiliteAuTir
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


enum E_UnitAttrErrorCode
{
    eUnitAttrErrorCode_NoError,
    eUnitAttrErrorCode_ErrorInvalidUnit,
    eUnitAttrErrorCode_ErrorAutomateEmbraye,
    eUnitAttrErrorCode_ErrorInvalidAttribute,
    eUnitAttrErrorCode_ErrorUnitSurrendered,
    eNbrUnitAttrErrorCode
};


enum E_TypeMunitionTirIndirect
{
    eTypeMunitionTirIndirect_Explosif,
    eTypeMunitionTirIndirect_Grenade,
    eTypeMunitionTirIndirect_Aced,
    eTypeMunitionTirIndirect_Fumigene,
    eTypeMunitionTirIndirect_Eclairant,
    eTypeMunitionTirIndirect_Mine,
    eNbrTypeMunitionTirIndirect
};


enum E_ActionReagirFaceAEni
{
    eActionReagirFaceAEni_DetruireMoyens,
    eActionReagirFaceAEni_Defendre,
    eActionReagirFaceAEni_Demonter,
    eNbrActionReagirFaceAEni
};


enum E_TC2GererMaterielAvantDeplacement
{
    eTC2GererMaterielAvantDeplacement_DeborderMaterielsVersBld,
    eTC2GererMaterielAvantDeplacement_DetruireMaterielsEnReparation,
    eTC2GererMaterielAvantDeplacement_ReparerMaterielsSurPlace,
    eNbrTC2GererMaterielAvantDeplacement
};


enum E_PopulationAttitude
{
    ePopulationAttitude_Calme,
    ePopulationAttitude_Agitee,
    ePopulationAttitude_Excitee,
    ePopulationAttitude_Agressive,
    eNbrPopulationAttitude
};





enum E_AutomataMission
{
    eMission_Automate_ABC_ArmerPointDeControle,
    eMission_Automate_ABC_OrganiserAccueilColonneRefugies,
    eMission_Automate_ABC_ProtegerPC,
    eMission_Automate_ABC_Appuyer,
    eMission_Automate_ABC_Attaquer,
    eMission_Automate_ABC_ContreAttaquer,
    eMission_Automate_ABC_Controler,
    eMission_Automate_ABC_Couvrir,
    eMission_Automate_ABC_DonnerCoupArret,
    eMission_Automate_ABC_Eclairer,
    eMission_Automate_ABC_EffectuerRaid,
    eMission_Automate_ABC_Interdire,
    eMission_Automate_ABC_Jalonner,
    eMission_Automate_ABC_PrendrePreciserContact,
    eMission_Automate_ABC_Reconnaitre,
    eMission_Automate_ABC_ReconnaitrePoint,
    eMission_Automate_ABC_Reduire,
    eMission_Automate_ABC_ReleverEnDepassant,
    eMission_Automate_ABC_Semparer,
    eMission_Automate_ABC_Tenir,
    eMission_Automate_ABC_Soutenir,
    eMission_Automate_ABC_Surveiller,
    eMission_Automate_ABC_FlancGarder,
    eMission_Automate_ABC_RenseignerSur,
    eMission_Automate_ABC_Fixer,
    eMission_Automate_ABC_Freiner,
    eMission_Automate_ABC_SurveillerItineraire,
    eMission_Automate_ABC_RenseignerSurUnAxe,
    eMission_Automate_ABC_RecueillirUnite,
    eMission_Automate_ABC_EscorterUnConvoi,
    eMission_Automate_ALAT_Heliporter,
    eMission_Automate_ALAT_Jalonner,
    eMission_Automate_ALAT_Helitransporter,
    eMission_Automate_ALAT_ExtrairePersonnel,
    eMission_Automate_ALAT_Appuyer,
    eMission_Automate_ALAT_Couvrir,
    eMission_Automate_ALAT_Surveiller,
    eMission_Automate_ALAT_Reconnaitre,
    eMission_Automate_ALAT_Freiner,
    eMission_Automate_ALAT_Escorter,
    eMission_Automate_ALAT_Attaquer,
    eMission_Automate_ALAT_RecueillirFormationAeromobile,
    eMission_Automate_ALAT_AttaquerFormationAeromobile,
    eMission_Automate_ALAT_DonnerCoupArret,
    eMission_Automate_ALAT_DetruireNeutraliserDansProfondeur,
    eMission_Automate_ALAT_DetruireNeutraliserDansZone,
    eMission_Automate_ALAT_EffectuerRechercheEtSauvetage,
    eMission_Automate_ASA_DefendreZone,
    eMission_Automate_ASA_DefendreSite,
    eMission_Automate_ASA_Surveiller,
    eMission_Automate_ASA_DefendreItineraire,
    eMission_Automate_ASA_MISTRAL_DefendreZone,
    eMission_Automate_ASA_MISTRAL_DefendreSite,
    eMission_Automate_ASA_MISTRAL_Surveiller,
    eMission_Automate_ASA_HAWK_DefendreZone,
    eMission_Automate_ASA_HAWK_DefendreSite,
    eMission_Automate_ASA_HAWK_Surveiller,
    eMission_Automate_ASA_DefendreUnites,
    eMission_Automate_ASS_SeDeployer,
    eMission_Automate_ASS_ReconnaitreZonesDeploiement,
    eMission_Automate_ASS_AcquerirObjectifs,
    eMission_Automate_GEN_SeDeployer,
    eMission_Automate_GEN_FaireFranchir,
    eMission_Automate_GEN_RealiserSystemeObstacles,
    eMission_Automate_GEN_FranchirDeViveForce,
    eMission_Automate_GEN_RetablirItineraires,
    eMission_Automate_GEN_AmenagerAireStationnement,
    eMission_Automate_GEN_AmenagerTerrainPoserSommaire,
    eMission_Automate_GEN_RealiserZonePoser,
    eMission_Automate_GEN_ReconnaitreZoneDeploiement,
    eMission_Automate_GEN_RealiserCampPrisonniers,
    eMission_Automate_GEN_RealiserCampRefugies,
    eMission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial,
    eMission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation,
    eMission_Automate_GEN_AppuyerDebarquementTroupes,
    eMission_Automate_GEN_OuvrirAxesProgression,
    eMission_Automate_INF_AttaquerSouplesse,
    eMission_Automate_INF_Appuyer,
    eMission_Automate_INF_Controler,
    eMission_Automate_INF_Couvrir,
    eMission_Automate_INF_DonnerCoupArret,
    eMission_Automate_INF_Eclairer,
    eMission_Automate_INF_FlancGarder,
    eMission_Automate_INF_Freiner,
    eMission_Automate_INF_Harceler,
    eMission_Automate_INF_Interdire,
    eMission_Automate_INF_MenerDefenseUsure,
    eMission_Automate_INF_PrendrePreciserMaintenirContact,
    eMission_Automate_INF_ReconnaissanceOffensive,
    eMission_Automate_INF_RecueillirUnite,
    eMission_Automate_INF_Reduire,
    eMission_Automate_INF_Relever,
    eMission_Automate_INF_RompreContact,
    eMission_Automate_INF_Surveiller,
    eMission_Automate_INF_Semparer,
    eMission_Automate_INF_Fixer,
    eMission_Automate_INF_AttaquerEnTerrainDifficile,
    eMission_Automate_INF_BarrerDirection,
    eMission_Automate_INF_DefendreFerme,
    eMission_Automate_INF_Tenir,
    eMission_Automate_INF_AppuyerUnFreinage,
    eMission_Automate_INF_AppuyerUneAttaque,
    eMission_Automate_INF_Soutenir,
    eMission_Automate_LOG_SeDeployer,
    eMission_Automate_LOG_AppuyerMouvement,
    eMission_Automate_LOG_ReconnaitreItineraire,
    eMission_Automate_LOG_Surveiller,
    eMission_Automate_LOG_TransporterFormationBlindee,
    eMission_Automate_LOG_AppuyerMouvementDansZone,
    eMission_Automate_LOG_MettreEnOeuvreZoneStationnement,
    eMission_Automate_LOG_AppuyerFranchissementDansZone,
    eMission_Automate_LOG_BloquerZone,
    eMission_Automate_LOG_ReconnaitreZoneContaminee,
    eMission_Automate_LOG_ReconnaitreZoneDeDeploiement,
    eMission_Automate_LOG_ArmerPIAs,
    eMission_Automate_Test_MoveTo,
    eMission_Automate_Decrocher,
    eMission_Automate_ReleverSurPlace,
    eMission_Automate_FaireMouvement,
    eMission_Automate_SeFaireDecontaminer,
    eMission_Automate_Franchir,
    eMission_Automate_Stationner,
    eMission_Automate_SeFaireTransporter,
    eMission_Automate_SeRecompleter,
    eMission_Automate_SeRendre,
    eMission_Automate_FaireMouvementVersCampRefugies,
    eMission_Automate_Generique,
    eMission_Automate_NBC_ReconnaitreUnAxe,
    eMission_Automate_NBC_ReconnaitreUneZone,
    eMission_Automate_NBC_ArmerUnSiteDeDecontamination,
    eMission_Automate_NBC_DecontaminerUneZone,
    eMission_Automate_NBC_ReconnaitreDesSitesROTA,
    eMission_Automate_RENS_ROHUM_RenseignerSur,
    eMission_Automate_RENS_ROHUM_OrienterGuider,
    eMission_Automate_RENS_ROHUM_SExfiltrer,
    eMission_Automate_RENS_ROIM_CL289_RenseignerSur,
    eMission_Automate_RENS_ROIM_SDTI_RenseignerSur,
    eMission_Automate_RENS_ROEM_Appuyer,
    eMission_Automate_RENS_ROEM_RenseignerSur,
    eMission_Automate_RENS_RADINT_RenseignerSur,
    eMission_Automate_TRANS_Commander,

    eNbrAutomataMission
};


enum E_UnitMission
{
    eMission_Pion_ALAT_EvacuationSanitaire,
    eMission_Pion_ALAT_ReconnaitreDansLaProfondeur,
    eMission_Pion_ALAT_ReconnaitreContourEnnemi,
    eMission_Pion_ALAT_DetruireNeutraliserDansZone,
    eMission_Pion_ALAT_DetruireAeromobile,
    eMission_Pion_ALAT_Freiner,
    eMission_Pion_ALAT_Jalonner,
    eMission_Pion_ALAT_Escorter,
    eMission_Pion_ALAT_Heliporter,
    eMission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement,
    eMission_Pion_ALAT_Helitransporter,
    eMission_Pion_ALAT_EffectuerRechercheEtSauvetage,
    eMission_Pion_ALAT_IMEX,
    eMission_Pion_ALAT_Eclairer,
    eMission_Pion_ALAT_Surveiller,
    eMission_Pion_ALAT_AppuyerDirectAuContact,
    eMission_Pion_ALAT_Couvrir,
    eMission_Pion_ALAT_Attaquer,
    eMission_Pion_ASA_DefendreSite,
    eMission_Pion_ASA_DefendreSiteAPartirPosition,
    eMission_Pion_ASA_DefendreZone,
    eMission_Pion_ASA_DefendreZoneAPartirPosition,
    eMission_Pion_ASA_Surveiller,
    eMission_Pion_ASA_DefendreUnites,
    eMission_Pion_ASA_MISTRAL_DefendreSite,
    eMission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition,
    eMission_Pion_ASA_MISTRAL_DefendreZone,
    eMission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition,
    eMission_Pion_ASA_MISTRAL_Surveiller,
    eMission_Pion_ASS_AssurerMiseEnOeuvre,
    eMission_Pion_ASS_AssurerMiseEnOeuvreSurPosition,
    eMission_Pion_ASS_ReconnaitreZoneDeploiement,
    eMission_Pion_ASS_AcquerirLanceursEni,
    eMission_Pion_ASS_EO_AcquerirObjectifs,
    eMission_Pion_ASS_EOP_AcquerirObjectifs,
    eMission_Pion_ASS_RATAC_AcquerirObjectifs,
    eMission_Pion_ABC_ReconnaitreAxe,
    eMission_Pion_ABC_Soutenir,
    eMission_Pion_ABC_Couvrir,
    eMission_Pion_ABC_FlancGarder,
    eMission_Pion_ABC_Reduire,
    eMission_Pion_ABC_Appuyer,
    eMission_Pion_ABC_Fixer,
    eMission_Pion_ABC_RenseignerSur,
    eMission_Pion_ABC_ControlerZone,
    eMission_Pion_ABC_Surveiller,
    eMission_Pion_ABC_Barrer,
    eMission_Pion_ABC_Attaquer,
    eMission_Pion_ABC_ReconnaitrePoint,
    eMission_Pion_ABC_Eclairer,
    eMission_Pion_ABC_Jalonner,
    eMission_Pion_ABC_ContreAttaquer,
    eMission_Pion_ABC_SemparerZone,
    eMission_Pion_ABC_ReleverUnite,
    eMission_Pion_ABC_ArmerPIA,
    eMission_Pion_ABC_OuvrirItineraire,
    eMission_Pion_ABC_Freiner,
    eMission_Pion_ABC_EscorterUnConvoi,
    eMission_Pion_ABC_ArmerPointDeControle,
    eMission_Pion_ABC_OrganiserAccueilColonneRefugies,
    eMission_Pion_GEN_RealiserFosseAC,
    eMission_Pion_GEN_RealiserAbattisMine,
    eMission_Pion_GEN_RealiserPointMine,
    eMission_Pion_GEN_RealiserBarricade,
    eMission_Pion_GEN_RealiserEboulement,
    eMission_Pion_GEN_RealiserBouchonMine,
    eMission_Pion_GEN_DetruireRoute,
    eMission_Pion_GEN_DetruirePont,
    eMission_Pion_GEN_SecuriserItineraire,
    eMission_Pion_GEN_DegagerAbattisMine,
    eMission_Pion_GEN_DegagerEboulement,
    eMission_Pion_GEN_DegagerRues,
    eMission_Pion_GEN_ComblerEntonnoir,
    eMission_Pion_GEN_ComblerFosseAC,
    eMission_Pion_GEN_CreerContournement,
    eMission_Pion_GEN_RealiserZoneMineeLineaire,
    eMission_Pion_GEN_OuvrirTrouee,
    eMission_Pion_GEN_Renforcer,
    eMission_Pion_GEN_FranchirEntonnoir,
    eMission_Pion_GEN_FranchirFosseAC,
    eMission_Pion_GEN_RealiserZoneMineeParDispersion,
    eMission_Pion_GEN_RealiserPostesTir,
    eMission_Pion_GEN_RealiserTravauxProtection,
    eMission_Pion_GEN_RetablirItineraire,
    eMission_Pion_GEN_EquiperExploiter,
    eMission_Pion_GEN_ExecuterVariantement,
    eMission_Pion_GEN_ReconnaitreSiteFranchissement,
    eMission_Pion_GEN_ReconnaitreItineraire,
    eMission_Pion_GEN_DeminerSiteFranchissement,
    eMission_Pion_GEN_ReconnaitreZone,
    eMission_Pion_GEN_CreerPlateForme,
    eMission_Pion_GEN_CreerPiste,
    eMission_Pion_GEN_CreerAirePoser,
    eMission_Pion_GEN_NeutraliserEnginsExplosifs,
    eMission_Pion_GEN_ActiverObstacle,
    eMission_Pion_GEN_AmeliorerMobilite,
    eMission_Pion_GEN_RealiserAireLogistique,
    eMission_Pion_GEN_DepolluerZone,
    eMission_Pion_GEN_AmenagerTerrainPoser,
    eMission_Pion_GEN_RealiserPlateFormeDeploiement,
    eMission_Pion_GEN_ReprendreMissionConstruction,
    eMission_Pion_GEN_RealiserZonePoserHelicoptere,
    eMission_Pion_GEN_AppuyerDebarquementTroupesGU,
    eMission_Pion_GEN_RealiserCampPrisonniers,
    eMission_Pion_GEN_RealiserCampRefugies,
    eMission_Pion_INF_ReconnaitreAxe,
    eMission_Pion_INF_Couvrir,
    eMission_Pion_INF_Reduire,
    eMission_Pion_INF_Appuyer,
    eMission_Pion_INF_Fixer,
    eMission_Pion_INF_ControlerSecteur,
    eMission_Pion_INF_SurveillerSecteur,
    eMission_Pion_INF_Barrer,
    eMission_Pion_INF_ReconnaitrePoint,
    eMission_Pion_INF_Eclairer,
    eMission_Pion_INF_Interdire,
    eMission_Pion_INF_Semparer,
    eMission_Pion_INF_PrendrePreciserContact,
    eMission_Pion_INF_Defendre,
    eMission_Pion_INF_DetruireEmbuscade,
    eMission_Pion_INF_ContreAttaquer,
    eMission_Pion_INF_Freiner,
    eMission_Pion_INF_FlancGarder,
    eMission_Pion_INF_OrganiserUnCheckPoint,
    eMission_Pion_INF_Harceler,
    eMission_Pion_INF_DetruireParCoupDeMain,
    eMission_Pion_INF_EscorterUnConvoi,
    eMission_Pion_INF_RenseignerSur,
    eMission_Pion_INF_Soutenir,
    eMission_Pion_INF_Securiser,
    eMission_Pion_INF_Recueillir,
    eMission_Pion_INF_RepondreAAggression,
    eMission_Pion_LOG_SeDeployer,
    eMission_Pion_LOG_DistribuerMoyens,
    eMission_Pion_LOG_TrierBlesses,
    eMission_Pion_LOG_TraiterBlesses,
    eMission_Pion_LOG_TrierEtTraiterBlesses,
    eMission_Pion_LOG_SoignerBlesses,
    eMission_Pion_LOG_PrendreEnCompteReactionsMentales,
    eMission_Pion_LOG_DecontaminerBlesses,
    eMission_Pion_LOG_Reparer,
    eMission_Pion_LOG_Evacuer,
    eMission_Pion_LOG_Convoyer,
    eMission_Pion_LOG_DistribuerRessources,
    eMission_Pion_LOG_ConstituerConvoi,
    eMission_Pion_LOG_Livrer,
    eMission_Pion_LOG_PreparerBascule,
    eMission_Pion_LOG_AppuyerMouvement,
    eMission_Pion_LOG_ReconnaitreItineraire,
    eMission_Pion_LOG_TransporterUniteChars,
    eMission_Pion_LOG_ReconnaitreZoneContaminee,
    eMission_Pion_LOG_AppuyerMouvementDansZone,
    eMission_Pion_LOG_MettreEnOeuvreZoneStationnement,
    eMission_Pion_LOG_AppuyerFranchissementSurUnPoint,
    eMission_Pion_LOG_BloquerZone,
    eMission_Pion_LOG_ReconnaitreZoneDeDeploiement,
    eMission_Pion_LOG_ArmerPIA,
    eMission_Pion_LOG_Surveiller,
    eMission_Pion_Test_Heliporter,
    eMission_Pion_Test_MoveTo,
    eMission_Pion_Test_CreateObject,
    eMission_Pion_Test_DestroyObject,
    eMission_Pion_Test_CreateBypass,
    eMission_Pion_Test_Fire,
    eMission_Pion_Test_ChangePosture,
    eMission_Pion_Test_PathFind,
    eMission_Pion_Test_FindPosition,
    eMission_Pion_Test_Reinforce,
    eMission_Pion_PlastronEnnemi,
    eMission_Pion_FaireMouvement,
    eMission_Pion_Suivre,
    eMission_Pion_SeFaireDecontaminer,
    eMission_Pion_Franchir,
    eMission_Pion_Decrocher,
    eMission_Pion_Stationner,
    eMission_Pion_SeFaireTransporter,
    eMission_Pion_Generique,
    eMission_Pion_MILICE_Provoquer,
    eMission_Pion_MILICE_PrendreEtTenir,
    eMission_Pion_MILICE_DetruireEmbuscade,
    eMission_Pion_JOINT_MARINE_Transporter,
    eMission_Pion_JOINT_AIR_EffectuerRaid,
    eMission_Pion_NBC_AnimerUnPlotDeDecontamination,
    eMission_Pion_NBC_ReconnaitreItineraire,
    eMission_Pion_NBC_ReconnaitreZone,
    eMission_Pion_NBC_DecontaminerZone,
    eMission_Pion_NBC_ReconnaitreUnSiteROTA,
    eMission_Pion_RENS_ROHUM_RenseignerSur,
    eMission_Pion_RENS_ROHUM_SExfiltrer,
    eMission_Pion_RENS_ROHUM_OrienterGuider,
    eMission_Pion_RENS_ROEM_Appuyer,
    eMission_Pion_RENS_ROEM_RenseignerSur,
    eMission_Pion_RENS_ROEM_MettreEnOeuvre,
    eMission_Pion_RENS_RADINT_RenseignerSur,
    eMission_Pion_RENS_RADINT_MettreEnOeuvre,
    eMission_Pion_RENS_ROIM_MettreEnOeuvre,
    eMission_Pion_RENS_ROIM_SDTI_RenseignerSur,
    eMission_Pion_RENS_ROIM_CL289_RenseignerSur,
    eMission_Pion_TRANS_EtablirGererLiaison,

    eNbrUnitMission
};

enum E_PopulationMission
{
    eMission_Population_FaireMouvement,

    eNbrPopulationMission
};

enum E_FragOrder
{
    eOrdreConduite_MettreTenueNBC,
    eOrdreConduite_EnleverTenueNBC,
    eOrdreConduite_PasserEnSilenceRadio,
    eOrdreConduite_ArreterSilenceRadio,
    eOrdreConduite_PasserEnSilenceRadar,
    eOrdreConduite_ArreterSilenceRadar,
    eOrdreConduite_RecupererTransporteurs,
    eOrdreConduite_AttendreSePoster,
    eOrdreConduite_Poursuivre,
    eOrdreConduite_Interrompre,
    eOrdreConduite_ChangerReglesEngagement,
    eOrdreConduite_Deboucher,
    eOrdreConduite_Decrocher,
    eOrdreConduite_AcquerirObjectif,
    eOrdreConduite_Brouiller,
    eOrdreConduite_ChangerPositionDebarquement,
    eOrdreConduite_Pion_ChangerDePosition,
    eOrdreConduite_Pion_Contourner,
    eOrdreConduite_Pion_EnvoyerVehicule,
    eOrdreConduite_Pion_ActiverObstacle,
    eOrdreConduite_Pion_AppliquerFeux,
    eOrdreConduite_Pion_ExecuterTir,
    eOrdreConduite_Pion_Emettre,
    eOrdreConduite_Pion_StopperEmission,
    eOrdreConduite_Pion_RenforcerEnVSRAM,
    eOrdreConduite_Pion_TransfererVSRAM,
    eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM,
    eOrdreConduite_ModifierTempsBordeeMaintenance,
    eOrdreConduite_ModifierTempsBordeeSante,
    eOrdreConduite_ModifierPrioritesReparations,
    eOrdreConduite_ModifierPrioritesBlesses,
    eOrdreConduite_ModifierPrioritesTactiquesReparations,
    eOrdreConduite_ModifierPrioritesTactiquesBlesses,
    eOrdreConduite_Automate_RealiserVariantement,
    eOrdreConduite_Automate_DesequiperSiteFranchissement,
    eOrdreConduite_Automate_ReagirFaceAEni,
    eOrdreConduite_Automate_AffecterPionAObstacle,
    eOrdreConduite_Automate_ReconnaitreZoneImplantation,
    eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement,
    eOrdreConduite_Automate_ActiverObstacle,
    eOrdreConduite_Automate_TransfererRenforts,

    eNbrFragOrder
};


#endif // __ENT_Emuns_Gen_h_
