// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_ADN2_Enums_Gen_Skeleton.h $
// $Author: Ape $
// $Modtime: 18/02/05 16:06 $
// $Revision: 1 $
// $Workfile: AGR_ADN2_Enums_Gen_Skeleton.h $
//
// *****************************************************************************

#ifndef __ADN_Enums_Gen_Skeleton_h_
#define __ADN_Enums_Gen_Skeleton_h_

// Sorted missions enumeration
enum E_SMission
{
    // 
    // MISSIONS PION
    // 

    // Common
    eSMission_Pion_Common_Debut,

    eSMission_Pion_PlastronEnnemi,
    eSMission_Pion_FaireMouvement,
    eSMission_Pion_Suivre,
    eSMission_Pion_SeFaireDecontaminer,
    eSMission_Pion_Franchir,
    eSMission_Pion_Decrocher,
    eSMission_Pion_Stationner,
    eSMission_Pion_SeFaireTransporter,
    eSMission_Pion_InterdireFranchissementPopulation,
    eSMission_Pion_FilterPopulation,
    eSMission_Pion_RenseignerSurPopulation,
    eSMission_Pion_CanaliserPopulation,
    eSMission_Pion_SecuriserPopulationDansZone,
    eSMission_Pion_SecuriserZoneContreMenacesAsymetriques,
    eSMission_Pion_Generique,
    eSMission_Pion_TRANS_EtablirGererLiaison,

    eSMission_Pion_Common_Fin,

    // ABC
    eSMission_Pion_ABC_Debut,

    eSMission_Pion_ABC_ReconnaitreAxe,
    eSMission_Pion_ABC_Soutenir,
    eSMission_Pion_ABC_Couvrir,
    eSMission_Pion_ABC_FlancGarder,
    eSMission_Pion_ABC_Reduire,
    eSMission_Pion_ABC_Appuyer,
    eSMission_Pion_ABC_Fixer,
    eSMission_Pion_ABC_RenseignerSur,
    eSMission_Pion_ABC_ControlerZone,
    eSMission_Pion_ABC_Surveiller,
    eSMission_Pion_ABC_Barrer,
    eSMission_Pion_ABC_Attaquer,
    eSMission_Pion_ABC_ReconnaitrePoint,
    eSMission_Pion_ABC_Eclairer,
    eSMission_Pion_ABC_Jalonner,
    eSMission_Pion_ABC_ContreAttaquer,
    eSMission_Pion_ABC_SemparerZone,
    eSMission_Pion_ABC_ReleverUnite,
    eSMission_Pion_ABC_ArmerPIA,
    eSMission_Pion_ABC_OuvrirItineraire,
    eSMission_Pion_ABC_Freiner,
    eSMission_Pion_ABC_EscorterUnConvoi,
    eSMission_Pion_ABC_ArmerPointDeControle,
    eSMission_Pion_ABC_OrganiserAccueilColonneRefugies,

    eSMission_Pion_ABC_Fin,
          
    // INF
    eSMission_Pion_INF_Debut,

    eSMission_Pion_INF_ReconnaitreAxe,
    eSMission_Pion_INF_Couvrir,
    eSMission_Pion_INF_Reduire,
    eSMission_Pion_INF_Appuyer,
    eSMission_Pion_INF_Fixer,
    eSMission_Pion_INF_ControlerSecteur,
    eSMission_Pion_INF_SurveillerSecteur,
    eSMission_Pion_INF_Barrer,
    eSMission_Pion_INF_ReconnaitrePoint,
    eSMission_Pion_INF_Eclairer,
    eSMission_Pion_INF_Interdire,
    eSMission_Pion_INF_Semparer,
    eSMission_Pion_INF_PrendrePreciserContact,
    eSMission_Pion_INF_Defendre,
    eSMission_Pion_INF_DetruireEmbuscade,
    eSMission_Pion_INF_ContreAttaquer,
    eSMission_Pion_INF_Freiner,
    eSMission_Pion_INF_FlancGarder,
    eSMission_Pion_INF_OrganiserUnCheckPoint,
    eSMission_Pion_INF_Harceler,
    eSMission_Pion_INF_DetruireParCoupDeMain,
    eSMission_Pion_INF_EscorterUnConvoi,
    eSMission_Pion_INF_RenseignerSur,
    eSMission_Pion_INF_Soutenir,
    eSMission_Pion_INF_Securiser,
    eSMission_Pion_INF_Recueillir,
    eSMission_Pion_INF_RepondreAAggression,

