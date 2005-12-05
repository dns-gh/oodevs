// *****************************************************************************
//
// $Created: AGE 2004-09-16 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/Missions_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 14:20 $
// $Revision: 5 $
// $Workfile: Missions_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "Missions.h"

#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Orders/Population/MIL_PopulationMissionType.h"
#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"

#include "Automate\MIL_AutomateMission_ABC_ArmerPointDeControle.cpp"
#include "Automate\MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies.cpp"
#include "Automate\MIL_AutomateMission_ABC_ProtegerPC.cpp"
#include "Automate\MIL_AutomateMission_ABC_Appuyer.cpp"
#include "Automate\MIL_AutomateMission_ABC_Attaquer.cpp"
#include "Automate\MIL_AutomateMission_ABC_ContreAttaquer.cpp"
#include "Automate\MIL_AutomateMission_ABC_Controler.cpp"
#include "Automate\MIL_AutomateMission_ABC_Couvrir.cpp"
#include "Automate\MIL_AutomateMission_ABC_DonnerCoupArret.cpp"
#include "Automate\MIL_AutomateMission_ABC_Eclairer.cpp"
#include "Automate\MIL_AutomateMission_ABC_EffectuerRaid.cpp"
#include "Automate\MIL_AutomateMission_ABC_Interdire.cpp"
#include "Automate\MIL_AutomateMission_ABC_Jalonner.cpp"
#include "Automate\MIL_AutomateMission_ABC_PrendrePreciserContact.cpp"
#include "Automate\MIL_AutomateMission_ABC_Reconnaitre.cpp"
#include "Automate\MIL_AutomateMission_ABC_ReconnaitrePoint.cpp"
#include "Automate\MIL_AutomateMission_ABC_Reduire.cpp"
#include "Automate\MIL_AutomateMission_ABC_ReleverEnDepassant.cpp"
#include "Automate\MIL_AutomateMission_ABC_Semparer.cpp"
#include "Automate\MIL_AutomateMission_ABC_Tenir.cpp"
#include "Automate\MIL_AutomateMission_ABC_Soutenir.cpp"
#include "Automate\MIL_AutomateMission_ABC_Surveiller.cpp"
#include "Automate\MIL_AutomateMission_ABC_FlancGarder.cpp"
#include "Automate\MIL_AutomateMission_ABC_RenseignerSur.cpp"
#include "Automate\MIL_AutomateMission_ABC_Fixer.cpp"
#include "Automate\MIL_AutomateMission_ABC_Freiner.cpp"
#include "Automate\MIL_AutomateMission_ABC_SurveillerItineraire.cpp"
#include "Automate\MIL_AutomateMission_ABC_RenseignerSurUnAxe.cpp"
#include "Automate\MIL_AutomateMission_ABC_RecueillirUnite.cpp"
#include "Automate\MIL_AutomateMission_ABC_EscorterUnConvoi.cpp"
#include "Pion\MIL_PionMission_ALAT_EvacuationSanitaire.cpp"
#include "Pion\MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur.cpp"
#include "Pion\MIL_PionMission_ALAT_ReconnaitreContourEnnemi.cpp"
#include "Pion\MIL_PionMission_ALAT_DetruireNeutraliserDansZone.cpp"
#include "Pion\MIL_PionMission_ALAT_DetruireAeromobile.cpp"
#include "Pion\MIL_PionMission_ALAT_Freiner.cpp"
#include "Pion\MIL_PionMission_ALAT_Jalonner.cpp"
#include "Pion\MIL_PionMission_ALAT_Escorter.cpp"
#include "Pion\MIL_PionMission_ALAT_Heliporter.cpp"
#include "Pion\MIL_PionMission_ALAT_HeliporterHelitransporterPlotRavitaillement.cpp"
#include "Pion\MIL_PionMission_ALAT_Helitransporter.cpp"
#include "Pion\MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage.cpp"
#include "Pion\MIL_PionMission_ALAT_IMEX.cpp"
#include "Pion\MIL_PionMission_ALAT_Eclairer.cpp"
#include "Pion\MIL_PionMission_ALAT_Surveiller.cpp"
#include "Pion\MIL_PionMission_ALAT_AppuyerDirectAuContact.cpp"
#include "Pion\MIL_PionMission_ALAT_Couvrir.cpp"
#include "Pion\MIL_PionMission_ALAT_Attaquer.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Heliporter.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Jalonner.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Helitransporter.cpp"
#include "Automate\MIL_AutomateMission_ALAT_ExtrairePersonnel.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Appuyer.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Couvrir.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Surveiller.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Reconnaitre.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Freiner.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Escorter.cpp"
#include "Automate\MIL_AutomateMission_ALAT_Attaquer.cpp"
#include "Automate\MIL_AutomateMission_ALAT_RecueillirFormationAeromobile.cpp"
#include "Automate\MIL_AutomateMission_ALAT_AttaquerFormationAeromobile.cpp"
#include "Automate\MIL_AutomateMission_ALAT_DonnerCoupArret.cpp"
#include "Automate\MIL_AutomateMission_ALAT_DetruireNeutraliserDansProfondeur.cpp"
#include "Automate\MIL_AutomateMission_ALAT_DetruireNeutraliserDansZone.cpp"
#include "Automate\MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage.cpp"
#include "Automate\MIL_AutomateMission_ASA_DefendreZone.cpp"
#include "Automate\MIL_AutomateMission_ASA_DefendreSite.cpp"
#include "Automate\MIL_AutomateMission_ASA_Surveiller.cpp"
#include "Automate\MIL_AutomateMission_ASA_DefendreItineraire.cpp"
#include "Automate\MIL_AutomateMission_ASA_MISTRAL_DefendreZone.cpp"
#include "Automate\MIL_AutomateMission_ASA_MISTRAL_DefendreSite.cpp"
#include "Automate\MIL_AutomateMission_ASA_MISTRAL_Surveiller.cpp"
#include "Automate\MIL_AutomateMission_ASA_HAWK_DefendreZone.cpp"
#include "Automate\MIL_AutomateMission_ASA_HAWK_DefendreSite.cpp"
#include "Automate\MIL_AutomateMission_ASA_HAWK_Surveiller.cpp"
#include "Automate\MIL_AutomateMission_ASA_DefendreUnites.cpp"
#include "Automate\MIL_AutomateMission_ASS_SeDeployer.cpp"
#include "Automate\MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement.cpp"
#include "Automate\MIL_AutomateMission_ASS_AcquerirObjectifs.cpp"
#include "Automate\MIL_AutomateMission_GEN_SeDeployer.cpp"
#include "Automate\MIL_AutomateMission_GEN_FaireFranchir.cpp"
#include "Automate\MIL_AutomateMission_GEN_RealiserSystemeObstacles.cpp"
#include "Automate\MIL_AutomateMission_GEN_FranchirDeViveForce.cpp"
#include "Automate\MIL_AutomateMission_GEN_RetablirItineraires.cpp"
#include "Automate\MIL_AutomateMission_GEN_AmenagerAireStationnement.cpp"
#include "Automate\MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire.cpp"
#include "Automate\MIL_AutomateMission_GEN_RealiserZonePoser.cpp"
#include "Automate\MIL_AutomateMission_GEN_ReconnaitreZoneDeploiement.cpp"
#include "Automate\MIL_AutomateMission_GEN_RealiserCampPrisonniers.cpp"
#include "Automate\MIL_AutomateMission_GEN_RealiserCampRefugies.cpp"
#include "Automate\MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial.cpp"
#include "Automate\MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation.cpp"
#include "Automate\MIL_AutomateMission_GEN_AppuyerDebarquementTroupes.cpp"
#include "Automate\MIL_AutomateMission_GEN_OuvrirAxesProgression.cpp"
#include "Automate\MIL_AutomateMission_INF_AttaquerSouplesse.cpp"
#include "Automate\MIL_AutomateMission_INF_Appuyer.cpp"
#include "Automate\MIL_AutomateMission_INF_Controler.cpp"
#include "Automate\MIL_AutomateMission_INF_Couvrir.cpp"
#include "Automate\MIL_AutomateMission_INF_DonnerCoupArret.cpp"
#include "Automate\MIL_AutomateMission_INF_Eclairer.cpp"
#include "Automate\MIL_AutomateMission_INF_FlancGarder.cpp"
#include "Automate\MIL_AutomateMission_INF_Freiner.cpp"
#include "Automate\MIL_AutomateMission_INF_Harceler.cpp"
#include "Automate\MIL_AutomateMission_INF_Interdire.cpp"
#include "Automate\MIL_AutomateMission_INF_MenerDefenseUsure.cpp"
#include "Automate\MIL_AutomateMission_INF_PrendrePreciserMaintenirContact.cpp"
#include "Automate\MIL_AutomateMission_INF_ReconnaissanceOffensive.cpp"
#include "Automate\MIL_AutomateMission_INF_RecueillirUnite.cpp"
#include "Automate\MIL_AutomateMission_INF_Reduire.cpp"
#include "Automate\MIL_AutomateMission_INF_Relever.cpp"
#include "Automate\MIL_AutomateMission_INF_RompreContact.cpp"
#include "Automate\MIL_AutomateMission_INF_Surveiller.cpp"
#include "Automate\MIL_AutomateMission_INF_Semparer.cpp"
#include "Automate\MIL_AutomateMission_INF_Fixer.cpp"
#include "Automate\MIL_AutomateMission_INF_AttaquerEnTerrainDifficile.cpp"
#include "Automate\MIL_AutomateMission_INF_BarrerDirection.cpp"
#include "Automate\MIL_AutomateMission_INF_DefendreFerme.cpp"
#include "Automate\MIL_AutomateMission_INF_Tenir.cpp"
#include "Automate\MIL_AutomateMission_INF_AppuyerUnFreinage.cpp"
#include "Automate\MIL_AutomateMission_INF_AppuyerUneAttaque.cpp"
#include "Automate\MIL_AutomateMission_INF_Soutenir.cpp"
#include "Automate\MIL_AutomateMission_LOG_SeDeployer.cpp"
#include "Automate\MIL_AutomateMission_LOG_AppuyerMouvement.cpp"
#include "Automate\MIL_AutomateMission_LOG_ReconnaitreItineraire.cpp"
#include "Automate\MIL_AutomateMission_LOG_Surveiller.cpp"
#include "Automate\MIL_AutomateMission_LOG_TransporterFormationBlindee.cpp"
#include "Automate\MIL_AutomateMission_LOG_AppuyerMouvementDansZone.cpp"
#include "Automate\MIL_AutomateMission_LOG_MettreEnOeuvreZoneStationnement.cpp"
#include "Automate\MIL_AutomateMission_LOG_AppuyerFranchissementDansZone.cpp"
#include "Automate\MIL_AutomateMission_LOG_BloquerZone.cpp"
#include "Automate\MIL_AutomateMission_LOG_ReconnaitreZoneContaminee.cpp"
#include "Automate\MIL_AutomateMission_LOG_ReconnaitreZoneDeDeploiement.cpp"
#include "Automate\MIL_AutomateMission_LOG_ArmerPIAs.cpp"
#include "Automate\MIL_AutomateMission_Test_MoveTo.cpp"
#include "Automate\MIL_AutomateMission_Decrocher.cpp"
#include "Automate\MIL_AutomateMission_ReleverSurPlace.cpp"
#include "Automate\MIL_AutomateMission_FaireMouvement.cpp"
#include "Automate\MIL_AutomateMission_SeFaireDecontaminer.cpp"
#include "Automate\MIL_AutomateMission_Franchir.cpp"
#include "Automate\MIL_AutomateMission_Stationner.cpp"
#include "Automate\MIL_AutomateMission_SeFaireTransporter.cpp"
#include "Automate\MIL_AutomateMission_SeRecompleter.cpp"
#include "Automate\MIL_AutomateMission_SeRendre.cpp"
#include "Automate\MIL_AutomateMission_FaireMouvementVersCampRefugies.cpp"
#include "Automate\MIL_AutomateMission_Generique.cpp"
#include "Automate\MIL_AutomateMission_NBC_ReconnaitreUnAxe.cpp"
#include "Automate\MIL_AutomateMission_NBC_ReconnaitreUneZone.cpp"
#include "Automate\MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination.cpp"
#include "Automate\MIL_AutomateMission_NBC_DecontaminerUneZone.cpp"
#include "Automate\MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA.cpp"
#include "Automate\MIL_AutomateMission_RENS_ROHUM_RenseignerSur.cpp"
#include "Automate\MIL_AutomateMission_RENS_ROHUM_OrienterGuider.cpp"
#include "Automate\MIL_AutomateMission_RENS_ROHUM_SExfiltrer.cpp"
#include "Automate\MIL_AutomateMission_RENS_ROIM_CL289_RenseignerSur.cpp"
#include "Automate\MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur.cpp"
#include "Automate\MIL_AutomateMission_RENS_ROEM_Appuyer.cpp"
#include "Automate\MIL_AutomateMission_RENS_ROEM_RenseignerSur.cpp"
#include "Automate\MIL_AutomateMission_RENS_RADINT_RenseignerSur.cpp"
#include "Automate\MIL_AutomateMission_TRANS_Commander.cpp"
#include "Pion\MIL_PionMission_ASA_DefendreSite.cpp"
#include "Pion\MIL_PionMission_ASA_DefendreSiteAPartirPosition.cpp"
#include "Pion\MIL_PionMission_ASA_DefendreZone.cpp"
#include "Pion\MIL_PionMission_ASA_DefendreZoneAPartirPosition.cpp"
#include "Pion\MIL_PionMission_ASA_Surveiller.cpp"
#include "Pion\MIL_PionMission_ASA_DefendreUnites.cpp"
#include "Pion\MIL_PionMission_ASA_MISTRAL_DefendreSite.cpp"
#include "Pion\MIL_PionMission_ASA_MISTRAL_DefendreSiteAPartirPosition.cpp"
#include "Pion\MIL_PionMission_ASA_MISTRAL_DefendreZone.cpp"
#include "Pion\MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition.cpp"
#include "Pion\MIL_PionMission_ASA_MISTRAL_Surveiller.cpp"
#include "Pion\MIL_PionMission_ASS_AssurerMiseEnOeuvre.cpp"
#include "Pion\MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition.cpp"
#include "Pion\MIL_PionMission_ASS_ReconnaitreZoneDeploiement.cpp"
#include "Pion\MIL_PionMission_ASS_AcquerirLanceursEni.cpp"
#include "Pion\MIL_PionMission_ASS_EO_AcquerirObjectifs.cpp"
#include "Pion\MIL_PionMission_ASS_EOP_AcquerirObjectifs.cpp"
#include "Pion\MIL_PionMission_ASS_RATAC_AcquerirObjectifs.cpp"
#include "Pion\MIL_PionMission_ABC_ReconnaitreAxe.cpp"
#include "Pion\MIL_PionMission_ABC_Soutenir.cpp"
#include "Pion\MIL_PionMission_ABC_Couvrir.cpp"
#include "Pion\MIL_PionMission_ABC_FlancGarder.cpp"
#include "Pion\MIL_PionMission_ABC_Reduire.cpp"
#include "Pion\MIL_PionMission_ABC_Appuyer.cpp"
#include "Pion\MIL_PionMission_ABC_Fixer.cpp"
#include "Pion\MIL_PionMission_ABC_RenseignerSur.cpp"
#include "Pion\MIL_PionMission_ABC_ControlerZone.cpp"
#include "Pion\MIL_PionMission_ABC_Surveiller.cpp"
#include "Pion\MIL_PionMission_ABC_Barrer.cpp"
#include "Pion\MIL_PionMission_ABC_Attaquer.cpp"
#include "Pion\MIL_PionMission_ABC_ReconnaitrePoint.cpp"
#include "Pion\MIL_PionMission_ABC_Eclairer.cpp"
#include "Pion\MIL_PionMission_ABC_Jalonner.cpp"
#include "Pion\MIL_PionMission_ABC_ContreAttaquer.cpp"
#include "Pion\MIL_PionMission_ABC_SemparerZone.cpp"
#include "Pion\MIL_PionMission_ABC_ReleverUnite.cpp"
#include "Pion\MIL_PionMission_ABC_ArmerPIA.cpp"
#include "Pion\MIL_PionMission_ABC_OuvrirItineraire.cpp"
#include "Pion\MIL_PionMission_ABC_Freiner.cpp"
#include "Pion\MIL_PionMission_ABC_EscorterUnConvoi.cpp"
#include "Pion\MIL_PionMission_ABC_ArmerPointDeControle.cpp"
#include "Pion\MIL_PionMission_ABC_OrganiserAccueilColonneRefugies.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserFosseAC.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserAbattisMine.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserPointMine.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserBarricade.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserEboulement.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserBouchonMine.cpp"
#include "Pion\MIL_PionMission_GEN_DetruireRoute.cpp"
#include "Pion\MIL_PionMission_GEN_DetruirePont.cpp"
#include "Pion\MIL_PionMission_GEN_SecuriserItineraire.cpp"
#include "Pion\MIL_PionMission_GEN_DegagerAbattisMine.cpp"
#include "Pion\MIL_PionMission_GEN_DegagerEboulement.cpp"
#include "Pion\MIL_PionMission_GEN_DegagerRues.cpp"
#include "Pion\MIL_PionMission_GEN_ComblerEntonnoir.cpp"
#include "Pion\MIL_PionMission_GEN_ComblerFosseAC.cpp"
#include "Pion\MIL_PionMission_GEN_CreerContournement.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserZoneMineeLineaire.cpp"
#include "Pion\MIL_PionMission_GEN_OuvrirTrouee.cpp"
#include "Pion\MIL_PionMission_GEN_Renforcer.cpp"
#include "Pion\MIL_PionMission_GEN_FranchirEntonnoir.cpp"
#include "Pion\MIL_PionMission_GEN_FranchirFosseAC.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserZoneMineeParDispersion.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserPostesTir.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserTravauxProtection.cpp"
#include "Pion\MIL_PionMission_GEN_RetablirItineraire.cpp"
#include "Pion\MIL_PionMission_GEN_EquiperExploiter.cpp"
#include "Pion\MIL_PionMission_GEN_ExecuterVariantement.cpp"
#include "Pion\MIL_PionMission_GEN_ReconnaitreSiteFranchissement.cpp"
#include "Pion\MIL_PionMission_GEN_ReconnaitreItineraire.cpp"
#include "Pion\MIL_PionMission_GEN_DeminerSiteFranchissement.cpp"
#include "Pion\MIL_PionMission_GEN_ReconnaitreZone.cpp"
#include "Pion\MIL_PionMission_GEN_CreerPlateForme.cpp"
#include "Pion\MIL_PionMission_GEN_CreerPiste.cpp"
#include "Pion\MIL_PionMission_GEN_CreerAirePoser.cpp"
#include "Pion\MIL_PionMission_GEN_NeutraliserEnginsExplosifs.cpp"
#include "Pion\MIL_PionMission_GEN_ActiverObstacle.cpp"
#include "Pion\MIL_PionMission_GEN_AmeliorerMobilite.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserAireLogistique.cpp"
#include "Pion\MIL_PionMission_GEN_DepolluerZone.cpp"
#include "Pion\MIL_PionMission_GEN_AmenagerTerrainPoser.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserPlateFormeDeploiement.cpp"
#include "Pion\MIL_PionMission_GEN_ReprendreMissionConstruction.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserZonePoserHelicoptere.cpp"
#include "Pion\MIL_PionMission_GEN_AppuyerDebarquementTroupesGU.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserCampPrisonniers.cpp"
#include "Pion\MIL_PionMission_GEN_RealiserCampRefugies.cpp"
#include "Pion\MIL_PionMission_INF_ReconnaitreAxe.cpp"
#include "Pion\MIL_PionMission_INF_Couvrir.cpp"
#include "Pion\MIL_PionMission_INF_Reduire.cpp"
#include "Pion\MIL_PionMission_INF_Appuyer.cpp"
#include "Pion\MIL_PionMission_INF_Fixer.cpp"
#include "Pion\MIL_PionMission_INF_ControlerSecteur.cpp"
#include "Pion\MIL_PionMission_INF_SurveillerSecteur.cpp"
#include "Pion\MIL_PionMission_INF_Barrer.cpp"
#include "Pion\MIL_PionMission_INF_ReconnaitrePoint.cpp"
#include "Pion\MIL_PionMission_INF_Eclairer.cpp"
#include "Pion\MIL_PionMission_INF_Interdire.cpp"
#include "Pion\MIL_PionMission_INF_Semparer.cpp"
#include "Pion\MIL_PionMission_INF_PrendrePreciserContact.cpp"
#include "Pion\MIL_PionMission_INF_Defendre.cpp"
#include "Pion\MIL_PionMission_INF_DetruireEmbuscade.cpp"
#include "Pion\MIL_PionMission_INF_ContreAttaquer.cpp"
#include "Pion\MIL_PionMission_INF_Freiner.cpp"
#include "Pion\MIL_PionMission_INF_FlancGarder.cpp"
#include "Pion\MIL_PionMission_INF_OrganiserUnCheckPoint.cpp"
#include "Pion\MIL_PionMission_INF_Harceler.cpp"
#include "Pion\MIL_PionMission_INF_DetruireParCoupDeMain.cpp"
#include "Pion\MIL_PionMission_INF_EscorterUnConvoi.cpp"
#include "Pion\MIL_PionMission_INF_RenseignerSur.cpp"
#include "Pion\MIL_PionMission_INF_Soutenir.cpp"
#include "Pion\MIL_PionMission_INF_Securiser.cpp"
#include "Pion\MIL_PionMission_INF_Recueillir.cpp"
#include "Pion\MIL_PionMission_INF_RepondreAAggression.cpp"
#include "Pion\MIL_PionMission_LOG_SeDeployer.cpp"
#include "Pion\MIL_PionMission_LOG_DistribuerMoyens.cpp"
#include "Pion\MIL_PionMission_LOG_TrierBlesses.cpp"
#include "Pion\MIL_PionMission_LOG_TraiterBlesses.cpp"
#include "Pion\MIL_PionMission_LOG_TrierEtTraiterBlesses.cpp"
#include "Pion\MIL_PionMission_LOG_SoignerBlesses.cpp"
#include "Pion\MIL_PionMission_LOG_PrendreEnCompteReactionsMentales.cpp"
#include "Pion\MIL_PionMission_LOG_DecontaminerBlesses.cpp"
#include "Pion\MIL_PionMission_LOG_Reparer.cpp"
#include "Pion\MIL_PionMission_LOG_Evacuer.cpp"
#include "Pion\MIL_PionMission_LOG_Convoyer.cpp"
#include "Pion\MIL_PionMission_LOG_DistribuerRessources.cpp"
#include "Pion\MIL_PionMission_LOG_ConstituerConvoi.cpp"
#include "Pion\MIL_PionMission_LOG_Livrer.cpp"
#include "Pion\MIL_PionMission_LOG_PreparerBascule.cpp"
#include "Pion\MIL_PionMission_LOG_AppuyerMouvement.cpp"
#include "Pion\MIL_PionMission_LOG_ReconnaitreItineraire.cpp"
#include "Pion\MIL_PionMission_LOG_TransporterUniteChars.cpp"
#include "Pion\MIL_PionMission_LOG_ReconnaitreZoneContaminee.cpp"
#include "Pion\MIL_PionMission_LOG_AppuyerMouvementDansZone.cpp"
#include "Pion\MIL_PionMission_LOG_MettreEnOeuvreZoneStationnement.cpp"
#include "Pion\MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint.cpp"
#include "Pion\MIL_PionMission_LOG_BloquerZone.cpp"
#include "Pion\MIL_PionMission_LOG_ReconnaitreZoneDeDeploiement.cpp"
#include "Pion\MIL_PionMission_LOG_ArmerPIA.cpp"
#include "Pion\MIL_PionMission_LOG_Surveiller.cpp"
#include "Pion\MIL_PionMission_Test_Heliporter.cpp"
#include "Pion\MIL_PionMission_Test_MoveTo.cpp"
#include "Pion\MIL_PionMission_Test_CreateObject.cpp"
#include "Pion\MIL_PionMission_Test_DestroyObject.cpp"
#include "Pion\MIL_PionMission_Test_CreateBypass.cpp"
#include "Pion\MIL_PionMission_Test_Fire.cpp"
#include "Pion\MIL_PionMission_Test_ChangePosture.cpp"
#include "Pion\MIL_PionMission_Test_PathFind.cpp"
#include "Pion\MIL_PionMission_Test_FindPosition.cpp"
#include "Pion\MIL_PionMission_Test_Reinforce.cpp"
#include "Pion\MIL_PionMission_PlastronEnnemi.cpp"
#include "Pion\MIL_PionMission_FaireMouvement.cpp"
#include "Pion\MIL_PionMission_Suivre.cpp"
#include "Pion\MIL_PionMission_SeFaireDecontaminer.cpp"
#include "Pion\MIL_PionMission_Franchir.cpp"
#include "Pion\MIL_PionMission_Decrocher.cpp"
#include "Pion\MIL_PionMission_Stationner.cpp"
#include "Pion\MIL_PionMission_SeFaireTransporter.cpp"
#include "Pion\MIL_PionMission_InterdireFranchissementPopulation.cpp"
#include "Pion\MIL_PionMission_FilterPopulation.cpp"
#include "Pion\MIL_PionMission_RenseignerSurPopulation.cpp"
#include "Pion\MIL_PionMission_CanaliserPopulation.cpp"
#include "Pion\MIL_PionMission_SecuriserPopulationDansZone.cpp"
#include "Pion\MIL_PionMission_SecuriserZoneContreMenacesAsymetriques.cpp"
#include "Pion\MIL_PionMission_Generique.cpp"
#include "Pion\MIL_PionMission_MILICE_Provoquer.cpp"
#include "Pion\MIL_PionMission_MILICE_PrendreEtTenir.cpp"
#include "Pion\MIL_PionMission_MILICE_DetruireEmbuscade.cpp"
#include "Pion\MIL_PionMission_JOINT_MARINE_Transporter.cpp"
#include "Pion\MIL_PionMission_JOINT_AIR_EffectuerRaid.cpp"
#include "Pion\MIL_PionMission_NBC_AnimerUnPlotDeDecontamination.cpp"
#include "Pion\MIL_PionMission_NBC_ReconnaitreItineraire.cpp"
#include "Pion\MIL_PionMission_NBC_ReconnaitreZone.cpp"
#include "Pion\MIL_PionMission_NBC_DecontaminerZone.cpp"
#include "Pion\MIL_PionMission_NBC_ReconnaitreUnSiteROTA.cpp"
#include "Pion\MIL_PionMission_RENS_ROHUM_RenseignerSur.cpp"
#include "Pion\MIL_PionMission_RENS_ROHUM_SExfiltrer.cpp"
#include "Pion\MIL_PionMission_RENS_ROHUM_OrienterGuider.cpp"
#include "Pion\MIL_PionMission_RENS_ROEM_Appuyer.cpp"
#include "Pion\MIL_PionMission_RENS_ROEM_RenseignerSur.cpp"
#include "Pion\MIL_PionMission_RENS_ROEM_MettreEnOeuvre.cpp"
#include "Pion\MIL_PionMission_RENS_RADINT_RenseignerSur.cpp"
#include "Pion\MIL_PionMission_RENS_RADINT_MettreEnOeuvre.cpp"
#include "Pion\MIL_PionMission_RENS_ROIM_MettreEnOeuvre.cpp"
#include "Pion\MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur.cpp"
#include "Pion\MIL_PionMission_RENS_ROIM_CL289_RenseignerSur.cpp"
#include "Pion\MIL_PionMission_TRANS_EtablirGererLiaison.cpp"
#include "Population\MIL_PopulationMission_FaireMouvement.cpp"
#include "Population\MIL_PopulationMission_Manifester.cpp"
#include "Population\MIL_PopulationMission_CommettreExactions.cpp"
#include "Conduite\MIL_OrderConduite_MettreTenueNBC.cpp"
#include "Conduite\MIL_OrderConduite_EnleverTenueNBC.cpp"
#include "Conduite\MIL_OrderConduite_PasserEnSilenceRadio.cpp"
#include "Conduite\MIL_OrderConduite_ArreterSilenceRadio.cpp"
#include "Conduite\MIL_OrderConduite_PasserEnSilenceRadar.cpp"
#include "Conduite\MIL_OrderConduite_ArreterSilenceRadar.cpp"
#include "Conduite\MIL_OrderConduite_RecupererTransporteurs.cpp"
#include "Conduite\MIL_OrderConduite_AttendreSePoster.cpp"
#include "Conduite\MIL_OrderConduite_Poursuivre.cpp"
#include "Conduite\MIL_OrderConduite_Interrompre.cpp"
#include "Conduite\MIL_OrderConduite_ChangerReglesEngagement.cpp"
#include "Conduite\MIL_OrderConduite_ChangerReglesEngagementPopulation.cpp"
#include "Conduite\MIL_OrderConduite_Deboucher.cpp"
#include "Conduite\MIL_OrderConduite_Decrocher.cpp"
#include "Conduite\MIL_OrderConduite_AcquerirObjectif.cpp"
#include "Conduite\MIL_OrderConduite_Brouiller.cpp"
#include "Conduite\MIL_OrderConduite_ChangerPositionDebarquement.cpp"
#include "Conduite\MIL_OrderConduite_Pion_ChangerDePosition.cpp"
#include "Conduite\MIL_OrderConduite_Pion_Contourner.cpp"
#include "Conduite\MIL_OrderConduite_Pion_EnvoyerVehicule.cpp"
#include "Conduite\MIL_OrderConduite_Pion_ActiverObstacle.cpp"
#include "Conduite\MIL_OrderConduite_Pion_AppliquerFeux.cpp"
#include "Conduite\MIL_OrderConduite_Pion_ExecuterTir.cpp"
#include "Conduite\MIL_OrderConduite_Pion_Emettre.cpp"
#include "Conduite\MIL_OrderConduite_Pion_StopperEmission.cpp"
#include "Conduite\MIL_OrderConduite_Pion_RenforcerEnVSRAM.cpp"
#include "Conduite\MIL_OrderConduite_Pion_TransfererVSRAM.cpp"
#include "Conduite\MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM.cpp"
#include "Conduite\MIL_OrderConduite_ModifierTempsBordeeMaintenance.cpp"
#include "Conduite\MIL_OrderConduite_ModifierTempsBordeeSante.cpp"
#include "Conduite\MIL_OrderConduite_ModifierPrioritesReparations.cpp"
#include "Conduite\MIL_OrderConduite_ModifierPrioritesBlesses.cpp"
#include "Conduite\MIL_OrderConduite_ModifierPrioritesTactiquesReparations.cpp"
#include "Conduite\MIL_OrderConduite_ModifierPrioritesTactiquesBlesses.cpp"
#include "Conduite\MIL_OrderConduite_Automate_RealiserVariantement.cpp"
#include "Conduite\MIL_OrderConduite_Automate_DesequiperSiteFranchissement.cpp"
#include "Conduite\MIL_OrderConduite_Automate_ReagirFaceAEni.cpp"
#include "Conduite\MIL_OrderConduite_Automate_AffecterPionAObstacle.cpp"
#include "Conduite\MIL_OrderConduite_Automate_ReconnaitreZoneImplantation.cpp"
#include "Conduite\MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement.cpp"
#include "Conduite\MIL_OrderConduite_Automate_ActiverObstacle.cpp"
#include "Conduite\MIL_OrderConduite_Automate_TransfererRenforts.cpp"
#include "Conduite\MIL_OrderConduite_Population_ChangerAttitude.cpp"


