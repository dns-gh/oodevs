//*****************************************************************************
//
// $Created: NLD 2003-05-31 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Tools.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 9:59 $
// $Revision: 54 $
// $Workfile: MOS_Tools.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_Tools.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Tools.inl"
#endif


MOS_Tools::T_MissionConverter MOS_Tools::missionConverter_[] =
{
T_MissionConverter( "Automate ALAT Heliporter", eMission_Automate_ALAT_Heliporter, "Heliporter" ),
T_MissionConverter( "Automate ALAT Jalonner", eMission_Automate_ALAT_Jalonner, "Jalonner" ),
T_MissionConverter( "Automate ALAT Helitransporter", eMission_Automate_ALAT_Helitransporter, "Helitransporter" ),
T_MissionConverter( "Automate ALAT ExtrairePersonnel", eMission_Automate_ALAT_ExtrairePersonnel, "ExtrairePersonnel" ),
T_MissionConverter( "Automate ALAT Appuyer", eMission_Automate_ALAT_Appuyer, "Appuyer" ),
T_MissionConverter( "Automate ALAT Couvrir", eMission_Automate_ALAT_Couvrir, "Couvrir" ),
T_MissionConverter( "Automate ALAT Surveiller", eMission_Automate_ALAT_Surveiller, "Surveiller" ),
T_MissionConverter( "Automate ALAT Reconnaitre", eMission_Automate_ALAT_Reconnaitre, "Reconnaitre" ),
T_MissionConverter( "Automate ALAT Freiner", eMission_Automate_ALAT_Freiner, "Freiner" ),
T_MissionConverter( "Automate ALAT Escorter", eMission_Automate_ALAT_Escorter, "Escorter" ),
T_MissionConverter( "Automate ALAT Attaquer", eMission_Automate_ALAT_Attaquer, "Attaquer" ),
T_MissionConverter( "Automate ALAT RecueillirFormationAeromobile", eMission_Automate_ALAT_RecueillirFormationAeromobile, "RecueillirFormationAeromobile" ),
T_MissionConverter( "Automate ALAT AttaquerFormationAeromobile", eMission_Automate_ALAT_AttaquerFormationAeromobile, "AttaquerFormationAeromobile" ),
T_MissionConverter( "Automate ALAT DonnerCoupArret", eMission_Automate_ALAT_DonnerCoupArret, "DonnerCoupArret" ),
T_MissionConverter( "Automate ALAT DetruireNeutraliserDansProfondeur", eMission_Automate_ALAT_DetruireNeutraliserDansProfondeur, "DetruireNeutraliserDansProfondeur" ),
T_MissionConverter( "Automate ALAT DetruireNeutraliserDansZone", eMission_Automate_ALAT_DetruireNeutraliserDansZone, "DetruireNeutraliserDansZone" ),
T_MissionConverter( "Automate ALAT EffectuerRechercheEtSauvetage", eMission_Automate_ALAT_EffectuerRechercheEtSauvetage, "EffectuerRechercheEtSauvetage" ),
T_MissionConverter( "Automate ASA DefendreSite", eMission_Automate_ASA_DefendreSite, "DefendreSite" ),
T_MissionConverter( "Automate ASA DefendreZone", eMission_Automate_ASA_DefendreZone, "DefendreZone" ),
T_MissionConverter( "Automate ASA DefendreUnites", eMission_Automate_ASA_DefendreUnites, "DefendreUnites" ),
T_MissionConverter( "Automate ASA DefendreItineraire", eMission_Automate_ASA_DefendreItineraire, "DefendreItineraire" ),
T_MissionConverter( "Automate ASA Surveiller", eMission_Automate_ASA_Surveiller, "Surveiller" ),
T_MissionConverter( "Automate ASA MISTRAL DefendreZone", eMission_Automate_ASA_MISTRAL_DefendreZone, "MISTRAL DefendreZone" ),
T_MissionConverter( "Automate ASA MISTRAL DefendreSite", eMission_Automate_ASA_MISTRAL_DefendreSite, "MISTRAL DefendreSite" ),
T_MissionConverter( "Automate ASA MISTRAL Surveiller"  , eMission_Automate_ASA_MISTRAL_Surveiller  , "MISTRAL Surveiller" ),
T_MissionConverter( "Automate ASA HAWK DefendreZone", eMission_Automate_ASA_HAWK_DefendreZone, "HAWK DefendreZone" ),
T_MissionConverter( "Automate ASA HAWK DefendreSite", eMission_Automate_ASA_HAWK_DefendreSite, "HAWK DefendreSite" ),
T_MissionConverter( "Automate ASA HAWK Surveiller"  , eMission_Automate_ASA_HAWK_Surveiller  , "HAWK Surveiller" ),
T_MissionConverter( "Automate ASS SeDeployer", eMission_Automate_ASS_SeDeployer, "SeDeployer" ),
T_MissionConverter( "Automate ASS ReconnaitreZonesDeploiement", eMission_Automate_ASS_ReconnaitreZonesDeploiement, "ReconnaitreZonesDeploiement" ),
T_MissionConverter( "Automate ASS AcquerirObjectifs", eMission_Automate_ASS_AcquerirObjectifs, "AcquerirObjectifs" ),
T_MissionConverter( "Automate ABC ArmerPointDeControle", eMission_Automate_ABC_ArmerPointDeControle, "ArmerPointDeControle" ),
T_MissionConverter( "Automate ABC OrganiserAccueilColonneRefugies", eMission_Automate_ABC_OrganiserAccueilColonneRefugies, "OrganiserAccueilColonneRefugies" ),
T_MissionConverter( "Automate ABC ProtegerPC", eMission_Automate_ABC_ProtegerPC, "ProtegerPC" ),
T_MissionConverter( "Automate ABC Appuyer", eMission_Automate_ABC_Appuyer, "Appuyer" ),
T_MissionConverter( "Automate ABC Attaquer", eMission_Automate_ABC_Attaquer, "Attaquer" ),
T_MissionConverter( "Automate ABC ContreAttaquer", eMission_Automate_ABC_ContreAttaquer, "ContreAttaquer" ),
T_MissionConverter( "Automate ABC Controler", eMission_Automate_ABC_Controler, "Controler" ),
T_MissionConverter( "Automate ABC Couvrir", eMission_Automate_ABC_Couvrir, "Couvrir" ),
T_MissionConverter( "Automate ABC DonnerCoupArret", eMission_Automate_ABC_DonnerCoupArret, "DonnerCoupArret" ),
T_MissionConverter( "Automate ABC Eclairer", eMission_Automate_ABC_Eclairer, "Eclairer" ),
T_MissionConverter( "Automate ABC EffectuerRaid", eMission_Automate_ABC_EffectuerRaid, "EffectuerRaid" ),
T_MissionConverter( "Automate ABC Interdire", eMission_Automate_ABC_Interdire, "Interdire" ),
T_MissionConverter( "Automate ABC Jalonner", eMission_Automate_ABC_Jalonner, "Jalonner" ),
T_MissionConverter( "Automate ABC PrendrePreciserContact", eMission_Automate_ABC_PrendrePreciserContact, "PrendrePreciserContact" ),
T_MissionConverter( "Automate ABC Reconnaitre", eMission_Automate_ABC_Reconnaitre, "Reconnaitre" ),
T_MissionConverter( "Automate ABC ReconnaitrePoint", eMission_Automate_ABC_ReconnaitrePoint, "ReconnaitrePoint" ),
T_MissionConverter( "Automate ABC Reduire", eMission_Automate_ABC_Reduire, "Reduire" ),
T_MissionConverter( "Automate ABC ReleverEnDepassant", eMission_Automate_ABC_ReleverEnDepassant, "ReleverEnDepassant" ),
T_MissionConverter( "Automate ABC Semparer", eMission_Automate_ABC_Semparer, "Semparer" ),
T_MissionConverter( "Automate ABC Tenir", eMission_Automate_ABC_Tenir, "Tenir" ),
T_MissionConverter( "Automate ABC Soutenir", eMission_Automate_ABC_Soutenir, "Soutenir" ),
T_MissionConverter( "Automate ABC Surveiller", eMission_Automate_ABC_Surveiller, "Surveiller" ),
T_MissionConverter( "Automate ABC FlancGarder", eMission_Automate_ABC_FlancGarder, "FlancGarder" ),
T_MissionConverter( "Automate ABC RenseignerSur", eMission_Automate_ABC_RenseignerSur, "RenseignerSur" ),
T_MissionConverter( "Automate ABC Fixer", eMission_Automate_ABC_Fixer, "Fixer" ),
T_MissionConverter( "Automate ABC Freiner", eMission_Automate_ABC_Freiner, "Freiner" ),
T_MissionConverter( "Automate ABC SurveillerItineraire", eMission_Automate_ABC_SurveillerItineraire, "SurveillerItineraire" ),
T_MissionConverter( "Automate ABC RenseignerSurUnAxe", eMission_Automate_ABC_RenseignerSurUnAxe, "RenseignerSurUnAxe" ),
T_MissionConverter( "Automate ABC RecueillirUnite", eMission_Automate_ABC_RecueillirUnite, "RecueillirUnite" ),
T_MissionConverter( "Automate ABC EscorterUnConvoi", eMission_Automate_ABC_EscorterUnConvoi, "EscorterUnConvoi" ),
T_MissionConverter( "Automate GEN SeDeployer", eMission_Automate_GEN_SeDeployer, "SeDeployer" ),
T_MissionConverter( "Automate GEN FaireFranchir", eMission_Automate_GEN_FaireFranchir, "FaireFranchir" ),
T_MissionConverter( "Automate GEN RealiserSystemeObstacles", eMission_Automate_GEN_RealiserSystemeObstacles, "RealiserSystemeObstacles" ),
T_MissionConverter( "Automate GEN FranchirDeViveForce", eMission_Automate_GEN_FranchirDeViveForce, "FranchirDeViveForce" ),
T_MissionConverter( "Automate GEN RetablirItineraires", eMission_Automate_GEN_RetablirItineraires, "RetablirItineraires" ),
T_MissionConverter( "Automate GEN AmenagerAireStationnement", eMission_Automate_GEN_AmenagerAireStationnement, "AmenagerAireStationnement" ),
T_MissionConverter( "Automate GEN AmenagerTerrainPoserSommaire", eMission_Automate_GEN_AmenagerTerrainPoserSommaire, "AmenagerTerrainPoserSommaire" ),
T_MissionConverter( "Automate GEN RealiserZonePoser", eMission_Automate_GEN_RealiserZonePoser, "RealiserZonePoser" ),
T_MissionConverter( "Automate GEN ReconnaitreZoneDeploiement", eMission_Automate_GEN_ReconnaitreZoneDeploiement, "ReconnaitreZoneDeploiement" ),
T_MissionConverter( "Automate GEN RealiserCampPrisonniers", eMission_Automate_GEN_RealiserCampPrisonniers, "RealiserCampPrisonniers" ),
T_MissionConverter( "Automate GEN RealiserCampRefugies", eMission_Automate_GEN_RealiserCampRefugies, "RealiserCampRefugies" ),
T_MissionConverter( "Automate GEN RealiserPlatesformesSommairesDeDeploiementInitial", eMission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial, "RealiserPlatesformesSommairesDeDeploiementInitial" ),
T_MissionConverter( "Automate GEN RealiserTravauxSommairesAuProfitPopulation", eMission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation, "RealiserTravauxSommairesAuProfitPopulation" ),
T_MissionConverter( "Automate GEN AppuyerDebarquementTroupes", eMission_Automate_GEN_AppuyerDebarquementTroupes, "AppuyerDebarquementTroupes" ),
T_MissionConverter( "Automate GEN OuvrirAxesProgression", eMission_Automate_GEN_OuvrirAxesProgression, "GEN_OuvrirAxesProgression" ),
T_MissionConverter( "Automate INF AttaquerSouplesse", eMission_Automate_INF_AttaquerSouplesse, "AttaquerSouplesse" ),
T_MissionConverter( "Automate INF Appuyer", eMission_Automate_INF_Appuyer, "Appuyer" ),
T_MissionConverter( "Automate INF Controler", eMission_Automate_INF_Controler, "Controler" ),
T_MissionConverter( "Automate INF Couvrir", eMission_Automate_INF_Couvrir, "Couvrir" ),
T_MissionConverter( "Automate INF DonnerCoupArret", eMission_Automate_INF_DonnerCoupArret, "DonnerCoupArret" ),
T_MissionConverter( "Automate INF Eclairer", eMission_Automate_INF_Eclairer, "Eclairer" ),
T_MissionConverter( "Automate INF FlancGarder", eMission_Automate_INF_FlancGarder, "FlancGarder" ),
T_MissionConverter( "Automate INF Freiner", eMission_Automate_INF_Freiner, "Freiner" ),
T_MissionConverter( "Automate INF Harceler", eMission_Automate_INF_Harceler, "Harceler" ),
T_MissionConverter( "Automate INF Interdire", eMission_Automate_INF_Interdire, "Interdire" ),
T_MissionConverter( "Automate INF MenerDefenseUsure", eMission_Automate_INF_MenerDefenseUsure, "MenerDefenseUsure" ),
T_MissionConverter( "Automate INF PrendrePreciserMaintenirContact", eMission_Automate_INF_PrendrePreciserMaintenirContact, "PrendrePreciserMaintenirContact" ),
T_MissionConverter( "Automate INF ReconnaissanceOffensive", eMission_Automate_INF_ReconnaissanceOffensive, "ReconnaissanceOffensive" ),
T_MissionConverter( "Automate INF RecueillirUnite", eMission_Automate_INF_RecueillirUnite, "RecueillirUnite" ),
T_MissionConverter( "Automate INF Reduire", eMission_Automate_INF_Reduire, "Reduire" ),
T_MissionConverter( "Automate INF Relever", eMission_Automate_INF_Relever, "Relever" ),
T_MissionConverter( "Automate INF RompreContact", eMission_Automate_INF_RompreContact, "RompreContact" ),
T_MissionConverter( "Automate INF Surveiller", eMission_Automate_INF_Surveiller, "Surveiller" ),
T_MissionConverter( "Automate INF Semparer", eMission_Automate_INF_Semparer, "Semparer" ),
T_MissionConverter( "Automate INF Fixer", eMission_Automate_INF_Fixer, "Fixer" ),
T_MissionConverter( "Automate INF AttaquerEnTerrainDifficile", eMission_Automate_INF_AttaquerEnTerrainDifficile, "AttaquerEnTerrainDifficile" ),
T_MissionConverter( "Automate INF BarrerDirection", eMission_Automate_INF_BarrerDirection, "BarrerDirection" ),
T_MissionConverter( "Automate INF DefendreFerme", eMission_Automate_INF_DefendreFerme, "DefendreFerme" ),
T_MissionConverter( "Automate INF Tenir", eMission_Automate_INF_Tenir, "Tenir" ),
T_MissionConverter( "Automate INF AppuyerUnFreinage", eMission_Automate_INF_AppuyerUnFreinage, "AppuyerUnFreinage" ),
T_MissionConverter( "Automate INF AppuyerUneAttaque", eMission_Automate_INF_AppuyerUneAttaque, "AppuyerUneAttaque" ),
T_MissionConverter( "Automate INF Soutenir", eMission_Automate_INF_Soutenir, "Soutenir" ),
T_MissionConverter( "Automate LOG SeDeployer", eMission_Automate_LOG_SeDeployer, "SeDeployer" ),
T_MissionConverter( "Automate LOG AppuyerMouvement"               , eMission_Automate_LOG_AppuyerMouvement                  , "AppuyerMouvement" ),
T_MissionConverter( "Automate LOG ReconnaitreItineraire"          , eMission_Automate_LOG_ReconnaitreItineraire             , "ReconnaitreItineraire" ),
T_MissionConverter( "Automate LOG Surveiller"                     , eMission_Automate_LOG_Surveiller                        , "Surveiller" ),
T_MissionConverter( "Automate LOG TransporterFormationBlindee"    , eMission_Automate_LOG_TransporterFormationBlindee       , "TransporterFormationBlindee" ),
T_MissionConverter( "Automate LOG AppuyerMouvementDansZone"       , eMission_Automate_LOG_AppuyerMouvementDansZone          , "AppuyerMouvementDansZone" ),
T_MissionConverter( "Automate LOG MettreEnOeuvreZoneStationnement", eMission_Automate_LOG_MettreEnOeuvreZoneStationnement   , "MettreEnOeuvreZoneStationnement" ),
T_MissionConverter( "Automate LOG AppuyerFranchissementDansZone"  , eMission_Automate_LOG_AppuyerFranchissementDansZone     , "AppuyerFranchissementDansZone" ),
T_MissionConverter( "Automate LOG BloquerZone"                    , eMission_Automate_LOG_BloquerZone                       , "BloquerZone" ),
T_MissionConverter( "Automate LOG ReconnaitreZoneContaminee"      , eMission_Automate_LOG_ReconnaitreZoneContaminee         , "ReconnaitreZoneContaminee" ),
T_MissionConverter( "Automate LOG ReconnaitreZoneDeDeploiement"   , eMission_Automate_LOG_ReconnaitreZoneDeDeploiement      , "ReconnaitreZoneDeDeploiement" ),
T_MissionConverter( "Automate LOG ArmerPIAs"   , eMission_Automate_LOG_ArmerPIAs, "ArmerPIAs" ),
T_MissionConverter( "Automate Test MoveTo", eMission_Automate_Test_MoveTo, "MoveTo" ),
T_MissionConverter( "Automate Decrocher", eMission_Automate_Decrocher, "Decrocher" ),
T_MissionConverter( "Automate ReleverSurPlace", eMission_Automate_ReleverSurPlace, "ReleverSurPlace" ),
T_MissionConverter( "Automate FaireMouvement", eMission_Automate_FaireMouvement, "FaireMouvement" ),
T_MissionConverter( "Automate SeFaireDecontaminer", eMission_Automate_SeFaireDecontaminer, "SeFaireDecontaminer" ),
T_MissionConverter( "Automate Franchir", eMission_Automate_Franchir, "Franchir" ),
T_MissionConverter( "Automate Stationner", eMission_Automate_Stationner, "Stationner" ),
T_MissionConverter( "Automate SeFaireTransporter", eMission_Automate_SeFaireTransporter, "SeFaireTransporter" ),
T_MissionConverter( "Automate SeRecompleter", eMission_Automate_SeRecompleter, "SeRecompleter" ),
T_MissionConverter( "Automate SeRendre", eMission_Automate_SeRendre, "SeRendre" ),
T_MissionConverter( "Automate FaireMouvementVersCampRefugies", eMission_Automate_FaireMouvementVersCampRefugies, "FaireMouvementVersCampRefugies" ),
T_MissionConverter( "Automate Generique", eMission_Automate_Generique, "Generique" ),
T_MissionConverter( "Automate NBC ReconnaitreUnAxe", eMission_Automate_NBC_ReconnaitreUnAxe, "ReconnaitreUnAxe" ),
T_MissionConverter( "Automate NBC ReconnaitreUneZone", eMission_Automate_NBC_ReconnaitreUneZone, "ReconnaitreUneZone" ),
T_MissionConverter( "Automate NBC ArmerUnSiteDeDecontamination", eMission_Automate_NBC_ArmerUnSiteDeDecontamination, "ArmerUnSiteDeDecontamination" ),
T_MissionConverter( "Automate NBC DecontaminerUneZone", eMission_Automate_NBC_DecontaminerUneZone, "DecontaminerUneZone" ),
T_MissionConverter( "Automate NBC ReconnaitreDesSitesROTA", eMission_Automate_NBC_ReconnaitreDesSitesROTA, "ReconnaitreDesSitesROTA" ),
T_MissionConverter( "Automate RENS ROHUM RenseignerSur", eMission_Automate_RENS_ROHUM_RenseignerSur, "RenseignerSur" ),
T_MissionConverter( "Automate RENS ROHUM OrienterGuider", eMission_Automate_RENS_ROHUM_OrienterGuider, "OrienterGuider" ),
T_MissionConverter( "Automate RENS ROHUM SExfiltrer", eMission_Automate_RENS_ROHUM_SExfiltrer, "SExfiltrer" ),
T_MissionConverter( "Automate RENS ROIM CL289 RenseignerSur", eMission_Automate_RENS_ROIM_CL289_RenseignerSur, "CL289 RenseignerSur" ),
T_MissionConverter( "Automate RENS ROIM SDTI RenseignerSur", eMission_Automate_RENS_ROIM_SDTI_RenseignerSur, "SDTI RenseignerSur" ),
T_MissionConverter( "Automate RENS ROEM Appuyer", eMission_Automate_RENS_ROEM_Appuyer, "Appuyer" ),
T_MissionConverter( "Automate RENS ROEM RenseignerSur", eMission_Automate_RENS_ROEM_RenseignerSur, "ROEM RenseignerSur" ),
T_MissionConverter( "Automate RENS RADINT RenseignerSur", eMission_Automate_RENS_RADINT_RenseignerSur, "RADINT RenseignerSur" ),
T_MissionConverter( "Automate TRANS Commander", eMission_Automate_TRANS_Commander, "Commander" ),
T_MissionConverter( "Pion ALAT EvacuationSanitaire", eMission_Pion_ALAT_EvacuationSanitaire, "EvacuationSanitaire" ),
T_MissionConverter( "Pion ALAT ReconnaitreDansLaProfondeur", eMission_Pion_ALAT_ReconnaitreDansLaProfondeur, "ReconnaitreDansLaProfondeur" ),
T_MissionConverter( "Pion ALAT ReconnaitreContourEnnemi", eMission_Pion_ALAT_ReconnaitreContourEnnemi, "ReconnaitreContourEnnemi" ),
T_MissionConverter( "Pion ALAT DetruireNeutraliserDansZone", eMission_Pion_ALAT_DetruireNeutraliserDansZone, "DetruireNeutraliserDansZone" ),
T_MissionConverter( "Pion ALAT DetruireAeromobile", eMission_Pion_ALAT_DetruireAeromobile, "DetruireAeromobile" ),
T_MissionConverter( "Pion ALAT Freiner", eMission_Pion_ALAT_Freiner, "Freiner" ),
T_MissionConverter( "Pion ALAT Jalonner", eMission_Pion_ALAT_Jalonner, "Jalonner" ),
T_MissionConverter( "Pion ALAT Attaquer", eMission_Pion_ALAT_Attaquer, "Attaquer" ),
T_MissionConverter( "Pion ALAT Escorter", eMission_Pion_ALAT_Escorter, "Escorter" ),
T_MissionConverter( "Pion ALAT Heliporter", eMission_Pion_ALAT_Heliporter, "Heliporter" ),
T_MissionConverter( "Pion ALAT HeliporterHelitransporterPlotRavitaillement", eMission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement, "HeliporterHelitransporterPlotRavitaillement" ),
T_MissionConverter( "Pion ALAT Helitransporter", eMission_Pion_ALAT_Helitransporter, "Helitransporter" ),
T_MissionConverter( "Pion ALAT EffectuerRechercheEtSauvetage", eMission_Pion_ALAT_EffectuerRechercheEtSauvetage, "EffectuerRechercheEtSauvetage" ),
T_MissionConverter( "Pion ALAT IMEX", eMission_Pion_ALAT_IMEX, "IMEX" ),
T_MissionConverter( "Pion ALAT Eclairer", eMission_Pion_ALAT_Eclairer, "Eclairer" ),
T_MissionConverter( "Pion ALAT Surveiller", eMission_Pion_ALAT_Surveiller, "Surveiller" ),
T_MissionConverter( "Pion ALAT AppuyerDirectAuContact", eMission_Pion_ALAT_AppuyerDirectAuContact, "AppuyerDirectAuContact" ),
T_MissionConverter( "Pion ALAT Couvrir", eMission_Pion_ALAT_Couvrir, "Couvrir" ),
T_MissionConverter( "Pion ASA DefendreSite",                eMission_Pion_ASA_DefendreSite,                 "DefendreSite" ),
T_MissionConverter( "Pion ASA DefendreSiteAPartirPosition",  eMission_Pion_ASA_DefendreSiteAPartirPosition,  "DefendreSiteAPartirPosition" ),
T_MissionConverter( "Pion ASA DefendreZone",                eMission_Pion_ASA_DefendreZone,                 "DefendreZone" ),
T_MissionConverter( "Pion ASA DefendreZoneAPartirPosition",  eMission_Pion_ASA_DefendreZoneAPartirPosition,  "DefendreZoneAPartirPosition" ),
T_MissionConverter( "Pion ASA Surveiller",                  eMission_Pion_ASA_Surveiller,                   "Surveiller" ),  
T_MissionConverter( "Pion ASA DefendreUnites",              eMission_Pion_ASA_DefendreUnites,               "DefendreUnites" ),
T_MissionConverter( "Pion ASA MISTRAL DefendreSite",                eMission_Pion_ASA_MISTRAL_DefendreSite,                "DefendreSite" ),
T_MissionConverter( "Pion ASA MISTRAL DefendreSiteAPartirPosition", eMission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition, "DefendreSiteAPartirPosition" ),
T_MissionConverter( "Pion ASA MISTRAL DefendreZone",                eMission_Pion_ASA_MISTRAL_DefendreZone,                "DefendreZone" ),
T_MissionConverter( "Pion ASA MISTRAL DefendreZoneAPartirPosition", eMission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition, "DefendreZoneAPartirPosition" ),
T_MissionConverter( "Pion ASA MISTRAL Surveiller",                  eMission_Pion_ASA_MISTRAL_Surveiller,                  "Surveiller" ),
T_MissionConverter( "Pion ASS AssurerMiseEnOeuvre", eMission_Pion_ASS_AssurerMiseEnOeuvre, "AssurerMiseEnOeuvre" ),
T_MissionConverter( "Pion ASS AssurerMiseEnOeuvreSurPosition", eMission_Pion_ASS_AssurerMiseEnOeuvreSurPosition, "AssurerMiseEnOeuvreSurPosition" ),

T_MissionConverter( "Pion ASS ReconnaitreZoneDeploiement", eMission_Pion_ASS_ReconnaitreZoneDeploiement, "ReconnaitreZoneDeploiement" ),
T_MissionConverter( "Pion ASS AcquerirLanceursEni", eMission_Pion_ASS_AcquerirLanceursEni, "AcquerirLanceursEni" ),
T_MissionConverter( "Pion ASS EO AcquerirObjectifs", eMission_Pion_ASS_EO_AcquerirObjectifs, "EOAcquerirObjectifs" ),
T_MissionConverter( "Pion ASS RATAC AcquerirObjectifs", eMission_Pion_ASS_RATAC_AcquerirObjectifs, "RATACAcquerirObjectifs" ),
T_MissionConverter( "Pion ASS EOP AcquerirObjectifs", eMission_Pion_ASS_EOP_AcquerirObjectifs, "EOPAcquerirObjectifs" ),
T_MissionConverter( "Pion ABC ReconnaitreAxe", eMission_Pion_ABC_ReconnaitreAxe, "ReconnaitreAxe" ),
T_MissionConverter( "Pion ABC Soutenir", eMission_Pion_ABC_Soutenir, "Soutenir" ),
T_MissionConverter( "Pion ABC Couvrir", eMission_Pion_ABC_Couvrir, "Couvrir" ),
T_MissionConverter( "Pion ABC FlancGarder", eMission_Pion_ABC_FlancGarder, "FlancGarder" ),
T_MissionConverter( "Pion ABC Reduire", eMission_Pion_ABC_Reduire, "Reduire" ),
T_MissionConverter( "Pion ABC Appuyer", eMission_Pion_ABC_Appuyer, "Appuyer" ),
T_MissionConverter( "Pion ABC Fixer", eMission_Pion_ABC_Fixer, "Fixer" ),
T_MissionConverter( "Pion ABC RenseignerSur", eMission_Pion_ABC_RenseignerSur, "RenseignerSur" ),
T_MissionConverter( "Pion ABC ControlerZone", eMission_Pion_ABC_ControlerZone, "ControlerZone" ),
T_MissionConverter( "Pion ABC Surveiller", eMission_Pion_ABC_Surveiller, "Surveiller" ),
T_MissionConverter( "Pion ABC Barrer", eMission_Pion_ABC_Barrer, "Barrer" ),
T_MissionConverter( "Pion ABC Attaquer", eMission_Pion_ABC_Attaquer, "Attaquer" ),
T_MissionConverter( "Pion ABC ReconnaitrePoint", eMission_Pion_ABC_ReconnaitrePoint, "ReconnaitrePoint" ),
T_MissionConverter( "Pion ABC Eclairer", eMission_Pion_ABC_Eclairer, "Eclairer" ),
T_MissionConverter( "Pion ABC Jalonner", eMission_Pion_ABC_Jalonner, "Jalonner" ),
T_MissionConverter( "Pion ABC ContreAttaquer", eMission_Pion_ABC_ContreAttaquer, "ContreAttaquer" ),
T_MissionConverter( "Pion ABC SemparerZone", eMission_Pion_ABC_SemparerZone, "SemparerZone" ),
T_MissionConverter( "Pion ABC ReleverUnite", eMission_Pion_ABC_ReleverUnite, "ReleverUnite" ),
T_MissionConverter( "Pion ABC ArmerPIA", eMission_Pion_ABC_ArmerPIA, "ArmerPIA" ),
T_MissionConverter( "Pion ABC OuvrirItineraire", eMission_Pion_ABC_OuvrirItineraire, "OuvrirItineraire" ),
T_MissionConverter( "Pion ABC Freiner", eMission_Pion_ABC_Freiner, "Freiner" ),
T_MissionConverter( "Pion ABC EscorterUnConvoi", eMission_Pion_ABC_EscorterUnConvoi, "EscorterUnConvoi" ),
T_MissionConverter( "Pion ABC ArmerPointDeControle", eMission_Pion_ABC_ArmerPointDeControle, "ArmerPointDeControle" ),
T_MissionConverter( "Pion ABC OrganiserAccueilColonneRefugies", eMission_Pion_ABC_OrganiserAccueilColonneRefugies, "OrganiserAccueilColonneRefugies" ),
T_MissionConverter( "Pion GEN RealiserFosseAC", eMission_Pion_GEN_RealiserFosseAC, "RealiserFosseAC" ),
T_MissionConverter( "Pion GEN RealiserAbattisMine", eMission_Pion_GEN_RealiserAbattisMine, "RealiserAbattisMine" ),
T_MissionConverter( "Pion GEN RealiserPointMine", eMission_Pion_GEN_RealiserPointMine, "RealiserPointMine" ),
T_MissionConverter( "Pion GEN RealiserBarricade", eMission_Pion_GEN_RealiserBarricade, "RealiserBarricade" ),
T_MissionConverter( "Pion GEN RealiserEboulement", eMission_Pion_GEN_RealiserEboulement, "RealiserEboulement" ),
T_MissionConverter( "Pion GEN RealiserBouchonMine", eMission_Pion_GEN_RealiserBouchonMine, "RealiserBouchonMine" ),
T_MissionConverter( "Pion GEN DetruireRoute", eMission_Pion_GEN_DetruireRoute, "DetruireRoute" ),
T_MissionConverter( "Pion GEN DetruirePont", eMission_Pion_GEN_DetruirePont, "DetruirePont" ),
T_MissionConverter( "Pion GEN SecuriserItineraire", eMission_Pion_GEN_SecuriserItineraire, "SecuriserItineraire" ),
T_MissionConverter( "Pion GEN DegagerAbattisMine", eMission_Pion_GEN_DegagerAbattisMine, "DegagerAbattisMine" ),
T_MissionConverter( "Pion GEN DegagerEboulement", eMission_Pion_GEN_DegagerEboulement, "DegagerEboulement" ),
T_MissionConverter( "Pion GEN DegagerRues", eMission_Pion_GEN_DegagerRues, "DegagerRues" ),
T_MissionConverter( "Pion GEN ComblerEntonnoir", eMission_Pion_GEN_ComblerEntonnoir, "ComblerEntonnoir" ),
T_MissionConverter( "Pion GEN ComblerFosseAC", eMission_Pion_GEN_ComblerFosseAC, "ComblerFosseAC" ),
T_MissionConverter( "Pion GEN CreerContournement", eMission_Pion_GEN_CreerContournement, "CreerContournement" ),
T_MissionConverter( "Pion GEN RealiserZoneMineeLineaire", eMission_Pion_GEN_RealiserZoneMineeLineaire, "RealiserZoneMineeLineaire" ),
T_MissionConverter( "Pion GEN OuvrirTrouee", eMission_Pion_GEN_OuvrirTrouee, "OuvrirTrouee" ),
T_MissionConverter( "Pion GEN Renforcer", eMission_Pion_GEN_Renforcer, "Renforcer" ),
T_MissionConverter( "Pion GEN FranchirEntonnoir", eMission_Pion_GEN_FranchirEntonnoir, "FranchirEntonnoir" ),
T_MissionConverter( "Pion GEN FranchirFosseAC", eMission_Pion_GEN_FranchirFosseAC, "FranchirFosseAC" ),
T_MissionConverter( "Pion GEN RealiserZoneMineeParDispersion", eMission_Pion_GEN_RealiserZoneMineeParDispersion, "RealiserZoneMineeParDispersion" ),
T_MissionConverter( "Pion GEN RealiserPostesTir", eMission_Pion_GEN_RealiserPostesTir, "RealiserPostesTir" ),
T_MissionConverter( "Pion GEN RealiserTravauxProtection", eMission_Pion_GEN_RealiserTravauxProtection, "RealiserTravauxProtection" ),
T_MissionConverter( "Pion GEN RetablirItineraire", eMission_Pion_GEN_RetablirItineraire, "RetablirItineraire" ),
T_MissionConverter( "Pion GEN EquiperExploiter", eMission_Pion_GEN_EquiperExploiter, "EquiperExploiter" ),
T_MissionConverter( "Pion GEN ExecuterVariantement", eMission_Pion_GEN_ExecuterVariantement, "ExecuterVariantement" ),
T_MissionConverter( "Pion GEN ReconnaitreSiteFranchissement", eMission_Pion_GEN_ReconnaitreSiteFranchissement, "ReconnaitreSiteFranchissement" ),
T_MissionConverter( "Pion GEN ReconnaitreItineraire", eMission_Pion_GEN_ReconnaitreItineraire, "ReconnaitreItineraire" ),
T_MissionConverter( "Pion GEN DeminerSiteFranchissement", eMission_Pion_GEN_DeminerSiteFranchissement, "DeminerSiteFranchissement" ),
T_MissionConverter( "Pion GEN ReconnaitreZone", eMission_Pion_GEN_ReconnaitreZone, "ReconnaitreZone" ),
T_MissionConverter( "Pion GEN CreerPlateForme", eMission_Pion_GEN_CreerPlateForme, "CreerPlateForme" ),
T_MissionConverter( "Pion GEN CreerPiste", eMission_Pion_GEN_CreerPiste, "CreerPiste" ),
T_MissionConverter( "Pion GEN CreerAirePoser", eMission_Pion_GEN_CreerAirePoser, "CreerAirePoser" ),
T_MissionConverter( "Pion GEN NeutraliserEnginsExplosifs", eMission_Pion_GEN_NeutraliserEnginsExplosifs, "NeutraliserEnginsExplosifs" ),
T_MissionConverter( "Pion GEN ActiverObstacle", eMission_Pion_GEN_ActiverObstacle, "ActiverObstacle" ),
T_MissionConverter( "Pion GEN AmeliorerMobilite", eMission_Pion_GEN_AmeliorerMobilite, "AmeliorerMobilite" ),
T_MissionConverter( "Pion GEN RealiserAireLogistique", eMission_Pion_GEN_RealiserAireLogistique, "RealiserAireLogistique" ),
T_MissionConverter( "Pion GEN DepolluerZone", eMission_Pion_GEN_DepolluerZone, "DepolluerZone" ),
T_MissionConverter( "Pion GEN AmenagerTerrainPoser", eMission_Pion_GEN_AmenagerTerrainPoser, "AmenagerTerrainPoser" ),
T_MissionConverter( "Pion GEN RealiserPlateFormeDeploiement", eMission_Pion_GEN_RealiserPlateFormeDeploiement, "RealiserPlateFormeDeploiement" ),
T_MissionConverter( "Pion GEN ReprendreMissionConstruction", eMission_Pion_GEN_ReprendreMissionConstruction, "ReprendreMissionConstruction" ),
T_MissionConverter( "Pion GEN RealiserZonePoserHelicoptere", eMission_Pion_GEN_RealiserZonePoserHelicoptere, "RealiserZonePoserHelicoptere" ),
T_MissionConverter( "Pion GEN AppuyerDebarquementTroupesGU", eMission_Pion_GEN_AppuyerDebarquementTroupesGU, "AppuyerDebarquementTroupesGU" ),
T_MissionConverter( "Pion GEN RealiserCampPrisonniers", eMission_Pion_GEN_RealiserCampPrisonniers, "RealiserCampPrisonniers" ),
T_MissionConverter( "Pion GEN RealiserCampRefugies", eMission_Pion_GEN_RealiserCampRefugies, "RealiserCampRefugies" ),
T_MissionConverter( "Pion INF ReconnaitreAxe", eMission_Pion_INF_ReconnaitreAxe, "ReconnaitreAxe" ),
T_MissionConverter( "Pion INF Couvrir", eMission_Pion_INF_Couvrir, "Couvrir" ),
T_MissionConverter( "Pion INF Reduire", eMission_Pion_INF_Reduire, "Reduire" ),
T_MissionConverter( "Pion INF Appuyer", eMission_Pion_INF_Appuyer, "Appuyer" ),
T_MissionConverter( "Pion INF Fixer", eMission_Pion_INF_Fixer, "Fixer" ),
T_MissionConverter( "Pion INF ControlerSecteur", eMission_Pion_INF_ControlerSecteur, "ControlerSecteur" ),
T_MissionConverter( "Pion INF SurveillerSecteur", eMission_Pion_INF_SurveillerSecteur, "SurveillerSecteur" ),
T_MissionConverter( "Pion INF Barrer", eMission_Pion_INF_Barrer, "Barrer" ),
T_MissionConverter( "Pion INF ReconnaitrePoint", eMission_Pion_INF_ReconnaitrePoint, "ReconnaitrePoint" ),
T_MissionConverter( "Pion INF Eclairer", eMission_Pion_INF_Eclairer, "Eclairer" ),
T_MissionConverter( "Pion INF Interdire", eMission_Pion_INF_Interdire, "Interdire" ),
T_MissionConverter( "Pion INF Semparer", eMission_Pion_INF_Semparer, "Semparer" ),
T_MissionConverter( "Pion INF PrendrePreciserContact", eMission_Pion_INF_PrendrePreciserContact, "PrendrePreciserContact" ),
T_MissionConverter( "Pion INF Defendre", eMission_Pion_INF_Defendre, "Defendre" ),
T_MissionConverter( "Pion INF DetruireEmbuscade", eMission_Pion_INF_DetruireEmbuscade, "DetruireEmbuscade" ),
T_MissionConverter( "Pion INF ContreAttaquer", eMission_Pion_INF_ContreAttaquer, "ContreAttaquer" ),
T_MissionConverter( "Pion INF Freiner", eMission_Pion_INF_Freiner, "Freiner" ),
T_MissionConverter( "Pion INF FlancGarder", eMission_Pion_INF_FlancGarder, "FlancGarder" ),
T_MissionConverter( "Pion INF OrganiserUnCheckPoint", eMission_Pion_INF_OrganiserUnCheckPoint, "OrganiserUnCheckPoint" ),
T_MissionConverter( "Pion INF Harceler", eMission_Pion_INF_Harceler, "Harceler" ),
T_MissionConverter( "Pion INF DetruireParCoupDeMain", eMission_Pion_INF_DetruireParCoupDeMain, "DetruireParCoupDeMain" ),
T_MissionConverter( "Pion INF EscorterUnConvoi", eMission_Pion_INF_EscorterUnConvoi, "EscorterUnConvoi" ),
T_MissionConverter( "Pion INF RenseignerSur", eMission_Pion_INF_RenseignerSur, "RenseignerSur" ),
T_MissionConverter( "Pion INF Soutenir", eMission_Pion_INF_Soutenir, "Soutenir" ),
T_MissionConverter( "Pion INF Securiser", eMission_Pion_INF_Securiser, "Securiser" ),
T_MissionConverter( "Pion INF Recueillir", eMission_Pion_INF_Recueillir, "Recueillir" ),
T_MissionConverter( "Pion INF RepondreAAggression", eMission_Pion_INF_RepondreAAggression, "RepondreAAggression" ),
T_MissionConverter( "Pion LOG SeDeployer", eMission_Pion_LOG_SeDeployer, "SeDeployer" ),
T_MissionConverter( "Pion LOG DistribuerMoyens", eMission_Pion_LOG_DistribuerMoyens, "DistribuerMoyens" ),
T_MissionConverter( "Pion LOG TrierBlesses", eMission_Pion_LOG_TrierBlesses, "TrierBlesses" ),
T_MissionConverter( "Pion LOG TraiterBlesses", eMission_Pion_LOG_TraiterBlesses, "TraiterBlesses" ),
T_MissionConverter( "Pion LOG TrierEtTraiterBlesses", eMission_Pion_LOG_TrierEtTraiterBlesses, "TrierEtTraiterBlesses" ),
T_MissionConverter( "Pion LOG SoignerBlesses", eMission_Pion_LOG_SoignerBlesses, "SoignerBlesses" ),
T_MissionConverter( "Pion LOG PrendreEnCompteReactionsMentales", eMission_Pion_LOG_PrendreEnCompteReactionsMentales, "PrendreEnCompteReactionsMentales" ),
T_MissionConverter( "Pion LOG DecontaminerBlesses", eMission_Pion_LOG_DecontaminerBlesses, "DecontaminerBlesses" ),
T_MissionConverter( "Pion LOG Reparer", eMission_Pion_LOG_Reparer, "Reparer" ),
T_MissionConverter( "Pion LOG Evacuer", eMission_Pion_LOG_Evacuer, "Evacuer" ),
T_MissionConverter( "Pion LOG Convoyer", eMission_Pion_LOG_Convoyer, "Convoyer" ),
T_MissionConverter( "Pion LOG DistribuerRessources", eMission_Pion_LOG_DistribuerRessources, "DistribuerRessources" ),
T_MissionConverter( "Pion LOG ConstituerConvoi",     eMission_Pion_LOG_ConstituerConvoi,     "ConstituerConvoi" ),
T_MissionConverter( "Pion LOG Livrer",                eMission_Pion_LOG_Livrer,               "Livrer" ),
T_MissionConverter( "Pion LOG PreparerBascule",              eMission_Pion_LOG_PreparerBascule,  "PreparerBascule" ),   
T_MissionConverter( "Pion LOG AppuyerMouvement",        eMission_Pion_LOG_AppuyerMouvement , "AppuyerMouvement" ),
T_MissionConverter( "Pion LOG ReconnaitreItineraire", eMission_Pion_LOG_ReconnaitreItineraire, "ReconnaitreItineraire" ),
T_MissionConverter( "Pion LOG Surveiller",  eMission_Pion_LOG_Surveiller, "Surveiller" ),
T_MissionConverter( "Pion LOG TransporterUniteChars", eMission_Pion_LOG_TransporterUniteChars, "TransporterUniteChars" ),
T_MissionConverter( "Pion LOG ReconnaitreZoneContaminee",       eMission_Pion_LOG_ReconnaitreZoneContaminee         , "ReconnaitreZoneContaminee"       ), 
T_MissionConverter( "Pion LOG AppuyerMouvementDansZone",        eMission_Pion_LOG_AppuyerMouvementDansZone          , "AppuyerMouvementDansZone"        ),
T_MissionConverter( "Pion LOG MettreEnOeuvreZoneStationnement", eMission_Pion_LOG_MettreEnOeuvreZoneStationnement   , "MettreEnOeuvreZoneStationnement" ),
T_MissionConverter( "Pion LOG AppuyerFranchissementSurUnPoint" ,eMission_Pion_LOG_AppuyerFranchissementSurUnPoint   , "AppuyerFranchissementSurUnPoint" ),
T_MissionConverter( "Pion LOG BloquerZone" ,                    eMission_Pion_LOG_BloquerZone                       , "BloquerZone"                     ),
T_MissionConverter( "Pion LOG ReconnaitreZoneDeDeploiement" ,   eMission_Pion_LOG_ReconnaitreZoneDeDeploiement      , "ReconnaitreZoneDeDeploiement"    ),
T_MissionConverter( "Pion LOG ArmerPIA",                        eMission_Pion_LOG_ArmerPIA                          , "ArmerPIA"                        ),
T_MissionConverter( "Pion LOG Surveiller",                      eMission_Pion_LOG_Surveiller                        , "Surveiller"                      ),
T_MissionConverter( "Pion Test Heliporter", eMission_Pion_Test_Heliporter, "Heliporter" ),
T_MissionConverter( "Pion Test MoveTo", eMission_Pion_Test_MoveTo, "MoveTo" ),
T_MissionConverter( "Pion Test CreateObject", eMission_Pion_Test_CreateObject, "CreateObject" ),
T_MissionConverter( "Pion Test DestroyObject", eMission_Pion_Test_DestroyObject, "DestroyObject" ),
T_MissionConverter( "Pion Test CreateBypass", eMission_Pion_Test_CreateBypass, "CreateBypass" ),
T_MissionConverter( "Pion Test Fire", eMission_Pion_Test_Fire, "Fire" ),
T_MissionConverter( "Pion Test ChangePosture", eMission_Pion_Test_ChangePosture, "ChangePosture" ),
T_MissionConverter( "Pion Test PathFind", eMission_Pion_Test_PathFind, "PathFind" ),
T_MissionConverter( "Pion Test FindPosition", eMission_Pion_Test_FindPosition, "FindPosition" ),
T_MissionConverter( "Pion Test Reinforce", eMission_Pion_Test_Reinforce, "Reinforce" ),
T_MissionConverter( "Pion Generique", eMission_Pion_Generique, "Generique" ),
T_MissionConverter( "Pion PlastronEnnemi", eMission_Pion_PlastronEnnemi, "T_Mission_Pion_PlastronEnnemi" ),
T_MissionConverter( "Pion FaireMouvement", eMission_Pion_FaireMouvement, "T_Mission_Pion_FaireMouvement" ),
T_MissionConverter( "Pion Suivre", eMission_Pion_Suivre, "T_Mission_Pion_Suivre" ),
T_MissionConverter( "Pion SeFaireDecontaminer", eMission_Pion_SeFaireDecontaminer, "T_Mission_Pion_SeFaireDecontaminer" ),
T_MissionConverter( "Pion Franchir", eMission_Pion_Franchir, "T_Mission_Pion_Franchir" ),
T_MissionConverter( "Pion Decrocher", eMission_Pion_Decrocher, "T_Mission_Pion_Decrocher" ),
T_MissionConverter( "Pion Stationner", eMission_Pion_Stationner, "T_Mission_Pion_Stationner" ),
T_MissionConverter( "Pion SeFaireTransporter", eMission_Pion_SeFaireTransporter, "T_Mission_Pion_SeFaireTransporter" ),
T_MissionConverter( "Pion MILICE Provoquer"         , eMission_Pion_MILICE_Provoquer        , "T_Mission_Pion_MILICE_Provoquer" ),
T_MissionConverter( "Pion MILICE PrendreEtTenir"    , eMission_Pion_MILICE_PrendreEtTenir   , "T_Mission_Pion_MILICE_PrendreEtTenir" ),
T_MissionConverter( "Pion MILICE DetruireEmbuscade" , eMission_Pion_MILICE_DetruireEmbuscade, "T_Mission_Pion_MILICE_DetruireEmbuscade" ),
T_MissionConverter( "Pion JOINT MARINE Transporter" , eMission_Pion_JOINT_MARINE_Transporter, "T_Mission_Pion_JOINT_MARINE_Transporter" ),
T_MissionConverter( "Pion JOINT AIR EffectuerRaid"  , eMission_Pion_JOINT_AIR_EffectuerRaid , "eT_ission_Pion_JOINT_AIR_EffectuerRaid" ),
T_MissionConverter( "Pion NBC AnimerUnPlotDeDecontamination", eMission_Pion_NBC_AnimerUnPlotDeDecontamination, "AnimerUnPlotDeDecontamination" ),
T_MissionConverter( "Pion NBC ReconnaitreItineraire", eMission_Pion_NBC_ReconnaitreItineraire, "ReconnaitreItineraire" ),
T_MissionConverter( "Pion NBC ReconnaitreZone", eMission_Pion_NBC_ReconnaitreZone, "ReconnaitreZone" ),
T_MissionConverter( "Pion NBC DecontaminerZone", eMission_Pion_NBC_DecontaminerZone, "DecontaminerZone" ),
T_MissionConverter( "Pion NBC ReconnaitreUnSiteROTA", eMission_Pion_NBC_ReconnaitreUnSiteROTA, "ReconnaitreUnSiteROTA" ),
T_MissionConverter( "Pion RENS ROHUM RenseignerSur", eMission_Pion_RENS_ROHUM_RenseignerSur, "ROHUM RenseignerSur" ),
T_MissionConverter( "Pion RENS ROHUM SExfiltrer", eMission_Pion_RENS_ROHUM_SExfiltrer, "ROHUM SExfiltrer" ),
T_MissionConverter( "Pion RENS ROHUM OrienterGuider", eMission_Pion_RENS_ROHUM_OrienterGuider, "ROHUM OrienterGuider" ),
T_MissionConverter( "Pion RENS ROIM SDTI RenseignerSur", eMission_Pion_RENS_ROIM_SDTI_RenseignerSur  , "ROIM SDTI RenseignerSur" ),
T_MissionConverter( "Pion RENS ROIM CL289 RenseignerSur", eMission_Pion_RENS_ROIM_CL289_RenseignerSur, "ROIM CL289 RenseignerSur" ),
T_MissionConverter( "Pion RENS ROIM MettreEnOeuvre", eMission_Pion_RENS_ROIM_MettreEnOeuvre, "ROIM MettreEnOeuvre" ),
T_MissionConverter( "Pion RENS ROEM Appuyer", eMission_Pion_RENS_ROEM_Appuyer, "Appuyer" ),
T_MissionConverter( "Pion RENS ROEM RenseignerSur", eMission_Pion_RENS_ROEM_RenseignerSur, "ROEM RenseignerSur" ),
T_MissionConverter( "Pion RENS ROEM MettreEnOeuvre", eMission_Pion_RENS_ROEM_MettreEnOeuvre, "ROEM MettreEnOeuvre" ),
T_MissionConverter( "Pion RENS RADINT RenseignerSur", eMission_Pion_RENS_RADINT_RenseignerSur, "RADINT RenseignerSur" ),
T_MissionConverter( "Pion RENS RADINT MettreEnOeuvre", eMission_Pion_RENS_RADINT_MettreEnOeuvre, "RADINT MettreEnOeuvre" ),
T_MissionConverter( "Pion TRANS EtablirGererLiaison", eMission_Pion_TRANS_EtablirGererLiaison, "EtablirGererLiaison" ),
T_MissionConverter( ""           , (E_MissionID)-1, "" )
};

