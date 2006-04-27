// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_Type.h"

#include "Automat/Mission_Automat_ABC_ArmerPointDeControle.cpp"
#include "Automat/Mission_Automat_ABC_OrganiserAccueilColonneRefugies.cpp"
#include "Automat/Mission_Automat_ABC_ProtegerPC.cpp"
#include "Automat/Mission_Automat_ABC_Appuyer.cpp"
#include "Automat/Mission_Automat_ABC_Attaquer.cpp"
#include "Automat/Mission_Automat_ABC_ContreAttaquer.cpp"
#include "Automat/Mission_Automat_ABC_Controler.cpp"
#include "Automat/Mission_Automat_ABC_Couvrir.cpp"
#include "Automat/Mission_Automat_ABC_DonnerCoupArret.cpp"
#include "Automat/Mission_Automat_ABC_Eclairer.cpp"
#include "Automat/Mission_Automat_ABC_EffectuerRaid.cpp"
#include "Automat/Mission_Automat_ABC_Interdire.cpp"
#include "Automat/Mission_Automat_ABC_Jalonner.cpp"
#include "Automat/Mission_Automat_ABC_PrendrePreciserContact.cpp"
#include "Automat/Mission_Automat_ABC_Reconnaitre.cpp"
#include "Automat/Mission_Automat_ABC_ReconnaitrePoint.cpp"
#include "Automat/Mission_Automat_ABC_Reduire.cpp"
#include "Automat/Mission_Automat_ABC_ReleverEnDepassant.cpp"
#include "Automat/Mission_Automat_ABC_Semparer.cpp"
#include "Automat/Mission_Automat_ABC_Tenir.cpp"
#include "Automat/Mission_Automat_ABC_Soutenir.cpp"
#include "Automat/Mission_Automat_ABC_Surveiller.cpp"
#include "Automat/Mission_Automat_ABC_FlancGarder.cpp"
#include "Automat/Mission_Automat_ABC_RenseignerSur.cpp"
#include "Automat/Mission_Automat_ABC_Fixer.cpp"
#include "Automat/Mission_Automat_ABC_Freiner.cpp"
#include "Automat/Mission_Automat_ABC_SurveillerItineraire.cpp"
#include "Automat/Mission_Automat_ABC_RenseignerSurUnAxe.cpp"
#include "Automat/Mission_Automat_ABC_RecueillirUnite.cpp"
#include "Automat/Mission_Automat_ABC_EscorterUnite.cpp"
#include "Automat/Mission_Automat_ABC_EscorterUnites.cpp"
#include "Automat/Mission_Automat_ABC_AppuyerSurPosition.cpp"
#include "Automat/Mission_Automat_ALAT_Heliporter.cpp"
#include "Automat/Mission_Automat_ALAT_Jalonner.cpp"
#include "Automat/Mission_Automat_ALAT_Helitransporter.cpp"
#include "Automat/Mission_Automat_ALAT_ExtrairePersonnel.cpp"
#include "Automat/Mission_Automat_ALAT_Appuyer.cpp"
#include "Automat/Mission_Automat_ALAT_Couvrir.cpp"
#include "Automat/Mission_Automat_ALAT_Surveiller.cpp"
#include "Automat/Mission_Automat_ALAT_Reconnaitre.cpp"
#include "Automat/Mission_Automat_ALAT_Freiner.cpp"
#include "Automat/Mission_Automat_ALAT_Escorter.cpp"
#include "Automat/Mission_Automat_ALAT_Attaquer.cpp"
#include "Automat/Mission_Automat_ALAT_RecueillirFormationAeromobile.cpp"
#include "Automat/Mission_Automat_ALAT_AttaquerFormationAeromobile.cpp"
#include "Automat/Mission_Automat_ALAT_DonnerCoupArret.cpp"
#include "Automat/Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur.cpp"
#include "Automat/Mission_Automat_ALAT_DetruireNeutraliserDansZone.cpp"
#include "Automat/Mission_Automat_ALAT_EffectuerRechercheEtSauvetage.cpp"
#include "Automat/Mission_Automat_ASA_DefendreZone.cpp"
#include "Automat/Mission_Automat_ASA_DefendreSite.cpp"
#include "Automat/Mission_Automat_ASA_Surveiller.cpp"
#include "Automat/Mission_Automat_ASA_DefendreItineraire.cpp"
#include "Automat/Mission_Automat_ASA_MISTRAL_DefendreZone.cpp"
#include "Automat/Mission_Automat_ASA_MISTRAL_DefendreSite.cpp"
#include "Automat/Mission_Automat_ASA_MISTRAL_Surveiller.cpp"
#include "Automat/Mission_Automat_ASA_HAWK_DefendreZone.cpp"
#include "Automat/Mission_Automat_ASA_HAWK_DefendreSite.cpp"
#include "Automat/Mission_Automat_ASA_HAWK_Surveiller.cpp"
#include "Automat/Mission_Automat_ASA_DefendreUnites.cpp"
#include "Automat/Mission_Automat_ASS_SeDeployer.cpp"
#include "Automat/Mission_Automat_ASS_ReconnaitreZonesDeploiement.cpp"
#include "Automat/Mission_Automat_ASS_AcquerirObjectifs.cpp"
#include "Automat/Mission_Automat_GEN_SeDeployer.cpp"
#include "Automat/Mission_Automat_GEN_FaireFranchir.cpp"
#include "Automat/Mission_Automat_GEN_RealiserSystemeObstacles.cpp"
#include "Automat/Mission_Automat_GEN_FranchirDeViveForce.cpp"
#include "Automat/Mission_Automat_GEN_RetablirItineraires.cpp"
#include "Automat/Mission_Automat_GEN_AmenagerAireStationnement.cpp"
#include "Automat/Mission_Automat_GEN_AmenagerTerrainPoserSommaire.cpp"
#include "Automat/Mission_Automat_GEN_RealiserZonePoser.cpp"
#include "Automat/Mission_Automat_GEN_ReconnaitreZoneDeploiement.cpp"
#include "Automat/Mission_Automat_GEN_RealiserCampPrisonniers.cpp"
#include "Automat/Mission_Automat_GEN_RealiserCampRefugies.cpp"
#include "Automat/Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial.cpp"
#include "Automat/Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation.cpp"
#include "Automat/Mission_Automat_GEN_AppuyerDebarquementTroupes.cpp"
#include "Automat/Mission_Automat_GEN_OuvrirAxesProgression.cpp"
#include "Automat/Mission_Automat_INF_AttaquerSouplesse.cpp"
#include "Automat/Mission_Automat_INF_Appuyer.cpp"
#include "Automat/Mission_Automat_INF_Controler.cpp"
#include "Automat/Mission_Automat_INF_Couvrir.cpp"
#include "Automat/Mission_Automat_INF_DonnerCoupArret.cpp"
#include "Automat/Mission_Automat_INF_Eclairer.cpp"
#include "Automat/Mission_Automat_INF_FlancGarder.cpp"
#include "Automat/Mission_Automat_INF_Freiner.cpp"
#include "Automat/Mission_Automat_INF_Harceler.cpp"
#include "Automat/Mission_Automat_INF_Interdire.cpp"
#include "Automat/Mission_Automat_INF_MenerDefenseUsure.cpp"
#include "Automat/Mission_Automat_INF_PrendrePreciserMaintenirContact.cpp"
#include "Automat/Mission_Automat_INF_ReconnaissanceOffensive.cpp"
#include "Automat/Mission_Automat_INF_RecueillirUnite.cpp"
#include "Automat/Mission_Automat_INF_Reduire.cpp"
#include "Automat/Mission_Automat_INF_Relever.cpp"
#include "Automat/Mission_Automat_INF_RompreContact.cpp"
#include "Automat/Mission_Automat_INF_Surveiller.cpp"
#include "Automat/Mission_Automat_INF_Semparer.cpp"
#include "Automat/Mission_Automat_INF_Fixer.cpp"
#include "Automat/Mission_Automat_INF_AttaquerEnTerrainDifficile.cpp"
#include "Automat/Mission_Automat_INF_BarrerDirection.cpp"
#include "Automat/Mission_Automat_INF_DefendreFerme.cpp"
#include "Automat/Mission_Automat_INF_Tenir.cpp"
#include "Automat/Mission_Automat_INF_AppuyerUnFreinage.cpp"
#include "Automat/Mission_Automat_INF_AppuyerUneAttaque.cpp"
#include "Automat/Mission_Automat_INF_Soutenir.cpp"
#include "Automat/Mission_Automat_INF_EscorterUnite.cpp"
#include "Automat/Mission_Automat_INF_EscorterUnites.cpp"
#include "Automat/Mission_Automat_INF_AppuyerSurPosition.cpp"
#include "Automat/Mission_Automat_LOG_SeDeployer.cpp"
#include "Automat/Mission_Automat_LOG_AppuyerMouvement.cpp"
#include "Automat/Mission_Automat_LOG_ReconnaitreItineraire.cpp"
#include "Automat/Mission_Automat_LOG_Surveiller.cpp"
#include "Automat/Mission_Automat_LOG_TransporterUnites.cpp"
#include "Automat/Mission_Automat_LOG_AppuyerMouvementDansZone.cpp"
#include "Automat/Mission_Automat_LOG_MettreEnOeuvreZoneStationnement.cpp"
#include "Automat/Mission_Automat_LOG_AppuyerFranchissementDansZone.cpp"
#include "Automat/Mission_Automat_LOG_BloquerZone.cpp"
#include "Automat/Mission_Automat_LOG_ReconnaitreZoneContaminee.cpp"
#include "Automat/Mission_Automat_LOG_ReconnaitreZoneDeDeploiement.cpp"
#include "Automat/Mission_Automat_LOG_ArmerPIAs.cpp"
#include "Automat/Mission_Automat_Test_MoveTo.cpp"
#include "Automat/Mission_Automat_Decrocher.cpp"
#include "Automat/Mission_Automat_ReleverSurPlace.cpp"
#include "Automat/Mission_Automat_FaireMouvement.cpp"
#include "Automat/Mission_Automat_SeFaireDecontaminer.cpp"
#include "Automat/Mission_Automat_Franchir.cpp"
#include "Automat/Mission_Automat_Stationner.cpp"
#include "Automat/Mission_Automat_SeFaireTransporter.cpp"
#include "Automat/Mission_Automat_SeRecompleter.cpp"
#include "Automat/Mission_Automat_SeRendre.cpp"
#include "Automat/Mission_Automat_FaireMouvementVersCampRefugies.cpp"
#include "Automat/Mission_Automat_InterdireFranchissementPopulations.cpp"
#include "Automat/Mission_Automat_FiltrerPopulations.cpp"
#include "Automat/Mission_Automat_CanaliserPopulations.cpp"
#include "Automat/Mission_Automat_ControlerPopulationsDansZone.cpp"
#include "Automat/Mission_Automat_SecuriserZoneContrePopulations.cpp"
#include "Automat/Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone.cpp"
#include "Automat/Mission_Automat_Generique.cpp"
#include "Automat/Mission_Automat_NBC_ReconnaitreUnAxe.cpp"
#include "Automat/Mission_Automat_NBC_ReconnaitreUneZone.cpp"
#include "Automat/Mission_Automat_NBC_ArmerUnSiteDeDecontamination.cpp"
#include "Automat/Mission_Automat_NBC_DecontaminerUneZone.cpp"
#include "Automat/Mission_Automat_NBC_ReconnaitreDesSitesROTA.cpp"
#include "Automat/Mission_Automat_RENS_ROHUM_RenseignerSur.cpp"
#include "Automat/Mission_Automat_RENS_ROHUM_OrienterGuider.cpp"
#include "Automat/Mission_Automat_RENS_ROHUM_SExfiltrer.cpp"
#include "Automat/Mission_Automat_RENS_ROIM_CL289_RenseignerSur.cpp"
#include "Automat/Mission_Automat_RENS_ROIM_SDTI_RenseignerSur.cpp"
#include "Automat/Mission_Automat_RENS_ROEM_Appuyer.cpp"
#include "Automat/Mission_Automat_RENS_ROEM_RenseignerSur.cpp"
#include "Automat/Mission_Automat_RENS_RADINT_RenseignerSur.cpp"
#include "Automat/Mission_Automat_TRANS_Commander.cpp"