    eSMission_Pion_INF_Fin,
    
    // GEN
    eSMission_Pion_GEN_Debut,

    eSMission_Pion_GEN_RealiserFosseAC,
    eSMission_Pion_GEN_RealiserAbattisMine,
    eSMission_Pion_GEN_RealiserPointMine,
    eSMission_Pion_GEN_RealiserBarricade,
    eSMission_Pion_GEN_RealiserEboulement,
    eSMission_Pion_GEN_RealiserBouchonMine,
    eSMission_Pion_GEN_DetruireRoute,
    eSMission_Pion_GEN_DetruirePont,
    eSMission_Pion_GEN_SecuriserItineraire,
    eSMission_Pion_GEN_DegagerAbattisMine,
    eSMission_Pion_GEN_DegagerEboulement,
    eSMission_Pion_GEN_DegagerRues,
    eSMission_Pion_GEN_ComblerEntonnoir,
    eSMission_Pion_GEN_ComblerFosseAC,
    eSMission_Pion_GEN_CreerContournement,
    eSMission_Pion_GEN_RealiserZoneMineeLineaire,
    eSMission_Pion_GEN_OuvrirTrouee,
    eSMission_Pion_GEN_Renforcer,
    eSMission_Pion_GEN_FranchirEntonnoir,
    eSMission_Pion_GEN_FranchirFosseAC,
    eSMission_Pion_GEN_RealiserZoneMineeParDispersion,
    eSMission_Pion_GEN_RealiserPostesTir,
    eSMission_Pion_GEN_RealiserTravauxProtection,
    eSMission_Pion_GEN_RetablirItineraire,
    eSMission_Pion_GEN_EquiperExploiter,
    eSMission_Pion_GEN_ExecuterVariantement,
    eSMission_Pion_GEN_ReconnaitreSiteFranchissement,
    eSMission_Pion_GEN_ReconnaitreItineraire,
    eSMission_Pion_GEN_DeminerSiteFranchissement,
    eSMission_Pion_GEN_ReconnaitreZone,
    eSMission_Pion_GEN_CreerPlateForme,
    eSMission_Pion_GEN_CreerPiste,
    eSMission_Pion_GEN_CreerAirePoser,
    eSMission_Pion_GEN_NeutraliserEnginsExplosifs,
    eSMission_Pion_GEN_ActiverObstacle,
    eSMission_Pion_GEN_AmeliorerMobilite,
    eSMission_Pion_GEN_RealiserAireLogistique,
    eSMission_Pion_GEN_DepolluerZone,
    eSMission_Pion_GEN_AmenagerTerrainPoser,
    eSMission_Pion_GEN_RealiserPlateFormeDeploiement,
    eSMission_Pion_GEN_ReprendreMissionConstruction,
    eSMission_Pion_GEN_RealiserZonePoserHelicoptere,
    eSMission_Pion_GEN_AppuyerDebarquementTroupesGU,
    eSMission_Pion_GEN_RealiserCampPrisonniers,
    eSMission_Pion_GEN_RealiserCampRefugies,

    eSMission_Pion_GEN_Fin,
    
    // 3D - ASS
    eSMission_Pion_3D_Debut,

    eSMission_Pion_ASA_DefendreSite,
    eSMission_Pion_ASA_DefendreSiteAPartirPosition,
    eSMission_Pion_ASA_DefendreZone,
    eSMission_Pion_ASA_DefendreZoneAPartirPosition,
    eSMission_Pion_ASA_Surveiller,
    eSMission_Pion_ASA_DefendreUnites,
    eSMission_Pion_ASA_MISTRAL_DefendreSite,
    eSMission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition,
    eSMission_Pion_ASA_MISTRAL_DefendreZone,
    eSMission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition,
    eSMission_Pion_ASA_MISTRAL_Surveiller,
    eSMission_Pion_ASS_AssurerMiseEnOeuvre,
    eSMission_Pion_ASS_AssurerMiseEnOeuvreSurPosition,
    eSMission_Pion_ASS_ReconnaitreZoneDeploiement,
    eSMission_Pion_ASS_AcquerirLanceursEni,
    eSMission_Pion_ASS_EO_AcquerirObjectifs,
    eSMission_Pion_ASS_EOP_AcquerirObjectifs,
    eSMission_Pion_ASS_RATAC_AcquerirObjectifs,