MOS_Tools::T_OrderConverter MOS_Tools::orderConverter_[] =
{
    T_OrderConverter( "AttendreSePoster"                                , eOrdreConduite_AttendreSePoster ),
    T_OrderConverter( "Poursuivre"                                      , eOrdreConduite_Poursuivre ),
    T_OrderConverter( "Interrompre"                                     , eOrdreConduite_Interrompre ),
    T_OrderConverter( "Deboucher"                                       , eOrdreConduite_Deboucher ),
    T_OrderConverter( "Decrocher"                                       , eOrdreConduite_Decrocher ),
    T_OrderConverter( "MettreTenueNBC"                                  , eOrdreConduite_MettreTenueNBC ),
    T_OrderConverter( "EnleverTenueNBC"                                 , eOrdreConduite_EnleverTenueNBC ),
    T_OrderConverter( "PasserEnSilenceRadio"                            , eOrdreConduite_PasserEnSilenceRadio ),
    T_OrderConverter( "ArreterSilenceRadio"                             , eOrdreConduite_ArreterSilenceRadio ),
    T_OrderConverter( "PasserEnSilenceRadar"                            , eOrdreConduite_PasserEnSilenceRadar ),
    T_OrderConverter( "ArreterSilenceRadar"                             , eOrdreConduite_ArreterSilenceRadar ),
    T_OrderConverter( "ChangerReglesEngagement"                         , eOrdreConduite_ChangerReglesEngagement ),
    T_OrderConverter( "AssignerPositionEmbarquement"                    , eOrdreConduite_AssignerPositionEmbarquement ),
    T_OrderConverter( "AcquerirObjectif"                                , eOrdreConduite_AcquerirObjectif ),
    T_OrderConverter( "Brouiller"                                       , eOrdreConduite_Brouiller ),
    T_OrderConverter( "Pion ChangerDePosition"                          , eOrdreConduite_Pion_ChangerDePosition ),
    T_OrderConverter( "Pion Contourner"                                 , eOrdreConduite_Pion_Contourner ),
    T_OrderConverter( "Pion EnvoyerVehicule"                            , eOrdreConduite_Pion_EnvoyerVehicule ),
    T_OrderConverter( "Pion ActiverObstacle"                            , eOrdreConduite_Pion_ActiverObstacle ),
    T_OrderConverter( "Pion AppliquerFeux"                              , eOrdreConduite_Pion_AppliquerFeux ),
    T_OrderConverter( "Pion ExecuterTir"                                , eOrdreConduite_Pion_ExecuterTir ),
    T_OrderConverter( "Pion Emettre"                                    , eOrdreConduite_Pion_Emettre ),
    T_OrderConverter( "Pion StopperEmission"                            , eOrdreConduite_Pion_StopperEmission ),
    T_OrderConverter( "Pion RenforcerEnVSRAM"                            , eOrdreConduite_Pion_RenforcerEnVSRAM ),
    T_OrderConverter( "Pion TransfererVSRAM"                             , eOrdreConduite_Pion_TransfererVSRAM ),
    T_OrderConverter( "Pion ReprendreAuxOrdresVSRAM"                     , eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM ),
    T_OrderConverter( "ModifierTempsBordeeMaintenance"                         , eOrdreConduite_ModifierTempsBordeeMaintenance ),
    T_OrderConverter( "ModifierTempsBordeeSante"                         , eOrdreConduite_ModifierTempsBordeeSante ),
    T_OrderConverter( "ModifierPrioritesReparations"                 , eOrdreConduite_ModifierPrioritesReparations ),
    T_OrderConverter( "ModifierPrioritesBlesses"                    , eOrdreConduite_ModifierPrioritesBlesses ),
    T_OrderConverter( "ModifierPrioritesTactiquesReparations"           , eOrdreConduite_ModifierPrioritesTactiquesReparations ),
    T_OrderConverter( "ModifierPrioritesTactiquesBlesses"               , eOrdreConduite_ModifierPrioritesTactiquesBlesses ),

    T_OrderConverter( "Automate RealiserVariantement"                     , eOrdreConduite_Automate_RealiserVariantement ),
    T_OrderConverter( "Automate DesequiperSiteFranchissement"             , eOrdreConduite_Automate_DesequiperSiteFranchissement ),
    T_OrderConverter( "Automate ReagirFaceAEni"                           , eOrdreConduite_Automate_ReagirFaceAEni ),
    T_OrderConverter( "Automate AffecterPionAObstacle"                    , eOrdreConduite_Automate_AffecterPionAObstacle ),   
    T_OrderConverter( "Automate ReconnaitreZoneImplantation"              , eOrdreConduite_Automate_ReconnaitreZoneImplantation ),

    T_OrderConverter( "Automate TC2 GererMaterielAvantDeplacement"  , eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement ),
    T_OrderConverter( "Automate ActiverObstacle"                    , eOrdreConduite_Automate_ActiverObstacle ),
    T_OrderConverter( "Automate TransfererRenforts"                  , eOrdreConduite_Automate_TransfererRenforts ),

    T_OrderConverter( ""                                                  , (E_OrderConduiteID)-1 )
};