using namespace missions;

//-----------------------------------------------------------------------------
// Name: RegisterMissions
// Created: AGR 
//-----------------------------------------------------------------------------
void missions::RegisterMissions()
{
    RegisterAutomateMissions  ();
    RegisterPionMissions      ();
    RegisterPopulationMissions();
    RegisterOrderConduites    ();
}

//-----------------------------------------------------------------------------
// Name: RegisterPopulationMissions
// Created: AGR 
//-----------------------------------------------------------------------------
void missions::RegisterPopulationMissions()
{
    MIL_PopulationMissionType::RegisterMission< MIL_PopulationMission_FaireMouvement>( "Population FaireMouvement", eMission_Population_FaireMouvement, T_Mission_Population_mission_population_faire_mouvement, "T_Mission_Population_FaireMouvement", "MIS_Population_FaireMouvement");
    MIL_PopulationMissionType::RegisterMission< MIL_PopulationMission_Manifester>( "Population Manifester", eMission_Population_Manifester, T_Mission_Population_mission_population_manifester, "T_Mission_Population_Manifester", "MIS_Population_Manifester");
    MIL_PopulationMissionType::RegisterMission< MIL_PopulationMission_CommettreExactions>( "Population CommettreExactions", eMission_Population_CommettreExactions, T_Mission_Population_mission_population_commettre_exactions, "T_Mission_Population_CommettreExactions", "MIS_Population_CommettreExactions");

}