    eSMission_Pion_3D_Fin,

    // 3D - ALAT
    eSMission_Pion_ALAT_Debut,

    eSMission_Pion_ALAT_EvacuationSanitaire,
    eSMission_Pion_ALAT_ReconnaitreDansLaProfondeur,
    eSMission_Pion_ALAT_ReconnaitreContourEnnemi,
    eSMission_Pion_ALAT_DetruireNeutraliserDansZone,
    eSMission_Pion_ALAT_DetruireAeromobile,
    eSMission_Pion_ALAT_Freiner,
    eSMission_Pion_ALAT_Jalonner,
    eSMission_Pion_ALAT_Escorter,
    eSMission_Pion_ALAT_Heliporter,
    eSMission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement,
    eSMission_Pion_ALAT_Helitransporter,
    eSMission_Pion_ALAT_EffectuerRechercheEtSauvetage,
    eSMission_Pion_ALAT_IMEX,
    eSMission_Pion_ALAT_Eclairer,
    eSMission_Pion_ALAT_Surveiller,
    eSMission_Pion_ALAT_AppuyerDirectAuContact,
    eSMission_Pion_ALAT_Couvrir,
    eSMission_Pion_ALAT_Attaquer,

    eSMission_Pion_ALAT_Fin,

    // LOG - SANTE
    eSMission_Pion_LOG_Debut,

    eSMission_Pion_LOG_SeDeployer,
    eSMission_Pion_LOG_DistribuerMoyens,
    eSMission_Pion_LOG_TrierBlesses,
    eSMission_Pion_LOG_TraiterBlesses,
    eSMission_Pion_LOG_TrierEtTraiterBlesses,
    eSMission_Pion_LOG_SoignerBlesses,
    eSMission_Pion_LOG_PrendreEnCompteReactionsMentales,
    eSMission_Pion_LOG_DecontaminerBlesses,
    eSMission_Pion_LOG_Reparer,
    eSMission_Pion_LOG_Evacuer,
    eSMission_Pion_LOG_Convoyer,
    eSMission_Pion_LOG_DistribuerRessources,
    eSMission_Pion_LOG_ConstituerConvoi,
    eSMission_Pion_LOG_Livrer,
    eSMission_Pion_LOG_PreparerBascule,
    eSMission_Pion_LOG_AppuyerMouvement,
    eSMission_Pion_LOG_ReconnaitreItineraire,
    eSMission_Pion_LOG_TransporterUnite,
    eSMission_Pion_LOG_ReconnaitreZoneContaminee,
    eSMission_Pion_LOG_AppuyerMouvementDansZone,
    eSMission_Pion_LOG_MettreEnOeuvreZoneStationnement,
    eSMission_Pion_LOG_AppuyerFranchissementSurUnPoint,
    eSMission_Pion_LOG_BloquerZone,
    eSMission_Pion_LOG_ReconnaitreZoneDeDeploiement,
    eSMission_Pion_LOG_ArmerPIA,
    eSMission_Pion_LOG_Surveiller,

    eSMission_Pion_LOG_Fin,

    // RENS
    eSMission_Pion_RENS_Debut,

    eSMission_Pion_RENS_ROHUM_RenseignerSur,
    eSMission_Pion_RENS_ROHUM_SExfiltrer,
    eSMission_Pion_RENS_ROHUM_OrienterGuider,
    eSMission_Pion_RENS_ROEM_Appuyer,
    eSMission_Pion_RENS_ROEM_RenseignerSur,
    eSMission_Pion_RENS_ROEM_MettreEnOeuvre,
    eSMission_Pion_RENS_RADINT_RenseignerSur,
    eSMission_Pion_RENS_RADINT_MettreEnOeuvre,
    eSMission_Pion_RENS_ROIM_MettreEnOeuvre,
    eSMission_Pion_RENS_ROIM_SDTI_RenseignerSur,
    eSMission_Pion_RENS_ROIM_CL289_RenseignerSur,