MOS_Tools::converter< E_NatureLevelType > MOS_Tools::natureLevelConverter_[] = 
{
    converter< E_NatureLevelType >( "None" , eNatureLevel_None  ),
    converter< E_NatureLevelType >( "o"    , eNatureLevel_o     ),
    converter< E_NatureLevelType >( "oo"   , eNatureLevel_oo    ),
    converter< E_NatureLevelType >( "ooo"  , eNatureLevel_ooo   ),
    converter< E_NatureLevelType >( "i"    , eNatureLevel_i     ),
    converter< E_NatureLevelType >( "ii"   , eNatureLevel_ii    ),
    converter< E_NatureLevelType >( "iii"  , eNatureLevel_iii   ),
    converter< E_NatureLevelType >( "x"    , eNatureLevel_x     ),
    converter< E_NatureLevelType >( "xx"   , eNatureLevel_xx    ),
    converter< E_NatureLevelType >( "xxx"  , eNatureLevel_xxx   ),
    converter< E_NatureLevelType >( "xxxx" , eNatureLevel_xxxx  ),
    converter< E_NatureLevelType >( "xxxxx", eNatureLevel_xxxxx ),
    converter< E_NatureLevelType >( ""     , (E_NatureLevelType)-1 )   
};

MOS_Tools::converter< E_NatureWeaponType > MOS_Tools::natureWeaponConverter_[] = 
{
    converter< E_NatureWeaponType >( "Anti-char"                                   , eNatureWeapon_Antichar                         ),                            
    converter< E_NatureWeaponType >( "Administration"                              , eNatureWeapon_Administration                   ),                      
    converter< E_NatureWeaponType >( "Defense aerienne"                            , eNatureWeapon_DefenseAerienne                  ),                     
    converter< E_NatureWeaponType >( "Centre de soutien des operations aeriennes"  , eNatureWeapon_CentreSoutienOperationsAeriennes ),    
    converter< E_NatureWeaponType >( "Blinde"                                      , eNatureWeapon_Blinde                           ),                              
    converter< E_NatureWeaponType >( "Aviation"                                    , eNatureWeapon_Aviation                         ),                            
    converter< E_NatureWeaponType >( "Cavalerie"                                   , eNatureWeapon_Cavalerie                        ),                           
    converter< E_NatureWeaponType >( "Genie"                                       , eNatureWeapon_Genie                            ),                               
    converter< E_NatureWeaponType >( "Deminage"                                    , eNatureWeapon_Deminage                         ),                            
    converter< E_NatureWeaponType >( "Soutien artillerie"                          , eNatureWeapon_SoutienArtillerie                ),                   
    converter< E_NatureWeaponType >( "Artillerie"                                  , eNatureWeapon_Artillerie                       ),                          
    converter< E_NatureWeaponType >( "Infanterie"                                  , eNatureWeapon_Infanterie                       ),                          
    converter< E_NatureWeaponType >( "Guerre de l'information"                     , eNatureWeapon_GuerreInformation                ),                   
    converter< E_NatureWeaponType >( "Forces de securite interne"                  , eNatureWeapon_ForcesSecuriteInterne            ),               
    converter< E_NatureWeaponType >( "Execution de la loi"                         , eNatureWeapon_ExecutionLoi                     ),                        
    converter< E_NatureWeaponType >( "Soutien atterrissage"                        , eNatureWeapon_SoutienAtterrissage              ),                 
    converter< E_NatureWeaponType >( "Logistique"                                  , eNatureWeapon_Logistique                       ),                          
    converter< E_NatureWeaponType >( "Reparation"                                  , eNatureWeapon_Reparation                       ),                          
    converter< E_NatureWeaponType >( "Manoeuvre"                                   , eNatureWeapon_Manoeuvre                        ),                           
    converter< E_NatureWeaponType >( "Medical"                                     , eNatureWeapon_Medical                          ),                             
    converter< E_NatureWeaponType >( "Renseignement militaire"                     , eNatureWeapon_RenseignementMilitaire           ),              
    converter< E_NatureWeaponType >( "Missile"                                     , eNatureWeapon_Missile                          ),                             
    converter< E_NatureWeaponType >( "NBC"                                         , eNatureWeapon_Nbc                              ),                                 
    converter< E_NatureWeaponType >( "Inconnue"                                    , eNatureWeapon_Inconnue                         ),                            
    converter< E_NatureWeaponType >( "specifiee"                                   , eNatureWeapon_NonSpecifiee                        ),                           
    converter< E_NatureWeaponType >( "Reconnaissance"                              , eNatureWeapon_Reconnaissance                   ),                      
    converter< E_NatureWeaponType >( "Transmissions"                               , eNatureWeapon_Transmissions                    ),                       
    converter< E_NatureWeaponType >( "Ravitaillement"                              , eNatureWeapon_Ravitaillement                   ),                      
    converter< E_NatureWeaponType >( "Tactical air control party"                  , eNatureWeapon_TacticalAirControlParty          ),             
    converter< E_NatureWeaponType >( "Transport"                                   , eNatureWeapon_Transport                        ),
    converter< E_NatureWeaponType >( "None"                                        , eNatureWeapon_None                             ),
    converter< E_NatureWeaponType >( ""                                            , (E_NatureWeaponType)-1       )
};                           