using namespace TEST;

Mission_Automat_Type::T_MissionAutomatTypeMap Mission_Automat_Type::missions_;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Automat_Type::Mission_Automat_Type( const std::string& strName, T_MissionAllocator missionAllocator )
    : strName_          ( strName )
    , missionAllocator_ ( missionAllocator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Automat_Type::~Mission_Automat_Type()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type::Initialize
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Automat_Type::Initialize()
{
    Register< Mission_Automat_ABC_ArmerPointDeControle >( "Automate ABC ArmerPointDeControle" );
    Register< Mission_Automat_ABC_OrganiserAccueilColonneRefugies >( "Automate ABC OrganiserAccueilColonneRefugies" );
    Register< Mission_Automat_ABC_ProtegerPC >( "Automate ABC ProtegerPC" );
    Register< Mission_Automat_ABC_Appuyer >( "Automate ABC Appuyer" );
    Register< Mission_Automat_ABC_Attaquer >( "Automate ABC Attaquer" );
    Register< Mission_Automat_ABC_ContreAttaquer >( "Automate ABC ContreAttaquer" );
    Register< Mission_Automat_ABC_Controler >( "Automate ABC Controler" );
    Register< Mission_Automat_ABC_Couvrir >( "Automate ABC Couvrir" );
    Register< Mission_Automat_ABC_DonnerCoupArret >( "Automate ABC DonnerCoupArret" );
    Register< Mission_Automat_ABC_Eclairer >( "Automate ABC Eclairer" );
    Register< Mission_Automat_ABC_EffectuerRaid >( "Automate ABC EffectuerRaid" );
    Register< Mission_Automat_ABC_Interdire >( "Automate ABC Interdire" );
    Register< Mission_Automat_ABC_Jalonner >( "Automate ABC Jalonner" );
    Register< Mission_Automat_ABC_PrendrePreciserContact >( "Automate ABC PrendrePreciserContact" );
    Register< Mission_Automat_ABC_Reconnaitre >( "Automate ABC Reconnaitre" );
    Register< Mission_Automat_ABC_ReconnaitrePoint >( "Automate ABC ReconnaitrePoint" );
    Register< Mission_Automat_ABC_Reduire >( "Automate ABC Reduire" );
    Register< Mission_Automat_ABC_ReleverEnDepassant >( "Automate ABC ReleverEnDepassant" );
    Register< Mission_Automat_ABC_Semparer >( "Automate ABC Semparer" );
    Register< Mission_Automat_ABC_Tenir >( "Automate ABC Tenir" );
    Register< Mission_Automat_ABC_Soutenir >( "Automate ABC Soutenir" );
    Register< Mission_Automat_ABC_Surveiller >( "Automate ABC Surveiller" );
    Register< Mission_Automat_ABC_FlancGarder >( "Automate ABC FlancGarder" );
    Register< Mission_Automat_ABC_RenseignerSur >( "Automate ABC RenseignerSur" );
    Register< Mission_Automat_ABC_Fixer >( "Automate ABC Fixer" );
    Register< Mission_Automat_ABC_Freiner >( "Automate ABC Freiner" );
    Register< Mission_Automat_ABC_SurveillerItineraire >( "Automate ABC SurveillerItineraire" );
    Register< Mission_Automat_ABC_RenseignerSurUnAxe >( "Automate ABC RenseignerSurUnAxe" );
    Register< Mission_Automat_ABC_RecueillirUnite >( "Automate ABC RecueillirUnite" );
    Register< Mission_Automat_ABC_EscorterUnite >( "Automate ABC EscorterUnite" );
    Register< Mission_Automat_ABC_EscorterUnites >( "Automate ABC EscorterUnites" );
    Register< Mission_Automat_ABC_AppuyerSurPosition >( "Automate ABC AppuyerSurPosition" );
    Register< Mission_Automat_ALAT_Heliporter >( "Automate ALAT Heliporter" );
    Register< Mission_Automat_ALAT_Jalonner >( "Automate ALAT Jalonner" );
    Register< Mission_Automat_ALAT_Helitransporter >( "Automate ALAT Helitransporter" );
    Register< Mission_Automat_ALAT_ExtrairePersonnel >( "Automate ALAT ExtrairePersonnel" );
    Register< Mission_Automat_ALAT_Appuyer >( "Automate ALAT Appuyer" );
    Register< Mission_Automat_ALAT_Couvrir >( "Automate ALAT Couvrir" );
    Register< Mission_Automat_ALAT_Surveiller >( "Automate ALAT Surveiller" );
    Register< Mission_Automat_ALAT_Reconnaitre >( "Automate ALAT Reconnaitre" );
    Register< Mission_Automat_ALAT_Freiner >( "Automate ALAT Freiner" );
    Register< Mission_Automat_ALAT_Escorter >( "Automate ALAT Escorter" );
    Register< Mission_Automat_ALAT_Attaquer >( "Automate ALAT Attaquer" );
    Register< Mission_Automat_ALAT_RecueillirFormationAeromobile >( "Automate ALAT RecueillirFormationAeromobile" );
    Register< Mission_Automat_ALAT_AttaquerFormationAeromobile >( "Automate ALAT AttaquerFormationAeromobile" );
    Register< Mission_Automat_ALAT_DonnerCoupArret >( "Automate ALAT DonnerCoupArret" );
    Register< Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur >( "Automate ALAT DetruireNeutraliserDansProfondeur" );
    Register< Mission_Automat_ALAT_DetruireNeutraliserDansZone >( "Automate ALAT DetruireNeutraliserDansZone" );
    Register< Mission_Automat_ALAT_EffectuerRechercheEtSauvetage >( "Automate ALAT EffectuerRechercheEtSauvetage" );
    Register< Mission_Automat_ASA_DefendreZone >( "Automate ASA DefendreZone" );
    Register< Mission_Automat_ASA_DefendreSite >( "Automate ASA DefendreSite" );
    Register< Mission_Automat_ASA_Surveiller >( "Automate ASA Surveiller" );
    Register< Mission_Automat_ASA_DefendreItineraire >( "Automate ASA DefendreItineraire" );
    Register< Mission_Automat_ASA_MISTRAL_DefendreZone >( "Automate ASA MISTRAL DefendreZone" );
    Register< Mission_Automat_ASA_MISTRAL_DefendreSite >( "Automate ASA MISTRAL DefendreSite" );
    Register< Mission_Automat_ASA_MISTRAL_Surveiller >( "Automate ASA MISTRAL Surveiller" );
    Register< Mission_Automat_ASA_HAWK_DefendreZone >( "Automate ASA HAWK DefendreZone" );
    Register< Mission_Automat_ASA_HAWK_DefendreSite >( "Automate ASA HAWK DefendreSite" );
    Register< Mission_Automat_ASA_HAWK_Surveiller >( "Automate ASA HAWK Surveiller" );
    Register< Mission_Automat_ASA_DefendreUnites >( "Automate ASA DefendreUnites" );
    Register< Mission_Automat_ASS_SeDeployer >( "Automate ASS SeDeployer" );
    Register< Mission_Automat_ASS_ReconnaitreZonesDeploiement >( "Automate ASS ReconnaitreZonesDeploiement" );
    Register< Mission_Automat_ASS_AcquerirObjectifs >( "Automate ASS AcquerirObjectifs" );
    Register< Mission_Automat_GEN_SeDeployer >( "Automate GEN SeDeployer" );
    Register< Mission_Automat_GEN_FaireFranchir >( "Automate GEN FaireFranchir" );
    Register< Mission_Automat_GEN_RealiserSystemeObstacles >( "Automate GEN RealiserSystemeObstacles" );
    Register< Mission_Automat_GEN_FranchirDeViveForce >( "Automate GEN FranchirDeViveForce" );
    Register< Mission_Automat_GEN_RetablirItineraires >( "Automate GEN RetablirItineraires" );
    Register< Mission_Automat_GEN_AmenagerAireStationnement >( "Automate GEN AmenagerAireStationnement" );
    Register< Mission_Automat_GEN_AmenagerTerrainPoserSommaire >( "Automate GEN AmenagerTerrainPoserSommaire" );
    Register< Mission_Automat_GEN_RealiserZonePoser >( "Automate GEN RealiserZonePoser" );
    Register< Mission_Automat_GEN_ReconnaitreZoneDeploiement >( "Automate GEN ReconnaitreZoneDeploiement" );
    Register< Mission_Automat_GEN_RealiserCampPrisonniers >( "Automate GEN RealiserCampPrisonniers" );
    Register< Mission_Automat_GEN_RealiserCampRefugies >( "Automate GEN RealiserCampRefugies" );
    Register< Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial >( "Automate GEN RealiserPlatesformesSommairesDeDeploiementInitial" );
    Register< Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation >( "Automate GEN RealiserTravauxSommairesAuProfitPopulation" );
    Register< Mission_Automat_GEN_AppuyerDebarquementTroupes >( "Automate GEN AppuyerDebarquementTroupes" );
    Register< Mission_Automat_GEN_OuvrirAxesProgression >( "Automate GEN OuvrirAxesProgression" );
    Register< Mission_Automat_INF_AttaquerSouplesse >( "Automate INF AttaquerSouplesse" );
    Register< Mission_Automat_INF_Appuyer >( "Automate INF Appuyer" );
    Register< Mission_Automat_INF_Controler >( "Automate INF Controler" );
    Register< Mission_Automat_INF_Couvrir >( "Automate INF Couvrir" );
    Register< Mission_Automat_INF_DonnerCoupArret >( "Automate INF DonnerCoupArret" );
    Register< Mission_Automat_INF_Eclairer >( "Automate INF Eclairer" );
    Register< Mission_Automat_INF_FlancGarder >( "Automate INF FlancGarder" );
    Register< Mission_Automat_INF_Freiner >( "Automate INF Freiner" );
    Register< Mission_Automat_INF_Harceler >( "Automate INF Harceler" );
    Register< Mission_Automat_INF_Interdire >( "Automate INF Interdire" );
    Register< Mission_Automat_INF_MenerDefenseUsure >( "Automate INF MenerDefenseUsure" );
    Register< Mission_Automat_INF_PrendrePreciserMaintenirContact >( "Automate INF PrendrePreciserMaintenirContact" );
    Register< Mission_Automat_INF_ReconnaissanceOffensive >( "Automate INF ReconnaissanceOffensive" );
    Register< Mission_Automat_INF_RecueillirUnite >( "Automate INF RecueillirUnite" );
    Register< Mission_Automat_INF_Reduire >( "Automate INF Reduire" );
    Register< Mission_Automat_INF_Relever >( "Automate INF Relever" );
    Register< Mission_Automat_INF_RompreContact >( "Automate INF RompreContact" );
    Register< Mission_Automat_INF_Surveiller >( "Automate INF Surveiller" );
    Register< Mission_Automat_INF_Semparer >( "Automate INF Semparer" );
    Register< Mission_Automat_INF_Fixer >( "Automate INF Fixer" );
    Register< Mission_Automat_INF_AttaquerEnTerrainDifficile >( "Automate INF AttaquerEnTerrainDifficile" );
    Register< Mission_Automat_INF_BarrerDirection >( "Automate INF BarrerDirection" );
    Register< Mission_Automat_INF_DefendreFerme >( "Automate INF DefendreFerme" );
    Register< Mission_Automat_INF_Tenir >( "Automate INF Tenir" );
    Register< Mission_Automat_INF_AppuyerUnFreinage >( "Automate INF AppuyerUnFreinage" );
    Register< Mission_Automat_INF_AppuyerUneAttaque >( "Automate INF AppuyerUneAttaque" );
    Register< Mission_Automat_INF_Soutenir >( "Automate INF Soutenir" );
    Register< Mission_Automat_INF_EscorterUnite >( "Automate INF EscorterUnite" );
    Register< Mission_Automat_INF_EscorterUnites >( "Automate INF EscorterUnites" );
    Register< Mission_Automat_INF_AppuyerSurPosition >( "Automate INF AppuyerSurPosition" );
    Register< Mission_Automat_LOG_SeDeployer >( "Automate LOG SeDeployer" );
    Register< Mission_Automat_LOG_AppuyerMouvement >( "Automate LOG AppuyerMouvement" );
    Register< Mission_Automat_LOG_ReconnaitreItineraire >( "Automate LOG ReconnaitreItineraire" );
    Register< Mission_Automat_LOG_Surveiller >( "Automate LOG Surveiller" );
    Register< Mission_Automat_LOG_TransporterUnites >( "Automate LOG TransporterUnites" );
    Register< Mission_Automat_LOG_AppuyerMouvementDansZone >( "Automate LOG AppuyerMouvementDansZone" );
    Register< Mission_Automat_LOG_MettreEnOeuvreZoneStationnement >( "Automate LOG MettreEnOeuvreZoneStationnement" );
    Register< Mission_Automat_LOG_AppuyerFranchissementDansZone >( "Automate LOG AppuyerFranchissementDansZone" );
    Register< Mission_Automat_LOG_BloquerZone >( "Automate LOG BloquerZone" );
    Register< Mission_Automat_LOG_ReconnaitreZoneContaminee >( "Automate LOG ReconnaitreZoneContaminee" );
    Register< Mission_Automat_LOG_ReconnaitreZoneDeDeploiement >( "Automate LOG ReconnaitreZoneDeDeploiement" );
    Register< Mission_Automat_LOG_ArmerPIAs >( "Automate LOG ArmerPIAs" );
    Register< Mission_Automat_Test_MoveTo >( "Automate Test MoveTo" );
    Register< Mission_Automat_Decrocher >( "Automate Decrocher" );
    Register< Mission_Automat_ReleverSurPlace >( "Automate ReleverSurPlace" );
    Register< Mission_Automat_FaireMouvement >( "Automate FaireMouvement" );
    Register< Mission_Automat_SeFaireDecontaminer >( "Automate SeFaireDecontaminer" );
    Register< Mission_Automat_Franchir >( "Automate Franchir" );
    Register< Mission_Automat_Stationner >( "Automate Stationner" );
    Register< Mission_Automat_SeFaireTransporter >( "Automate SeFaireTransporter" );
    Register< Mission_Automat_SeRecompleter >( "Automate SeRecompleter" );
    Register< Mission_Automat_SeRendre >( "Automate SeRendre" );
    Register< Mission_Automat_FaireMouvementVersCampRefugies >( "Automate FaireMouvementVersCampRefugies" );
    Register< Mission_Automat_InterdireFranchissementPopulations >( "Automate InterdireFranchissementPopulations" );
    Register< Mission_Automat_FiltrerPopulations >( "Automate FiltrerPopulations" );
    Register< Mission_Automat_CanaliserPopulations >( "Automate CanaliserPopulations" );
    Register< Mission_Automat_ControlerPopulationsDansZone >( "Automate ControlerPopulationsDansZone" );
    Register< Mission_Automat_SecuriserZoneContrePopulations >( "Automate SecuriserZoneContrePopulations" );
    Register< Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone >( "Automate ASY CommettreExactionsSurPopulationDansZone" );
    Register< Mission_Automat_Generique >( "Automate Generique" );
    Register< Mission_Automat_NBC_ReconnaitreUnAxe >( "Automate NBC ReconnaitreUnAxe" );
    Register< Mission_Automat_NBC_ReconnaitreUneZone >( "Automate NBC ReconnaitreUneZone" );
    Register< Mission_Automat_NBC_ArmerUnSiteDeDecontamination >( "Automate NBC ArmerUnSiteDeDecontamination" );
    Register< Mission_Automat_NBC_DecontaminerUneZone >( "Automate NBC DecontaminerUneZone" );
    Register< Mission_Automat_NBC_ReconnaitreDesSitesROTA >( "Automate NBC ReconnaitreDesSitesROTA" );
    Register< Mission_Automat_RENS_ROHUM_RenseignerSur >( "Automate RENS ROHUM RenseignerSur" );
    Register< Mission_Automat_RENS_ROHUM_OrienterGuider >( "Automate RENS ROHUM OrienterGuider" );
    Register< Mission_Automat_RENS_ROHUM_SExfiltrer >( "Automate RENS ROHUM SExfiltrer" );
    Register< Mission_Automat_RENS_ROIM_CL289_RenseignerSur >( "Automate RENS ROIM CL289 RenseignerSur" );
    Register< Mission_Automat_RENS_ROIM_SDTI_RenseignerSur >( "Automate RENS ROIM SDTI RenseignerSur" );
    Register< Mission_Automat_RENS_ROEM_Appuyer >( "Automate RENS ROEM Appuyer" );
    Register< Mission_Automat_RENS_ROEM_RenseignerSur >( "Automate RENS ROEM RenseignerSur" );
    Register< Mission_Automat_RENS_RADINT_RenseignerSur >( "Automate RENS RADINT RenseignerSur" );
    Register< Mission_Automat_TRANS_Commander >( "Automate TRANS Commander" );

}
    
// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type::Terminate
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Automat_Type::Terminate()
{
    for( CIT_MissionAutomatTypeMap it = missions_.begin(); it != missions_.end(); ++it )
        delete it->second;
    missions_.clear();
}