    eSMission_Pion_RENS_Fin,

    // NBC
    eSMission_Pion_NBC_Debut,

    eSMission_Pion_NBC_AnimerUnPlotDeDecontamination,
    eSMission_Pion_NBC_ReconnaitreItineraire,
    eSMission_Pion_NBC_ReconnaitreZone,
    eSMission_Pion_NBC_DecontaminerZone,
    eSMission_Pion_NBC_ReconnaitreUnSiteROTA,

    eSMission_Pion_NBC_Fin,

    // MILICE
    eSMission_Pion_MILICE_Debut,

    eSMission_Pion_MILICE_Provoquer,
    eSMission_Pion_MILICE_PrendreEtTenir,
    eSMission_Pion_MILICE_DetruireEmbuscade,

    eSMission_Pion_MILICE_Fin,

    // JOINT
    eSMission_Pion_JOINT_Debut,

    eSMission_Pion_JOINT_MARINE_Transporter,
    eSMission_Pion_JOINT_AIR_EffectuerRaid,

    eSMission_Pion_JOINT_Fin,

    // ASY
    eSMission_Pion_ASY_Debut,

    eSMission_Pion_ASY_SInfiltrer,
    eSMission_Pion_ASY_CommettreAttentatContrePopulation,
    eSMission_Pion_ASY_CommettreAttentatContreForcesArmees,
    eSMission_Pion_ASY_CommettreAttentatContreInstallation,
    eSMission_Pion_ASY_CommettreExactionsSurPopulationDansZone,
    eSMission_Pion_ASY_Renseigner,

    eSMission_Pion_ASY_Fin,

    // 
    // MISSIONS AUTOMATE
    // 

    // Common
    eSMission_Automate_Common_Debut,

    eSMission_Automate_Decrocher,
    eSMission_Automate_ReleverSurPlace,
    eSMission_Automate_FaireMouvement,
    eSMission_Automate_SeFaireDecontaminer,
    eSMission_Automate_Franchir,
    eSMission_Automate_Stationner,
    eSMission_Automate_SeFaireTransporter,
    eSMission_Automate_SeRecompleter,
    eSMission_Automate_SeRendre,
    eSMission_Automate_FaireMouvementVersCampRefugies,
    eSMission_Automate_Generique,
    eSMission_Automate_TRANS_Commander,

    eSMission_Automate_Common_Fin,

    // ABC
    eSMission_Automate_ABC_Debut,

    eSMission_Automate_ABC_ArmerPointDeControle,
    eSMission_Automate_ABC_OrganiserAccueilColonneRefugies,
    eSMission_Automate_ABC_ProtegerPC,
    eSMission_Automate_ABC_Appuyer,
    eSMission_Automate_ABC_Attaquer,
    eSMission_Automate_ABC_ContreAttaquer,
    eSMission_Automate_ABC_Controler,
    eSMission_Automate_ABC_Couvrir,
    eSMission_Automate_ABC_DonnerCoupArret,
    eSMission_Automate_ABC_Eclairer,
    eSMission_Automate_ABC_EffectuerRaid,
    eSMission_Automate_ABC_Interdire,
    eSMission_Automate_ABC_Jalonner,
    eSMission_Automate_ABC_PrendrePreciserContact,
    eSMission_Automate_ABC_Reconnaitre,
    eSMission_Automate_ABC_ReconnaitrePoint,
    eSMission_Automate_ABC_Reduire,
    eSMission_Automate_ABC_ReleverEnDepassant,
    eSMission_Automate_ABC_Semparer,
    eSMission_Automate_ABC_Tenir,
    eSMission_Automate_ABC_Soutenir,
    eSMission_Automate_ABC_Surveiller,
    eSMission_Automate_ABC_FlancGarder,
    eSMission_Automate_ABC_RenseignerSur,
    eSMission_Automate_ABC_Fixer,
    eSMission_Automate_ABC_Freiner,
    eSMission_Automate_ABC_SurveillerItineraire,
    eSMission_Automate_ABC_RenseignerSurUnAxe,
    eSMission_Automate_ABC_RecueillirUnite,
    eSMission_Automate_ABC_EscorterUnConvoi,