MOS_Tools::converter< E_NatureSpecializationType >        MOS_Tools::natureSpecializationConverter_[] = 
{
    converter< E_NatureSpecializationType >( "Jag"                                      , eNatureSpec_Jag                              ),
    converter< E_NatureSpecializationType >( "Riverine"                                 , eNatureSpec_Riverine                         ),
    converter< E_NatureSpecializationType >( "Security police"                          , eNatureSpec_SecurityPolice                   ),
    converter< E_NatureSpecializationType >( "Aerien"                                   , eNatureSpec_Aerien                           ),
    converter< E_NatureSpecializationType >( "Aeroporte"                                , eNatureSpec_Aeroporte                        ),
    converter< E_NatureSpecializationType >( "Attaque aerienne"                         , eNatureSpec_AttaqueAerienne                  ),
    converter< E_NatureSpecializationType >( "Blinde"                                   , eNatureSpec_Blinde                           ),  
    converter< E_NatureSpecializationType >( "Chemin de fer"                            , eNatureSpec_CheminDeFer                      ),
    converter< E_NatureSpecializationType >( "Chirurgie"                                , eNatureSpec_Chirurgie                        ),
    converter< E_NatureSpecializationType >( "Compose"                                  , eNatureSpec_Compose                          ),
    converter< E_NatureSpecializationType >( "Construction"                             , eNatureSpec_Construction                     ),
    converter< E_NatureSpecializationType >( "Courrier"                                 , eNatureSpec_Courrier                         ),
    converter< E_NatureSpecializationType >( "Decollage - atterissage vertical - court" , eNatureSpec_DecollageAtterissageVerticalCourt),
    converter< E_NatureSpecializationType >( "Dentiste"                                 , eNatureSpec_Dentiste                         ),
    converter< E_NatureSpecializationType >( "Finance"                                  , eNatureSpec_Finance                          ),
    converter< E_NatureSpecializationType >( "Guerre electronique"                      , eNatureSpec_GuerreElectronique               ),
    converter< E_NatureSpecializationType >( "Helicoptere"                              , eNatureSpec_Helicoptere                      ),
    converter< E_NatureSpecializationType >( "Mecanise"                                 , eNatureSpec_Mecanise                         ),
    converter< E_NatureSpecializationType >( "Motorise"                                 , eNatureSpec_Motorise                         ),
    converter< E_NatureSpecializationType >( "Naval"                                    , eNatureSpec_Naval                            ),
    converter< E_NatureSpecializationType >( "Patrouille cotiere"                       , eNatureSpec_PatrouilleCotiere                ),
    converter< E_NatureSpecializationType >( "Police militaire"                         , eNatureSpec_PoliceMilitaire                  ),
    converter< E_NatureSpecializationType >( "Psychologue"                              , eNatureSpec_Psychologique                    ),
    converter< E_NatureSpecializationType >( "Religieux"                                , eNatureSpec_Religieux                        ),
    converter< E_NatureSpecializationType >( "Service du personnel"                     , eNatureSpec_ServiceDuPersonnel               ),
    converter< E_NatureSpecializationType >( "Terrestre"                                , eNatureSpec_Terrestre                        ),
    converter< E_NatureSpecializationType >( "Veterinaire"                              , eNatureSpec_Veterinaire                      ),
    converter< E_NatureSpecializationType >( "Voilure fixe"                             , eNatureSpec_VoilureFixe                      ),
    converter< E_NatureSpecializationType >( "None"                                     , eNatureSpec_None                             ),
    converter< E_NatureSpecializationType >( ""                                         , (E_NatureSpecializationType)-1               )
};