//-----------------------------------------------------------------------------
// Name: RegisterAutomateMissions
// Created: AGR 
//-----------------------------------------------------------------------------
void missions::RegisterAutomateMissions()
{
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_ArmerPointDeControle>( "Automate ABC ArmerPointDeControle", eMission_Automate_ABC_ArmerPointDeControle, T_Mission_Automate_mission_automate_abc_armer_point_de_controle, "T_Mission_Automate_ABC_ArmerPointDeControle", "MIS_Automate_MRT_ABC_ArmerPointDeControle", "MIS_Automate_CDT_ABC_ArmerPointDeControle");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies>( "Automate ABC OrganiserAccueilColonneRefugies", eMission_Automate_ABC_OrganiserAccueilColonneRefugies, T_Mission_Automate_mission_automate_abc_organiser_accueil_colonne_refugies, "T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies", "MIS_Automate_MRT_ABC_OrganiserAccueilColonneRefugies", "MIS_Automate_CDT_ABC_OrganiserAccueilColonneRefugies");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_ProtegerPC>( "Automate ABC ProtegerPC", eMission_Automate_ABC_ProtegerPC, T_Mission_Automate_mission_automate_abc_proteger_pc, "T_Mission_Automate_ABC_ProtegerPC", "MIS_Automate_MRT_ABC_ProtegerPC", "MIS_Automate_CDT_ABC_ProtegerPC");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Appuyer>( "Automate ABC Appuyer", eMission_Automate_ABC_Appuyer, T_Mission_Automate_mission_automate_abc_appuyer, "T_Mission_Automate_ABC_Appuyer", "MIS_Automate_MRT_ABC_Appuyer", "MIS_Automate_CDT_ABC_Appuyer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Attaquer>( "Automate ABC Attaquer", eMission_Automate_ABC_Attaquer, T_Mission_Automate_mission_automate_abc_attaquer, "T_Mission_Automate_ABC_Attaquer", "MIS_Automate_MRT_ABC_Attaquer", "MIS_Automate_CDT_ABC_Attaquer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_ContreAttaquer>( "Automate ABC ContreAttaquer", eMission_Automate_ABC_ContreAttaquer, T_Mission_Automate_mission_automate_abc_contre_attaquer, "T_Mission_Automate_ABC_ContreAttaquer", "MIS_Automate_MRT_ABC_ContreAttaquer", "MIS_Automate_CDT_ABC_ContreAttaquer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Controler>( "Automate ABC Controler", eMission_Automate_ABC_Controler, T_Mission_Automate_mission_automate_abc_controler, "T_Mission_Automate_ABC_Controler", "MIS_Automate_MRT_ABC_Controler", "MIS_Automate_CDT_ABC_Controler");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Couvrir>( "Automate ABC Couvrir", eMission_Automate_ABC_Couvrir, T_Mission_Automate_mission_automate_abc_couvrir, "T_Mission_Automate_ABC_Couvrir", "MIS_Automate_MRT_ABC_Couvrir", "MIS_Automate_CDT_ABC_Couvrir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_DonnerCoupArret>( "Automate ABC DonnerCoupArret", eMission_Automate_ABC_DonnerCoupArret, T_Mission_Automate_mission_automate_abc_donner_coup_arret, "T_Mission_Automate_ABC_DonnerCoupArret", "MIS_Automate_MRT_ABC_DonnerCoupArret", "MIS_Automate_CDT_ABC_DonnerCoupArret");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Eclairer>( "Automate ABC Eclairer", eMission_Automate_ABC_Eclairer, T_Mission_Automate_mission_automate_abc_eclairer, "T_Mission_Automate_ABC_Eclairer", "MIS_Automate_MRT_ABC_Eclairer", "MIS_Automate_CDT_ABC_Eclairer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_EffectuerRaid>( "Automate ABC EffectuerRaid", eMission_Automate_ABC_EffectuerRaid, T_Mission_Automate_mission_automate_abc_effectuer_raid, "T_Mission_Automate_ABC_EffectuerRaid", "MIS_Automate_MRT_ABC_EffectuerRaid", "MIS_Automate_CDT_ABC_EffectuerRaid");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Interdire>( "Automate ABC Interdire", eMission_Automate_ABC_Interdire, T_Mission_Automate_mission_automate_abc_interdire, "T_Mission_Automate_ABC_Interdire", "MIS_Automate_MRT_ABC_Interdire", "MIS_Automate_CDT_ABC_Interdire");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Jalonner>( "Automate ABC Jalonner", eMission_Automate_ABC_Jalonner, T_Mission_Automate_mission_automate_abc_jalonner, "T_Mission_Automate_ABC_Jalonner", "MIS_Automate_MRT_ABC_Jalonner", "MIS_Automate_CDT_ABC_Jalonner");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_PrendrePreciserContact>( "Automate ABC PrendrePreciserContact", eMission_Automate_ABC_PrendrePreciserContact, T_Mission_Automate_mission_automate_abc_prendre_preciser_contact, "T_Mission_Automate_ABC_PrendrePreciserContact", "MIS_Automate_MRT_ABC_PrendrePreciserContact", "MIS_Automate_CDT_ABC_PrendrePreciserContact");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Reconnaitre>( "Automate ABC Reconnaitre", eMission_Automate_ABC_Reconnaitre, T_Mission_Automate_mission_automate_abc_reconnaitre, "T_Mission_Automate_ABC_Reconnaitre", "MIS_Automate_MRT_ABC_Reconnaitre", "MIS_Automate_CDT_ABC_Reconnaitre");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_ReconnaitrePoint>( "Automate ABC ReconnaitrePoint", eMission_Automate_ABC_ReconnaitrePoint, T_Mission_Automate_mission_automate_abc_reconnaitre_point, "T_Mission_Automate_ABC_ReconnaitrePoint", "MIS_Automate_MRT_ABC_ReconnaitrePoint", "MIS_Automate_CDT_ABC_ReconnaitrePoint");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Reduire>( "Automate ABC Reduire", eMission_Automate_ABC_Reduire, T_Mission_Automate_mission_automate_abc_reduire, "T_Mission_Automate_ABC_Reduire", "MIS_Automate_MRT_ABC_Reduire", "MIS_Automate_CDT_ABC_Reduire");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_ReleverEnDepassant>( "Automate ABC ReleverEnDepassant", eMission_Automate_ABC_ReleverEnDepassant, T_Mission_Automate_mission_automate_abc_relever_en_depassant, "T_Mission_Automate_ABC_ReleverEnDepassant", "MIS_Automate_MRT_ABC_ReleverEnDepassant", "MIS_Automate_CDT_ABC_ReleverEnDepassant");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Semparer>( "Automate ABC Semparer", eMission_Automate_ABC_Semparer, T_Mission_Automate_mission_automate_abc_semparer, "T_Mission_Automate_ABC_Semparer", "MIS_Automate_MRT_ABC_Semparer", "MIS_Automate_CDT_ABC_Semparer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Tenir>( "Automate ABC Tenir", eMission_Automate_ABC_Tenir, T_Mission_Automate_mission_automate_abc_tenir, "T_Mission_Automate_ABC_Tenir", "MIS_Automate_MRT_ABC_Tenir", "MIS_Automate_CDT_ABC_Tenir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Soutenir>( "Automate ABC Soutenir", eMission_Automate_ABC_Soutenir, T_Mission_Automate_mission_automate_abc_soutenir, "T_Mission_Automate_ABC_Soutenir", "MIS_Automate_MRT_ABC_Soutenir", "MIS_Automate_CDT_ABC_Soutenir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Surveiller>( "Automate ABC Surveiller", eMission_Automate_ABC_Surveiller, T_Mission_Automate_mission_automate_abc_surveiller, "T_Mission_Automate_ABC_Surveiller", "MIS_Automate_MRT_ABC_Surveiller", "MIS_Automate_CDT_ABC_Surveiller");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_FlancGarder>( "Automate ABC FlancGarder", eMission_Automate_ABC_FlancGarder, T_Mission_Automate_mission_automate_abc_flanc_garder, "T_Mission_Automate_ABC_FlancGarder", "MIS_Automate_MRT_ABC_FlancGarder", "MIS_Automate_CDT_ABC_FlancGarder");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_RenseignerSur>( "Automate ABC RenseignerSur", eMission_Automate_ABC_RenseignerSur, T_Mission_Automate_mission_automate_abc_renseigner_sur, "T_Mission_Automate_ABC_RenseignerSur", "MIS_Automate_MRT_ABC_RenseignerSur", "MIS_Automate_CDT_ABC_RenseignerSur");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Fixer>( "Automate ABC Fixer", eMission_Automate_ABC_Fixer, T_Mission_Automate_mission_automate_abc_fixer, "T_Mission_Automate_ABC_Fixer", "MIS_Automate_MRT_ABC_Fixer", "MIS_Automate_CDT_ABC_Fixer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_Freiner>( "Automate ABC Freiner", eMission_Automate_ABC_Freiner, T_Mission_Automate_mission_automate_abc_freiner, "T_Mission_Automate_ABC_Freiner", "MIS_Automate_MRT_ABC_Freiner", "MIS_Automate_CDT_ABC_Freiner");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_SurveillerItineraire>( "Automate ABC SurveillerItineraire", eMission_Automate_ABC_SurveillerItineraire, T_Mission_Automate_mission_automate_abc_surveiller_itineraire, "T_Mission_Automate_ABC_SurveillerItineraire", "MIS_Automate_MRT_ABC_SurveillerItineraire", "MIS_Automate_CDT_ABC_SurveillerItineraire");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_RenseignerSurUnAxe>( "Automate ABC RenseignerSurUnAxe", eMission_Automate_ABC_RenseignerSurUnAxe, T_Mission_Automate_mission_automate_abc_renseigner_sur_un_axe, "T_Mission_Automate_ABC_RenseignerSurUnAxe", "MIS_Automate_MRT_ABC_RenseignerSurUnAxe", "MIS_Automate_CDT_ABC_RenseignerSurUnAxe");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_RecueillirUnite>( "Automate ABC RecueillirUnite", eMission_Automate_ABC_RecueillirUnite, T_Mission_Automate_mission_automate_abc_recueillir_unite, "T_Mission_Automate_ABC_RecueillirUnite", "MIS_Automate_MRT_ABC_RecueillirUnite", "MIS_Automate_CDT_ABC_RecueillirUnite");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ABC_EscorterUnConvoi>( "Automate ABC EscorterUnConvoi", eMission_Automate_ABC_EscorterUnConvoi, T_Mission_Automate_mission_automate_abc_escorter_un_convoi, "T_Mission_Automate_ABC_EscorterUnConvoi", "MIS_Automate_MRT_ABC_EscorterUnConvoi", "MIS_Automate_CDT_ABC_EscorterUnConvoi");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Heliporter>( "Automate ALAT Heliporter", eMission_Automate_ALAT_Heliporter, T_Mission_Automate_mission_automate_alat_heliporter, "T_Mission_Automate_ALAT_Heliporter", "MIS_Automate_MRT_ALAT_Heliporter", "MIS_Automate_CDT_ALAT_Heliporter");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Jalonner>( "Automate ALAT Jalonner", eMission_Automate_ALAT_Jalonner, T_Mission_Automate_mission_automate_alat_jalonner, "T_Mission_Automate_ALAT_Jalonner", "MIS_Automate_MRT_ALAT_Jalonner", "MIS_Automate_CDT_ALAT_Jalonner");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Helitransporter>( "Automate ALAT Helitransporter", eMission_Automate_ALAT_Helitransporter, T_Mission_Automate_mission_automate_alat_helitransporter, "T_Mission_Automate_ALAT_Helitransporter", "MIS_Automate_MRT_ALAT_Helitransporter", "MIS_Automate_CDT_ALAT_Helitransporter");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_ExtrairePersonnel>( "Automate ALAT ExtrairePersonnel", eMission_Automate_ALAT_ExtrairePersonnel, T_Mission_Automate_mission_automate_alat_extraire_personnel, "T_Mission_Automate_ALAT_ExtrairePersonnel", "MIS_Automate_MRT_ALAT_ExtrairePersonnel", "MIS_Automate_CDT_ALAT_ExtrairePersonnel");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Appuyer>( "Automate ALAT Appuyer", eMission_Automate_ALAT_Appuyer, T_Mission_Automate_mission_automate_alat_appuyer, "T_Mission_Automate_ALAT_Appuyer", "MIS_Automate_MRT_ALAT_Appuyer", "MIS_Automate_CDT_ALAT_Appuyer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Couvrir>( "Automate ALAT Couvrir", eMission_Automate_ALAT_Couvrir, T_Mission_Automate_mission_automate_alat_couvrir, "T_Mission_Automate_ALAT_Couvrir", "MIS_Automate_MRT_ALAT_Couvrir", "MIS_Automate_CDT_ALAT_Couvrir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Surveiller>( "Automate ALAT Surveiller", eMission_Automate_ALAT_Surveiller, T_Mission_Automate_mission_automate_alat_surveiller, "T_Mission_Automate_ALAT_Surveiller", "MIS_Automate_MRT_ALAT_Surveiller", "MIS_Automate_CDT_ALAT_Surveiller");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Reconnaitre>( "Automate ALAT Reconnaitre", eMission_Automate_ALAT_Reconnaitre, T_Mission_Automate_mission_automate_alat_reconnaitre, "T_Mission_Automate_ALAT_Reconnaitre", "MIS_Automate_MRT_ALAT_Reconnaitre", "MIS_Automate_CDT_ALAT_Reconnaitre");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Freiner>( "Automate ALAT Freiner", eMission_Automate_ALAT_Freiner, T_Mission_Automate_mission_automate_alat_freiner, "T_Mission_Automate_ALAT_Freiner", "MIS_Automate_MRT_ALAT_Freiner", "MIS_Automate_CDT_ALAT_Freiner");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Escorter>( "Automate ALAT Escorter", eMission_Automate_ALAT_Escorter, T_Mission_Automate_mission_automate_alat_escorter, "T_Mission_Automate_ALAT_Escorter", "MIS_Automate_MRT_ALAT_Escorter", "MIS_Automate_CDT_ALAT_Escorter");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_Attaquer>( "Automate ALAT Attaquer", eMission_Automate_ALAT_Attaquer, T_Mission_Automate_mission_automate_alat_attaquer, "T_Mission_Automate_ALAT_Attaquer", "MIS_Automate_MRT_ALAT_Attaquer", "MIS_Automate_CDT_ALAT_Attaquer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_RecueillirFormationAeromobile>( "Automate ALAT RecueillirFormationAeromobile", eMission_Automate_ALAT_RecueillirFormationAeromobile, T_Mission_Automate_mission_automate_alat_recueillir_formation_aeromobile, "T_Mission_Automate_ALAT_RecueillirFormationAeromobile", "MIS_Automate_MRT_ALAT_RecueillirFormationAeromobile", "MIS_Automate_CDT_ALAT_RecueillirFormationAeromobile");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_AttaquerFormationAeromobile>( "Automate ALAT AttaquerFormationAeromobile", eMission_Automate_ALAT_AttaquerFormationAeromobile, T_Mission_Automate_mission_automate_alat_attaquer_formation_aeromobile, "T_Mission_Automate_ALAT_AttaquerFormationAeromobile", "MIS_Automate_MRT_ALAT_AttaquerFormationAeromobile", "MIS_Automate_CDT_ALAT_AttaquerFormationAeromobile");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_DonnerCoupArret>( "Automate ALAT DonnerCoupArret", eMission_Automate_ALAT_DonnerCoupArret, T_Mission_Automate_mission_automate_alat_donner_coup_arret, "T_Mission_Automate_ALAT_DonnerCoupArret", "MIS_Automate_MRT_ALAT_DonnerCoupArret", "MIS_Automate_CDT_ALAT_DonnerCoupArret");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_DetruireNeutraliserDansProfondeur>( "Automate ALAT DetruireNeutraliserDansProfondeur", eMission_Automate_ALAT_DetruireNeutraliserDansProfondeur, T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_profondeur, "T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur", "MIS_Automate_MRT_ALAT_DetruireNeutraliserDansProfondeur", "MIS_Automate_CDT_ALAT_DetruireNeutraliserDansProfondeur");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_DetruireNeutraliserDansZone>( "Automate ALAT DetruireNeutraliserDansZone", eMission_Automate_ALAT_DetruireNeutraliserDansZone, T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_zone, "T_Mission_Automate_ALAT_DetruireNeutraliserDansZone", "MIS_Automate_MRT_ALAT_DetruireNeutraliserDansZone", "MIS_Automate_CDT_ALAT_DetruireNeutraliserDansZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ALAT_EffectuerRechercheEtSauvetage>( "Automate ALAT EffectuerRechercheEtSauvetage", eMission_Automate_ALAT_EffectuerRechercheEtSauvetage, T_Mission_Automate_mission_automate_alat_effectuer_recherche_et_sauvetage, "T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage", "MIS_Automate_MRT_ALAT_EffectuerRechercheEtSauvetage", "MIS_Automate_CDT_ALAT_EffectuerRechercheEtSauvetage");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_DefendreZone>( "Automate ASA DefendreZone", eMission_Automate_ASA_DefendreZone, T_Mission_Automate_mission_automate_asa_defendre_zone, "T_Mission_Automate_ASA_DefendreZone", "MIS_Automate_MRT_ASA_DefendreZone", "MIS_Automate_CDT_ASA_DefendreZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_DefendreSite>( "Automate ASA DefendreSite", eMission_Automate_ASA_DefendreSite, T_Mission_Automate_mission_automate_asa_defendre_site, "T_Mission_Automate_ASA_DefendreSite", "MIS_Automate_MRT_ASA_DefendreSite", "MIS_Automate_CDT_ASA_DefendreSite");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_Surveiller>( "Automate ASA Surveiller", eMission_Automate_ASA_Surveiller, T_Mission_Automate_mission_automate_asa_surveiller, "T_Mission_Automate_ASA_Surveiller", "MIS_Automate_MRT_ASA_Surveiller", "MIS_Automate_CDT_ASA_Surveiller");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_DefendreItineraire>( "Automate ASA DefendreItineraire", eMission_Automate_ASA_DefendreItineraire, T_Mission_Automate_mission_automate_asa_defendre_itineraire, "T_Mission_Automate_ASA_DefendreItineraire", "MIS_Automate_MRT_ASA_DefendreItineraire", "MIS_Automate_CDT_ASA_DefendreItineraire");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_MISTRAL_DefendreZone>( "Automate ASA MISTRAL DefendreZone", eMission_Automate_ASA_MISTRAL_DefendreZone, T_Mission_Automate_mission_automate_asa_mistral_defendre_zone, "T_Mission_Automate_ASA_MISTRAL_DefendreZone", "MIS_Automate_MRT_ASA_MISTRAL_DefendreZone", "MIS_Automate_CDT_ASA_MISTRAL_DefendreZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_MISTRAL_DefendreSite>( "Automate ASA MISTRAL DefendreSite", eMission_Automate_ASA_MISTRAL_DefendreSite, T_Mission_Automate_mission_automate_asa_mistral_defendre_site, "T_Mission_Automate_ASA_MISTRAL_DefendreSite", "MIS_Automate_MRT_ASA_MISTRAL_DefendreSite", "MIS_Automate_CDT_ASA_MISTRAL_DefendreSite");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_MISTRAL_Surveiller>( "Automate ASA MISTRAL Surveiller", eMission_Automate_ASA_MISTRAL_Surveiller, T_Mission_Automate_mission_automate_asa_mistral_surveiller, "T_Mission_Automate_ASA_MISTRAL_Surveiller", "MIS_Automate_MRT_ASA_MISTRAL_Surveiller", "MIS_Automate_CDT_ASA_MISTRAL_Surveiller");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_HAWK_DefendreZone>( "Automate ASA HAWK DefendreZone", eMission_Automate_ASA_HAWK_DefendreZone, T_Mission_Automate_mission_automate_asa_hawk_defendre_zone, "T_Mission_Automate_ASA_HAWK_DefendreZone", "MIS_Automate_MRT_ASA_HAWK_DefendreZone", "MIS_Automate_CDT_ASA_HAWK_DefendreZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_HAWK_DefendreSite>( "Automate ASA HAWK DefendreSite", eMission_Automate_ASA_HAWK_DefendreSite, T_Mission_Automate_mission_automate_asa_hawk_defendre_site, "T_Mission_Automate_ASA_HAWK_DefendreSite", "MIS_Automate_MRT_ASA_HAWK_DefendreSite", "MIS_Automate_CDT_ASA_HAWK_DefendreSite");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_HAWK_Surveiller>( "Automate ASA HAWK Surveiller", eMission_Automate_ASA_HAWK_Surveiller, T_Mission_Automate_mission_automate_asa_hawk_surveiller, "T_Mission_Automate_ASA_HAWK_Surveiller", "MIS_Automate_MRT_ASA_HAWK_Surveiller", "MIS_Automate_CDT_ASA_HAWK_Surveiller");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASA_DefendreUnites>( "Automate ASA DefendreUnites", eMission_Automate_ASA_DefendreUnites, T_Mission_Automate_mission_automate_asa_defendre_unites, "T_Mission_Automate_ASA_DefendreUnites", "MIS_Automate_MRT_ASA_DefendreUnites", "MIS_Automate_CDT_ASA_DefendreUnites");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASS_SeDeployer>( "Automate ASS SeDeployer", eMission_Automate_ASS_SeDeployer, T_Mission_Automate_mission_automate_ass_se_deployer, "T_Mission_Automate_ASS_SeDeployer", "MIS_Automate_MRT_ASS_SeDeployer", "MIS_Automate_CDT_ASS_SeDeployer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement>( "Automate ASS ReconnaitreZonesDeploiement", eMission_Automate_ASS_ReconnaitreZonesDeploiement, T_Mission_Automate_mission_automate_ass_reconnaitre_zones_deploiement, "T_Mission_Automate_ASS_ReconnaitreZonesDeploiement", "MIS_Automate_MRT_ASS_ReconnaitreZonesDeploiement", "MIS_Automate_CDT_ASS_ReconnaitreZonesDeploiement");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ASS_AcquerirObjectifs>( "Automate ASS AcquerirObjectifs", eMission_Automate_ASS_AcquerirObjectifs, T_Mission_Automate_mission_automate_ass_acquerir_objectifs, "T_Mission_Automate_ASS_AcquerirObjectifs", "MIS_Automate_MRT_ASS_AcquerirObjectifs", "MIS_Automate_CDT_ASS_AcquerirObjectifs");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_SeDeployer>( "Automate GEN SeDeployer", eMission_Automate_GEN_SeDeployer, T_Mission_Automate_mission_automate_gen_se_deployer, "T_Mission_Automate_GEN_SeDeployer", "MIS_Automate_MRT_GEN_SeDeployer", "MIS_Automate_CDT_GEN_SeDeployer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_FaireFranchir>( "Automate GEN FaireFranchir", eMission_Automate_GEN_FaireFranchir, T_Mission_Automate_mission_automate_gen_faire_franchir, "T_Mission_Automate_GEN_FaireFranchir", "MIS_Automate_MRT_GEN_FaireFranchir", "MIS_Automate_CDT_GEN_FaireFranchir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_RealiserSystemeObstacles>( "Automate GEN RealiserSystemeObstacles", eMission_Automate_GEN_RealiserSystemeObstacles, T_Mission_Automate_mission_automate_gen_realiser_systeme_obstacles, "T_Mission_Automate_GEN_RealiserSystemeObstacles", "MIS_Automate_MRT_GEN_RealiserSystemeObstacles", "MIS_Automate_CDT_GEN_RealiserSystemeObstacles");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_FranchirDeViveForce>( "Automate GEN FranchirDeViveForce", eMission_Automate_GEN_FranchirDeViveForce, T_Mission_Automate_mission_automate_gen_franchir_de_vive_force, "T_Mission_Automate_GEN_FranchirDeViveForce", "MIS_Automate_MRT_GEN_FranchirDeViveForce", "MIS_Automate_CDT_GEN_FranchirDeViveForce");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_RetablirItineraires>( "Automate GEN RetablirItineraires", eMission_Automate_GEN_RetablirItineraires, T_Mission_Automate_mission_automate_gen_retablir_itineraires, "T_Mission_Automate_GEN_RetablirItineraires", "MIS_Automate_MRT_GEN_RetablirItineraires", "MIS_Automate_CDT_GEN_RetablirItineraires");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_AmenagerAireStationnement>( "Automate GEN AmenagerAireStationnement", eMission_Automate_GEN_AmenagerAireStationnement, T_Mission_Automate_mission_automate_gen_amenager_aire_stationnement, "T_Mission_Automate_GEN_AmenagerAireStationnement", "MIS_Automate_MRT_GEN_AmenagerAireStationnement", "MIS_Automate_CDT_GEN_AmenagerAireStationnement");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire>( "Automate GEN AmenagerTerrainPoserSommaire", eMission_Automate_GEN_AmenagerTerrainPoserSommaire, T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire, "T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire", "MIS_Automate_MRT_GEN_AmenagerTerrainPoserSommaire", "MIS_Automate_CDT_GEN_AmenagerTerrainPoserSommaire");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_RealiserZonePoser>( "Automate GEN RealiserZonePoser", eMission_Automate_GEN_RealiserZonePoser, T_Mission_Automate_mission_automate_gen_realiser_zone_poser, "T_Mission_Automate_GEN_RealiserZonePoser", "MIS_Automate_MRT_GEN_RealiserZonePoser", "MIS_Automate_CDT_GEN_RealiserZonePoser");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_ReconnaitreZoneDeploiement>( "Automate GEN ReconnaitreZoneDeploiement", eMission_Automate_GEN_ReconnaitreZoneDeploiement, T_Mission_Automate_mission_automate_gen_reconnaitre_zone_deploiement, "T_Mission_Automate_GEN_ReconnaitreZoneDeploiement", "MIS_Automate_MRT_GEN_ReconnaitreZoneDeploiement", "MIS_Automate_CDT_GEN_ReconnaitreZoneDeploiement");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_RealiserCampPrisonniers>( "Automate GEN RealiserCampPrisonniers", eMission_Automate_GEN_RealiserCampPrisonniers, T_Mission_Automate_mission_automate_gen_realiser_camp_prisonniers, "T_Mission_Automate_GEN_RealiserCampPrisonniers", "MIS_Automate_MRT_GEN_RealiserCampPrisonniers", "MIS_Automate_CDT_GEN_RealiserCampPrisonniers");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_RealiserCampRefugies>( "Automate GEN RealiserCampRefugies", eMission_Automate_GEN_RealiserCampRefugies, T_Mission_Automate_mission_automate_gen_realiser_camp_refugies, "T_Mission_Automate_GEN_RealiserCampRefugies", "MIS_Automate_MRT_GEN_RealiserCampRefugies", "MIS_Automate_CDT_GEN_RealiserCampRefugies");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial>( "Automate GEN RealiserPlatesformesSommairesDeDeploiementInitial", eMission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial, T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial, "T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial", "MIS_Automate_MRT_GEN_RealiserPlatesformesSommairesDeDeploiementInitial", "MIS_Automate_CDT_GEN_RealiserPlatesformesSommairesDeDeploiementInitial");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation>( "Automate GEN RealiserTravauxSommairesAuProfitPopulation", eMission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation, T_Mission_Automate_mission_automate_gen_realiser_travaux_sommaires_au_profit_population, "T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation", "MIS_Automate_MRT_GEN_RealiserTravauxSommairesAuProfitPopulation", "MIS_Automate_CDT_GEN_RealiserTravauxSommairesAuProfitPopulation");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_AppuyerDebarquementTroupes>( "Automate GEN AppuyerDebarquementTroupes", eMission_Automate_GEN_AppuyerDebarquementTroupes, T_Mission_Automate_mission_automate_gen_appuyer_debarquement_troupes, "T_Mission_Automate_GEN_AppuyerDebarquementTroupes", "MIS_Automate_MRT_GEN_AppuyerDebarquementTroupes", "MIS_Automate_CDT_GEN_AppuyerDebarquementTroupes");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_GEN_OuvrirAxesProgression>( "Automate GEN OuvrirAxesProgression", eMission_Automate_GEN_OuvrirAxesProgression, T_Mission_Automate_mission_automate_gen_ouvrir_axes_progression, "T_Mission_Automate_GEN_OuvrirAxesProgression", "MIS_Automate_MRT_GEN_OuvrirAxesProgression", "MIS_Automate_CDT_GEN_OuvrirAxesProgression");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_AttaquerSouplesse>( "Automate INF AttaquerSouplesse", eMission_Automate_INF_AttaquerSouplesse, T_Mission_Automate_mission_automate_inf_attaquer_souplesse, "T_Mission_Automate_INF_AttaquerSouplesse", "MIS_Automate_MRT_INF_AttaquerSouplesse", "MIS_Automate_CDT_INF_AttaquerSouplesse");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Appuyer>( "Automate INF Appuyer", eMission_Automate_INF_Appuyer, T_Mission_Automate_mission_automate_inf_appuyer, "T_Mission_Automate_INF_Appuyer", "MIS_Automate_MRT_INF_Appuyer", "MIS_Automate_CDT_INF_Appuyer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Controler>( "Automate INF Controler", eMission_Automate_INF_Controler, T_Mission_Automate_mission_automate_inf_controler, "T_Mission_Automate_INF_Controler", "MIS_Automate_MRT_INF_Controler", "MIS_Automate_CDT_INF_Controler");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Couvrir>( "Automate INF Couvrir", eMission_Automate_INF_Couvrir, T_Mission_Automate_mission_automate_inf_couvrir, "T_Mission_Automate_INF_Couvrir", "MIS_Automate_MRT_INF_Couvrir", "MIS_Automate_CDT_INF_Couvrir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_DonnerCoupArret>( "Automate INF DonnerCoupArret", eMission_Automate_INF_DonnerCoupArret, T_Mission_Automate_mission_automate_inf_donner_coup_arret, "T_Mission_Automate_INF_DonnerCoupArret", "MIS_Automate_MRT_INF_DonnerCoupArret", "MIS_Automate_CDT_INF_DonnerCoupArret");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Eclairer>( "Automate INF Eclairer", eMission_Automate_INF_Eclairer, T_Mission_Automate_mission_automate_inf_eclairer, "T_Mission_Automate_INF_Eclairer", "MIS_Automate_MRT_INF_Eclairer", "MIS_Automate_CDT_INF_Eclairer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_FlancGarder>( "Automate INF FlancGarder", eMission_Automate_INF_FlancGarder, T_Mission_Automate_mission_automate_inf_flanc_garder, "T_Mission_Automate_INF_FlancGarder", "MIS_Automate_MRT_INF_FlancGarder", "MIS_Automate_CDT_INF_FlancGarder");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Freiner>( "Automate INF Freiner", eMission_Automate_INF_Freiner, T_Mission_Automate_mission_automate_inf_freiner, "T_Mission_Automate_INF_Freiner", "MIS_Automate_MRT_INF_Freiner", "MIS_Automate_CDT_INF_Freiner");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Harceler>( "Automate INF Harceler", eMission_Automate_INF_Harceler, T_Mission_Automate_mission_automate_inf_harceler, "T_Mission_Automate_INF_Harceler", "MIS_Automate_MRT_INF_Harceler", "MIS_Automate_CDT_INF_Harceler");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Interdire>( "Automate INF Interdire", eMission_Automate_INF_Interdire, T_Mission_Automate_mission_automate_inf_interdire, "T_Mission_Automate_INF_Interdire", "MIS_Automate_MRT_INF_Interdire", "MIS_Automate_CDT_INF_Interdire");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_MenerDefenseUsure>( "Automate INF MenerDefenseUsure", eMission_Automate_INF_MenerDefenseUsure, T_Mission_Automate_mission_automate_inf_mener_defense_usure, "T_Mission_Automate_INF_MenerDefenseUsure", "MIS_Automate_MRT_INF_MenerDefenseUsure", "MIS_Automate_CDT_INF_MenerDefenseUsure");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_PrendrePreciserMaintenirContact>( "Automate INF PrendrePreciserMaintenirContact", eMission_Automate_INF_PrendrePreciserMaintenirContact, T_Mission_Automate_mission_automate_inf_prendre_preciser_maintenir_contact, "T_Mission_Automate_INF_PrendrePreciserMaintenirContact", "MIS_Automate_MRT_INF_PrendrePreciserMaintenirContact", "MIS_Automate_CDT_INF_PrendrePreciserMaintenirContact");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_ReconnaissanceOffensive>( "Automate INF ReconnaissanceOffensive", eMission_Automate_INF_ReconnaissanceOffensive, T_Mission_Automate_mission_automate_inf_reconnaissance_offensive, "T_Mission_Automate_INF_ReconnaissanceOffensive", "MIS_Automate_MRT_INF_ReconnaissanceOffensive", "MIS_Automate_CDT_INF_ReconnaissanceOffensive");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_RecueillirUnite>( "Automate INF RecueillirUnite", eMission_Automate_INF_RecueillirUnite, T_Mission_Automate_mission_automate_inf_recueillir_unite, "T_Mission_Automate_INF_RecueillirUnite", "MIS_Automate_MRT_INF_RecueillirUnite", "MIS_Automate_CDT_INF_RecueillirUnite");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Reduire>( "Automate INF Reduire", eMission_Automate_INF_Reduire, T_Mission_Automate_mission_automate_inf_reduire, "T_Mission_Automate_INF_Reduire", "MIS_Automate_MRT_INF_Reduire", "MIS_Automate_CDT_INF_Reduire");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Relever>( "Automate INF Relever", eMission_Automate_INF_Relever, T_Mission_Automate_mission_automate_inf_relever, "T_Mission_Automate_INF_Relever", "MIS_Automate_MRT_INF_Relever", "MIS_Automate_CDT_INF_Relever");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_RompreContact>( "Automate INF RompreContact", eMission_Automate_INF_RompreContact, T_Mission_Automate_mission_automate_inf_rompre_contact, "T_Mission_Automate_INF_RompreContact", "MIS_Automate_MRT_INF_RompreContact", "MIS_Automate_CDT_INF_RompreContact");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Surveiller>( "Automate INF Surveiller", eMission_Automate_INF_Surveiller, T_Mission_Automate_mission_automate_inf_surveiller, "T_Mission_Automate_INF_Surveiller", "MIS_Automate_MRT_INF_Surveiller", "MIS_Automate_CDT_INF_Surveiller");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Semparer>( "Automate INF Semparer", eMission_Automate_INF_Semparer, T_Mission_Automate_mission_automate_inf_semparer, "T_Mission_Automate_INF_Semparer", "MIS_Automate_MRT_INF_Semparer", "MIS_Automate_CDT_INF_Semparer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Fixer>( "Automate INF Fixer", eMission_Automate_INF_Fixer, T_Mission_Automate_mission_automate_inf_fixer, "T_Mission_Automate_INF_Fixer", "MIS_Automate_MRT_INF_Fixer", "MIS_Automate_CDT_INF_Fixer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_AttaquerEnTerrainDifficile>( "Automate INF AttaquerEnTerrainDifficile", eMission_Automate_INF_AttaquerEnTerrainDifficile, T_Mission_Automate_mission_automate_inf_attaquer_en_terrain_difficile, "T_Mission_Automate_INF_AttaquerEnTerrainDifficile", "MIS_Automate_MRT_INF_AttaquerEnTerrainDifficile", "MIS_Automate_CDT_INF_AttaquerEnTerrainDifficile");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_BarrerDirection>( "Automate INF BarrerDirection", eMission_Automate_INF_BarrerDirection, T_Mission_Automate_mission_automate_inf_barrer_direction, "T_Mission_Automate_INF_BarrerDirection", "MIS_Automate_MRT_INF_BarrerDirection", "MIS_Automate_CDT_INF_BarrerDirection");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_DefendreFerme>( "Automate INF DefendreFerme", eMission_Automate_INF_DefendreFerme, T_Mission_Automate_mission_automate_inf_defendre_ferme, "T_Mission_Automate_INF_DefendreFerme", "MIS_Automate_MRT_INF_DefendreFerme", "MIS_Automate_CDT_INF_DefendreFerme");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Tenir>( "Automate INF Tenir", eMission_Automate_INF_Tenir, T_Mission_Automate_mission_automate_inf_tenir, "T_Mission_Automate_INF_Tenir", "MIS_Automate_MRT_INF_Tenir", "MIS_Automate_CDT_INF_Tenir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_AppuyerUnFreinage>( "Automate INF AppuyerUnFreinage", eMission_Automate_INF_AppuyerUnFreinage, T_Mission_Automate_mission_automate_inf_appuyer_un_freinage, "T_Mission_Automate_INF_AppuyerUnFreinage", "MIS_Automate_MRT_INF_AppuyerUnFreinage", "MIS_Automate_CDT_INF_AppuyerUnFreinage");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_AppuyerUneAttaque>( "Automate INF AppuyerUneAttaque", eMission_Automate_INF_AppuyerUneAttaque, T_Mission_Automate_mission_automate_inf_appuyer_une_attaque, "T_Mission_Automate_INF_AppuyerUneAttaque", "MIS_Automate_MRT_INF_AppuyerUneAttaque", "MIS_Automate_CDT_INF_AppuyerUneAttaque");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_INF_Soutenir>( "Automate INF Soutenir", eMission_Automate_INF_Soutenir, T_Mission_Automate_mission_automate_inf_soutenir, "T_Mission_Automate_INF_Soutenir", "MIS_Automate_MRT_INF_Soutenir", "MIS_Automate_CDT_INF_Soutenir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_SeDeployer>( "Automate LOG SeDeployer", eMission_Automate_LOG_SeDeployer, T_Mission_Automate_mission_automate_log_se_deployer, "T_Mission_Automate_LOG_SeDeployer", "MIS_Automate_MRT_LOG_SeDeployer", "MIS_Automate_CDT_LOG_SeDeployer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_AppuyerMouvement>( "Automate LOG AppuyerMouvement", eMission_Automate_LOG_AppuyerMouvement, T_Mission_Automate_mission_automate_log_appuyer_mouvement, "T_Mission_Automate_LOG_AppuyerMouvement", "MIS_Automate_MRT_LOG_AppuyerMouvement", "MIS_Automate_CDT_LOG_AppuyerMouvement");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_ReconnaitreItineraire>( "Automate LOG ReconnaitreItineraire", eMission_Automate_LOG_ReconnaitreItineraire, T_Mission_Automate_mission_automate_log_reconnaitre_itineraire, "T_Mission_Automate_LOG_ReconnaitreItineraire", "MIS_Automate_MRT_LOG_ReconnaitreItineraire", "MIS_Automate_CDT_LOG_ReconnaitreItineraire");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_Surveiller>( "Automate LOG Surveiller", eMission_Automate_LOG_Surveiller, T_Mission_Automate_mission_automate_log_surveiller, "T_Mission_Automate_LOG_Surveiller", "MIS_Automate_MRT_LOG_Surveiller", "MIS_Automate_CDT_LOG_Surveiller");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_TransporterFormationBlindee>( "Automate LOG TransporterFormationBlindee", eMission_Automate_LOG_TransporterFormationBlindee, T_Mission_Automate_mission_automate_log_transporter_formation_blindee, "T_Mission_Automate_LOG_TransporterFormationBlindee", "MIS_Automate_MRT_LOG_TransporterFormationBlindee", "MIS_Automate_CDT_LOG_TransporterFormationBlindee");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_AppuyerMouvementDansZone>( "Automate LOG AppuyerMouvementDansZone", eMission_Automate_LOG_AppuyerMouvementDansZone, T_Mission_Automate_mission_automate_log_appuyer_mouvement_dans_zone, "T_Mission_Automate_LOG_AppuyerMouvementDansZone", "MIS_Automate_MRT_LOG_AppuyerMouvementDansZone", "MIS_Automate_CDT_LOG_AppuyerMouvementDansZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_MettreEnOeuvreZoneStationnement>( "Automate LOG MettreEnOeuvreZoneStationnement", eMission_Automate_LOG_MettreEnOeuvreZoneStationnement, T_Mission_Automate_mission_automate_log_mettre_en_oeuvre_zone_stationnement, "T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement", "MIS_Automate_MRT_LOG_MettreEnOeuvreZoneStationnement", "MIS_Automate_CDT_LOG_MettreEnOeuvreZoneStationnement");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_AppuyerFranchissementDansZone>( "Automate LOG AppuyerFranchissementDansZone", eMission_Automate_LOG_AppuyerFranchissementDansZone, T_Mission_Automate_mission_automate_log_appuyer_franchissement_dans_zone, "T_Mission_Automate_LOG_AppuyerFranchissementDansZone", "MIS_Automate_MRT_LOG_AppuyerFranchissementDansZone", "MIS_Automate_CDT_LOG_AppuyerFranchissementDansZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_BloquerZone>( "Automate LOG BloquerZone", eMission_Automate_LOG_BloquerZone, T_Mission_Automate_mission_automate_log_bloquer_zone, "T_Mission_Automate_LOG_BloquerZone", "MIS_Automate_MRT_LOG_BloquerZone", "MIS_Automate_CDT_LOG_BloquerZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_ReconnaitreZoneContaminee>( "Automate LOG ReconnaitreZoneContaminee", eMission_Automate_LOG_ReconnaitreZoneContaminee, T_Mission_Automate_mission_automate_log_reconnaitre_zone_contaminee, "T_Mission_Automate_LOG_ReconnaitreZoneContaminee", "MIS_Automate_MRT_LOG_ReconnaitreZoneContaminee", "MIS_Automate_CDT_LOG_ReconnaitreZoneContaminee");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_ReconnaitreZoneDeDeploiement>( "Automate LOG ReconnaitreZoneDeDeploiement", eMission_Automate_LOG_ReconnaitreZoneDeDeploiement, T_Mission_Automate_mission_automate_log_reconnaitre_zone_de_deploiement, "T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement", "MIS_Automate_MRT_LOG_ReconnaitreZoneDeDeploiement", "MIS_Automate_CDT_LOG_ReconnaitreZoneDeDeploiement");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_LOG_ArmerPIAs>( "Automate LOG ArmerPIAs", eMission_Automate_LOG_ArmerPIAs, T_Mission_Automate_mission_automate_log_armer_pias, "T_Mission_Automate_LOG_ArmerPIAs", "MIS_Automate_MRT_LOG_ArmerPIAs", "MIS_Automate_CDT_LOG_ArmerPIAs");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_Test_MoveTo>( "Automate Test MoveTo", eMission_Automate_Test_MoveTo, T_Mission_Automate_mission_automate_test_move_to, "T_Mission_Automate_Test_MoveTo", "MIS_Automate_MRT_Test_MoveTo", "MIS_Automate_CDT_Test_MoveTo");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_Decrocher>( "Automate Decrocher", eMission_Automate_Decrocher, T_Mission_Automate_mission_automate_decrocher, "T_Mission_Automate_Decrocher", "MIS_Automate_MRT_Decrocher", "MIS_Automate_CDT_Decrocher");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_ReleverSurPlace>( "Automate ReleverSurPlace", eMission_Automate_ReleverSurPlace, T_Mission_Automate_mission_automate_relever_sur_place, "T_Mission_Automate_ReleverSurPlace", "MIS_Automate_MRT_ReleverSurPlace", "MIS_Automate_CDT_ReleverSurPlace");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_FaireMouvement>( "Automate FaireMouvement", eMission_Automate_FaireMouvement, T_Mission_Automate_mission_automate_faire_mouvement, "T_Mission_Automate_FaireMouvement", "MIS_Automate_MRT_FaireMouvement", "MIS_Automate_CDT_FaireMouvement");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_SeFaireDecontaminer>( "Automate SeFaireDecontaminer", eMission_Automate_SeFaireDecontaminer, T_Mission_Automate_mission_automate_se_faire_decontaminer, "T_Mission_Automate_SeFaireDecontaminer", "MIS_Automate_MRT_SeFaireDecontaminer", "MIS_Automate_CDT_SeFaireDecontaminer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_Franchir>( "Automate Franchir", eMission_Automate_Franchir, T_Mission_Automate_mission_automate_franchir, "T_Mission_Automate_Franchir", "MIS_Automate_MRT_Franchir", "MIS_Automate_CDT_Franchir");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_Stationner>( "Automate Stationner", eMission_Automate_Stationner, T_Mission_Automate_mission_automate_stationner, "T_Mission_Automate_Stationner", "MIS_Automate_MRT_Stationner", "MIS_Automate_CDT_Stationner");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_SeFaireTransporter>( "Automate SeFaireTransporter", eMission_Automate_SeFaireTransporter, T_Mission_Automate_mission_automate_se_faire_transporter, "T_Mission_Automate_SeFaireTransporter", "MIS_Automate_MRT_SeFaireTransporter", "MIS_Automate_CDT_SeFaireTransporter");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_SeRecompleter>( "Automate SeRecompleter", eMission_Automate_SeRecompleter, T_Mission_Automate_mission_automate_se_recompleter, "T_Mission_Automate_SeRecompleter", "MIS_Automate_MRT_SeRecompleter", "MIS_Automate_CDT_SeRecompleter");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_SeRendre>( "Automate SeRendre", eMission_Automate_SeRendre, T_Mission_Automate_mission_automate_se_rendre, "T_Mission_Automate_SeRendre", "MIS_Automate_MRT_SeRendre", "MIS_Automate_CDT_SeRendre");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_FaireMouvementVersCampRefugies>( "Automate FaireMouvementVersCampRefugies", eMission_Automate_FaireMouvementVersCampRefugies, T_Mission_Automate_mission_automate_faire_mouvement_vers_camp_refugies, "T_Mission_Automate_FaireMouvementVersCampRefugies", "MIS_Automate_MRT_FaireMouvementVersCampRefugies", "MIS_Automate_CDT_FaireMouvementVersCampRefugies");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_Generique>( "Automate Generique", eMission_Automate_Generique, T_Mission_Automate_mission_automate_generique, "T_Mission_Automate_Generique", "MIS_Automate_MRT_Generique", "MIS_Automate_CDT_Generique");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_NBC_ReconnaitreUnAxe>( "Automate NBC ReconnaitreUnAxe", eMission_Automate_NBC_ReconnaitreUnAxe, T_Mission_Automate_mission_automate_nbc_reconnaitre_un_axe, "T_Mission_Automate_NBC_ReconnaitreUnAxe", "MIS_Automate_MRT_NBC_ReconnaitreUnAxe", "MIS_Automate_CDT_NBC_ReconnaitreUnAxe");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_NBC_ReconnaitreUneZone>( "Automate NBC ReconnaitreUneZone", eMission_Automate_NBC_ReconnaitreUneZone, T_Mission_Automate_mission_automate_nbc_reconnaitre_une_zone, "T_Mission_Automate_NBC_ReconnaitreUneZone", "MIS_Automate_MRT_NBC_ReconnaitreUneZone", "MIS_Automate_CDT_NBC_ReconnaitreUneZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_NBC_ArmerUnSiteDeDecontamination>( "Automate NBC ArmerUnSiteDeDecontamination", eMission_Automate_NBC_ArmerUnSiteDeDecontamination, T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination, "T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination", "MIS_Automate_MRT_NBC_ArmerUnSiteDeDecontamination", "MIS_Automate_CDT_NBC_ArmerUnSiteDeDecontamination");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_NBC_DecontaminerUneZone>( "Automate NBC DecontaminerUneZone", eMission_Automate_NBC_DecontaminerUneZone, T_Mission_Automate_mission_automate_nbc_decontaminer_une_zone, "T_Mission_Automate_NBC_DecontaminerUneZone", "MIS_Automate_MRT_NBC_DecontaminerUneZone", "MIS_Automate_CDT_NBC_DecontaminerUneZone");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_NBC_ReconnaitreDesSitesROTA>( "Automate NBC ReconnaitreDesSitesROTA", eMission_Automate_NBC_ReconnaitreDesSitesROTA, T_Mission_Automate_mission_automate_nbc_reconnaitre_des_sites_rota, "T_Mission_Automate_NBC_ReconnaitreDesSitesROTA", "MIS_Automate_MRT_NBC_ReconnaitreDesSitesROTA", "MIS_Automate_CDT_NBC_ReconnaitreDesSitesROTA");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_RENS_ROHUM_RenseignerSur>( "Automate RENS ROHUM RenseignerSur", eMission_Automate_RENS_ROHUM_RenseignerSur, T_Mission_Automate_mission_automate_rens_rohum_renseigner_sur, "T_Mission_Automate_RENS_ROHUM_RenseignerSur", "MIS_Automate_MRT_RENS_ROHUM_RenseignerSur", "MIS_Automate_CDT_RENS_ROHUM_RenseignerSur");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_RENS_ROHUM_OrienterGuider>( "Automate RENS ROHUM OrienterGuider", eMission_Automate_RENS_ROHUM_OrienterGuider, T_Mission_Automate_mission_automate_rens_rohum_orienter_guider, "T_Mission_Automate_RENS_ROHUM_OrienterGuider", "MIS_Automate_MRT_RENS_ROHUM_OrienterGuider", "MIS_Automate_CDT_RENS_ROHUM_OrienterGuider");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_RENS_ROHUM_SExfiltrer>( "Automate RENS ROHUM SExfiltrer", eMission_Automate_RENS_ROHUM_SExfiltrer, T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer, "T_Mission_Automate_RENS_ROHUM_SExfiltrer", "MIS_Automate_MRT_RENS_ROHUM_SExfiltrer", "MIS_Automate_CDT_RENS_ROHUM_SExfiltrer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_RENS_ROIM_CL289_RenseignerSur>( "Automate RENS ROIM CL289 RenseignerSur", eMission_Automate_RENS_ROIM_CL289_RenseignerSur, T_Mission_Automate_mission_automate_rens_roim_cl289_renseigner_sur, "T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur", "MIS_Automate_MRT_RENS_ROIM_CL289_RenseignerSur", "MIS_Automate_CDT_RENS_ROIM_CL289_RenseignerSur");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur>( "Automate RENS ROIM SDTI RenseignerSur", eMission_Automate_RENS_ROIM_SDTI_RenseignerSur, T_Mission_Automate_mission_automate_rens_roim_sdti_renseigner_sur, "T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur", "MIS_Automate_MRT_RENS_ROIM_SDTI_RenseignerSur", "MIS_Automate_CDT_RENS_ROIM_SDTI_RenseignerSur");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_RENS_ROEM_Appuyer>( "Automate RENS ROEM Appuyer", eMission_Automate_RENS_ROEM_Appuyer, T_Mission_Automate_mission_automate_rens_roem_appuyer, "T_Mission_Automate_RENS_ROEM_Appuyer", "MIS_Automate_MRT_RENS_ROEM_Appuyer", "MIS_Automate_CDT_RENS_ROEM_Appuyer");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_RENS_ROEM_RenseignerSur>( "Automate RENS ROEM RenseignerSur", eMission_Automate_RENS_ROEM_RenseignerSur, T_Mission_Automate_mission_automate_rens_roem_renseignersur, "T_Mission_Automate_RENS_ROEM_RenseignerSur", "MIS_Automate_MRT_RENS_ROEM_RenseignerSur", "MIS_Automate_CDT_RENS_ROEM_RenseignerSur");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_RENS_RADINT_RenseignerSur>( "Automate RENS RADINT RenseignerSur", eMission_Automate_RENS_RADINT_RenseignerSur, T_Mission_Automate_mission_automate_rens_radint_renseignersur, "T_Mission_Automate_RENS_RADINT_RenseignerSur", "MIS_Automate_MRT_RENS_RADINT_RenseignerSur", "MIS_Automate_CDT_RENS_RADINT_RenseignerSur");
    MIL_AutomateMissionType::RegisterMission< MIL_AutomateMission_TRANS_Commander>( "Automate TRANS Commander", eMission_Automate_TRANS_Commander, T_Mission_Automate_mission_automate_trans_commander, "T_Mission_Automate_TRANS_Commander", "MIS_Automate_MRT_TRANS_Commander", "MIS_Automate_CDT_TRANS_Commander");

}

// -----------------------------------------------------------------------------
// Name: RegisterPionMissions
// Created: AGR 
// -----------------------------------------------------------------------------
void missions::RegisterPionMissions()
{
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_EvacuationSanitaire>( "Pion ALAT EvacuationSanitaire", eMission_Pion_ALAT_EvacuationSanitaire, T_Mission_Pion_mission_pion_alat_evacuation_sanitaire, "T_Mission_Pion_ALAT_EvacuationSanitaire", "MIS_Pion_ALAT_EvacuationSanitaire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_ReconnaitreDansLaProfondeur>( "Pion ALAT ReconnaitreDansLaProfondeur", eMission_Pion_ALAT_ReconnaitreDansLaProfondeur, T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur, "T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur", "MIS_Pion_ALAT_ReconnaitreDansLaProfondeur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_ReconnaitreContourEnnemi>( "Pion ALAT ReconnaitreContourEnnemi", eMission_Pion_ALAT_ReconnaitreContourEnnemi, T_Mission_Pion_mission_pion_alat_reconnaitre_contour_ennemi, "T_Mission_Pion_ALAT_ReconnaitreContourEnnemi", "MIS_Pion_ALAT_ReconnaitreContourEnnemi");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_DetruireNeutraliserDansZone>( "Pion ALAT DetruireNeutraliserDansZone", eMission_Pion_ALAT_DetruireNeutraliserDansZone, T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone, "T_Mission_Pion_ALAT_DetruireNeutraliserDansZone", "MIS_Pion_ALAT_DetruireNeutraliserDansZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_DetruireAeromobile>( "Pion ALAT DetruireAeromobile", eMission_Pion_ALAT_DetruireAeromobile, T_Mission_Pion_mission_pion_alat_detruire_aeromobile, "T_Mission_Pion_ALAT_DetruireAeromobile", "MIS_Pion_ALAT_DetruireAeromobile");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Freiner>( "Pion ALAT Freiner", eMission_Pion_ALAT_Freiner, T_Mission_Pion_mission_pion_alat_freiner, "T_Mission_Pion_ALAT_Freiner", "MIS_Pion_ALAT_Freiner");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Jalonner>( "Pion ALAT Jalonner", eMission_Pion_ALAT_Jalonner, T_Mission_Pion_mission_pion_alat_jalonner, "T_Mission_Pion_ALAT_Jalonner", "MIS_Pion_ALAT_Jalonner");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Escorter>( "Pion ALAT Escorter", eMission_Pion_ALAT_Escorter, T_Mission_Pion_mission_pion_alat_escorter, "T_Mission_Pion_ALAT_Escorter", "MIS_Pion_ALAT_Escorter");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Heliporter>( "Pion ALAT Heliporter", eMission_Pion_ALAT_Heliporter, T_Mission_Pion_mission_pion_alat_heliporter, "T_Mission_Pion_ALAT_Heliporter", "MIS_Pion_ALAT_Heliporter");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_HeliporterHelitransporterPlotRavitaillement>( "Pion ALAT HeliporterHelitransporterPlotRavitaillement", eMission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement, T_Mission_Pion_mission_pion_alat_heliporter_helitransporter_plot_ravitaillement, "T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement", "MIS_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Helitransporter>( "Pion ALAT Helitransporter", eMission_Pion_ALAT_Helitransporter, T_Mission_Pion_mission_pion_alat_helitransporter, "T_Mission_Pion_ALAT_Helitransporter", "MIS_Pion_ALAT_Helitransporter");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_EffectuerRechercheEtSauvetage>( "Pion ALAT EffectuerRechercheEtSauvetage", eMission_Pion_ALAT_EffectuerRechercheEtSauvetage, T_Mission_Pion_mission_pion_alat_effectuer_recherche_et_sauvetage, "T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage", "MIS_Pion_ALAT_EffectuerRechercheEtSauvetage");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_IMEX>( "Pion ALAT IMEX", eMission_Pion_ALAT_IMEX, T_Mission_Pion_mission_pion_alat_imex, "T_Mission_Pion_ALAT_IMEX", "MIS_Pion_ALAT_IMEX");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Eclairer>( "Pion ALAT Eclairer", eMission_Pion_ALAT_Eclairer, T_Mission_Pion_mission_pion_alat_eclairer, "T_Mission_Pion_ALAT_Eclairer", "MIS_Pion_ALAT_Eclairer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Surveiller>( "Pion ALAT Surveiller", eMission_Pion_ALAT_Surveiller, T_Mission_Pion_mission_pion_alat_surveiller, "T_Mission_Pion_ALAT_Surveiller", "MIS_Pion_ALAT_Surveiller");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_AppuyerDirectAuContact>( "Pion ALAT AppuyerDirectAuContact", eMission_Pion_ALAT_AppuyerDirectAuContact, T_Mission_Pion_mission_pion_alat_appuyer_direct_au_contact, "T_Mission_Pion_ALAT_AppuyerDirectAuContact", "MIS_Pion_ALAT_AppuyerDirectAuContact");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Couvrir>( "Pion ALAT Couvrir", eMission_Pion_ALAT_Couvrir, T_Mission_Pion_mission_pion_alat_couvrir, "T_Mission_Pion_ALAT_Couvrir", "MIS_Pion_ALAT_Couvrir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ALAT_Attaquer>( "Pion ALAT Attaquer", eMission_Pion_ALAT_Attaquer, T_Mission_Pion_mission_pion_alat_attaquer, "T_Mission_Pion_ALAT_Attaquer", "MIS_Pion_ALAT_Attaquer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_DefendreSite>( "Pion ASA DefendreSite", eMission_Pion_ASA_DefendreSite, T_Mission_Pion_mission_pion_asa_defendre_site, "T_Mission_Pion_ASA_DefendreSite", "MIS_Pion_ASA_DefendreSite");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_DefendreSiteAPartirPosition>( "Pion ASA DefendreSiteAPartirPosition", eMission_Pion_ASA_DefendreSiteAPartirPosition, T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position, "T_Mission_Pion_ASA_DefendreSiteAPartirPosition", "MIS_Pion_ASA_DefendreSiteAPartirPosition");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_DefendreZone>( "Pion ASA DefendreZone", eMission_Pion_ASA_DefendreZone, T_Mission_Pion_mission_pion_asa_defendre_zone, "T_Mission_Pion_ASA_DefendreZone", "MIS_Pion_ASA_DefendreZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_DefendreZoneAPartirPosition>( "Pion ASA DefendreZoneAPartirPosition", eMission_Pion_ASA_DefendreZoneAPartirPosition, T_Mission_Pion_mission_pion_asa_defendre_zone_a_partir_position, "T_Mission_Pion_ASA_DefendreZoneAPartirPosition", "MIS_Pion_ASA_DefendreZoneAPartirPosition");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_Surveiller>( "Pion ASA Surveiller", eMission_Pion_ASA_Surveiller, T_Mission_Pion_mission_pion_asa_surveiller, "T_Mission_Pion_ASA_Surveiller", "MIS_Pion_ASA_Surveiller");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_DefendreUnites>( "Pion ASA DefendreUnites", eMission_Pion_ASA_DefendreUnites, T_Mission_Pion_mission_pion_asa_defendre_unites, "T_Mission_Pion_ASA_DefendreUnites", "MIS_Pion_ASA_DefendreUnites");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_MISTRAL_DefendreSite>( "Pion ASA MISTRAL DefendreSite", eMission_Pion_ASA_MISTRAL_DefendreSite, T_Mission_Pion_mission_pion_asa_mistral_defendre_site, "T_Mission_Pion_ASA_MISTRAL_DefendreSite", "MIS_Pion_ASA_MISTRAL_DefendreSite");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_MISTRAL_DefendreSiteAPartirPosition>( "Pion ASA MISTRAL DefendreSiteAPartirPosition", eMission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition, T_Mission_Pion_mission_pion_asa_mistral_defendre_site_a_partir_position, "T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition", "MIS_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_MISTRAL_DefendreZone>( "Pion ASA MISTRAL DefendreZone", eMission_Pion_ASA_MISTRAL_DefendreZone, T_Mission_Pion_mission_pion_asa_mistral_defendre_zone, "T_Mission_Pion_ASA_MISTRAL_DefendreZone", "MIS_Pion_ASA_MISTRAL_DefendreZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_MISTRAL_DefendreZoneAPartirPosition>( "Pion ASA MISTRAL DefendreZoneAPartirPosition", eMission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition, T_Mission_Pion_mission_pion_asa_mistral_defendre_zone_a_partir_position, "T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition", "MIS_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASA_MISTRAL_Surveiller>( "Pion ASA MISTRAL Surveiller", eMission_Pion_ASA_MISTRAL_Surveiller, T_Mission_Pion_mission_pion_asa_mistral_surveiller, "T_Mission_Pion_ASA_MISTRAL_Surveiller", "MIS_Pion_ASA_MISTRAL_Surveiller");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASS_AssurerMiseEnOeuvre>( "Pion ASS AssurerMiseEnOeuvre", eMission_Pion_ASS_AssurerMiseEnOeuvre, T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre, "T_Mission_Pion_ASS_AssurerMiseEnOeuvre", "MIS_Pion_ASS_AssurerMiseEnOeuvre");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition>( "Pion ASS AssurerMiseEnOeuvreSurPosition", eMission_Pion_ASS_AssurerMiseEnOeuvreSurPosition, T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre_sur_position, "T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition", "MIS_Pion_ASS_AssurerMiseEnOeuvreSurPosition");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASS_ReconnaitreZoneDeploiement>( "Pion ASS ReconnaitreZoneDeploiement", eMission_Pion_ASS_ReconnaitreZoneDeploiement, T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement, "T_Mission_Pion_ASS_ReconnaitreZoneDeploiement", "MIS_Pion_ASS_ReconnaitreZoneDeploiement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASS_AcquerirLanceursEni>( "Pion ASS AcquerirLanceursEni", eMission_Pion_ASS_AcquerirLanceursEni, T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni, "T_Mission_Pion_ASS_AcquerirLanceursEni", "MIS_Pion_ASS_AcquerirLanceursEni");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASS_EO_AcquerirObjectifs>( "Pion ASS EO AcquerirObjectifs", eMission_Pion_ASS_EO_AcquerirObjectifs, T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs, "T_Mission_Pion_ASS_EO_AcquerirObjectifs", "MIS_Pion_ASS_EO_AcquerirObjectifs");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASS_EOP_AcquerirObjectifs>( "Pion ASS EOP AcquerirObjectifs", eMission_Pion_ASS_EOP_AcquerirObjectifs, T_Mission_Pion_mission_pion_ass_eop_acquerir_objectifs, "T_Mission_Pion_ASS_EOP_AcquerirObjectifs", "MIS_Pion_ASS_EOP_AcquerirObjectifs");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ASS_RATAC_AcquerirObjectifs>( "Pion ASS RATAC AcquerirObjectifs", eMission_Pion_ASS_RATAC_AcquerirObjectifs, T_Mission_Pion_mission_pion_ass_ratac_acquerir_objectifs, "T_Mission_Pion_ASS_RATAC_AcquerirObjectifs", "MIS_Pion_ASS_RATAC_AcquerirObjectifs");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_ReconnaitreAxe>( "Pion ABC ReconnaitreAxe", eMission_Pion_ABC_ReconnaitreAxe, T_Mission_Pion_mission_pion_abc_reconnaitre_axe, "T_Mission_Pion_ABC_ReconnaitreAxe", "MIS_Pion_ABC_ReconnaitreAxe");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Soutenir>( "Pion ABC Soutenir", eMission_Pion_ABC_Soutenir, T_Mission_Pion_mission_pion_abc_soutenir, "T_Mission_Pion_ABC_Soutenir", "MIS_Pion_ABC_Soutenir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Couvrir>( "Pion ABC Couvrir", eMission_Pion_ABC_Couvrir, T_Mission_Pion_mission_pion_abc_couvrir, "T_Mission_Pion_ABC_Couvrir", "MIS_Pion_ABC_Couvrir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_FlancGarder>( "Pion ABC FlancGarder", eMission_Pion_ABC_FlancGarder, T_Mission_Pion_mission_pion_abc_flanc_garder, "T_Mission_Pion_ABC_FlancGarder", "MIS_Pion_ABC_FlancGarder");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Reduire>( "Pion ABC Reduire", eMission_Pion_ABC_Reduire, T_Mission_Pion_mission_pion_abc_reduire, "T_Mission_Pion_ABC_Reduire", "MIS_Pion_ABC_Reduire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Appuyer>( "Pion ABC Appuyer", eMission_Pion_ABC_Appuyer, T_Mission_Pion_mission_pion_abc_appuyer, "T_Mission_Pion_ABC_Appuyer", "MIS_Pion_ABC_Appuyer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Fixer>( "Pion ABC Fixer", eMission_Pion_ABC_Fixer, T_Mission_Pion_mission_pion_abc_fixer, "T_Mission_Pion_ABC_Fixer", "MIS_Pion_ABC_Fixer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_RenseignerSur>( "Pion ABC RenseignerSur", eMission_Pion_ABC_RenseignerSur, T_Mission_Pion_mission_pion_abc_renseigner_sur, "T_Mission_Pion_ABC_RenseignerSur", "MIS_Pion_ABC_RenseignerSur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_ControlerZone>( "Pion ABC ControlerZone", eMission_Pion_ABC_ControlerZone, T_Mission_Pion_mission_pion_abc_controler_zone, "T_Mission_Pion_ABC_ControlerZone", "MIS_Pion_ABC_ControlerZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Surveiller>( "Pion ABC Surveiller", eMission_Pion_ABC_Surveiller, T_Mission_Pion_mission_pion_abc_surveiller, "T_Mission_Pion_ABC_Surveiller", "MIS_Pion_ABC_Surveiller");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Barrer>( "Pion ABC Barrer", eMission_Pion_ABC_Barrer, T_Mission_Pion_mission_pion_abc_barrer, "T_Mission_Pion_ABC_Barrer", "MIS_Pion_ABC_Barrer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Attaquer>( "Pion ABC Attaquer", eMission_Pion_ABC_Attaquer, T_Mission_Pion_mission_pion_abc_attaquer, "T_Mission_Pion_ABC_Attaquer", "MIS_Pion_ABC_Attaquer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_ReconnaitrePoint>( "Pion ABC ReconnaitrePoint", eMission_Pion_ABC_ReconnaitrePoint, T_Mission_Pion_mission_pion_abc_reconnaitre_point, "T_Mission_Pion_ABC_ReconnaitrePoint", "MIS_Pion_ABC_ReconnaitrePoint");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Eclairer>( "Pion ABC Eclairer", eMission_Pion_ABC_Eclairer, T_Mission_Pion_mission_pion_abc_eclairer, "T_Mission_Pion_ABC_Eclairer", "MIS_Pion_ABC_Eclairer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Jalonner>( "Pion ABC Jalonner", eMission_Pion_ABC_Jalonner, T_Mission_Pion_mission_pion_abc_jalonner, "T_Mission_Pion_ABC_Jalonner", "MIS_Pion_ABC_Jalonner");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_ContreAttaquer>( "Pion ABC ContreAttaquer", eMission_Pion_ABC_ContreAttaquer, T_Mission_Pion_mission_pion_abc_contre_attaquer, "T_Mission_Pion_ABC_ContreAttaquer", "MIS_Pion_ABC_ContreAttaquer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_SemparerZone>( "Pion ABC SemparerZone", eMission_Pion_ABC_SemparerZone, T_Mission_Pion_mission_pion_abc_semparer_zone, "T_Mission_Pion_ABC_SemparerZone", "MIS_Pion_ABC_SemparerZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_ReleverUnite>( "Pion ABC ReleverUnite", eMission_Pion_ABC_ReleverUnite, T_Mission_Pion_mission_pion_abc_relever_unite, "T_Mission_Pion_ABC_ReleverUnite", "MIS_Pion_ABC_ReleverUnite");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_ArmerPIA>( "Pion ABC ArmerPIA", eMission_Pion_ABC_ArmerPIA, T_Mission_Pion_mission_pion_abc_armer_pia, "T_Mission_Pion_ABC_ArmerPIA", "MIS_Pion_ABC_ArmerPIA");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_OuvrirItineraire>( "Pion ABC OuvrirItineraire", eMission_Pion_ABC_OuvrirItineraire, T_Mission_Pion_mission_pion_abc_ouvrir_itineraire, "T_Mission_Pion_ABC_OuvrirItineraire", "MIS_Pion_ABC_OuvrirItineraire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_Freiner>( "Pion ABC Freiner", eMission_Pion_ABC_Freiner, T_Mission_Pion_mission_pion_abc_freiner, "T_Mission_Pion_ABC_Freiner", "MIS_Pion_ABC_Freiner");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_EscorterUnConvoi>( "Pion ABC EscorterUnConvoi", eMission_Pion_ABC_EscorterUnConvoi, T_Mission_Pion_mission_pion_abc_escorter_un_convoi, "T_Mission_Pion_ABC_EscorterUnConvoi", "MIS_Pion_ABC_EscorterUnConvoi");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_ArmerPointDeControle>( "Pion ABC ArmerPointDeControle", eMission_Pion_ABC_ArmerPointDeControle, T_Mission_Pion_mission_pion_abc_armer_point_de_controle, "T_Mission_Pion_ABC_ArmerPointDeControle", "MIS_Pion_ABC_ArmerPointDeControle");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_ABC_OrganiserAccueilColonneRefugies>( "Pion ABC OrganiserAccueilColonneRefugies", eMission_Pion_ABC_OrganiserAccueilColonneRefugies, T_Mission_Pion_mission_pion_abc_organiser_accueil_colonne_refugies, "T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies", "MIS_Pion_ABC_OrganiserAccueilColonneRefugies");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserFosseAC>( "Pion GEN RealiserFosseAC", eMission_Pion_GEN_RealiserFosseAC, T_Mission_Pion_mission_pion_gen_realiser_fosse_ac, "T_Mission_Pion_GEN_RealiserFosseAC", "MIS_Pion_GEN_RealiserFosseAC");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserAbattisMine>( "Pion GEN RealiserAbattisMine", eMission_Pion_GEN_RealiserAbattisMine, T_Mission_Pion_mission_pion_gen_realiser_abattis_mine, "T_Mission_Pion_GEN_RealiserAbattisMine", "MIS_Pion_GEN_RealiserAbattisMine");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserPointMine>( "Pion GEN RealiserPointMine", eMission_Pion_GEN_RealiserPointMine, T_Mission_Pion_mission_pion_gen_realiser_point_mine, "T_Mission_Pion_GEN_RealiserPointMine", "MIS_Pion_GEN_RealiserPointMine");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserBarricade>( "Pion GEN RealiserBarricade", eMission_Pion_GEN_RealiserBarricade, T_Mission_Pion_mission_pion_gen_realiser_barricade, "T_Mission_Pion_GEN_RealiserBarricade", "MIS_Pion_GEN_RealiserBarricade");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserEboulement>( "Pion GEN RealiserEboulement", eMission_Pion_GEN_RealiserEboulement, T_Mission_Pion_mission_pion_gen_realiser_eboulement, "T_Mission_Pion_GEN_RealiserEboulement", "MIS_Pion_GEN_RealiserEboulement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserBouchonMine>( "Pion GEN RealiserBouchonMine", eMission_Pion_GEN_RealiserBouchonMine, T_Mission_Pion_mission_pion_gen_realiser_bouchon_mine, "T_Mission_Pion_GEN_RealiserBouchonMine", "MIS_Pion_GEN_RealiserBouchonMine");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_DetruireRoute>( "Pion GEN DetruireRoute", eMission_Pion_GEN_DetruireRoute, T_Mission_Pion_mission_pion_gen_detruire_route, "T_Mission_Pion_GEN_DetruireRoute", "MIS_Pion_GEN_DetruireRoute");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_DetruirePont>( "Pion GEN DetruirePont", eMission_Pion_GEN_DetruirePont, T_Mission_Pion_mission_pion_gen_detruire_pont, "T_Mission_Pion_GEN_DetruirePont", "MIS_Pion_GEN_DetruirePont");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_SecuriserItineraire>( "Pion GEN SecuriserItineraire", eMission_Pion_GEN_SecuriserItineraire, T_Mission_Pion_mission_pion_gen_securiser_itineraire, "T_Mission_Pion_GEN_SecuriserItineraire", "MIS_Pion_GEN_SecuriserItineraire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_DegagerAbattisMine>( "Pion GEN DegagerAbattisMine", eMission_Pion_GEN_DegagerAbattisMine, T_Mission_Pion_mission_pion_gen_degager_abattis_mine, "T_Mission_Pion_GEN_DegagerAbattisMine", "MIS_Pion_GEN_DegagerAbattisMine");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_DegagerEboulement>( "Pion GEN DegagerEboulement", eMission_Pion_GEN_DegagerEboulement, T_Mission_Pion_mission_pion_gen_degager_eboulement, "T_Mission_Pion_GEN_DegagerEboulement", "MIS_Pion_GEN_DegagerEboulement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_DegagerRues>( "Pion GEN DegagerRues", eMission_Pion_GEN_DegagerRues, T_Mission_Pion_mission_pion_gen_degager_rues, "T_Mission_Pion_GEN_DegagerRues", "MIS_Pion_GEN_DegagerRues");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_ComblerEntonnoir>( "Pion GEN ComblerEntonnoir", eMission_Pion_GEN_ComblerEntonnoir, T_Mission_Pion_mission_pion_gen_combler_entonnoir, "T_Mission_Pion_GEN_ComblerEntonnoir", "MIS_Pion_GEN_ComblerEntonnoir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_ComblerFosseAC>( "Pion GEN ComblerFosseAC", eMission_Pion_GEN_ComblerFosseAC, T_Mission_Pion_mission_pion_gen_combler_fosse_ac, "T_Mission_Pion_GEN_ComblerFosseAC", "MIS_Pion_GEN_ComblerFosseAC");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_CreerContournement>( "Pion GEN CreerContournement", eMission_Pion_GEN_CreerContournement, T_Mission_Pion_mission_pion_gen_creer_contournement, "T_Mission_Pion_GEN_CreerContournement", "MIS_Pion_GEN_CreerContournement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserZoneMineeLineaire>( "Pion GEN RealiserZoneMineeLineaire", eMission_Pion_GEN_RealiserZoneMineeLineaire, T_Mission_Pion_mission_pion_gen_realiser_zone_minee_lineaire, "T_Mission_Pion_GEN_RealiserZoneMineeLineaire", "MIS_Pion_GEN_RealiserZoneMineeLineaire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_OuvrirTrouee>( "Pion GEN OuvrirTrouee", eMission_Pion_GEN_OuvrirTrouee, T_Mission_Pion_mission_pion_gen_ouvrir_trouee, "T_Mission_Pion_GEN_OuvrirTrouee", "MIS_Pion_GEN_OuvrirTrouee");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_Renforcer>( "Pion GEN Renforcer", eMission_Pion_GEN_Renforcer, T_Mission_Pion_mission_pion_gen_renforcer, "T_Mission_Pion_GEN_Renforcer", "MIS_Pion_GEN_Renforcer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_FranchirEntonnoir>( "Pion GEN FranchirEntonnoir", eMission_Pion_GEN_FranchirEntonnoir, T_Mission_Pion_mission_pion_gen_franchir_entonnoir, "T_Mission_Pion_GEN_FranchirEntonnoir", "MIS_Pion_GEN_FranchirEntonnoir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_FranchirFosseAC>( "Pion GEN FranchirFosseAC", eMission_Pion_GEN_FranchirFosseAC, T_Mission_Pion_mission_pion_gen_franchir_fosse_ac, "T_Mission_Pion_GEN_FranchirFosseAC", "MIS_Pion_GEN_FranchirFosseAC");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserZoneMineeParDispersion>( "Pion GEN RealiserZoneMineeParDispersion", eMission_Pion_GEN_RealiserZoneMineeParDispersion, T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion, "T_Mission_Pion_GEN_RealiserZoneMineeParDispersion", "MIS_Pion_GEN_RealiserZoneMineeParDispersion");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserPostesTir>( "Pion GEN RealiserPostesTir", eMission_Pion_GEN_RealiserPostesTir, T_Mission_Pion_mission_pion_gen_realiser_postes_tir, "T_Mission_Pion_GEN_RealiserPostesTir", "MIS_Pion_GEN_RealiserPostesTir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserTravauxProtection>( "Pion GEN RealiserTravauxProtection", eMission_Pion_GEN_RealiserTravauxProtection, T_Mission_Pion_mission_pion_gen_realiser_travaux_protection, "T_Mission_Pion_GEN_RealiserTravauxProtection", "MIS_Pion_GEN_RealiserTravauxProtection");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RetablirItineraire>( "Pion GEN RetablirItineraire", eMission_Pion_GEN_RetablirItineraire, T_Mission_Pion_mission_pion_gen_retablir_itineraire, "T_Mission_Pion_GEN_RetablirItineraire", "MIS_Pion_GEN_RetablirItineraire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_EquiperExploiter>( "Pion GEN EquiperExploiter", eMission_Pion_GEN_EquiperExploiter, T_Mission_Pion_mission_pion_gen_equiper_exploiter, "T_Mission_Pion_GEN_EquiperExploiter", "MIS_Pion_GEN_EquiperExploiter");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_ExecuterVariantement>( "Pion GEN ExecuterVariantement", eMission_Pion_GEN_ExecuterVariantement, T_Mission_Pion_mission_pion_gen_executer_variantement, "T_Mission_Pion_GEN_ExecuterVariantement", "MIS_Pion_GEN_ExecuterVariantement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_ReconnaitreSiteFranchissement>( "Pion GEN ReconnaitreSiteFranchissement", eMission_Pion_GEN_ReconnaitreSiteFranchissement, T_Mission_Pion_mission_pion_gen_reconnaitre_site_franchissement, "T_Mission_Pion_GEN_ReconnaitreSiteFranchissement", "MIS_Pion_GEN_ReconnaitreSiteFranchissement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_ReconnaitreItineraire>( "Pion GEN ReconnaitreItineraire", eMission_Pion_GEN_ReconnaitreItineraire, T_Mission_Pion_mission_pion_gen_reconnaitre_itineraire, "T_Mission_Pion_GEN_ReconnaitreItineraire", "MIS_Pion_GEN_ReconnaitreItineraire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_DeminerSiteFranchissement>( "Pion GEN DeminerSiteFranchissement", eMission_Pion_GEN_DeminerSiteFranchissement, T_Mission_Pion_mission_pion_gen_deminer_site_franchissement, "T_Mission_Pion_GEN_DeminerSiteFranchissement", "MIS_Pion_GEN_DeminerSiteFranchissement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_ReconnaitreZone>( "Pion GEN ReconnaitreZone", eMission_Pion_GEN_ReconnaitreZone, T_Mission_Pion_mission_pion_gen_reconnaitre_zone, "T_Mission_Pion_GEN_ReconnaitreZone", "MIS_Pion_GEN_ReconnaitreZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_CreerPlateForme>( "Pion GEN CreerPlateForme", eMission_Pion_GEN_CreerPlateForme, T_Mission_Pion_mission_pion_gen_creer_plate_forme, "T_Mission_Pion_GEN_CreerPlateForme", "MIS_Pion_GEN_CreerPlateForme");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_CreerPiste>( "Pion GEN CreerPiste", eMission_Pion_GEN_CreerPiste, T_Mission_Pion_mission_pion_gen_creer_piste, "T_Mission_Pion_GEN_CreerPiste", "MIS_Pion_GEN_CreerPiste");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_CreerAirePoser>( "Pion GEN CreerAirePoser", eMission_Pion_GEN_CreerAirePoser, T_Mission_Pion_mission_pion_gen_creer_aire_poser, "T_Mission_Pion_GEN_CreerAirePoser", "MIS_Pion_GEN_CreerAirePoser");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_NeutraliserEnginsExplosifs>( "Pion GEN NeutraliserEnginsExplosifs", eMission_Pion_GEN_NeutraliserEnginsExplosifs, T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs, "T_Mission_Pion_GEN_NeutraliserEnginsExplosifs", "MIS_Pion_GEN_NeutraliserEnginsExplosifs");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_ActiverObstacle>( "Pion GEN ActiverObstacle", eMission_Pion_GEN_ActiverObstacle, T_Mission_Pion_mission_pion_gen_activer_obstacle, "T_Mission_Pion_GEN_ActiverObstacle", "MIS_Pion_GEN_ActiverObstacle");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_AmeliorerMobilite>( "Pion GEN AmeliorerMobilite", eMission_Pion_GEN_AmeliorerMobilite, T_Mission_Pion_mission_pion_gen_ameliorer_mobilite, "T_Mission_Pion_GEN_AmeliorerMobilite", "MIS_Pion_GEN_AmeliorerMobilite");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserAireLogistique>( "Pion GEN RealiserAireLogistique", eMission_Pion_GEN_RealiserAireLogistique, T_Mission_Pion_mission_pion_gen_realiser_aire_logistique, "T_Mission_Pion_GEN_RealiserAireLogistique", "MIS_Pion_GEN_RealiserAireLogistique");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_DepolluerZone>( "Pion GEN DepolluerZone", eMission_Pion_GEN_DepolluerZone, T_Mission_Pion_mission_pion_gen_depolluer_zone, "T_Mission_Pion_GEN_DepolluerZone", "MIS_Pion_GEN_DepolluerZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_AmenagerTerrainPoser>( "Pion GEN AmenagerTerrainPoser", eMission_Pion_GEN_AmenagerTerrainPoser, T_Mission_Pion_mission_pion_gen_amenager_terrain_poser, "T_Mission_Pion_GEN_AmenagerTerrainPoser", "MIS_Pion_GEN_AmenagerTerrainPoser");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserPlateFormeDeploiement>( "Pion GEN RealiserPlateFormeDeploiement", eMission_Pion_GEN_RealiserPlateFormeDeploiement, T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement, "T_Mission_Pion_GEN_RealiserPlateFormeDeploiement", "MIS_Pion_GEN_RealiserPlateFormeDeploiement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_ReprendreMissionConstruction>( "Pion GEN ReprendreMissionConstruction", eMission_Pion_GEN_ReprendreMissionConstruction, T_Mission_Pion_mission_pion_gen_reprendre_mission_construction, "T_Mission_Pion_GEN_ReprendreMissionConstruction", "MIS_Pion_GEN_ReprendreMissionConstruction");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserZonePoserHelicoptere>( "Pion GEN RealiserZonePoserHelicoptere", eMission_Pion_GEN_RealiserZonePoserHelicoptere, T_Mission_Pion_mission_pion_gen_realiser_zone_poser_helicoptere, "T_Mission_Pion_GEN_RealiserZonePoserHelicoptere", "MIS_Pion_GEN_RealiserZonePoserHelicoptere");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_AppuyerDebarquementTroupesGU>( "Pion GEN AppuyerDebarquementTroupesGU", eMission_Pion_GEN_AppuyerDebarquementTroupesGU, T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu, "T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU", "MIS_Pion_GEN_AppuyerDebarquementTroupesGU");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserCampPrisonniers>( "Pion GEN RealiserCampPrisonniers", eMission_Pion_GEN_RealiserCampPrisonniers, T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers, "T_Mission_Pion_GEN_RealiserCampPrisonniers", "MIS_Pion_GEN_RealiserCampPrisonniers");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_GEN_RealiserCampRefugies>( "Pion GEN RealiserCampRefugies", eMission_Pion_GEN_RealiserCampRefugies, T_Mission_Pion_mission_pion_gen_realiser_camp_refugies, "T_Mission_Pion_GEN_RealiserCampRefugies", "MIS_Pion_GEN_RealiserCampRefugies");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_ReconnaitreAxe>( "Pion INF ReconnaitreAxe", eMission_Pion_INF_ReconnaitreAxe, T_Mission_Pion_mission_pion_inf_reconnaitre_axe, "T_Mission_Pion_INF_ReconnaitreAxe", "MIS_Pion_INF_ReconnaitreAxe");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Couvrir>( "Pion INF Couvrir", eMission_Pion_INF_Couvrir, T_Mission_Pion_mission_pion_inf_couvrir, "T_Mission_Pion_INF_Couvrir", "MIS_Pion_INF_Couvrir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Reduire>( "Pion INF Reduire", eMission_Pion_INF_Reduire, T_Mission_Pion_mission_pion_inf_reduire, "T_Mission_Pion_INF_Reduire", "MIS_Pion_INF_Reduire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Appuyer>( "Pion INF Appuyer", eMission_Pion_INF_Appuyer, T_Mission_Pion_mission_pion_inf_appuyer, "T_Mission_Pion_INF_Appuyer", "MIS_Pion_INF_Appuyer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Fixer>( "Pion INF Fixer", eMission_Pion_INF_Fixer, T_Mission_Pion_mission_pion_inf_fixer, "T_Mission_Pion_INF_Fixer", "MIS_Pion_INF_Fixer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_ControlerSecteur>( "Pion INF ControlerSecteur", eMission_Pion_INF_ControlerSecteur, T_Mission_Pion_mission_pion_inf_controler_secteur, "T_Mission_Pion_INF_ControlerSecteur", "MIS_Pion_INF_ControlerSecteur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_SurveillerSecteur>( "Pion INF SurveillerSecteur", eMission_Pion_INF_SurveillerSecteur, T_Mission_Pion_mission_pion_inf_surveiller_secteur, "T_Mission_Pion_INF_SurveillerSecteur", "MIS_Pion_INF_SurveillerSecteur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Barrer>( "Pion INF Barrer", eMission_Pion_INF_Barrer, T_Mission_Pion_mission_pion_inf_barrer, "T_Mission_Pion_INF_Barrer", "MIS_Pion_INF_Barrer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_ReconnaitrePoint>( "Pion INF ReconnaitrePoint", eMission_Pion_INF_ReconnaitrePoint, T_Mission_Pion_mission_pion_inf_reconnaitre_point, "T_Mission_Pion_INF_ReconnaitrePoint", "MIS_Pion_INF_ReconnaitrePoint");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Eclairer>( "Pion INF Eclairer", eMission_Pion_INF_Eclairer, T_Mission_Pion_mission_pion_inf_eclairer, "T_Mission_Pion_INF_Eclairer", "MIS_Pion_INF_Eclairer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Interdire>( "Pion INF Interdire", eMission_Pion_INF_Interdire, T_Mission_Pion_mission_pion_inf_interdire, "T_Mission_Pion_INF_Interdire", "MIS_Pion_INF_Interdire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Semparer>( "Pion INF Semparer", eMission_Pion_INF_Semparer, T_Mission_Pion_mission_pion_inf_semparer, "T_Mission_Pion_INF_Semparer", "MIS_Pion_INF_Semparer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_PrendrePreciserContact>( "Pion INF PrendrePreciserContact", eMission_Pion_INF_PrendrePreciserContact, T_Mission_Pion_mission_pion_inf_prendre_preciser_contact, "T_Mission_Pion_INF_PrendrePreciserContact", "MIS_Pion_INF_PrendrePreciserContact");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Defendre>( "Pion INF Defendre", eMission_Pion_INF_Defendre, T_Mission_Pion_mission_pion_inf_defendre, "T_Mission_Pion_INF_Defendre", "MIS_Pion_INF_Defendre");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_DetruireEmbuscade>( "Pion INF DetruireEmbuscade", eMission_Pion_INF_DetruireEmbuscade, T_Mission_Pion_mission_pion_inf_detruire_embuscade, "T_Mission_Pion_INF_DetruireEmbuscade", "MIS_Pion_INF_DetruireEmbuscade");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_ContreAttaquer>( "Pion INF ContreAttaquer", eMission_Pion_INF_ContreAttaquer, T_Mission_Pion_mission_pion_inf_contre_attaquer, "T_Mission_Pion_INF_ContreAttaquer", "MIS_Pion_INF_ContreAttaquer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Freiner>( "Pion INF Freiner", eMission_Pion_INF_Freiner, T_Mission_Pion_mission_pion_inf_freiner, "T_Mission_Pion_INF_Freiner", "MIS_Pion_INF_Freiner");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_FlancGarder>( "Pion INF FlancGarder", eMission_Pion_INF_FlancGarder, T_Mission_Pion_mission_pion_inf_flanc_garder, "T_Mission_Pion_INF_FlancGarder", "MIS_Pion_INF_FlancGarder");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_OrganiserUnCheckPoint>( "Pion INF OrganiserUnCheckPoint", eMission_Pion_INF_OrganiserUnCheckPoint, T_Mission_Pion_mission_pion_inf_organiser_un_check_point, "T_Mission_Pion_INF_OrganiserUnCheckPoint", "MIS_Pion_INF_OrganiserUnCheckPoint");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Harceler>( "Pion INF Harceler", eMission_Pion_INF_Harceler, T_Mission_Pion_mission_pion_inf_harceler, "T_Mission_Pion_INF_Harceler", "MIS_Pion_INF_Harceler");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_DetruireParCoupDeMain>( "Pion INF DetruireParCoupDeMain", eMission_Pion_INF_DetruireParCoupDeMain, T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main, "T_Mission_Pion_INF_DetruireParCoupDeMain", "MIS_Pion_INF_DetruireParCoupDeMain");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_EscorterUnConvoi>( "Pion INF EscorterUnConvoi", eMission_Pion_INF_EscorterUnConvoi, T_Mission_Pion_mission_pion_inf_escorter_un_convoi, "T_Mission_Pion_INF_EscorterUnConvoi", "MIS_Pion_INF_EscorterUnConvoi");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_RenseignerSur>( "Pion INF RenseignerSur", eMission_Pion_INF_RenseignerSur, T_Mission_Pion_mission_pion_inf_renseigner_sur, "T_Mission_Pion_INF_RenseignerSur", "MIS_Pion_INF_RenseignerSur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Soutenir>( "Pion INF Soutenir", eMission_Pion_INF_Soutenir, T_Mission_Pion_mission_pion_inf_soutenir, "T_Mission_Pion_INF_Soutenir", "MIS_Pion_INF_Soutenir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Securiser>( "Pion INF Securiser", eMission_Pion_INF_Securiser, T_Mission_Pion_mission_pion_inf_securiser, "T_Mission_Pion_INF_Securiser", "MIS_Pion_INF_Securiser");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_Recueillir>( "Pion INF Recueillir", eMission_Pion_INF_Recueillir, T_Mission_Pion_mission_pion_inf_recueillir, "T_Mission_Pion_INF_Recueillir", "MIS_Pion_INF_Recueillir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_INF_RepondreAAggression>( "Pion INF RepondreAAggression", eMission_Pion_INF_RepondreAAggression, T_Mission_Pion_mission_pion_inf_repondre_a_aggression, "T_Mission_Pion_INF_RepondreAAggression", "MIS_Pion_INF_RepondreAAggression");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_SeDeployer>( "Pion LOG SeDeployer", eMission_Pion_LOG_SeDeployer, T_Mission_Pion_mission_pion_log_se_deployer, "T_Mission_Pion_LOG_SeDeployer", "MIS_Pion_LOG_SeDeployer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_DistribuerMoyens>( "Pion LOG DistribuerMoyens", eMission_Pion_LOG_DistribuerMoyens, T_Mission_Pion_mission_pion_log_distribuer_moyens, "T_Mission_Pion_LOG_DistribuerMoyens", "MIS_Pion_LOG_DistribuerMoyens");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_TrierBlesses>( "Pion LOG TrierBlesses", eMission_Pion_LOG_TrierBlesses, T_Mission_Pion_mission_pion_log_trier_blesses, "T_Mission_Pion_LOG_TrierBlesses", "MIS_Pion_LOG_TrierBlesses");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_TraiterBlesses>( "Pion LOG TraiterBlesses", eMission_Pion_LOG_TraiterBlesses, T_Mission_Pion_mission_pion_log_traiter_blesses, "T_Mission_Pion_LOG_TraiterBlesses", "MIS_Pion_LOG_TraiterBlesses");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_TrierEtTraiterBlesses>( "Pion LOG TrierEtTraiterBlesses", eMission_Pion_LOG_TrierEtTraiterBlesses, T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses, "T_Mission_Pion_LOG_TrierEtTraiterBlesses", "MIS_Pion_LOG_TrierEtTraiterBlesses");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_SoignerBlesses>( "Pion LOG SoignerBlesses", eMission_Pion_LOG_SoignerBlesses, T_Mission_Pion_mission_pion_log_soigner_blesses, "T_Mission_Pion_LOG_SoignerBlesses", "MIS_Pion_LOG_SoignerBlesses");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_PrendreEnCompteReactionsMentales>( "Pion LOG PrendreEnCompteReactionsMentales", eMission_Pion_LOG_PrendreEnCompteReactionsMentales, T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales, "T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales", "MIS_Pion_LOG_PrendreEnCompteReactionsMentales");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_DecontaminerBlesses>( "Pion LOG DecontaminerBlesses", eMission_Pion_LOG_DecontaminerBlesses, T_Mission_Pion_mission_pion_log_decontaminer_blesses, "T_Mission_Pion_LOG_DecontaminerBlesses", "MIS_Pion_LOG_DecontaminerBlesses");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_Reparer>( "Pion LOG Reparer", eMission_Pion_LOG_Reparer, T_Mission_Pion_mission_pion_log_reparer, "T_Mission_Pion_LOG_Reparer", "MIS_Pion_LOG_Reparer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_Evacuer>( "Pion LOG Evacuer", eMission_Pion_LOG_Evacuer, T_Mission_Pion_mission_pion_log_evacuer, "T_Mission_Pion_LOG_Evacuer", "MIS_Pion_LOG_Evacuer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_Convoyer>( "Pion LOG Convoyer", eMission_Pion_LOG_Convoyer, T_Mission_Pion_mission_pion_log_convoyer, "T_Mission_Pion_LOG_Convoyer", "MIS_Pion_LOG_Convoyer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_DistribuerRessources>( "Pion LOG DistribuerRessources", eMission_Pion_LOG_DistribuerRessources, T_Mission_Pion_mission_pion_log_distribuer_ressources, "T_Mission_Pion_LOG_DistribuerRessources", "MIS_Pion_LOG_DistribuerRessources");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_ConstituerConvoi>( "Pion LOG ConstituerConvoi", eMission_Pion_LOG_ConstituerConvoi, T_Mission_Pion_mission_pion_log_constituer_convoi, "T_Mission_Pion_LOG_ConstituerConvoi", "MIS_Pion_LOG_ConstituerConvoi");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_Livrer>( "Pion LOG Livrer", eMission_Pion_LOG_Livrer, T_Mission_Pion_mission_pion_log_livrer, "T_Mission_Pion_LOG_Livrer", "MIS_Pion_LOG_Livrer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_PreparerBascule>( "Pion LOG PreparerBascule", eMission_Pion_LOG_PreparerBascule, T_Mission_Pion_mission_pion_log_preparer_bascule, "T_Mission_Pion_LOG_PreparerBascule", "MIS_Pion_LOG_PreparerBascule");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_AppuyerMouvement>( "Pion LOG AppuyerMouvement", eMission_Pion_LOG_AppuyerMouvement, T_Mission_Pion_mission_pion_log_appuyer_mouvement, "T_Mission_Pion_LOG_AppuyerMouvement", "MIS_Pion_LOG_AppuyerMouvement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_ReconnaitreItineraire>( "Pion LOG ReconnaitreItineraire", eMission_Pion_LOG_ReconnaitreItineraire, T_Mission_Pion_mission_pion_log_reconnaitre_itineraire, "T_Mission_Pion_LOG_ReconnaitreItineraire", "MIS_Pion_LOG_ReconnaitreItineraire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_TransporterUniteChars>( "Pion LOG TransporterUniteChars", eMission_Pion_LOG_TransporterUniteChars, T_Mission_Pion_mission_pion_log_transporter_unite_chars, "T_Mission_Pion_LOG_TransporterUniteChars", "MIS_Pion_LOG_TransporterUniteChars");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_ReconnaitreZoneContaminee>( "Pion LOG ReconnaitreZoneContaminee", eMission_Pion_LOG_ReconnaitreZoneContaminee, T_Mission_Pion_mission_pion_log_reconnaitre_zone_contaminee, "T_Mission_Pion_LOG_ReconnaitreZoneContaminee", "MIS_Pion_LOG_ReconnaitreZoneContaminee");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_AppuyerMouvementDansZone>( "Pion LOG AppuyerMouvementDansZone", eMission_Pion_LOG_AppuyerMouvementDansZone, T_Mission_Pion_mission_pion_log_appuyer_mouvement_dans_zone, "T_Mission_Pion_LOG_AppuyerMouvementDansZone", "MIS_Pion_LOG_AppuyerMouvementDansZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_MettreEnOeuvreZoneStationnement>( "Pion LOG MettreEnOeuvreZoneStationnement", eMission_Pion_LOG_MettreEnOeuvreZoneStationnement, T_Mission_Pion_mission_pion_log_mettre_en_oeuvre_zone_stationnement, "T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement", "MIS_Pion_LOG_MettreEnOeuvreZoneStationnement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_AppuyerFranchissementSurUnPoint>( "Pion LOG AppuyerFranchissementSurUnPoint", eMission_Pion_LOG_AppuyerFranchissementSurUnPoint, T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point, "T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint", "MIS_Pion_LOG_AppuyerFranchissementSurUnPoint");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_BloquerZone>( "Pion LOG BloquerZone", eMission_Pion_LOG_BloquerZone, T_Mission_Pion_mission_pion_log_bloquer_zone, "T_Mission_Pion_LOG_BloquerZone", "MIS_Pion_LOG_BloquerZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_ReconnaitreZoneDeDeploiement>( "Pion LOG ReconnaitreZoneDeDeploiement", eMission_Pion_LOG_ReconnaitreZoneDeDeploiement, T_Mission_Pion_mission_pion_log_reconnaitre_zone_de_deploiement, "T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement", "MIS_Pion_LOG_ReconnaitreZoneDeDeploiement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_ArmerPIA>( "Pion LOG ArmerPIA", eMission_Pion_LOG_ArmerPIA, T_Mission_Pion_mission_pion_log_armer_pia, "T_Mission_Pion_LOG_ArmerPIA", "MIS_Pion_LOG_ArmerPIA");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_LOG_Surveiller>( "Pion LOG Surveiller", eMission_Pion_LOG_Surveiller, T_Mission_Pion_mission_pion_log_surveiller, "T_Mission_Pion_LOG_Surveiller", "MIS_Pion_LOG_Surveiller");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_Heliporter>( "Pion Test Heliporter", eMission_Pion_Test_Heliporter, T_Mission_Pion_mission_pion_test_heliporter, "T_Mission_Pion_Test_Heliporter", "MIS_Pion_Test_Heliporter");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_MoveTo>( "Pion Test MoveTo", eMission_Pion_Test_MoveTo, T_Mission_Pion_mission_pion_test_move_to, "T_Mission_Pion_Test_MoveTo", "MIS_Pion_Test_MoveTo");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_CreateObject>( "Pion Test CreateObject", eMission_Pion_Test_CreateObject, T_Mission_Pion_mission_pion_test_create_object, "T_Mission_Pion_Test_CreateObject", "MIS_Pion_Test_CreateObject");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_DestroyObject>( "Pion Test DestroyObject", eMission_Pion_Test_DestroyObject, T_Mission_Pion_mission_pion_test_destroy_object, "T_Mission_Pion_Test_DestroyObject", "MIS_Pion_Test_DestroyObject");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_CreateBypass>( "Pion Test CreateBypass", eMission_Pion_Test_CreateBypass, T_Mission_Pion_mission_pion_test_create_bypass, "T_Mission_Pion_Test_CreateBypass", "MIS_Pion_Test_CreateBypass");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_Fire>( "Pion Test Fire", eMission_Pion_Test_Fire, T_Mission_Pion_mission_pion_test_fire, "T_Mission_Pion_Test_Fire", "MIS_Pion_Test_Fire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_ChangePosture>( "Pion Test ChangePosture", eMission_Pion_Test_ChangePosture, T_Mission_Pion_mission_pion_test_change_posture, "T_Mission_Pion_Test_ChangePosture", "MIS_Pion_Test_ChangePosture");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_PathFind>( "Pion Test PathFind", eMission_Pion_Test_PathFind, T_Mission_Pion_mission_pion_test_path_find, "T_Mission_Pion_Test_PathFind", "MIS_Pion_Test_PathFind");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_FindPosition>( "Pion Test FindPosition", eMission_Pion_Test_FindPosition, T_Mission_Pion_mission_pion_test_find_position, "T_Mission_Pion_Test_FindPosition", "MIS_Pion_Test_FindPosition");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Test_Reinforce>( "Pion Test Reinforce", eMission_Pion_Test_Reinforce, T_Mission_Pion_mission_pion_test_reinforce, "T_Mission_Pion_Test_Reinforce", "MIS_Pion_Test_Reinforce");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_PlastronEnnemi>( "Pion PlastronEnnemi", eMission_Pion_PlastronEnnemi, T_Mission_Pion_mission_pion_plastron_ennemi, "T_Mission_Pion_PlastronEnnemi", "MIS_Pion_PlastronEnnemi");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_FaireMouvement>( "Pion FaireMouvement", eMission_Pion_FaireMouvement, T_Mission_Pion_mission_pion_faire_mouvement, "T_Mission_Pion_FaireMouvement", "MIS_Pion_FaireMouvement");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Suivre>( "Pion Suivre", eMission_Pion_Suivre, T_Mission_Pion_mission_pion_suivre, "T_Mission_Pion_Suivre", "MIS_Pion_Suivre");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_SeFaireDecontaminer>( "Pion SeFaireDecontaminer", eMission_Pion_SeFaireDecontaminer, T_Mission_Pion_mission_pion_se_faire_decontaminer, "T_Mission_Pion_SeFaireDecontaminer", "MIS_Pion_SeFaireDecontaminer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Franchir>( "Pion Franchir", eMission_Pion_Franchir, T_Mission_Pion_mission_pion_franchir, "T_Mission_Pion_Franchir", "MIS_Pion_Franchir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Decrocher>( "Pion Decrocher", eMission_Pion_Decrocher, T_Mission_Pion_mission_pion_decrocher, "T_Mission_Pion_Decrocher", "MIS_Pion_Decrocher");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Stationner>( "Pion Stationner", eMission_Pion_Stationner, T_Mission_Pion_mission_pion_stationner, "T_Mission_Pion_Stationner", "MIS_Pion_Stationner");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_SeFaireTransporter>( "Pion SeFaireTransporter", eMission_Pion_SeFaireTransporter, T_Mission_Pion_mission_pion_se_faire_transporter, "T_Mission_Pion_SeFaireTransporter", "MIS_Pion_SeFaireTransporter");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_InterdireFranchissementPopulation>( "Pion InterdireFranchissementPopulation", eMission_Pion_InterdireFranchissementPopulation, T_Mission_Pion_mission_pion_interdire_franchissement_population, "T_Mission_Pion_InterdireFranchissementPopulation", "MIS_Pion_InterdireFranchissementPopulation");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_FilterPopulation>( "Pion FilterPopulation", eMission_Pion_FilterPopulation, T_Mission_Pion_mission_pion_filter_population, "T_Mission_Pion_FilterPopulation", "MIS_Pion_FilterPopulation");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RenseignerSurPopulation>( "Pion RenseignerSurPopulation", eMission_Pion_RenseignerSurPopulation, T_Mission_Pion_mission_pion_renseigner_sur_population, "T_Mission_Pion_RenseignerSurPopulation", "MIS_Pion_RenseignerSurPopulation");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_CanaliserPopulation>( "Pion CanaliserPopulation", eMission_Pion_CanaliserPopulation, T_Mission_Pion_mission_pion_canaliser_population, "T_Mission_Pion_CanaliserPopulation", "MIS_Pion_CanaliserPopulation");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_SecuriserPopulationDansZone>( "Pion SecuriserPopulationDansZone", eMission_Pion_SecuriserPopulationDansZone, T_Mission_Pion_mission_pion_securiser_population_dans_zone, "T_Mission_Pion_SecuriserPopulationDansZone", "MIS_Pion_SecuriserPopulationDansZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_SecuriserZoneContreMenacesAsymetriques>( "Pion SecuriserZoneContreMenacesAsymetriques", eMission_Pion_SecuriserZoneContreMenacesAsymetriques, T_Mission_Pion_mission_pion_securiser_zone_contre_menaces_asymetriques, "T_Mission_Pion_SecuriserZoneContreMenacesAsymetriques", "MIS_Pion_SecuriserZoneContreMenacesAsymetriques");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_Generique>( "Pion Generique", eMission_Pion_Generique, T_Mission_Pion_mission_pion_generique, "T_Mission_Pion_Generique", "MIS_Pion_Generique");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_MILICE_Provoquer>( "Pion MILICE Provoquer", eMission_Pion_MILICE_Provoquer, T_Mission_Pion_mission_pion_milice_provoquer, "T_Mission_Pion_MILICE_Provoquer", "MIS_Pion_MILICE_Provoquer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_MILICE_PrendreEtTenir>( "Pion MILICE PrendreEtTenir", eMission_Pion_MILICE_PrendreEtTenir, T_Mission_Pion_mission_pion_milice_prendre_et_tenir, "T_Mission_Pion_MILICE_PrendreEtTenir", "MIS_Pion_MILICE_PrendreEtTenir");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_MILICE_DetruireEmbuscade>( "Pion MILICE DetruireEmbuscade", eMission_Pion_MILICE_DetruireEmbuscade, T_Mission_Pion_mission_pion_milice_detruire_embuscade, "T_Mission_Pion_MILICE_DetruireEmbuscade", "MIS_Pion_MILICE_DetruireEmbuscade");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_JOINT_MARINE_Transporter>( "Pion JOINT MARINE Transporter", eMission_Pion_JOINT_MARINE_Transporter, T_Mission_Pion_mission_pion_joint_marine_transporter, "T_Mission_Pion_JOINT_MARINE_Transporter", "MIS_Pion_JOINT_MARINE_Transporter");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_JOINT_AIR_EffectuerRaid>( "Pion JOINT AIR EffectuerRaid", eMission_Pion_JOINT_AIR_EffectuerRaid, T_Mission_Pion_mission_pion_joint_air_effectuer_raid, "T_Mission_Pion_JOINT_AIR_EffectuerRaid", "MIS_Pion_JOINT_AIR_EffectuerRaid");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_NBC_AnimerUnPlotDeDecontamination>( "Pion NBC AnimerUnPlotDeDecontamination", eMission_Pion_NBC_AnimerUnPlotDeDecontamination, T_Mission_Pion_mission_pion_nbc_animer_un_plot_de_decontamination, "T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination", "MIS_Pion_NBC_AnimerUnPlotDeDecontamination");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_NBC_ReconnaitreItineraire>( "Pion NBC ReconnaitreItineraire", eMission_Pion_NBC_ReconnaitreItineraire, T_Mission_Pion_mission_pion_nbc_reconnaitre_itineraire, "T_Mission_Pion_NBC_ReconnaitreItineraire", "MIS_Pion_NBC_ReconnaitreItineraire");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_NBC_ReconnaitreZone>( "Pion NBC ReconnaitreZone", eMission_Pion_NBC_ReconnaitreZone, T_Mission_Pion_mission_pion_nbc_reconnaitre_zone, "T_Mission_Pion_NBC_ReconnaitreZone", "MIS_Pion_NBC_ReconnaitreZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_NBC_DecontaminerZone>( "Pion NBC DecontaminerZone", eMission_Pion_NBC_DecontaminerZone, T_Mission_Pion_mission_pion_nbc_decontaminer_zone, "T_Mission_Pion_NBC_DecontaminerZone", "MIS_Pion_NBC_DecontaminerZone");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_NBC_ReconnaitreUnSiteROTA>( "Pion NBC ReconnaitreUnSiteROTA", eMission_Pion_NBC_ReconnaitreUnSiteROTA, T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota, "T_Mission_Pion_NBC_ReconnaitreUnSiteROTA", "MIS_Pion_NBC_ReconnaitreUnSiteROTA");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROHUM_RenseignerSur>( "Pion RENS ROHUM RenseignerSur", eMission_Pion_RENS_ROHUM_RenseignerSur, T_Mission_Pion_mission_pion_rens_rohum_renseigner_sur, "T_Mission_Pion_RENS_ROHUM_RenseignerSur", "MIS_Pion_RENS_ROHUM_RenseignerSur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROHUM_SExfiltrer>( "Pion RENS ROHUM SExfiltrer", eMission_Pion_RENS_ROHUM_SExfiltrer, T_Mission_Pion_mission_pion_rens_rohum_sexfiltrer, "T_Mission_Pion_RENS_ROHUM_SExfiltrer", "MIS_Pion_RENS_ROHUM_SExfiltrer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROHUM_OrienterGuider>( "Pion RENS ROHUM OrienterGuider", eMission_Pion_RENS_ROHUM_OrienterGuider, T_Mission_Pion_mission_pion_rens_rohum_orienter_guider, "T_Mission_Pion_RENS_ROHUM_OrienterGuider", "MIS_Pion_RENS_ROHUM_OrienterGuider");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROEM_Appuyer>( "Pion RENS ROEM Appuyer", eMission_Pion_RENS_ROEM_Appuyer, T_Mission_Pion_mission_pion_rens_roem_appuyer, "T_Mission_Pion_RENS_ROEM_Appuyer", "MIS_Pion_RENS_ROEM_Appuyer");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROEM_RenseignerSur>( "Pion RENS ROEM RenseignerSur", eMission_Pion_RENS_ROEM_RenseignerSur, T_Mission_Pion_mission_pion_rens_roem_renseigner_sur, "T_Mission_Pion_RENS_ROEM_RenseignerSur", "MIS_Pion_RENS_ROEM_RenseignerSur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROEM_MettreEnOeuvre>( "Pion RENS ROEM MettreEnOeuvre", eMission_Pion_RENS_ROEM_MettreEnOeuvre, T_Mission_Pion_mission_pion_rens_roem_mettre_en_oeuvre, "T_Mission_Pion_RENS_ROEM_MettreEnOeuvre", "MIS_Pion_RENS_ROEM_MettreEnOeuvre");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_RADINT_RenseignerSur>( "Pion RENS RADINT RenseignerSur", eMission_Pion_RENS_RADINT_RenseignerSur, T_Mission_Pion_mission_pion_rens_radint_renseigner_sur, "T_Mission_Pion_RENS_RADINT_RenseignerSur", "MIS_Pion_RENS_RADINT_RenseignerSur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_RADINT_MettreEnOeuvre>( "Pion RENS RADINT MettreEnOeuvre", eMission_Pion_RENS_RADINT_MettreEnOeuvre, T_Mission_Pion_mission_pion_rens_radint_mettre_en_oeuvre, "T_Mission_Pion_RENS_RADINT_MettreEnOeuvre", "MIS_Pion_RENS_RADINT_MettreEnOeuvre");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROIM_MettreEnOeuvre>( "Pion RENS ROIM MettreEnOeuvre", eMission_Pion_RENS_ROIM_MettreEnOeuvre, T_Mission_Pion_mission_pion_rens_roim_mettre_en_oeuvre, "T_Mission_Pion_RENS_ROIM_MettreEnOeuvre", "MIS_Pion_RENS_ROIM_MettreEnOeuvre");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur>( "Pion RENS ROIM SDTI RenseignerSur", eMission_Pion_RENS_ROIM_SDTI_RenseignerSur, T_Mission_Pion_mission_pion_rens_roim_sdti_renseigner_sur, "T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur", "MIS_Pion_RENS_ROIM_SDTI_RenseignerSur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_RENS_ROIM_CL289_RenseignerSur>( "Pion RENS ROIM CL289 RenseignerSur", eMission_Pion_RENS_ROIM_CL289_RenseignerSur, T_Mission_Pion_mission_pion_rens_roim_cl289_renseigner_sur, "T_Mission_Pion_RENS_ROIM_CL289_RenseignerSur", "MIS_Pion_RENS_ROIM_CL289_RenseignerSur");
    MIL_PionMissionType::RegisterMission< MIL_PionMission_TRANS_EtablirGererLiaison>( "Pion TRANS EtablirGererLiaison", eMission_Pion_TRANS_EtablirGererLiaison, T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison, "T_Mission_Pion_TRANS_EtablirGererLiaison", "MIS_Pion_TRANS_EtablirGererLiaison");

}

// -----------------------------------------------------------------------------
// Name: RegisterOrderConduites
// Created: AGR 
// -----------------------------------------------------------------------------
void missions::RegisterOrderConduites()
{
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_MettreTenueNBC>( "MettreTenueNBC", eOrdreConduite_MettreTenueNBC, T_MsgOrderConduite_order_conduite_order_conduite_mettre_tenue_nbc, "Rep_OrderConduite_MettreTenueNBC", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_EnleverTenueNBC>( "EnleverTenueNBC", eOrdreConduite_EnleverTenueNBC, T_MsgOrderConduite_order_conduite_order_conduite_enlever_tenue_nbc, "Rep_OrderConduite_EnleverTenueNBC", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_PasserEnSilenceRadio>( "PasserEnSilenceRadio", eOrdreConduite_PasserEnSilenceRadio, T_MsgOrderConduite_order_conduite_order_conduite_passer_en_silence_radio, "Rep_OrderConduite_PasserEnSilenceRadio", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ArreterSilenceRadio>( "ArreterSilenceRadio", eOrdreConduite_ArreterSilenceRadio, T_MsgOrderConduite_order_conduite_order_conduite_arreter_silence_radio, "Rep_OrderConduite_ArreterSilenceRadio", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_PasserEnSilenceRadar>( "PasserEnSilenceRadar", eOrdreConduite_PasserEnSilenceRadar, T_MsgOrderConduite_order_conduite_order_conduite_passer_en_silence_radar, "Rep_OrderConduite_PasserEnSilenceRadar", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ArreterSilenceRadar>( "ArreterSilenceRadar", eOrdreConduite_ArreterSilenceRadar, T_MsgOrderConduite_order_conduite_order_conduite_arreter_silence_radar, "Rep_OrderConduite_ArreterSilenceRadar", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_RecupererTransporteurs>( "RecupererTransporteurs", eOrdreConduite_RecupererTransporteurs, T_MsgOrderConduite_order_conduite_order_conduite_recuperer_transporteurs, "Rep_OrderConduite_RecupererTransporteurs", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_AttendreSePoster>( "AttendreSePoster", eOrdreConduite_AttendreSePoster, T_MsgOrderConduite_order_conduite_order_conduite_attendre_se_poster, "Rep_OrderConduite_AttendreSePoster", true, false, T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_attendre_se_poster );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Poursuivre>( "Poursuivre", eOrdreConduite_Poursuivre, T_MsgOrderConduite_order_conduite_order_conduite_poursuivre, "Rep_OrderConduite_Poursuivre", true, false, T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_poursuivre );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Interrompre>( "Interrompre", eOrdreConduite_Interrompre, T_MsgOrderConduite_order_conduite_order_conduite_interrompre, "Rep_OrderConduite_Interrompre", true, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ChangerReglesEngagement>( "ChangerReglesEngagement", eOrdreConduite_ChangerReglesEngagement, T_MsgOrderConduite_order_conduite_order_conduite_changer_regles_engagement, "Rep_OrderConduite_ChangerReglesEngagement", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ChangerReglesEngagementPopulation>( "ChangerReglesEngagementPopulation", eOrdreConduite_ChangerReglesEngagementPopulation, T_MsgOrderConduite_order_conduite_order_conduite_changer_regles_engagement_population, "Rep_OrderConduite_ChangerReglesEngagementPopulation", true, true );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Deboucher>( "Deboucher", eOrdreConduite_Deboucher, T_MsgOrderConduite_order_conduite_order_conduite_deboucher, "Rep_OrderConduite_Deboucher", false, false, T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_deboucher );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Decrocher>( "Decrocher", eOrdreConduite_Decrocher, T_MsgOrderConduite_order_conduite_order_conduite_decrocher, "Rep_OrderConduite_Decrocher", false, false, T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_decrocher );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_AcquerirObjectif>( "AcquerirObjectif", eOrdreConduite_AcquerirObjectif, T_MsgOrderConduite_order_conduite_order_conduite_acquerir_objectif, "Rep_OrderConduite_AcquerirObjectif", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Brouiller>( "Brouiller", eOrdreConduite_Brouiller, T_MsgOrderConduite_order_conduite_order_conduite_brouiller, "Rep_OrderConduite_Brouiller", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ChangerPositionDebarquement>( "ChangerPositionDebarquement", eOrdreConduite_ChangerPositionDebarquement, T_MsgOrderConduite_order_conduite_order_conduite_changer_position_debarquement, "Rep_OrderConduite_ChangerPositionDebarquement", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_ChangerDePosition>( "Pion ChangerDePosition", eOrdreConduite_Pion_ChangerDePosition, T_MsgOrderConduite_order_conduite_order_conduite_pion_changer_de_position, "Rep_OrderConduite_Pion_ChangerDePosition", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_Contourner>( "Pion Contourner", eOrdreConduite_Pion_Contourner, T_MsgOrderConduite_order_conduite_order_conduite_pion_contourner, "Rep_OrderConduite_Pion_Contourner", false, false, T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_contourner );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_EnvoyerVehicule>( "Pion EnvoyerVehicule", eOrdreConduite_Pion_EnvoyerVehicule, T_MsgOrderConduite_order_conduite_order_conduite_pion_envoyer_vehicule, "Rep_OrderConduite_Pion_EnvoyerVehicule", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_ActiverObstacle>( "Pion ActiverObstacle", eOrdreConduite_Pion_ActiverObstacle, T_MsgOrderConduite_order_conduite_order_conduite_pion_activer_obstacle, "Rep_OrderConduite_Pion_ActiverObstacle", false, false, T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_activer_obstacle );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_AppliquerFeux>( "Pion AppliquerFeux", eOrdreConduite_Pion_AppliquerFeux, T_MsgOrderConduite_order_conduite_order_conduite_pion_appliquer_feux, "Rep_OrderConduite_Pion_AppliquerFeux", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_ExecuterTir>( "Pion ExecuterTir", eOrdreConduite_Pion_ExecuterTir, T_MsgOrderConduite_order_conduite_order_conduite_pion_executer_tir, "Rep_OrderConduite_Pion_ExecuterTir", false, false, T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_executer_tir );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_Emettre>( "Pion Emettre", eOrdreConduite_Pion_Emettre, T_MsgOrderConduite_order_conduite_order_conduite_pion_emettre, "Rep_OrderConduite_Pion_Emettre", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_StopperEmission>( "Pion StopperEmission", eOrdreConduite_Pion_StopperEmission, T_MsgOrderConduite_order_conduite_order_conduite_pion_stopper_emission, "Rep_OrderConduite_Pion_StopperEmission", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_RenforcerEnVSRAM>( "Pion RenforcerEnVSRAM", eOrdreConduite_Pion_RenforcerEnVSRAM, T_MsgOrderConduite_order_conduite_order_conduite_pion_renforcer_en_vs_ram, "Rep_OrderConduite_Pion_RenforcerEnVSRAM", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_TransfererVSRAM>( "Pion TransfererVSRAM", eOrdreConduite_Pion_TransfererVSRAM, T_MsgOrderConduite_order_conduite_order_conduite_pion_transferer_vs_ram, "Rep_OrderConduite_Pion_TransfererVSRAM", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM>( "Pion ReprendreAuxOrdresVSRAM", eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM, T_MsgOrderConduite_order_conduite_order_conduite_pion_reprendre_aux_ordres_vs_ram, "Rep_OrderConduite_Pion_ReprendreAuxOrdresVSRAM", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ModifierTempsBordeeMaintenance>( "ModifierTempsBordeeMaintenance", eOrdreConduite_ModifierTempsBordeeMaintenance, T_MsgOrderConduite_order_conduite_order_conduite_modifier_temps_bordee_maintenance, "Rep_OrderConduite_ModifierTempsBordeeMaintenance", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ModifierTempsBordeeSante>( "ModifierTempsBordeeSante", eOrdreConduite_ModifierTempsBordeeSante, T_MsgOrderConduite_order_conduite_order_conduite_modifier_temps_bordee_sante, "Rep_OrderConduite_ModifierTempsBordeeSante", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ModifierPrioritesReparations>( "ModifierPrioritesReparations", eOrdreConduite_ModifierPrioritesReparations, T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_reparations, "Rep_OrderConduite_ModifierPrioritesReparations", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ModifierPrioritesBlesses>( "ModifierPrioritesBlesses", eOrdreConduite_ModifierPrioritesBlesses, T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_blesses, "Rep_OrderConduite_ModifierPrioritesBlesses", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ModifierPrioritesTactiquesReparations>( "ModifierPrioritesTactiquesReparations", eOrdreConduite_ModifierPrioritesTactiquesReparations, T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_tactiques_reparations, "Rep_OrderConduite_ModifierPrioritesTactiquesReparations", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_ModifierPrioritesTactiquesBlesses>( "ModifierPrioritesTactiquesBlesses", eOrdreConduite_ModifierPrioritesTactiquesBlesses, T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_tactiques_blesses, "Rep_OrderConduite_ModifierPrioritesTactiquesBlesses", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Automate_RealiserVariantement>( "Automate RealiserVariantement", eOrdreConduite_Automate_RealiserVariantement, T_MsgOrderConduite_order_conduite_order_conduite_automate_realiser_variantement, "Rep_OrderConduite_Automate_RealiserVariantement", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Automate_DesequiperSiteFranchissement>( "Automate DesequiperSiteFranchissement", eOrdreConduite_Automate_DesequiperSiteFranchissement, T_MsgOrderConduite_order_conduite_order_conduite_automate_desequiper_site_franchissement, "Rep_OrderConduite_Automate_DesequiperSiteFranchissement", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Automate_ReagirFaceAEni>( "Automate ReagirFaceAEni", eOrdreConduite_Automate_ReagirFaceAEni, T_MsgOrderConduite_order_conduite_order_conduite_automate_reagir_face_a_eni, "Rep_OrderConduite_Automate_ReagirFaceAEni", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Automate_AffecterPionAObstacle>( "Automate AffecterPionAObstacle", eOrdreConduite_Automate_AffecterPionAObstacle, T_MsgOrderConduite_order_conduite_order_conduite_automate_affecter_pion_a_obstacle, "Rep_OrderConduite_Automate_AffecterPionAObstacle", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Automate_ReconnaitreZoneImplantation>( "Automate ReconnaitreZoneImplantation", eOrdreConduite_Automate_ReconnaitreZoneImplantation, T_MsgOrderConduite_order_conduite_order_conduite_automate_reconnaitre_zone_implantation, "Rep_OrderConduite_Automate_ReconnaitreZoneImplantation", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement>( "Automate TC2 GererMaterielAvantDeplacement", eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement, T_MsgOrderConduite_order_conduite_order_conduite_automate_tc2_gerer_materiel_avant_deplacement, "Rep_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Automate_ActiverObstacle>( "Automate ActiverObstacle", eOrdreConduite_Automate_ActiverObstacle, T_MsgOrderConduite_order_conduite_order_conduite_automate_activer_obstacle, "Rep_OrderConduite_Automate_ActiverObstacle", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Automate_TransfererRenforts>( "Automate TransfererRenforts", eOrdreConduite_Automate_TransfererRenforts, T_MsgOrderConduite_order_conduite_order_conduite_automate_transferer_renforts, "Rep_OrderConduite_Automate_TransfererRenforts", false, false );
    MIL_OrderConduiteType::RegisterOrderConduite< MIL_OrderConduite_Population_ChangerAttitude>( "Population ChangerAttitude", eOrdreConduite_Population_ChangerAttitude, T_MsgOrderConduite_order_conduite_order_conduite_population_changer_attitude, "Rep_OrderConduite_Population_ChangerAttitude", true, true );

}