    eSMission_Automate_ABC_Fin,

    // INF
    eSMission_Automate_INF_Debut,

    eSMission_Automate_INF_AttaquerSouplesse,
    eSMission_Automate_INF_Appuyer,
    eSMission_Automate_INF_Controler,
    eSMission_Automate_INF_Couvrir,
    eSMission_Automate_INF_DonnerCoupArret,
    eSMission_Automate_INF_Eclairer,
    eSMission_Automate_INF_FlancGarder,
    eSMission_Automate_INF_Freiner,
    eSMission_Automate_INF_Harceler,
    eSMission_Automate_INF_Interdire,
    eSMission_Automate_INF_MenerDefenseUsure,
    eSMission_Automate_INF_PrendrePreciserMaintenirContact,
    eSMission_Automate_INF_ReconnaissanceOffensive,
    eSMission_Automate_INF_RecueillirUnite,
    eSMission_Automate_INF_Reduire,
    eSMission_Automate_INF_Relever,
    eSMission_Automate_INF_RompreContact,
    eSMission_Automate_INF_Surveiller,
    eSMission_Automate_INF_Semparer,
    eSMission_Automate_INF_Fixer,
    eSMission_Automate_INF_AttaquerEnTerrainDifficile,
    eSMission_Automate_INF_BarrerDirection,
    eSMission_Automate_INF_DefendreFerme,
    eSMission_Automate_INF_Tenir,
    eSMission_Automate_INF_AppuyerUnFreinage,
    eSMission_Automate_INF_AppuyerUneAttaque,
    eSMission_Automate_INF_Soutenir,

    eSMission_Automate_INF_Fin,

    // GEN                                               
    eSMission_Automate_GEN_Debut,

    eSMission_Automate_GEN_SeDeployer,
    eSMission_Automate_GEN_FaireFranchir,
    eSMission_Automate_GEN_RealiserSystemeObstacles,
    eSMission_Automate_GEN_FranchirDeViveForce,
    eSMission_Automate_GEN_RetablirItineraires,
    eSMission_Automate_GEN_AmenagerAireStationnement,
    eSMission_Automate_GEN_AmenagerTerrainPoserSommaire,
    eSMission_Automate_GEN_RealiserZonePoser,
    eSMission_Automate_GEN_ReconnaitreZoneDeploiement,
    eSMission_Automate_GEN_RealiserCampPrisonniers,
    eSMission_Automate_GEN_RealiserCampRefugies,
    eSMission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial,
    eSMission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation,
    eSMission_Automate_GEN_AppuyerDebarquementTroupes,
    eSMission_Automate_GEN_OuvrirAxesProgression,

    eSMission_Automate_GEN_Fin,

    // LOG : SANTE
    eSMission_Automate_LOG_Debut,

    eSMission_Automate_LOG_SeDeployer,
    eSMission_Automate_LOG_AppuyerMouvement,
    eSMission_Automate_LOG_ReconnaitreItineraire,
    eSMission_Automate_LOG_Surveiller,
    eSMission_Automate_LOG_TransporterUnites,
    eSMission_Automate_LOG_AppuyerMouvementDansZone,
    eSMission_Automate_LOG_MettreEnOeuvreZoneStationnement,
    eSMission_Automate_LOG_AppuyerFranchissementDansZone,
    eSMission_Automate_LOG_BloquerZone,
    eSMission_Automate_LOG_ReconnaitreZoneContaminee,
    eSMission_Automate_LOG_ReconnaitreZoneDeDeploiement,
    eSMission_Automate_LOG_ArmerPIAs,

    eSMission_Automate_LOG_Fin,

    // ASS
    eSMission_Automate_3D_Debut,