MOS_Tools::converter< E_NatureQualifierType >  MOS_Tools::natureQualifierConverter_[] = 
{
    converter< E_NatureQualifierType >( "Lourd" , eNatureQualifier_Lourd    ),
    converter< E_NatureQualifierType >( "Leger" , eNatureQualifier_Leger    ),
    converter< E_NatureQualifierType >( "Medium", eNatureQualifier_Moyen    ),
    converter< E_NatureQualifierType >( "None"  , eNatureQualifier_None     ),
    converter< E_NatureQualifierType >( ""      , (E_NatureQualifierType)-1 )
};


MOS_Tools::converter< E_NatureCategoryType >  MOS_Tools::natureCategoryConverter_[] = 
{
    converter< E_NatureCategoryType >( "Combat"            , eNatureCategory_Combat             ),
    converter< E_NatureCategoryType >( "Soutien logistique", eNatureCategory_SoutienLogistique  ), 
    converter< E_NatureCategoryType >( "Combat support"    , eNatureCategory_CombatSupport      ),     
    converter< E_NatureCategoryType >( "Inconnu"           , eNatureCategory_Inconnu            ),           
    converter< E_NatureCategoryType >( "None"              , eNatureCategory_None               ),              
    converter< E_NatureCategoryType >( ""                  , (E_NatureCategoryType)-1           )
};

MOS_Tools::converter< E_NatureMobilityType > MOS_Tools::natureMobilityConverter_[] = 
{
    converter< E_NatureMobilityType >( "Aeroporte"                  , eNatureMobility_Aeroporte             ),
    converter< E_NatureMobilityType >( "Aerien compose"             , eNatureMobility_AerienCompose         ),
    converter< E_NatureMobilityType >( "Aerien voilure fixe"        , eNatureMobility_AerienVoilureFixe     ),
    converter< E_NatureMobilityType >( "Aerien plus leger que l'air", eNatureMobility_AerienPlusLegerQueAir ),
    converter< E_NatureMobilityType >( "Aerien ailes rotatives"     , eNatureMobility_AerienAilesRotatives  ),
    converter< E_NatureMobilityType >( "Aerien amphibie"            , eNatureMobility_AerienAmphibie              ),
    converter< E_NatureMobilityType >( "Animal monte"               , eNatureMobility_AnimalMonte           ),                  
    converter< E_NatureMobilityType >( "Demonte"                    , eNatureMobility_Demonte               ),
    converter< E_NatureMobilityType >( "Terrestre chemin de fer"    , eNatureMobility_TerrestreCheminDeFer  ),
    converter< E_NatureMobilityType >( "Terrestre auto propulse"    , eNatureMobility_TerrestreAutoPropulse ),       
    converter< E_NatureMobilityType >( "Terrestre chenille"         , eNatureMobility_TerrestreChenille     ),       
    converter< E_NatureMobilityType >( "Terrestre remorque"         , eNatureMobility_TerrestreRemorque     ),       
    converter< E_NatureMobilityType >( "Terrestre roues"            , eNatureMobility_TerrestreRoues        ),       
    converter< E_NatureMobilityType >( "Maritime sous marin"        , eNatureMobility_MaritimeSousMarin     ),       
    converter< E_NatureMobilityType >( "Maritime surface"           , eNatureMobility_MaritimeSurface       ),       
    converter< E_NatureMobilityType >( "Inconnu"                    , eNatureMobility_Inconnu               ),       
    converter< E_NatureMobilityType >( "None"                       , eNatureMobility_None                  ),       
    converter< E_NatureMobilityType >( ""                           , (E_NatureMobilityType)-1              )
};