    eSMission_Automate_ASA_DefendreZone,
    eSMission_Automate_ASA_DefendreSite,
    eSMission_Automate_ASA_Surveiller,
    eSMission_Automate_ASA_DefendreItineraire,
    eSMission_Automate_ASA_MISTRAL_DefendreZone,
    eSMission_Automate_ASA_MISTRAL_DefendreSite,
    eSMission_Automate_ASA_MISTRAL_Surveiller,
    eSMission_Automate_ASA_HAWK_DefendreZone,
    eSMission_Automate_ASA_HAWK_DefendreSite,
    eSMission_Automate_ASA_HAWK_Surveiller,
    eSMission_Automate_ASA_DefendreUnites,
    eSMission_Automate_ASS_SeDeployer,
    eSMission_Automate_ASS_ReconnaitreZonesDeploiement,
    eSMission_Automate_ASS_AcquerirObjectifs,

    eSMission_Automate_3D_Fin,

    // ALAT
    eSMission_Automate_ALAT_Debut,

    eSMission_Automate_ALAT_Heliporter,
    eSMission_Automate_ALAT_Jalonner,
    eSMission_Automate_ALAT_Helitransporter,
    eSMission_Automate_ALAT_ExtrairePersonnel,
    eSMission_Automate_ALAT_Appuyer,
    eSMission_Automate_ALAT_Couvrir,
    eSMission_Automate_ALAT_Surveiller,
    eSMission_Automate_ALAT_Reconnaitre,
    eSMission_Automate_ALAT_Freiner,
    eSMission_Automate_ALAT_Escorter,
    eSMission_Automate_ALAT_Attaquer,
    eSMission_Automate_ALAT_RecueillirFormationAeromobile,
    eSMission_Automate_ALAT_AttaquerFormationAeromobile,
    eSMission_Automate_ALAT_DonnerCoupArret,
    eSMission_Automate_ALAT_DetruireNeutraliserDansProfondeur,
    eSMission_Automate_ALAT_DetruireNeutraliserDansZone,
    eSMission_Automate_ALAT_EffectuerRechercheEtSauvetage,

    eSMission_Automate_ALAT_Fin,

    // RENS
    eSMission_Automate_RENS_Debut,

    eSMission_Automate_RENS_ROHUM_RenseignerSur,
    eSMission_Automate_RENS_ROHUM_OrienterGuider,
    eSMission_Automate_RENS_ROHUM_SExfiltrer,
    eSMission_Automate_RENS_ROIM_CL289_RenseignerSur,
    eSMission_Automate_RENS_ROIM_SDTI_RenseignerSur,
    eSMission_Automate_RENS_ROEM_Appuyer,
    eSMission_Automate_RENS_ROEM_RenseignerSur,
    eSMission_Automate_RENS_RADINT_RenseignerSur,

    eSMission_Automate_RENS_Fin,

    // NBC
    eSMission_Automate_NBC_Debut,

    eSMission_Automate_NBC_ReconnaitreUnAxe,
    eSMission_Automate_NBC_ReconnaitreUneZone,
    eSMission_Automate_NBC_ArmerUnSiteDeDecontamination,
    eSMission_Automate_NBC_DecontaminerUneZone,
    eSMission_Automate_NBC_ReconnaitreDesSitesROTA,

    eSMission_Automate_NBC_Fin,

    // MILICE
    eSMission_Automate_MILICE_Debut,


    eSMission_Automate_MILICE_Fin,

    // JOINT
    eSMission_Automate_JOINT_Debut,


    eSMission_Automate_JOINT_Fin,

    // ASY
    eSMission_Automate_ASY_Debut,


    eSMission_Automate_ASY_Fin,

    // 
    // MISSIONS AUTOMATE
    // 

    // Common
    eSMission_Population_Common_Debut,

    eSMission_Population_FaireMouvement,
    eSMission_Population_Manifester,
    eSMission_Population_CommettreExactions,

    eSMission_Population_Common_Fin,

    eNbrSMission,

    eSMission_Automate_Test_MoveTo,
    eSMission_Pion_Test_Heliporter,
    eSMission_Pion_Test_MoveTo,
    eSMission_Pion_Test_CreateObject,
    eSMission_Pion_Test_DestroyObject,
    eSMission_Pion_Test_CreateBypass,
    eSMission_Pion_Test_Fire,
    eSMission_Pion_Test_ChangePosture,
    eSMission_Pion_Test_PathFind,
    eSMission_Pion_Test_FindPosition,
    eSMission_Pion_Test_Reinforce,


    eNbrSMissionTest
};

#endif // __ADN_Enums_Gen_Skeleton_h_