MOS_Tools::converter< E_NatureAtlasType > MOS_Tools::natureAtlasConverter_[] = 
{
    converter< E_NatureAtlasType >( "Blinde"          , eNatureAtlas_Blinde          ),
    converter< E_NatureAtlasType >( "ASS"             , eNatureAtlas_ASS             ),
    converter< E_NatureAtlasType >( "PC"              , eNatureAtlas_PC              ),
    converter< E_NatureAtlasType >( "Log"             , eNatureAtlas_Log             ),
    converter< E_NatureAtlasType >( "Mortier"         , eNatureAtlas_Mortier         ),
    converter< E_NatureAtlasType >( "Lance roquette"  , eNatureAtlas_LanceRoquette   ),
    converter< E_NatureAtlasType >( "Vehicule"        , eNatureAtlas_Vehicule        ),
    converter< E_NatureAtlasType >( "Personnel a pied", eNatureAtlas_PersonnelAPied  ),
    converter< E_NatureAtlasType >( "Helicoptere"     , eNatureAtlas_Helicoptere     ),
    converter< E_NatureAtlasType >( "Indefini"        , eNatureAtlas_Indefini        ),
    converter< E_NatureAtlasType >( "None"            , eNatureAtlas_None            ),
    converter< E_NatureAtlasType >( ""                , (E_NatureAtlasType)-1        )
};

MOS_Tools::converter< E_NatureCapaciteMissionType > MOS_Tools::natureCapaciteMissionConverter_[] =
{
    converter< E_NatureCapaciteMissionType >( "Civilian law enforcement",                    eNatureCapaciteMission_CivilianLawEnforcement                 ),
    converter< E_NatureCapaciteMissionType >( "Command operations",                          eNatureCapaciteMission_CommandOperations                      ),
    converter< E_NatureCapaciteMissionType >( "Couverture aerienne",                         eNatureCapaciteMission_CouvertureAerienne                     ),
    converter< E_NatureCapaciteMissionType >( "Joint intelligence",                          eNatureCapaciteMission_JointIntelligence                      ),
    converter< E_NatureCapaciteMissionType >( "Morale, welfare recreation",                  eNatureCapaciteMission_MoraleWelfareRecreation                ),
    converter< E_NatureCapaciteMissionType >( "Reglage electronique",                        eNatureCapaciteMission_ReglageElectronique                    ),
    converter< E_NatureCapaciteMissionType >( "Remplacement",                                eNatureCapaciteMission_Remplacement                           ),
    converter< E_NatureCapaciteMissionType >( "Service mortuaire",                           eNatureCapaciteMission_ServiceMortuaire                       ),
    converter< E_NatureCapaciteMissionType >( "Signals intelligence (SIGINT)",               eNatureCapaciteMission_SignalsIntelligence                    ),
    converter< E_NatureCapaciteMissionType >( "Theatre missile defence",                     eNatureCapaciteMission_TheatreMissileDefence                  ),
    converter< E_NatureCapaciteMissionType >( "Utilitaire",                                  eNatureCapaciteMission_Utilitaire                             ),
    converter< E_NatureCapaciteMissionType >( "Aeroporte",                                   eNatureCapaciteMission_Aéroporte                              ),
    converter< E_NatureCapaciteMissionType >( "Affaires publiques",                          eNatureCapaciteMission_AffairesPubliques                      ),
    converter< E_NatureCapaciteMissionType >( "Affaires publiques, communication",           eNatureCapaciteMission_AffairesPubliques_Communication        ),
    converter< E_NatureCapaciteMissionType >( "Affaires publiques, joint information",       eNatureCapaciteMission_AffairesPubliques_JointInformation     ),
    converter< E_NatureCapaciteMissionType >( "Alpin",                                       eNatureCapaciteMission_Alpin                                  ),
    converter< E_NatureCapaciteMissionType >( "Amphibie",                                    eNatureCapaciteMission_Amphibie                               ),
    converter< E_NatureCapaciteMissionType >( "Arctique",                                    eNatureCapaciteMission_Arctique                               ),
    converter< E_NatureCapaciteMissionType >( "Artillerie - reconnaissance",                 eNatureCapaciteMission_ArtillerieReconnaissance               ),
    converter< E_NatureCapaciteMissionType >( "Attaque",                                     eNatureCapaciteMission_Attaque                                ),
    converter< E_NatureCapaciteMissionType >( "Attaque aerienne",                            eNatureCapaciteMission_AttaqueAerienne                        ),
    converter< E_NatureCapaciteMissionType >( "C2",                                          eNatureCapaciteMission_C2                                     ),
    converter< E_NatureCapaciteMissionType >( "Chemin de fer",                               eNatureCapaciteMission_CheminDeFer                            ),
    converter< E_NatureCapaciteMissionType >( "Ciblage",                                     eNatureCapaciteMission_Ciblage                                ),
    converter< E_NatureCapaciteMissionType >( "Contre espionnage",                           eNatureCapaciteMission_ContreEspionnage                       ),
    converter< E_NatureCapaciteMissionType >( "Eclairage",                                   eNatureCapaciteMission_Eclairage                              ),
    converter< E_NatureCapaciteMissionType >( "Entretien",                                   eNatureCapaciteMission_Entretien                              ),
    converter< E_NatureCapaciteMissionType >( "Entretien, materiel",                         eNatureCapaciteMission_EntretienMateriel                      ),
    converter< E_NatureCapaciteMissionType >( "Entretien, missile",                          eNatureCapaciteMission_EntretienMissile                       ),
    converter< E_NatureCapaciteMissionType >( "Entretien, opto-electronique",                eNatureCapaciteMission_EntretienOptoElectronique              ),
    converter< E_NatureCapaciteMissionType >( "Espionnage",                                  eNatureCapaciteMission_Espionnage                             ),
    converter< E_NatureCapaciteMissionType >( "Espionnage militaire, aerial exploitation",   eNatureCapaciteMission_EspionnageMilitaireAerialExploitation  ),
    converter< E_NatureCapaciteMissionType >( "Espionnage militaire, operation",             eNatureCapaciteMission_EspionnageMilitaireOperation           ),
    converter< E_NatureCapaciteMissionType >( "Espionnage militaire, tactical exploitation", eNatureCapaciteMission_EspionnageMilitaireTacticalExploitation),
    converter< E_NatureCapaciteMissionType >( "Evacuation sanitaire",                        eNatureCapaciteMission_EvacuationSanitaire                    ),
    converter< E_NatureCapaciteMissionType >( "Finance",                                     eNatureCapaciteMission_Finance                                ),
    converter< E_NatureCapaciteMissionType >( "Genie, combat",                               eNatureCapaciteMission_GenieCombat                            ),
    converter< E_NatureCapaciteMissionType >( "Genie, construction",                         eNatureCapaciteMission_GenieConstruction                      ),
    converter< E_NatureCapaciteMissionType >( "Genie, construction navale",                  eNatureCapaciteMission_GenieConstructionNavale                ),
    converter< E_NatureCapaciteMissionType >( "Guerre electronique",                         eNatureCapaciteMission_GuerreElectronique                     ),
    converter< E_NatureCapaciteMissionType >( "Guerre electronique, interception",           eNatureCapaciteMission_GuerreElectroniqueInterception         ),
    converter< E_NatureCapaciteMissionType >( "Guerre electronique, brouillage",             eNatureCapaciteMission_GuerreElectroniqueBrouillage           ),
    converter< E_NatureCapaciteMissionType >( "Guerre electronique, detection",              eNatureCapaciteMission_GuerreElectroniqueDétection            ),
    converter< E_NatureCapaciteMissionType >( "Interrogation",                               eNatureCapaciteMission_Interrogation                          ),
    converter< E_NatureCapaciteMissionType >( "Main d'oeuvre",                               eNatureCapaciteMission_MainDOeuvre                            ),
    converter< E_NatureCapaciteMissionType >( "Maintien de la paix",                         eNatureCapaciteMission_MaintienDeLaPaix                       ),
    converter< E_NatureCapaciteMissionType >( "Marine",                                      eNatureCapaciteMission_Marine                                 ),
    converter< E_NatureCapaciteMissionType >( "Mine de contre-mesure",                       eNatureCapaciteMission_MineDeContreMesure                     ),
    converter< E_NatureCapaciteMissionType >( "NBC, biologique",                             eNatureCapaciteMission_NBCBiologique                          ),
    converter< E_NatureCapaciteMissionType >( "NBC, chimique",                               eNatureCapaciteMission_NBCChimique                            ),
    converter< E_NatureCapaciteMissionType >( "NBC, chimique, fumee",                        eNatureCapaciteMission_NBCChimiqueFumee                       ),
    converter< E_NatureCapaciteMissionType >( "NBC, decontamination",                        eNatureCapaciteMission_NBCDecontamination                     ),
    converter< E_NatureCapaciteMissionType >( "NBC, decontamination chimique",               eNatureCapaciteMission_NBCDecontaminationChimique             ),
    converter< E_NatureCapaciteMissionType >( "NBC, nucleaire",                              eNatureCapaciteMission_NBCNucleaire                           ),
    converter< E_NatureCapaciteMissionType >( "Non renseigne",                               eNatureCapaciteMission_NonRenseigne                           ),
    converter< E_NatureCapaciteMissionType >( "Observation",                                 eNatureCapaciteMission_Observation                            ),
    converter< E_NatureCapaciteMissionType >( "Observation, capteur",                        eNatureCapaciteMission_ObservationCapteur                     ),
    converter< E_NatureCapaciteMissionType >( "Observation, longue portee",                  eNatureCapaciteMission_ObservationLonguePortee                ),
    converter< E_NatureCapaciteMissionType >( "Observation, meteo",                          eNatureCapaciteMission_ObservationMeteo                       ),
    converter< E_NatureCapaciteMissionType >( "Observation, module terrestre",               eNatureCapaciteMission_ObservationModuleTerrestre             ),
    converter< E_NatureCapaciteMissionType >( "purification de l'eau",                       eNatureCapaciteMission_PurificationDeLEau                     ),
    converter< E_NatureCapaciteMissionType >( "Ravitaillement (class I)",                    eNatureCapaciteMission_Ravitaillement_ClasseI                 ),
    converter< E_NatureCapaciteMissionType >( "Ravitaillement (class II)",                   eNatureCapaciteMission_Ravitaillement_ClasseII                ),
    converter< E_NatureCapaciteMissionType >( "Ravitaillement (class III aviation)",         eNatureCapaciteMission_Ravitaillement_ClasseIIIAviation       ),
    converter< E_NatureCapaciteMissionType >( "Ravitaillement (class III)",                  eNatureCapaciteMission_Ravitaillement_ClasseIII               ),
    converter< E_NatureCapaciteMissionType >( "Ravitaillement (class IV)",                   eNatureCapaciteMission_Ravitaillement_ClasseIV                ),
    converter< E_NatureCapaciteMissionType >( "Ravitaillement (class V)",                    eNatureCapaciteMission_Ravitaillement_ClasseV                 ),
    converter< E_NatureCapaciteMissionType >( "Ravitaillement (eau)",                        eNatureCapaciteMission_Ravitaillement_Eau                     ),
    converter< E_NatureCapaciteMissionType >( "Ravitaillement, blanchisserie/bain",          eNatureCapaciteMission_RavitaillementBlanchisserieBain        ),
    converter< E_NatureCapaciteMissionType >( "Recherche & sauvetage",                       eNatureCapaciteMission_RechercheEtSauvetage                   ),
    converter< E_NatureCapaciteMissionType >( "Reconnaissance",                              eNatureCapaciteMission_Reconnaissance                         ),
    converter< E_NatureCapaciteMissionType >( "Reconnaissance aerienne tactique",            eNatureCapaciteMission_ReconnaissanceAerienneTactique         ),
    converter< E_NatureCapaciteMissionType >( "Recuperation",                                eNatureCapaciteMission_Recuperation                           ),
    converter< E_NatureCapaciteMissionType >( "Service du personnel",                        eNatureCapaciteMission_ServiceDuPersonnel                     ),
    converter< E_NatureCapaciteMissionType >( "Service juridique",                           eNatureCapaciteMission_ServiceJuridique                       ),
    converter< E_NatureCapaciteMissionType >( "Support aerien rapproche",                    eNatureCapaciteMission_SupportAerienRapproche                 ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, forward communications",       eNatureCapaciteMission_Transmissions_ForwardCommunications    ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, node centre",                  eNatureCapaciteMission_Transmissions_NodeCentre               ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, node, large extension",        eNatureCapaciteMission_Transmissions_NodeLargeExtension       ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, node, small extension",        eNatureCapaciteMission_Transmissions_NodeSmallExtension       ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, radio relay",                  eNatureCapaciteMission_Transmissions_RadioRelay               ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, radio tactical satellite",     eNatureCapaciteMission_Transmissions_RadioTacticalSatellite   ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, radio teletype",               eNatureCapaciteMission_Transmissions_RadioTeletype            ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, support",                      eNatureCapaciteMission_Transmissions_Support                  ),
    converter< E_NatureCapaciteMissionType >( "Transmissions, radio",                        eNatureCapaciteMission_Transmissions_Radio                    ),
    converter< E_NatureCapaciteMissionType >( "Transport, allocation des mouvements",        eNatureCapaciteMission_Transport_AllocationDesMouvements      ),
    converter< E_NatureCapaciteMissionType >( "Transport, APOD/APOE",                        eNatureCapaciteMission_Transport_APOD_APOE                    ),
    converter< E_NatureCapaciteMissionType >( "Transport, SPOD/SPOE",                        eNatureCapaciteMission_Transport_SPOD_SPOE                    ),
    converter< E_NatureCapaciteMissionType >( "Transport, missile",                          eNatureCapaciteMission_TransportMissile                       ),
    converter< E_NatureCapaciteMissionType >( "Verrouillage de cible",                       eNatureCapaciteMission_VerrouillageDeCible                    ),
    converter< E_NatureCapaciteMissionType >( "Verrouillage de cible, flash",                eNatureCapaciteMission_VerrouillageDeCible_Flash              ),
    converter< E_NatureCapaciteMissionType >( "Verrouillage de cible, radar",                eNatureCapaciteMission_VerrouillageDeCible_Radar              ),
    converter< E_NatureCapaciteMissionType >( "Verrouillage de cible, sonore",               eNatureCapaciteMission_VerrouillageDeCible_Sonore             ),
    converter< E_NatureCapaciteMissionType >( "",                                            (E_NatureCapaciteMissionType)-1                               )    
};


MOS_Tools::converter< E_PerceptionResult > MOS_Tools::perceptionLevelConverter_[] = 
{
    converter< E_PerceptionResult >( "Identification", eIdentification       ),
    converter< E_PerceptionResult >( "Reconnaissance", eRecognition          ),
    converter< E_PerceptionResult >( "Detection"     , eDetection            ),
    converter< E_PerceptionResult >( "Non vu"        , eNotSeen              ),
    converter< E_PerceptionResult >( ""              , (E_PerceptionResult)-1 )
};

MOS_Tools::converter< E_PostureType > MOS_Tools::postureConverter_[] = 
{
    converter< E_PostureType >( "Mouvement"        , ePostureMouvement         ),
    converter< E_PostureType >( "MouvementDiscret" , ePostureMouvementDiscret  ), 
    converter< E_PostureType >( "Arret"            , ePostureArret             ), 
    converter< E_PostureType >( "PosteReflexe"     , ePosturePosteReflexe      ), 
    converter< E_PostureType >( "Poste"            , ePosturePoste             ), 
    converter< E_PostureType >( "PosteAmenage"     , ePosturePosteAmenage      ), 
    converter< E_PostureType >( "PostePrepareGenie", ePosturePostePrepareGenie ), 
    converter< E_PostureType >( "Transporte"       , ePostureTransporte        ), 
    converter< E_PostureType >( ""                 , (E_PostureType)-1         )
};


MOS_Tools::converter< E_WeatherType > MOS_Tools::weatherConverter_[] =
{
    converter< E_WeatherType >( "PasDePrecipitation", eWeatherTypeNone            ),
    converter< E_WeatherType >( "TempeteDeSable"    , eWeatherTypeRainOrSnowStorm ),
    converter< E_WeatherType >( "Brouillard"        , eWeatherTypeFog             ),
    converter< E_WeatherType >( "Crachin"           , eWeatherTypeDrizzle         ),
    converter< E_WeatherType >( "Pluie"             , eWeatherTypeRain            ),
    converter< E_WeatherType >( "Neige"             , eWeatherTypeSnow            ),
    converter< E_WeatherType >( "Fumigene"          , eWeatherTypeSmoke           ),
    converter< E_WeatherType >( ""                  , (E_WeatherType)-1 )
};

MOS_Tools::converter< E_LightingType > MOS_Tools::lightingConverter_[] =
{
    converter< E_LightingType >( "JourSansNuage"         , eLightingJourSansNuage          ),
    converter< E_LightingType >( "JourPeuNuageux"        , eLightingJourPeuNuageux         ),
    converter< E_LightingType >( "JourMoyennementNuageux", eLightingJourMoyennementNuageux ),
    converter< E_LightingType >( "JourAssezNuageux"      , eLightingJourAssezNuageux       ),
    converter< E_LightingType >( "JourTresNuageux"       , eLightingJourTresNuageux        ),
    converter< E_LightingType >( "NuitPleineLune"        , eLightingNuitPleineLune         ),
    converter< E_LightingType >( "NuitTroisQuartDeLune"  , eLightingNuitTroisQuartDeLune   ),
    converter< E_LightingType >( "NuitDemiLune"          , eLightingNuitDemiLune           ),
    converter< E_LightingType >( "NuitQuartDeLune"       , eLightingNuitQuartDeLune        ),
    converter< E_LightingType >( "NuitNouvelleLune"      , eLightingNuitNouvelleLune       ),
    converter< E_LightingType >( "Eclairant"             , eLightingEclairant              ),
    converter< E_LightingType >( ""                      , (E_LightingType)-1 )
};

MOS_Tools::converter< MOS_RawVisionData::E_VisionObject > MOS_Tools::environnementConverter_[] =
{
    converter< MOS_RawVisionData::E_VisionObject >( "Sol"   , MOS_RawVisionData::eVisionGround ),
    converter< MOS_RawVisionData::E_VisionObject >( "Vide"  , MOS_RawVisionData::eVisionEmpty  ),
    converter< MOS_RawVisionData::E_VisionObject >( "Foret" , MOS_RawVisionData::eVisionForest ),
    converter< MOS_RawVisionData::E_VisionObject >( "Urbain", MOS_RawVisionData::eVisionUrban  ),
    converter< MOS_RawVisionData::E_VisionObject >( ""      , (MOS_RawVisionData::E_VisionObject)-1 )
};

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertEnvironementType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertEnvironementType( MOS_RawVisionData::E_VisionObject nObject )
{
    return InverseFindInConverter( environnementConverter_, nObject );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertWeatherType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertWeatherType( E_WeatherType nWeatherType )
{
    return InverseFindInConverter( weatherConverter_, nWeatherType );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertLightingType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertLightingType( E_LightingType nLightingType )
{
    return InverseFindInConverter( lightingConverter_, nLightingType );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::GetLightingFromName
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
E_LightingType MOS_Tools::GetLightingFromName( const std::string& sName )
{
    return FindInConverter( lightingConverter_, sName );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertPostureType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertPostureType( E_PostureType nPostureType )
{
    return InverseFindInConverter( postureConverter_, nPostureType, eConverterSim );
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertMissionType
/** @param  strMissionName 
    @return 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
E_MissionID MOS_Tools::ConvertMissionType( const std::string& strMissionName )
{
    return FindInConverter( missionConverter_, strMissionName );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::GetMissionDisplay
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
std::string MOS_Tools::GetMissionDisplay( E_MissionID nMission )
{
    return InverseFindInConverter( missionConverter_, nMission, eConverterSim );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::GetOrderDisplay
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
std::string MOS_Tools::GetOrderDisplay( E_OrderConduiteID nOrder )
{
    return InverseFindInConverter( orderConverter_, nOrder, eConverterSim );    
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureLevel
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureLevel( E_NatureLevelType n )
{
    return InverseFindInConverter( natureLevelConverter_, n );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureWeapon
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureWeapon( E_NatureWeaponType n )
{
    return InverseFindInConverter( natureWeaponConverter_, n );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureSpecialization
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureSpecialization( E_NatureSpecializationType n )
{
    return InverseFindInConverter( natureSpecializationConverter_, n );  
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureQualifier
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureQualifier( E_NatureQualifierType n )
{
    return InverseFindInConverter( natureQualifierConverter_, n ); 
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureCategory
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureCategory( E_NatureCategoryType n )
{
    return InverseFindInConverter( natureCategoryConverter_, n ); 
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureMobility
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureMobility( E_NatureMobilityType n )
{
    return InverseFindInConverter( natureMobilityConverter_, n );  
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureAtlas
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureAtlas( E_NatureAtlasType n  )
{
    return InverseFindInConverter( natureAtlasConverter_, n ); 
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertMissionCapacity
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertMissionCapacity( E_NatureCapaciteMissionType n )
{
    return InverseFindInConverter( natureCapaciteMissionConverter_, n );
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertPerceptionLevel
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertPerceptionLevel( E_PerceptionResult n )
{
    return InverseFindInConverter( perceptionLevelConverter_, n );     
}

