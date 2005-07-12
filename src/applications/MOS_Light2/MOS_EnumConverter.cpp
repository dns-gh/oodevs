// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGR 2005-5-11 - 10:0:21 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/MOS_EnumConverter_Skeleton.cpp $
// $Author: Age $
// $Modtime: 22/10/04 17:12 $
// $Revision: 1 $
// $Workfile: MOS_EnumConverter_Skeleton.cpp $
//
// *****************************************************************************

#include "MOS_pch.h"
#include "MOS_EnumConverter.h"

// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::AutomataMissionToString
// Created: AGR 2005-5-11 - 10:0:21
// -----------------------------------------------------------------------------
QString MOS_EnumConverter::AutomataMissionToString( unsigned int nAutomataMission )
{
    QObject a;
    switch( nAutomataMission )
    {
        case eMission_Automate_ABC_ArmerPointDeControle : return a.tr( "Automate ABC ArmerPointDeControle" );
        case eMission_Automate_ABC_OrganiserAccueilColonneRefugies : return a.tr( "Automate ABC OrganiserAccueilColonneRefugies" );
        case eMission_Automate_ABC_ProtegerPC : return a.tr( "Automate ABC ProtegerPC" );
        case eMission_Automate_ABC_Appuyer : return a.tr( "Automate ABC Appuyer" );
        case eMission_Automate_ABC_Attaquer : return a.tr( "Automate ABC Attaquer" );
        case eMission_Automate_ABC_ContreAttaquer : return a.tr( "Automate ABC ContreAttaquer" );
        case eMission_Automate_ABC_Controler : return a.tr( "Automate ABC Controler" );
        case eMission_Automate_ABC_Couvrir : return a.tr( "Automate ABC Couvrir" );
        case eMission_Automate_ABC_DonnerCoupArret : return a.tr( "Automate ABC DonnerCoupArret" );
        case eMission_Automate_ABC_Eclairer : return a.tr( "Automate ABC Eclairer" );
        case eMission_Automate_ABC_EffectuerRaid : return a.tr( "Automate ABC EffectuerRaid" );
        case eMission_Automate_ABC_Interdire : return a.tr( "Automate ABC Interdire" );
        case eMission_Automate_ABC_Jalonner : return a.tr( "Automate ABC Jalonner" );
        case eMission_Automate_ABC_PrendrePreciserContact : return a.tr( "Automate ABC PrendrePreciserContact" );
        case eMission_Automate_ABC_Reconnaitre : return a.tr( "Automate ABC Reconnaitre" );
        case eMission_Automate_ABC_ReconnaitrePoint : return a.tr( "Automate ABC ReconnaitrePoint" );
        case eMission_Automate_ABC_Reduire : return a.tr( "Automate ABC Reduire" );
        case eMission_Automate_ABC_ReleverEnDepassant : return a.tr( "Automate ABC ReleverEnDepassant" );
        case eMission_Automate_ABC_Semparer : return a.tr( "Automate ABC Semparer" );
        case eMission_Automate_ABC_Tenir : return a.tr( "Automate ABC Tenir" );
        case eMission_Automate_ABC_Soutenir : return a.tr( "Automate ABC Soutenir" );
        case eMission_Automate_ABC_Surveiller : return a.tr( "Automate ABC Surveiller" );
        case eMission_Automate_ABC_FlancGarder : return a.tr( "Automate ABC FlancGarder" );
        case eMission_Automate_ABC_RenseignerSur : return a.tr( "Automate ABC RenseignerSur" );
        case eMission_Automate_ABC_Fixer : return a.tr( "Automate ABC Fixer" );
        case eMission_Automate_ABC_Freiner : return a.tr( "Automate ABC Freiner" );
        case eMission_Automate_ABC_SurveillerItineraire : return a.tr( "Automate ABC SurveillerItineraire" );
        case eMission_Automate_ABC_RenseignerSurUnAxe : return a.tr( "Automate ABC RenseignerSurUnAxe" );
        case eMission_Automate_ABC_RecueillirUnite : return a.tr( "Automate ABC RecueillirUnite" );
        case eMission_Automate_ABC_EscorterUnConvoi : return a.tr( "Automate ABC EscorterUnConvoi" );
        case eMission_Automate_ALAT_Heliporter : return a.tr( "Automate ALAT Heliporter" );
        case eMission_Automate_ALAT_Jalonner : return a.tr( "Automate ALAT Jalonner" );
        case eMission_Automate_ALAT_Helitransporter : return a.tr( "Automate ALAT Helitransporter" );
        case eMission_Automate_ALAT_ExtrairePersonnel : return a.tr( "Automate ALAT ExtrairePersonnel" );
        case eMission_Automate_ALAT_Appuyer : return a.tr( "Automate ALAT Appuyer" );
        case eMission_Automate_ALAT_Couvrir : return a.tr( "Automate ALAT Couvrir" );
        case eMission_Automate_ALAT_Surveiller : return a.tr( "Automate ALAT Surveiller" );
        case eMission_Automate_ALAT_Reconnaitre : return a.tr( "Automate ALAT Reconnaitre" );
        case eMission_Automate_ALAT_Freiner : return a.tr( "Automate ALAT Freiner" );
        case eMission_Automate_ALAT_Escorter : return a.tr( "Automate ALAT Escorter" );
        case eMission_Automate_ALAT_Attaquer : return a.tr( "Automate ALAT Attaquer" );
        case eMission_Automate_ALAT_RecueillirFormationAeromobile : return a.tr( "Automate ALAT RecueillirFormationAeromobile" );
        case eMission_Automate_ALAT_AttaquerFormationAeromobile : return a.tr( "Automate ALAT AttaquerFormationAeromobile" );
        case eMission_Automate_ALAT_DonnerCoupArret : return a.tr( "Automate ALAT DonnerCoupArret" );
        case eMission_Automate_ALAT_DetruireNeutraliserDansProfondeur : return a.tr( "Automate ALAT DetruireNeutraliserDansProfondeur" );
        case eMission_Automate_ALAT_DetruireNeutraliserDansZone : return a.tr( "Automate ALAT DetruireNeutraliserDansZone" );
        case eMission_Automate_ALAT_EffectuerRechercheEtSauvetage : return a.tr( "Automate ALAT EffectuerRechercheEtSauvetage" );
        case eMission_Automate_ASA_DefendreZone : return a.tr( "Automate ASA DefendreZone" );
        case eMission_Automate_ASA_DefendreSite : return a.tr( "Automate ASA DefendreSite" );
        case eMission_Automate_ASA_Surveiller : return a.tr( "Automate ASA Surveiller" );
        case eMission_Automate_ASA_DefendreItineraire : return a.tr( "Automate ASA DefendreItineraire" );
        case eMission_Automate_ASA_MISTRAL_DefendreZone : return a.tr( "Automate ASA MISTRAL DefendreZone" );
        case eMission_Automate_ASA_MISTRAL_DefendreSite : return a.tr( "Automate ASA MISTRAL DefendreSite" );
        case eMission_Automate_ASA_MISTRAL_Surveiller : return a.tr( "Automate ASA MISTRAL Surveiller" );
        case eMission_Automate_ASA_HAWK_DefendreZone : return a.tr( "Automate ASA HAWK DefendreZone" );
        case eMission_Automate_ASA_HAWK_DefendreSite : return a.tr( "Automate ASA HAWK DefendreSite" );
        case eMission_Automate_ASA_HAWK_Surveiller : return a.tr( "Automate ASA HAWK Surveiller" );
        case eMission_Automate_ASA_DefendreUnites : return a.tr( "Automate ASA DefendreUnites" );
        case eMission_Automate_ASS_SeDeployer : return a.tr( "Automate ASS SeDeployer" );
        case eMission_Automate_ASS_ReconnaitreZonesDeploiement : return a.tr( "Automate ASS ReconnaitreZonesDeploiement" );
        case eMission_Automate_ASS_AcquerirObjectifs : return a.tr( "Automate ASS AcquerirObjectifs" );
        case eMission_Automate_GEN_SeDeployer : return a.tr( "Automate GEN SeDeployer" );
        case eMission_Automate_GEN_FaireFranchir : return a.tr( "Automate GEN FaireFranchir" );
        case eMission_Automate_GEN_RealiserSystemeObstacles : return a.tr( "Automate GEN RealiserSystemeObstacles" );
        case eMission_Automate_GEN_FranchirDeViveForce : return a.tr( "Automate GEN FranchirDeViveForce" );
        case eMission_Automate_GEN_RetablirItineraires : return a.tr( "Automate GEN RetablirItineraires" );
        case eMission_Automate_GEN_AmenagerAireStationnement : return a.tr( "Automate GEN AmenagerAireStationnement" );
        case eMission_Automate_GEN_AmenagerTerrainPoserSommaire : return a.tr( "Automate GEN AmenagerTerrainPoserSommaire" );
        case eMission_Automate_GEN_RealiserZonePoser : return a.tr( "Automate GEN RealiserZonePoser" );
        case eMission_Automate_GEN_ReconnaitreZoneDeploiement : return a.tr( "Automate GEN ReconnaitreZoneDeploiement" );
        case eMission_Automate_GEN_RealiserCampPrisonniers : return a.tr( "Automate GEN RealiserCampPrisonniers" );
        case eMission_Automate_GEN_RealiserCampRefugies : return a.tr( "Automate GEN RealiserCampRefugies" );
        case eMission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial : return a.tr( "Automate GEN RealiserPlatesformesSommairesDeDeploiementInitial" );
        case eMission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation : return a.tr( "Automate GEN RealiserTravauxSommairesAuProfitPopulation" );
        case eMission_Automate_GEN_AppuyerDebarquementTroupes : return a.tr( "Automate GEN AppuyerDebarquementTroupes" );
        case eMission_Automate_GEN_OuvrirAxesProgression : return a.tr( "Automate GEN OuvrirAxesProgression" );
        case eMission_Automate_INF_AttaquerSouplesse : return a.tr( "Automate INF AttaquerSouplesse" );
        case eMission_Automate_INF_Appuyer : return a.tr( "Automate INF Appuyer" );
        case eMission_Automate_INF_Controler : return a.tr( "Automate INF Controler" );
        case eMission_Automate_INF_Couvrir : return a.tr( "Automate INF Couvrir" );
        case eMission_Automate_INF_DonnerCoupArret : return a.tr( "Automate INF DonnerCoupArret" );
        case eMission_Automate_INF_Eclairer : return a.tr( "Automate INF Eclairer" );
        case eMission_Automate_INF_FlancGarder : return a.tr( "Automate INF FlancGarder" );
        case eMission_Automate_INF_Freiner : return a.tr( "Automate INF Freiner" );
        case eMission_Automate_INF_Harceler : return a.tr( "Automate INF Harceler" );
        case eMission_Automate_INF_Interdire : return a.tr( "Automate INF Interdire" );
        case eMission_Automate_INF_MenerDefenseUsure : return a.tr( "Automate INF MenerDefenseUsure" );
        case eMission_Automate_INF_PrendrePreciserMaintenirContact : return a.tr( "Automate INF PrendrePreciserMaintenirContact" );
        case eMission_Automate_INF_ReconnaissanceOffensive : return a.tr( "Automate INF ReconnaissanceOffensive" );
        case eMission_Automate_INF_RecueillirUnite : return a.tr( "Automate INF RecueillirUnite" );
        case eMission_Automate_INF_Reduire : return a.tr( "Automate INF Reduire" );
        case eMission_Automate_INF_Relever : return a.tr( "Automate INF Relever" );
        case eMission_Automate_INF_RompreContact : return a.tr( "Automate INF RompreContact" );
        case eMission_Automate_INF_Surveiller : return a.tr( "Automate INF Surveiller" );
        case eMission_Automate_INF_Semparer : return a.tr( "Automate INF Semparer" );
        case eMission_Automate_INF_Fixer : return a.tr( "Automate INF Fixer" );
        case eMission_Automate_INF_AttaquerEnTerrainDifficile : return a.tr( "Automate INF AttaquerEnTerrainDifficile" );
        case eMission_Automate_INF_BarrerDirection : return a.tr( "Automate INF BarrerDirection" );
        case eMission_Automate_INF_DefendreFerme : return a.tr( "Automate INF DefendreFerme" );
        case eMission_Automate_INF_Tenir : return a.tr( "Automate INF Tenir" );
        case eMission_Automate_INF_AppuyerUnFreinage : return a.tr( "Automate INF AppuyerUnFreinage" );
        case eMission_Automate_INF_AppuyerUneAttaque : return a.tr( "Automate INF AppuyerUneAttaque" );
        case eMission_Automate_INF_Soutenir : return a.tr( "Automate INF Soutenir" );
        case eMission_Automate_LOG_SeDeployer : return a.tr( "Automate LOG SeDeployer" );
        case eMission_Automate_LOG_AppuyerMouvement : return a.tr( "Automate LOG AppuyerMouvement" );
        case eMission_Automate_LOG_ReconnaitreItineraire : return a.tr( "Automate LOG ReconnaitreItineraire" );
        case eMission_Automate_LOG_Surveiller : return a.tr( "Automate LOG Surveiller" );
        case eMission_Automate_LOG_TransporterFormationBlindee : return a.tr( "Automate LOG TransporterFormationBlindee" );
        case eMission_Automate_LOG_AppuyerMouvementDansZone : return a.tr( "Automate LOG AppuyerMouvementDansZone" );
        case eMission_Automate_LOG_MettreEnOeuvreZoneStationnement : return a.tr( "Automate LOG MettreEnOeuvreZoneStationnement" );
        case eMission_Automate_LOG_AppuyerFranchissementDansZone : return a.tr( "Automate LOG AppuyerFranchissementDansZone" );
        case eMission_Automate_LOG_BloquerZone : return a.tr( "Automate LOG BloquerZone" );
        case eMission_Automate_LOG_ReconnaitreZoneContaminee : return a.tr( "Automate LOG ReconnaitreZoneContaminee" );
        case eMission_Automate_LOG_ReconnaitreZoneDeDeploiement : return a.tr( "Automate LOG ReconnaitreZoneDeDeploiement" );
        case eMission_Automate_LOG_ArmerPIAs : return a.tr( "Automate LOG ArmerPIAs" );
        case eMission_Automate_Test_MoveTo : return a.tr( "Automate Test MoveTo" );
        case eMission_Automate_Decrocher : return a.tr( "Automate Decrocher" );
        case eMission_Automate_ReleverSurPlace : return a.tr( "Automate ReleverSurPlace" );
        case eMission_Automate_FaireMouvement : return a.tr( "Automate FaireMouvement" );
        case eMission_Automate_SeFaireDecontaminer : return a.tr( "Automate SeFaireDecontaminer" );
        case eMission_Automate_Franchir : return a.tr( "Automate Franchir" );
        case eMission_Automate_Stationner : return a.tr( "Automate Stationner" );
        case eMission_Automate_SeFaireTransporter : return a.tr( "Automate SeFaireTransporter" );
        case eMission_Automate_SeRecompleter : return a.tr( "Automate SeRecompleter" );
        case eMission_Automate_SeRendre : return a.tr( "Automate SeRendre" );
        case eMission_Automate_FaireMouvementVersCampRefugies : return a.tr( "Automate FaireMouvementVersCampRefugies" );
        case eMission_Automate_Generique : return a.tr( "Automate Generique" );
        case eMission_Automate_NBC_ReconnaitreUnAxe : return a.tr( "Automate NBC ReconnaitreUnAxe" );
        case eMission_Automate_NBC_ReconnaitreUneZone : return a.tr( "Automate NBC ReconnaitreUneZone" );
        case eMission_Automate_NBC_ArmerUnSiteDeDecontamination : return a.tr( "Automate NBC ArmerUnSiteDeDecontamination" );
        case eMission_Automate_NBC_DecontaminerUneZone : return a.tr( "Automate NBC DecontaminerUneZone" );
        case eMission_Automate_NBC_ReconnaitreDesSitesROTA : return a.tr( "Automate NBC ReconnaitreDesSitesROTA" );
        case eMission_Automate_RENS_ROHUM_RenseignerSur : return a.tr( "Automate RENS ROHUM RenseignerSur" );
        case eMission_Automate_RENS_ROHUM_OrienterGuider : return a.tr( "Automate RENS ROHUM OrienterGuider" );
        case eMission_Automate_RENS_ROHUM_SExfiltrer : return a.tr( "Automate RENS ROHUM SExfiltrer" );
        case eMission_Automate_RENS_ROIM_CL289_RenseignerSur : return a.tr( "Automate RENS ROIM CL289 RenseignerSur" );
        case eMission_Automate_RENS_ROIM_SDTI_RenseignerSur : return a.tr( "Automate RENS ROIM SDTI RenseignerSur" );
        case eMission_Automate_RENS_ROEM_Appuyer : return a.tr( "Automate RENS ROEM Appuyer" );
        case eMission_Automate_RENS_ROEM_RenseignerSur : return a.tr( "Automate RENS ROEM RenseignerSur" );
        case eMission_Automate_RENS_RADINT_RenseignerSur : return a.tr( "Automate RENS RADINT RenseignerSur" );
        case eMission_Automate_TRANS_Commander : return a.tr( "Automate TRANS Commander" );

        default:
            return a.tr( "Unknown" );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::AutomataMissionToId
// Created: AGR 2005-5-11 - 10:0:21
// -----------------------------------------------------------------------------
unsigned int MOS_EnumConverter::AutomataMissionToId( const std::string& strMission )
{
    if( strMission == "Automate ABC ArmerPointDeControle" ) return eMission_Automate_ABC_ArmerPointDeControle;
    if( strMission == "Automate ABC OrganiserAccueilColonneRefugies" ) return eMission_Automate_ABC_OrganiserAccueilColonneRefugies;
    if( strMission == "Automate ABC ProtegerPC" ) return eMission_Automate_ABC_ProtegerPC;
    if( strMission == "Automate ABC Appuyer" ) return eMission_Automate_ABC_Appuyer;
    if( strMission == "Automate ABC Attaquer" ) return eMission_Automate_ABC_Attaquer;
    if( strMission == "Automate ABC ContreAttaquer" ) return eMission_Automate_ABC_ContreAttaquer;
    if( strMission == "Automate ABC Controler" ) return eMission_Automate_ABC_Controler;
    if( strMission == "Automate ABC Couvrir" ) return eMission_Automate_ABC_Couvrir;
    if( strMission == "Automate ABC DonnerCoupArret" ) return eMission_Automate_ABC_DonnerCoupArret;
    if( strMission == "Automate ABC Eclairer" ) return eMission_Automate_ABC_Eclairer;
    if( strMission == "Automate ABC EffectuerRaid" ) return eMission_Automate_ABC_EffectuerRaid;
    if( strMission == "Automate ABC Interdire" ) return eMission_Automate_ABC_Interdire;
    if( strMission == "Automate ABC Jalonner" ) return eMission_Automate_ABC_Jalonner;
    if( strMission == "Automate ABC PrendrePreciserContact" ) return eMission_Automate_ABC_PrendrePreciserContact;
    if( strMission == "Automate ABC Reconnaitre" ) return eMission_Automate_ABC_Reconnaitre;
    if( strMission == "Automate ABC ReconnaitrePoint" ) return eMission_Automate_ABC_ReconnaitrePoint;
    if( strMission == "Automate ABC Reduire" ) return eMission_Automate_ABC_Reduire;
    if( strMission == "Automate ABC ReleverEnDepassant" ) return eMission_Automate_ABC_ReleverEnDepassant;
    if( strMission == "Automate ABC Semparer" ) return eMission_Automate_ABC_Semparer;
    if( strMission == "Automate ABC Tenir" ) return eMission_Automate_ABC_Tenir;
    if( strMission == "Automate ABC Soutenir" ) return eMission_Automate_ABC_Soutenir;
    if( strMission == "Automate ABC Surveiller" ) return eMission_Automate_ABC_Surveiller;
    if( strMission == "Automate ABC FlancGarder" ) return eMission_Automate_ABC_FlancGarder;
    if( strMission == "Automate ABC RenseignerSur" ) return eMission_Automate_ABC_RenseignerSur;
    if( strMission == "Automate ABC Fixer" ) return eMission_Automate_ABC_Fixer;
    if( strMission == "Automate ABC Freiner" ) return eMission_Automate_ABC_Freiner;
    if( strMission == "Automate ABC SurveillerItineraire" ) return eMission_Automate_ABC_SurveillerItineraire;
    if( strMission == "Automate ABC RenseignerSurUnAxe" ) return eMission_Automate_ABC_RenseignerSurUnAxe;
    if( strMission == "Automate ABC RecueillirUnite" ) return eMission_Automate_ABC_RecueillirUnite;
    if( strMission == "Automate ABC EscorterUnConvoi" ) return eMission_Automate_ABC_EscorterUnConvoi;
    if( strMission == "Automate ALAT Heliporter" ) return eMission_Automate_ALAT_Heliporter;
    if( strMission == "Automate ALAT Jalonner" ) return eMission_Automate_ALAT_Jalonner;
    if( strMission == "Automate ALAT Helitransporter" ) return eMission_Automate_ALAT_Helitransporter;
    if( strMission == "Automate ALAT ExtrairePersonnel" ) return eMission_Automate_ALAT_ExtrairePersonnel;
    if( strMission == "Automate ALAT Appuyer" ) return eMission_Automate_ALAT_Appuyer;
    if( strMission == "Automate ALAT Couvrir" ) return eMission_Automate_ALAT_Couvrir;
    if( strMission == "Automate ALAT Surveiller" ) return eMission_Automate_ALAT_Surveiller;
    if( strMission == "Automate ALAT Reconnaitre" ) return eMission_Automate_ALAT_Reconnaitre;
    if( strMission == "Automate ALAT Freiner" ) return eMission_Automate_ALAT_Freiner;
    if( strMission == "Automate ALAT Escorter" ) return eMission_Automate_ALAT_Escorter;
    if( strMission == "Automate ALAT Attaquer" ) return eMission_Automate_ALAT_Attaquer;
    if( strMission == "Automate ALAT RecueillirFormationAeromobile" ) return eMission_Automate_ALAT_RecueillirFormationAeromobile;
    if( strMission == "Automate ALAT AttaquerFormationAeromobile" ) return eMission_Automate_ALAT_AttaquerFormationAeromobile;
    if( strMission == "Automate ALAT DonnerCoupArret" ) return eMission_Automate_ALAT_DonnerCoupArret;
    if( strMission == "Automate ALAT DetruireNeutraliserDansProfondeur" ) return eMission_Automate_ALAT_DetruireNeutraliserDansProfondeur;
    if( strMission == "Automate ALAT DetruireNeutraliserDansZone" ) return eMission_Automate_ALAT_DetruireNeutraliserDansZone;
    if( strMission == "Automate ALAT EffectuerRechercheEtSauvetage" ) return eMission_Automate_ALAT_EffectuerRechercheEtSauvetage;
    if( strMission == "Automate ASA DefendreZone" ) return eMission_Automate_ASA_DefendreZone;
    if( strMission == "Automate ASA DefendreSite" ) return eMission_Automate_ASA_DefendreSite;
    if( strMission == "Automate ASA Surveiller" ) return eMission_Automate_ASA_Surveiller;
    if( strMission == "Automate ASA DefendreItineraire" ) return eMission_Automate_ASA_DefendreItineraire;
    if( strMission == "Automate ASA MISTRAL DefendreZone" ) return eMission_Automate_ASA_MISTRAL_DefendreZone;
    if( strMission == "Automate ASA MISTRAL DefendreSite" ) return eMission_Automate_ASA_MISTRAL_DefendreSite;
    if( strMission == "Automate ASA MISTRAL Surveiller" ) return eMission_Automate_ASA_MISTRAL_Surveiller;
    if( strMission == "Automate ASA HAWK DefendreZone" ) return eMission_Automate_ASA_HAWK_DefendreZone;
    if( strMission == "Automate ASA HAWK DefendreSite" ) return eMission_Automate_ASA_HAWK_DefendreSite;
    if( strMission == "Automate ASA HAWK Surveiller" ) return eMission_Automate_ASA_HAWK_Surveiller;
    if( strMission == "Automate ASA DefendreUnites" ) return eMission_Automate_ASA_DefendreUnites;
    if( strMission == "Automate ASS SeDeployer" ) return eMission_Automate_ASS_SeDeployer;
    if( strMission == "Automate ASS ReconnaitreZonesDeploiement" ) return eMission_Automate_ASS_ReconnaitreZonesDeploiement;
    if( strMission == "Automate ASS AcquerirObjectifs" ) return eMission_Automate_ASS_AcquerirObjectifs;
    if( strMission == "Automate GEN SeDeployer" ) return eMission_Automate_GEN_SeDeployer;
    if( strMission == "Automate GEN FaireFranchir" ) return eMission_Automate_GEN_FaireFranchir;
    if( strMission == "Automate GEN RealiserSystemeObstacles" ) return eMission_Automate_GEN_RealiserSystemeObstacles;
    if( strMission == "Automate GEN FranchirDeViveForce" ) return eMission_Automate_GEN_FranchirDeViveForce;
    if( strMission == "Automate GEN RetablirItineraires" ) return eMission_Automate_GEN_RetablirItineraires;
    if( strMission == "Automate GEN AmenagerAireStationnement" ) return eMission_Automate_GEN_AmenagerAireStationnement;
    if( strMission == "Automate GEN AmenagerTerrainPoserSommaire" ) return eMission_Automate_GEN_AmenagerTerrainPoserSommaire;
    if( strMission == "Automate GEN RealiserZonePoser" ) return eMission_Automate_GEN_RealiserZonePoser;
    if( strMission == "Automate GEN ReconnaitreZoneDeploiement" ) return eMission_Automate_GEN_ReconnaitreZoneDeploiement;
    if( strMission == "Automate GEN RealiserCampPrisonniers" ) return eMission_Automate_GEN_RealiserCampPrisonniers;
    if( strMission == "Automate GEN RealiserCampRefugies" ) return eMission_Automate_GEN_RealiserCampRefugies;
    if( strMission == "Automate GEN RealiserPlatesformesSommairesDeDeploiementInitial" ) return eMission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial;
    if( strMission == "Automate GEN RealiserTravauxSommairesAuProfitPopulation" ) return eMission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation;
    if( strMission == "Automate GEN AppuyerDebarquementTroupes" ) return eMission_Automate_GEN_AppuyerDebarquementTroupes;
    if( strMission == "Automate GEN OuvrirAxesProgression" ) return eMission_Automate_GEN_OuvrirAxesProgression;
    if( strMission == "Automate INF AttaquerSouplesse" ) return eMission_Automate_INF_AttaquerSouplesse;
    if( strMission == "Automate INF Appuyer" ) return eMission_Automate_INF_Appuyer;
    if( strMission == "Automate INF Controler" ) return eMission_Automate_INF_Controler;
    if( strMission == "Automate INF Couvrir" ) return eMission_Automate_INF_Couvrir;
    if( strMission == "Automate INF DonnerCoupArret" ) return eMission_Automate_INF_DonnerCoupArret;
    if( strMission == "Automate INF Eclairer" ) return eMission_Automate_INF_Eclairer;
    if( strMission == "Automate INF FlancGarder" ) return eMission_Automate_INF_FlancGarder;
    if( strMission == "Automate INF Freiner" ) return eMission_Automate_INF_Freiner;
    if( strMission == "Automate INF Harceler" ) return eMission_Automate_INF_Harceler;
    if( strMission == "Automate INF Interdire" ) return eMission_Automate_INF_Interdire;
    if( strMission == "Automate INF MenerDefenseUsure" ) return eMission_Automate_INF_MenerDefenseUsure;
    if( strMission == "Automate INF PrendrePreciserMaintenirContact" ) return eMission_Automate_INF_PrendrePreciserMaintenirContact;
    if( strMission == "Automate INF ReconnaissanceOffensive" ) return eMission_Automate_INF_ReconnaissanceOffensive;
    if( strMission == "Automate INF RecueillirUnite" ) return eMission_Automate_INF_RecueillirUnite;
    if( strMission == "Automate INF Reduire" ) return eMission_Automate_INF_Reduire;
    if( strMission == "Automate INF Relever" ) return eMission_Automate_INF_Relever;
    if( strMission == "Automate INF RompreContact" ) return eMission_Automate_INF_RompreContact;
    if( strMission == "Automate INF Surveiller" ) return eMission_Automate_INF_Surveiller;
    if( strMission == "Automate INF Semparer" ) return eMission_Automate_INF_Semparer;
    if( strMission == "Automate INF Fixer" ) return eMission_Automate_INF_Fixer;
    if( strMission == "Automate INF AttaquerEnTerrainDifficile" ) return eMission_Automate_INF_AttaquerEnTerrainDifficile;
    if( strMission == "Automate INF BarrerDirection" ) return eMission_Automate_INF_BarrerDirection;
    if( strMission == "Automate INF DefendreFerme" ) return eMission_Automate_INF_DefendreFerme;
    if( strMission == "Automate INF Tenir" ) return eMission_Automate_INF_Tenir;
    if( strMission == "Automate INF AppuyerUnFreinage" ) return eMission_Automate_INF_AppuyerUnFreinage;
    if( strMission == "Automate INF AppuyerUneAttaque" ) return eMission_Automate_INF_AppuyerUneAttaque;
    if( strMission == "Automate INF Soutenir" ) return eMission_Automate_INF_Soutenir;
    if( strMission == "Automate LOG SeDeployer" ) return eMission_Automate_LOG_SeDeployer;
    if( strMission == "Automate LOG AppuyerMouvement" ) return eMission_Automate_LOG_AppuyerMouvement;
    if( strMission == "Automate LOG ReconnaitreItineraire" ) return eMission_Automate_LOG_ReconnaitreItineraire;
    if( strMission == "Automate LOG Surveiller" ) return eMission_Automate_LOG_Surveiller;
    if( strMission == "Automate LOG TransporterFormationBlindee" ) return eMission_Automate_LOG_TransporterFormationBlindee;
    if( strMission == "Automate LOG AppuyerMouvementDansZone" ) return eMission_Automate_LOG_AppuyerMouvementDansZone;
    if( strMission == "Automate LOG MettreEnOeuvreZoneStationnement" ) return eMission_Automate_LOG_MettreEnOeuvreZoneStationnement;
    if( strMission == "Automate LOG AppuyerFranchissementDansZone" ) return eMission_Automate_LOG_AppuyerFranchissementDansZone;
    if( strMission == "Automate LOG BloquerZone" ) return eMission_Automate_LOG_BloquerZone;
    if( strMission == "Automate LOG ReconnaitreZoneContaminee" ) return eMission_Automate_LOG_ReconnaitreZoneContaminee;
    if( strMission == "Automate LOG ReconnaitreZoneDeDeploiement" ) return eMission_Automate_LOG_ReconnaitreZoneDeDeploiement;
    if( strMission == "Automate LOG ArmerPIAs" ) return eMission_Automate_LOG_ArmerPIAs;
    if( strMission == "Automate Test MoveTo" ) return eMission_Automate_Test_MoveTo;
    if( strMission == "Automate Decrocher" ) return eMission_Automate_Decrocher;
    if( strMission == "Automate ReleverSurPlace" ) return eMission_Automate_ReleverSurPlace;
    if( strMission == "Automate FaireMouvement" ) return eMission_Automate_FaireMouvement;
    if( strMission == "Automate SeFaireDecontaminer" ) return eMission_Automate_SeFaireDecontaminer;
    if( strMission == "Automate Franchir" ) return eMission_Automate_Franchir;
    if( strMission == "Automate Stationner" ) return eMission_Automate_Stationner;
    if( strMission == "Automate SeFaireTransporter" ) return eMission_Automate_SeFaireTransporter;
    if( strMission == "Automate SeRecompleter" ) return eMission_Automate_SeRecompleter;
    if( strMission == "Automate SeRendre" ) return eMission_Automate_SeRendre;
    if( strMission == "Automate FaireMouvementVersCampRefugies" ) return eMission_Automate_FaireMouvementVersCampRefugies;
    if( strMission == "Automate Generique" ) return eMission_Automate_Generique;
    if( strMission == "Automate NBC ReconnaitreUnAxe" ) return eMission_Automate_NBC_ReconnaitreUnAxe;
    if( strMission == "Automate NBC ReconnaitreUneZone" ) return eMission_Automate_NBC_ReconnaitreUneZone;
    if( strMission == "Automate NBC ArmerUnSiteDeDecontamination" ) return eMission_Automate_NBC_ArmerUnSiteDeDecontamination;
    if( strMission == "Automate NBC DecontaminerUneZone" ) return eMission_Automate_NBC_DecontaminerUneZone;
    if( strMission == "Automate NBC ReconnaitreDesSitesROTA" ) return eMission_Automate_NBC_ReconnaitreDesSitesROTA;
    if( strMission == "Automate RENS ROHUM RenseignerSur" ) return eMission_Automate_RENS_ROHUM_RenseignerSur;
    if( strMission == "Automate RENS ROHUM OrienterGuider" ) return eMission_Automate_RENS_ROHUM_OrienterGuider;
    if( strMission == "Automate RENS ROHUM SExfiltrer" ) return eMission_Automate_RENS_ROHUM_SExfiltrer;
    if( strMission == "Automate RENS ROIM CL289 RenseignerSur" ) return eMission_Automate_RENS_ROIM_CL289_RenseignerSur;
    if( strMission == "Automate RENS ROIM SDTI RenseignerSur" ) return eMission_Automate_RENS_ROIM_SDTI_RenseignerSur;
    if( strMission == "Automate RENS ROEM Appuyer" ) return eMission_Automate_RENS_ROEM_Appuyer;
    if( strMission == "Automate RENS ROEM RenseignerSur" ) return eMission_Automate_RENS_ROEM_RenseignerSur;
    if( strMission == "Automate RENS RADINT RenseignerSur" ) return eMission_Automate_RENS_RADINT_RenseignerSur;
    if( strMission == "Automate TRANS Commander" ) return eMission_Automate_TRANS_Commander;

    return (unsigned int)( -1 );
}

// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::UnitMissionToString
// Created: AGR 2005-5-11 - 10:0:21
// -----------------------------------------------------------------------------
QString MOS_EnumConverter::UnitMissionToString( unsigned int nUnitMission )
{
    QObject a;
    switch( nUnitMission )
    {
        case eMission_Pion_ALAT_EvacuationSanitaire : return a.tr( "Pion ALAT EvacuationSanitaire" );
        case eMission_Pion_ALAT_ReconnaitreDansLaProfondeur : return a.tr( "Pion ALAT ReconnaitreDansLaProfondeur" );
        case eMission_Pion_ALAT_ReconnaitreContourEnnemi : return a.tr( "Pion ALAT ReconnaitreContourEnnemi" );
        case eMission_Pion_ALAT_DetruireNeutraliserDansZone : return a.tr( "Pion ALAT DetruireNeutraliserDansZone" );
        case eMission_Pion_ALAT_DetruireAeromobile : return a.tr( "Pion ALAT DetruireAeromobile" );
        case eMission_Pion_ALAT_Freiner : return a.tr( "Pion ALAT Freiner" );
        case eMission_Pion_ALAT_Jalonner : return a.tr( "Pion ALAT Jalonner" );
        case eMission_Pion_ALAT_Escorter : return a.tr( "Pion ALAT Escorter" );
        case eMission_Pion_ALAT_Heliporter : return a.tr( "Pion ALAT Heliporter" );
        case eMission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement : return a.tr( "Pion ALAT HeliporterHelitransporterPlotRavitaillement" );
        case eMission_Pion_ALAT_Helitransporter : return a.tr( "Pion ALAT Helitransporter" );
        case eMission_Pion_ALAT_EffectuerRechercheEtSauvetage : return a.tr( "Pion ALAT EffectuerRechercheEtSauvetage" );
        case eMission_Pion_ALAT_IMEX : return a.tr( "Pion ALAT IMEX" );
        case eMission_Pion_ALAT_Eclairer : return a.tr( "Pion ALAT Eclairer" );
        case eMission_Pion_ALAT_Surveiller : return a.tr( "Pion ALAT Surveiller" );
        case eMission_Pion_ALAT_AppuyerDirectAuContact : return a.tr( "Pion ALAT AppuyerDirectAuContact" );
        case eMission_Pion_ALAT_Couvrir : return a.tr( "Pion ALAT Couvrir" );
        case eMission_Pion_ALAT_Attaquer : return a.tr( "Pion ALAT Attaquer" );
        case eMission_Pion_ASA_DefendreSite : return a.tr( "Pion ASA DefendreSite" );
        case eMission_Pion_ASA_DefendreSiteAPartirPosition : return a.tr( "Pion ASA DefendreSiteAPartirPosition" );
        case eMission_Pion_ASA_DefendreZone : return a.tr( "Pion ASA DefendreZone" );
        case eMission_Pion_ASA_DefendreZoneAPartirPosition : return a.tr( "Pion ASA DefendreZoneAPartirPosition" );
        case eMission_Pion_ASA_Surveiller : return a.tr( "Pion ASA Surveiller" );
        case eMission_Pion_ASA_DefendreUnites : return a.tr( "Pion ASA DefendreUnites" );
        case eMission_Pion_ASA_MISTRAL_DefendreSite : return a.tr( "Pion ASA MISTRAL DefendreSite" );
        case eMission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition : return a.tr( "Pion ASA MISTRAL DefendreSiteAPartirPosition" );
        case eMission_Pion_ASA_MISTRAL_DefendreZone : return a.tr( "Pion ASA MISTRAL DefendreZone" );
        case eMission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition : return a.tr( "Pion ASA MISTRAL DefendreZoneAPartirPosition" );
        case eMission_Pion_ASA_MISTRAL_Surveiller : return a.tr( "Pion ASA MISTRAL Surveiller" );
        case eMission_Pion_ASS_AssurerMiseEnOeuvre : return a.tr( "Pion ASS AssurerMiseEnOeuvre" );
        case eMission_Pion_ASS_AssurerMiseEnOeuvreSurPosition : return a.tr( "Pion ASS AssurerMiseEnOeuvreSurPosition" );
        case eMission_Pion_ASS_ReconnaitreZoneDeploiement : return a.tr( "Pion ASS ReconnaitreZoneDeploiement" );
        case eMission_Pion_ASS_AcquerirLanceursEni : return a.tr( "Pion ASS AcquerirLanceursEni" );
        case eMission_Pion_ASS_EO_AcquerirObjectifs : return a.tr( "Pion ASS EO AcquerirObjectifs" );
        case eMission_Pion_ASS_EOP_AcquerirObjectifs : return a.tr( "Pion ASS EOP AcquerirObjectifs" );
        case eMission_Pion_ASS_RATAC_AcquerirObjectifs : return a.tr( "Pion ASS RATAC AcquerirObjectifs" );
        case eMission_Pion_ABC_ReconnaitreAxe : return a.tr( "Pion ABC ReconnaitreAxe" );
        case eMission_Pion_ABC_Soutenir : return a.tr( "Pion ABC Soutenir" );
        case eMission_Pion_ABC_Couvrir : return a.tr( "Pion ABC Couvrir" );
        case eMission_Pion_ABC_FlancGarder : return a.tr( "Pion ABC FlancGarder" );
        case eMission_Pion_ABC_Reduire : return a.tr( "Pion ABC Reduire" );
        case eMission_Pion_ABC_Appuyer : return a.tr( "Pion ABC Appuyer" );
        case eMission_Pion_ABC_Fixer : return a.tr( "Pion ABC Fixer" );
        case eMission_Pion_ABC_RenseignerSur : return a.tr( "Pion ABC RenseignerSur" );
        case eMission_Pion_ABC_ControlerZone : return a.tr( "Pion ABC ControlerZone" );
        case eMission_Pion_ABC_Surveiller : return a.tr( "Pion ABC Surveiller" );
        case eMission_Pion_ABC_Barrer : return a.tr( "Pion ABC Barrer" );
        case eMission_Pion_ABC_Attaquer : return a.tr( "Pion ABC Attaquer" );
        case eMission_Pion_ABC_ReconnaitrePoint : return a.tr( "Pion ABC ReconnaitrePoint" );
        case eMission_Pion_ABC_Eclairer : return a.tr( "Pion ABC Eclairer" );
        case eMission_Pion_ABC_Jalonner : return a.tr( "Pion ABC Jalonner" );
        case eMission_Pion_ABC_ContreAttaquer : return a.tr( "Pion ABC ContreAttaquer" );
        case eMission_Pion_ABC_SemparerZone : return a.tr( "Pion ABC SemparerZone" );
        case eMission_Pion_ABC_ReleverUnite : return a.tr( "Pion ABC ReleverUnite" );
        case eMission_Pion_ABC_ArmerPIA : return a.tr( "Pion ABC ArmerPIA" );
        case eMission_Pion_ABC_OuvrirItineraire : return a.tr( "Pion ABC OuvrirItineraire" );
        case eMission_Pion_ABC_Freiner : return a.tr( "Pion ABC Freiner" );
        case eMission_Pion_ABC_EscorterUnConvoi : return a.tr( "Pion ABC EscorterUnConvoi" );
        case eMission_Pion_ABC_ArmerPointDeControle : return a.tr( "Pion ABC ArmerPointDeControle" );
        case eMission_Pion_ABC_OrganiserAccueilColonneRefugies : return a.tr( "Pion ABC OrganiserAccueilColonneRefugies" );
        case eMission_Pion_GEN_RealiserFosseAC : return a.tr( "Pion GEN RealiserFosseAC" );
        case eMission_Pion_GEN_RealiserAbattisMine : return a.tr( "Pion GEN RealiserAbattisMine" );
        case eMission_Pion_GEN_RealiserPointMine : return a.tr( "Pion GEN RealiserPointMine" );
        case eMission_Pion_GEN_RealiserBarricade : return a.tr( "Pion GEN RealiserBarricade" );
        case eMission_Pion_GEN_RealiserEboulement : return a.tr( "Pion GEN RealiserEboulement" );
        case eMission_Pion_GEN_RealiserBouchonMine : return a.tr( "Pion GEN RealiserBouchonMine" );
        case eMission_Pion_GEN_DetruireRoute : return a.tr( "Pion GEN DetruireRoute" );
        case eMission_Pion_GEN_DetruirePont : return a.tr( "Pion GEN DetruirePont" );
        case eMission_Pion_GEN_SecuriserItineraire : return a.tr( "Pion GEN SecuriserItineraire" );
        case eMission_Pion_GEN_DegagerAbattisMine : return a.tr( "Pion GEN DegagerAbattisMine" );
        case eMission_Pion_GEN_DegagerEboulement : return a.tr( "Pion GEN DegagerEboulement" );
        case eMission_Pion_GEN_DegagerRues : return a.tr( "Pion GEN DegagerRues" );
        case eMission_Pion_GEN_ComblerEntonnoir : return a.tr( "Pion GEN ComblerEntonnoir" );
        case eMission_Pion_GEN_ComblerFosseAC : return a.tr( "Pion GEN ComblerFosseAC" );
        case eMission_Pion_GEN_CreerContournement : return a.tr( "Pion GEN CreerContournement" );
        case eMission_Pion_GEN_RealiserZoneMineeLineaire : return a.tr( "Pion GEN RealiserZoneMineeLineaire" );
        case eMission_Pion_GEN_OuvrirTrouee : return a.tr( "Pion GEN OuvrirTrouee" );
        case eMission_Pion_GEN_Renforcer : return a.tr( "Pion GEN Renforcer" );
        case eMission_Pion_GEN_FranchirEntonnoir : return a.tr( "Pion GEN FranchirEntonnoir" );
        case eMission_Pion_GEN_FranchirFosseAC : return a.tr( "Pion GEN FranchirFosseAC" );
        case eMission_Pion_GEN_RealiserZoneMineeParDispersion : return a.tr( "Pion GEN RealiserZoneMineeParDispersion" );
        case eMission_Pion_GEN_RealiserPostesTir : return a.tr( "Pion GEN RealiserPostesTir" );
        case eMission_Pion_GEN_RealiserTravauxProtection : return a.tr( "Pion GEN RealiserTravauxProtection" );
        case eMission_Pion_GEN_RetablirItineraire : return a.tr( "Pion GEN RetablirItineraire" );
        case eMission_Pion_GEN_EquiperExploiter : return a.tr( "Pion GEN EquiperExploiter" );
        case eMission_Pion_GEN_ExecuterVariantement : return a.tr( "Pion GEN ExecuterVariantement" );
        case eMission_Pion_GEN_ReconnaitreSiteFranchissement : return a.tr( "Pion GEN ReconnaitreSiteFranchissement" );
        case eMission_Pion_GEN_ReconnaitreItineraire : return a.tr( "Pion GEN ReconnaitreItineraire" );
        case eMission_Pion_GEN_DeminerSiteFranchissement : return a.tr( "Pion GEN DeminerSiteFranchissement" );
        case eMission_Pion_GEN_ReconnaitreZone : return a.tr( "Pion GEN ReconnaitreZone" );
        case eMission_Pion_GEN_CreerPlateForme : return a.tr( "Pion GEN CreerPlateForme" );
        case eMission_Pion_GEN_CreerPiste : return a.tr( "Pion GEN CreerPiste" );
        case eMission_Pion_GEN_CreerAirePoser : return a.tr( "Pion GEN CreerAirePoser" );
        case eMission_Pion_GEN_NeutraliserEnginsExplosifs : return a.tr( "Pion GEN NeutraliserEnginsExplosifs" );
        case eMission_Pion_GEN_ActiverObstacle : return a.tr( "Pion GEN ActiverObstacle" );
        case eMission_Pion_GEN_AmeliorerMobilite : return a.tr( "Pion GEN AmeliorerMobilite" );
        case eMission_Pion_GEN_RealiserAireLogistique : return a.tr( "Pion GEN RealiserAireLogistique" );
        case eMission_Pion_GEN_DepolluerZone : return a.tr( "Pion GEN DepolluerZone" );
        case eMission_Pion_GEN_AmenagerTerrainPoser : return a.tr( "Pion GEN AmenagerTerrainPoser" );
        case eMission_Pion_GEN_RealiserPlateFormeDeploiement : return a.tr( "Pion GEN RealiserPlateFormeDeploiement" );
        case eMission_Pion_GEN_ReprendreMissionConstruction : return a.tr( "Pion GEN ReprendreMissionConstruction" );
        case eMission_Pion_GEN_RealiserZonePoserHelicoptere : return a.tr( "Pion GEN RealiserZonePoserHelicoptere" );
        case eMission_Pion_GEN_AppuyerDebarquementTroupesGU : return a.tr( "Pion GEN AppuyerDebarquementTroupesGU" );
        case eMission_Pion_GEN_RealiserCampPrisonniers : return a.tr( "Pion GEN RealiserCampPrisonniers" );
        case eMission_Pion_GEN_RealiserCampRefugies : return a.tr( "Pion GEN RealiserCampRefugies" );
        case eMission_Pion_INF_ReconnaitreAxe : return a.tr( "Pion INF ReconnaitreAxe" );
        case eMission_Pion_INF_Couvrir : return a.tr( "Pion INF Couvrir" );
        case eMission_Pion_INF_Reduire : return a.tr( "Pion INF Reduire" );
        case eMission_Pion_INF_Appuyer : return a.tr( "Pion INF Appuyer" );
        case eMission_Pion_INF_Fixer : return a.tr( "Pion INF Fixer" );
        case eMission_Pion_INF_ControlerSecteur : return a.tr( "Pion INF ControlerSecteur" );
        case eMission_Pion_INF_SurveillerSecteur : return a.tr( "Pion INF SurveillerSecteur" );
        case eMission_Pion_INF_Barrer : return a.tr( "Pion INF Barrer" );
        case eMission_Pion_INF_ReconnaitrePoint : return a.tr( "Pion INF ReconnaitrePoint" );
        case eMission_Pion_INF_Eclairer : return a.tr( "Pion INF Eclairer" );
        case eMission_Pion_INF_Interdire : return a.tr( "Pion INF Interdire" );
        case eMission_Pion_INF_Semparer : return a.tr( "Pion INF Semparer" );
        case eMission_Pion_INF_PrendrePreciserContact : return a.tr( "Pion INF PrendrePreciserContact" );
        case eMission_Pion_INF_Defendre : return a.tr( "Pion INF Defendre" );
        case eMission_Pion_INF_DetruireEmbuscade : return a.tr( "Pion INF DetruireEmbuscade" );
        case eMission_Pion_INF_ContreAttaquer : return a.tr( "Pion INF ContreAttaquer" );
        case eMission_Pion_INF_Freiner : return a.tr( "Pion INF Freiner" );
        case eMission_Pion_INF_FlancGarder : return a.tr( "Pion INF FlancGarder" );
        case eMission_Pion_INF_OrganiserUnCheckPoint : return a.tr( "Pion INF OrganiserUnCheckPoint" );
        case eMission_Pion_INF_Harceler : return a.tr( "Pion INF Harceler" );
        case eMission_Pion_INF_DetruireParCoupDeMain : return a.tr( "Pion INF DetruireParCoupDeMain" );
        case eMission_Pion_INF_EscorterUnConvoi : return a.tr( "Pion INF EscorterUnConvoi" );
        case eMission_Pion_INF_RenseignerSur : return a.tr( "Pion INF RenseignerSur" );
        case eMission_Pion_INF_Soutenir : return a.tr( "Pion INF Soutenir" );
        case eMission_Pion_INF_Securiser : return a.tr( "Pion INF Securiser" );
        case eMission_Pion_INF_Recueillir : return a.tr( "Pion INF Recueillir" );
        case eMission_Pion_INF_RepondreAAggression : return a.tr( "Pion INF RepondreAAggression" );
        case eMission_Pion_LOG_SeDeployer : return a.tr( "Pion LOG SeDeployer" );
        case eMission_Pion_LOG_DistribuerMoyens : return a.tr( "Pion LOG DistribuerMoyens" );
        case eMission_Pion_LOG_TrierBlesses : return a.tr( "Pion LOG TrierBlesses" );
        case eMission_Pion_LOG_TraiterBlesses : return a.tr( "Pion LOG TraiterBlesses" );
        case eMission_Pion_LOG_TrierEtTraiterBlesses : return a.tr( "Pion LOG TrierEtTraiterBlesses" );
        case eMission_Pion_LOG_SoignerBlesses : return a.tr( "Pion LOG SoignerBlesses" );
        case eMission_Pion_LOG_PrendreEnCompteReactionsMentales : return a.tr( "Pion LOG PrendreEnCompteReactionsMentales" );
        case eMission_Pion_LOG_DecontaminerBlesses : return a.tr( "Pion LOG DecontaminerBlesses" );
        case eMission_Pion_LOG_Reparer : return a.tr( "Pion LOG Reparer" );
        case eMission_Pion_LOG_Evacuer : return a.tr( "Pion LOG Evacuer" );
        case eMission_Pion_LOG_Convoyer : return a.tr( "Pion LOG Convoyer" );
        case eMission_Pion_LOG_DistribuerRessources : return a.tr( "Pion LOG DistribuerRessources" );
        case eMission_Pion_LOG_ConstituerConvoi : return a.tr( "Pion LOG ConstituerConvoi" );
        case eMission_Pion_LOG_Livrer : return a.tr( "Pion LOG Livrer" );
        case eMission_Pion_LOG_PreparerBascule : return a.tr( "Pion LOG PreparerBascule" );
        case eMission_Pion_LOG_AppuyerMouvement : return a.tr( "Pion LOG AppuyerMouvement" );
        case eMission_Pion_LOG_ReconnaitreItineraire : return a.tr( "Pion LOG ReconnaitreItineraire" );
        case eMission_Pion_LOG_TransporterUniteChars : return a.tr( "Pion LOG TransporterUniteChars" );
        case eMission_Pion_LOG_ReconnaitreZoneContaminee : return a.tr( "Pion LOG ReconnaitreZoneContaminee" );
        case eMission_Pion_LOG_AppuyerMouvementDansZone : return a.tr( "Pion LOG AppuyerMouvementDansZone" );
        case eMission_Pion_LOG_MettreEnOeuvreZoneStationnement : return a.tr( "Pion LOG MettreEnOeuvreZoneStationnement" );
        case eMission_Pion_LOG_AppuyerFranchissementSurUnPoint : return a.tr( "Pion LOG AppuyerFranchissementSurUnPoint" );
        case eMission_Pion_LOG_BloquerZone : return a.tr( "Pion LOG BloquerZone" );
        case eMission_Pion_LOG_ReconnaitreZoneDeDeploiement : return a.tr( "Pion LOG ReconnaitreZoneDeDeploiement" );
        case eMission_Pion_LOG_ArmerPIA : return a.tr( "Pion LOG ArmerPIA" );
        case eMission_Pion_LOG_Surveiller : return a.tr( "Pion LOG Surveiller" );
        case eMission_Pion_Test_Heliporter : return a.tr( "Pion Test Heliporter" );
        case eMission_Pion_Test_MoveTo : return a.tr( "Pion Test MoveTo" );
        case eMission_Pion_Test_CreateObject : return a.tr( "Pion Test CreateObject" );
        case eMission_Pion_Test_DestroyObject : return a.tr( "Pion Test DestroyObject" );
        case eMission_Pion_Test_CreateBypass : return a.tr( "Pion Test CreateBypass" );
        case eMission_Pion_Test_Fire : return a.tr( "Pion Test Fire" );
        case eMission_Pion_Test_ChangePosture : return a.tr( "Pion Test ChangePosture" );
        case eMission_Pion_Test_PathFind : return a.tr( "Pion Test PathFind" );
        case eMission_Pion_Test_FindPosition : return a.tr( "Pion Test FindPosition" );
        case eMission_Pion_Test_Reinforce : return a.tr( "Pion Test Reinforce" );
        case eMission_Pion_PlastronEnnemi : return a.tr( "Pion PlastronEnnemi" );
        case eMission_Pion_FaireMouvement : return a.tr( "Pion FaireMouvement" );
        case eMission_Pion_Suivre : return a.tr( "Pion Suivre" );
        case eMission_Pion_SeFaireDecontaminer : return a.tr( "Pion SeFaireDecontaminer" );
        case eMission_Pion_Franchir : return a.tr( "Pion Franchir" );
        case eMission_Pion_Decrocher : return a.tr( "Pion Decrocher" );
        case eMission_Pion_Stationner : return a.tr( "Pion Stationner" );
        case eMission_Pion_SeFaireTransporter : return a.tr( "Pion SeFaireTransporter" );
        case eMission_Pion_Generique : return a.tr( "Pion Generique" );
        case eMission_Pion_MILICE_Provoquer : return a.tr( "Pion MILICE Provoquer" );
        case eMission_Pion_MILICE_PrendreEtTenir : return a.tr( "Pion MILICE PrendreEtTenir" );
        case eMission_Pion_MILICE_DetruireEmbuscade : return a.tr( "Pion MILICE DetruireEmbuscade" );
        case eMission_Pion_JOINT_MARINE_Transporter : return a.tr( "Pion JOINT MARINE Transporter" );
        case eMission_Pion_JOINT_AIR_EffectuerRaid : return a.tr( "Pion JOINT AIR EffectuerRaid" );
        case eMission_Pion_NBC_AnimerUnPlotDeDecontamination : return a.tr( "Pion NBC AnimerUnPlotDeDecontamination" );
        case eMission_Pion_NBC_ReconnaitreItineraire : return a.tr( "Pion NBC ReconnaitreItineraire" );
        case eMission_Pion_NBC_ReconnaitreZone : return a.tr( "Pion NBC ReconnaitreZone" );
        case eMission_Pion_NBC_DecontaminerZone : return a.tr( "Pion NBC DecontaminerZone" );
        case eMission_Pion_NBC_ReconnaitreUnSiteROTA : return a.tr( "Pion NBC ReconnaitreUnSiteROTA" );
        case eMission_Pion_RENS_ROHUM_RenseignerSur : return a.tr( "Pion RENS ROHUM RenseignerSur" );
        case eMission_Pion_RENS_ROHUM_SExfiltrer : return a.tr( "Pion RENS ROHUM SExfiltrer" );
        case eMission_Pion_RENS_ROHUM_OrienterGuider : return a.tr( "Pion RENS ROHUM OrienterGuider" );
        case eMission_Pion_RENS_ROEM_Appuyer : return a.tr( "Pion RENS ROEM Appuyer" );
        case eMission_Pion_RENS_ROEM_RenseignerSur : return a.tr( "Pion RENS ROEM RenseignerSur" );
        case eMission_Pion_RENS_ROEM_MettreEnOeuvre : return a.tr( "Pion RENS ROEM MettreEnOeuvre" );
        case eMission_Pion_RENS_RADINT_RenseignerSur : return a.tr( "Pion RENS RADINT RenseignerSur" );
        case eMission_Pion_RENS_RADINT_MettreEnOeuvre : return a.tr( "Pion RENS RADINT MettreEnOeuvre" );
        case eMission_Pion_RENS_ROIM_MettreEnOeuvre : return a.tr( "Pion RENS ROIM MettreEnOeuvre" );
        case eMission_Pion_RENS_ROIM_SDTI_RenseignerSur : return a.tr( "Pion RENS ROIM SDTI RenseignerSur" );
        case eMission_Pion_RENS_ROIM_CL289_RenseignerSur : return a.tr( "Pion RENS ROIM CL289 RenseignerSur" );
        case eMission_Pion_TRANS_EtablirGererLiaison : return a.tr( "Pion TRANS EtablirGererLiaison" );

        default:
            return a.tr( "Unknown" );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::UnitMissionToId
// Created: AGR 2005-5-11 - 10:0:21
// -----------------------------------------------------------------------------
unsigned int MOS_EnumConverter::UnitMissionToId( const std::string& strMission )
{
    if( strMission == "Pion ALAT EvacuationSanitaire" ) return eMission_Pion_ALAT_EvacuationSanitaire;
    if( strMission == "Pion ALAT ReconnaitreDansLaProfondeur" ) return eMission_Pion_ALAT_ReconnaitreDansLaProfondeur;
    if( strMission == "Pion ALAT ReconnaitreContourEnnemi" ) return eMission_Pion_ALAT_ReconnaitreContourEnnemi;
    if( strMission == "Pion ALAT DetruireNeutraliserDansZone" ) return eMission_Pion_ALAT_DetruireNeutraliserDansZone;
    if( strMission == "Pion ALAT DetruireAeromobile" ) return eMission_Pion_ALAT_DetruireAeromobile;
    if( strMission == "Pion ALAT Freiner" ) return eMission_Pion_ALAT_Freiner;
    if( strMission == "Pion ALAT Jalonner" ) return eMission_Pion_ALAT_Jalonner;
    if( strMission == "Pion ALAT Escorter" ) return eMission_Pion_ALAT_Escorter;
    if( strMission == "Pion ALAT Heliporter" ) return eMission_Pion_ALAT_Heliporter;
    if( strMission == "Pion ALAT HeliporterHelitransporterPlotRavitaillement" ) return eMission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement;
    if( strMission == "Pion ALAT Helitransporter" ) return eMission_Pion_ALAT_Helitransporter;
    if( strMission == "Pion ALAT EffectuerRechercheEtSauvetage" ) return eMission_Pion_ALAT_EffectuerRechercheEtSauvetage;
    if( strMission == "Pion ALAT IMEX" ) return eMission_Pion_ALAT_IMEX;
    if( strMission == "Pion ALAT Eclairer" ) return eMission_Pion_ALAT_Eclairer;
    if( strMission == "Pion ALAT Surveiller" ) return eMission_Pion_ALAT_Surveiller;
    if( strMission == "Pion ALAT AppuyerDirectAuContact" ) return eMission_Pion_ALAT_AppuyerDirectAuContact;
    if( strMission == "Pion ALAT Couvrir" ) return eMission_Pion_ALAT_Couvrir;
    if( strMission == "Pion ALAT Attaquer" ) return eMission_Pion_ALAT_Attaquer;
    if( strMission == "Pion ASA DefendreSite" ) return eMission_Pion_ASA_DefendreSite;
    if( strMission == "Pion ASA DefendreSiteAPartirPosition" ) return eMission_Pion_ASA_DefendreSiteAPartirPosition;
    if( strMission == "Pion ASA DefendreZone" ) return eMission_Pion_ASA_DefendreZone;
    if( strMission == "Pion ASA DefendreZoneAPartirPosition" ) return eMission_Pion_ASA_DefendreZoneAPartirPosition;
    if( strMission == "Pion ASA Surveiller" ) return eMission_Pion_ASA_Surveiller;
    if( strMission == "Pion ASA DefendreUnites" ) return eMission_Pion_ASA_DefendreUnites;
    if( strMission == "Pion ASA MISTRAL DefendreSite" ) return eMission_Pion_ASA_MISTRAL_DefendreSite;
    if( strMission == "Pion ASA MISTRAL DefendreSiteAPartirPosition" ) return eMission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition;
    if( strMission == "Pion ASA MISTRAL DefendreZone" ) return eMission_Pion_ASA_MISTRAL_DefendreZone;
    if( strMission == "Pion ASA MISTRAL DefendreZoneAPartirPosition" ) return eMission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition;
    if( strMission == "Pion ASA MISTRAL Surveiller" ) return eMission_Pion_ASA_MISTRAL_Surveiller;
    if( strMission == "Pion ASS AssurerMiseEnOeuvre" ) return eMission_Pion_ASS_AssurerMiseEnOeuvre;
    if( strMission == "Pion ASS AssurerMiseEnOeuvreSurPosition" ) return eMission_Pion_ASS_AssurerMiseEnOeuvreSurPosition;
    if( strMission == "Pion ASS ReconnaitreZoneDeploiement" ) return eMission_Pion_ASS_ReconnaitreZoneDeploiement;
    if( strMission == "Pion ASS AcquerirLanceursEni" ) return eMission_Pion_ASS_AcquerirLanceursEni;
    if( strMission == "Pion ASS EO AcquerirObjectifs" ) return eMission_Pion_ASS_EO_AcquerirObjectifs;
    if( strMission == "Pion ASS EOP AcquerirObjectifs" ) return eMission_Pion_ASS_EOP_AcquerirObjectifs;
    if( strMission == "Pion ASS RATAC AcquerirObjectifs" ) return eMission_Pion_ASS_RATAC_AcquerirObjectifs;
    if( strMission == "Pion ABC ReconnaitreAxe" ) return eMission_Pion_ABC_ReconnaitreAxe;
    if( strMission == "Pion ABC Soutenir" ) return eMission_Pion_ABC_Soutenir;
    if( strMission == "Pion ABC Couvrir" ) return eMission_Pion_ABC_Couvrir;
    if( strMission == "Pion ABC FlancGarder" ) return eMission_Pion_ABC_FlancGarder;
    if( strMission == "Pion ABC Reduire" ) return eMission_Pion_ABC_Reduire;
    if( strMission == "Pion ABC Appuyer" ) return eMission_Pion_ABC_Appuyer;
    if( strMission == "Pion ABC Fixer" ) return eMission_Pion_ABC_Fixer;
    if( strMission == "Pion ABC RenseignerSur" ) return eMission_Pion_ABC_RenseignerSur;
    if( strMission == "Pion ABC ControlerZone" ) return eMission_Pion_ABC_ControlerZone;
    if( strMission == "Pion ABC Surveiller" ) return eMission_Pion_ABC_Surveiller;
    if( strMission == "Pion ABC Barrer" ) return eMission_Pion_ABC_Barrer;
    if( strMission == "Pion ABC Attaquer" ) return eMission_Pion_ABC_Attaquer;
    if( strMission == "Pion ABC ReconnaitrePoint" ) return eMission_Pion_ABC_ReconnaitrePoint;
    if( strMission == "Pion ABC Eclairer" ) return eMission_Pion_ABC_Eclairer;
    if( strMission == "Pion ABC Jalonner" ) return eMission_Pion_ABC_Jalonner;
    if( strMission == "Pion ABC ContreAttaquer" ) return eMission_Pion_ABC_ContreAttaquer;
    if( strMission == "Pion ABC SemparerZone" ) return eMission_Pion_ABC_SemparerZone;
    if( strMission == "Pion ABC ReleverUnite" ) return eMission_Pion_ABC_ReleverUnite;
    if( strMission == "Pion ABC ArmerPIA" ) return eMission_Pion_ABC_ArmerPIA;
    if( strMission == "Pion ABC OuvrirItineraire" ) return eMission_Pion_ABC_OuvrirItineraire;
    if( strMission == "Pion ABC Freiner" ) return eMission_Pion_ABC_Freiner;
    if( strMission == "Pion ABC EscorterUnConvoi" ) return eMission_Pion_ABC_EscorterUnConvoi;
    if( strMission == "Pion ABC ArmerPointDeControle" ) return eMission_Pion_ABC_ArmerPointDeControle;
    if( strMission == "Pion ABC OrganiserAccueilColonneRefugies" ) return eMission_Pion_ABC_OrganiserAccueilColonneRefugies;
    if( strMission == "Pion GEN RealiserFosseAC" ) return eMission_Pion_GEN_RealiserFosseAC;
    if( strMission == "Pion GEN RealiserAbattisMine" ) return eMission_Pion_GEN_RealiserAbattisMine;
    if( strMission == "Pion GEN RealiserPointMine" ) return eMission_Pion_GEN_RealiserPointMine;
    if( strMission == "Pion GEN RealiserBarricade" ) return eMission_Pion_GEN_RealiserBarricade;
    if( strMission == "Pion GEN RealiserEboulement" ) return eMission_Pion_GEN_RealiserEboulement;
    if( strMission == "Pion GEN RealiserBouchonMine" ) return eMission_Pion_GEN_RealiserBouchonMine;
    if( strMission == "Pion GEN DetruireRoute" ) return eMission_Pion_GEN_DetruireRoute;
    if( strMission == "Pion GEN DetruirePont" ) return eMission_Pion_GEN_DetruirePont;
    if( strMission == "Pion GEN SecuriserItineraire" ) return eMission_Pion_GEN_SecuriserItineraire;
    if( strMission == "Pion GEN DegagerAbattisMine" ) return eMission_Pion_GEN_DegagerAbattisMine;
    if( strMission == "Pion GEN DegagerEboulement" ) return eMission_Pion_GEN_DegagerEboulement;
    if( strMission == "Pion GEN DegagerRues" ) return eMission_Pion_GEN_DegagerRues;
    if( strMission == "Pion GEN ComblerEntonnoir" ) return eMission_Pion_GEN_ComblerEntonnoir;
    if( strMission == "Pion GEN ComblerFosseAC" ) return eMission_Pion_GEN_ComblerFosseAC;
    if( strMission == "Pion GEN CreerContournement" ) return eMission_Pion_GEN_CreerContournement;
    if( strMission == "Pion GEN RealiserZoneMineeLineaire" ) return eMission_Pion_GEN_RealiserZoneMineeLineaire;
    if( strMission == "Pion GEN OuvrirTrouee" ) return eMission_Pion_GEN_OuvrirTrouee;
    if( strMission == "Pion GEN Renforcer" ) return eMission_Pion_GEN_Renforcer;
    if( strMission == "Pion GEN FranchirEntonnoir" ) return eMission_Pion_GEN_FranchirEntonnoir;
    if( strMission == "Pion GEN FranchirFosseAC" ) return eMission_Pion_GEN_FranchirFosseAC;
    if( strMission == "Pion GEN RealiserZoneMineeParDispersion" ) return eMission_Pion_GEN_RealiserZoneMineeParDispersion;
    if( strMission == "Pion GEN RealiserPostesTir" ) return eMission_Pion_GEN_RealiserPostesTir;
    if( strMission == "Pion GEN RealiserTravauxProtection" ) return eMission_Pion_GEN_RealiserTravauxProtection;
    if( strMission == "Pion GEN RetablirItineraire" ) return eMission_Pion_GEN_RetablirItineraire;
    if( strMission == "Pion GEN EquiperExploiter" ) return eMission_Pion_GEN_EquiperExploiter;
    if( strMission == "Pion GEN ExecuterVariantement" ) return eMission_Pion_GEN_ExecuterVariantement;
    if( strMission == "Pion GEN ReconnaitreSiteFranchissement" ) return eMission_Pion_GEN_ReconnaitreSiteFranchissement;
    if( strMission == "Pion GEN ReconnaitreItineraire" ) return eMission_Pion_GEN_ReconnaitreItineraire;
    if( strMission == "Pion GEN DeminerSiteFranchissement" ) return eMission_Pion_GEN_DeminerSiteFranchissement;
    if( strMission == "Pion GEN ReconnaitreZone" ) return eMission_Pion_GEN_ReconnaitreZone;
    if( strMission == "Pion GEN CreerPlateForme" ) return eMission_Pion_GEN_CreerPlateForme;
    if( strMission == "Pion GEN CreerPiste" ) return eMission_Pion_GEN_CreerPiste;
    if( strMission == "Pion GEN CreerAirePoser" ) return eMission_Pion_GEN_CreerAirePoser;
    if( strMission == "Pion GEN NeutraliserEnginsExplosifs" ) return eMission_Pion_GEN_NeutraliserEnginsExplosifs;
    if( strMission == "Pion GEN ActiverObstacle" ) return eMission_Pion_GEN_ActiverObstacle;
    if( strMission == "Pion GEN AmeliorerMobilite" ) return eMission_Pion_GEN_AmeliorerMobilite;
    if( strMission == "Pion GEN RealiserAireLogistique" ) return eMission_Pion_GEN_RealiserAireLogistique;
    if( strMission == "Pion GEN DepolluerZone" ) return eMission_Pion_GEN_DepolluerZone;
    if( strMission == "Pion GEN AmenagerTerrainPoser" ) return eMission_Pion_GEN_AmenagerTerrainPoser;
    if( strMission == "Pion GEN RealiserPlateFormeDeploiement" ) return eMission_Pion_GEN_RealiserPlateFormeDeploiement;
    if( strMission == "Pion GEN ReprendreMissionConstruction" ) return eMission_Pion_GEN_ReprendreMissionConstruction;
    if( strMission == "Pion GEN RealiserZonePoserHelicoptere" ) return eMission_Pion_GEN_RealiserZonePoserHelicoptere;
    if( strMission == "Pion GEN AppuyerDebarquementTroupesGU" ) return eMission_Pion_GEN_AppuyerDebarquementTroupesGU;
    if( strMission == "Pion GEN RealiserCampPrisonniers" ) return eMission_Pion_GEN_RealiserCampPrisonniers;
    if( strMission == "Pion GEN RealiserCampRefugies" ) return eMission_Pion_GEN_RealiserCampRefugies;
    if( strMission == "Pion INF ReconnaitreAxe" ) return eMission_Pion_INF_ReconnaitreAxe;
    if( strMission == "Pion INF Couvrir" ) return eMission_Pion_INF_Couvrir;
    if( strMission == "Pion INF Reduire" ) return eMission_Pion_INF_Reduire;
    if( strMission == "Pion INF Appuyer" ) return eMission_Pion_INF_Appuyer;
    if( strMission == "Pion INF Fixer" ) return eMission_Pion_INF_Fixer;
    if( strMission == "Pion INF ControlerSecteur" ) return eMission_Pion_INF_ControlerSecteur;
    if( strMission == "Pion INF SurveillerSecteur" ) return eMission_Pion_INF_SurveillerSecteur;
    if( strMission == "Pion INF Barrer" ) return eMission_Pion_INF_Barrer;
    if( strMission == "Pion INF ReconnaitrePoint" ) return eMission_Pion_INF_ReconnaitrePoint;
    if( strMission == "Pion INF Eclairer" ) return eMission_Pion_INF_Eclairer;
    if( strMission == "Pion INF Interdire" ) return eMission_Pion_INF_Interdire;
    if( strMission == "Pion INF Semparer" ) return eMission_Pion_INF_Semparer;
    if( strMission == "Pion INF PrendrePreciserContact" ) return eMission_Pion_INF_PrendrePreciserContact;
    if( strMission == "Pion INF Defendre" ) return eMission_Pion_INF_Defendre;
    if( strMission == "Pion INF DetruireEmbuscade" ) return eMission_Pion_INF_DetruireEmbuscade;
    if( strMission == "Pion INF ContreAttaquer" ) return eMission_Pion_INF_ContreAttaquer;
    if( strMission == "Pion INF Freiner" ) return eMission_Pion_INF_Freiner;
    if( strMission == "Pion INF FlancGarder" ) return eMission_Pion_INF_FlancGarder;
    if( strMission == "Pion INF OrganiserUnCheckPoint" ) return eMission_Pion_INF_OrganiserUnCheckPoint;
    if( strMission == "Pion INF Harceler" ) return eMission_Pion_INF_Harceler;
    if( strMission == "Pion INF DetruireParCoupDeMain" ) return eMission_Pion_INF_DetruireParCoupDeMain;
    if( strMission == "Pion INF EscorterUnConvoi" ) return eMission_Pion_INF_EscorterUnConvoi;
    if( strMission == "Pion INF RenseignerSur" ) return eMission_Pion_INF_RenseignerSur;
    if( strMission == "Pion INF Soutenir" ) return eMission_Pion_INF_Soutenir;
    if( strMission == "Pion INF Securiser" ) return eMission_Pion_INF_Securiser;
    if( strMission == "Pion INF Recueillir" ) return eMission_Pion_INF_Recueillir;
    if( strMission == "Pion INF RepondreAAggression" ) return eMission_Pion_INF_RepondreAAggression;
    if( strMission == "Pion LOG SeDeployer" ) return eMission_Pion_LOG_SeDeployer;
    if( strMission == "Pion LOG DistribuerMoyens" ) return eMission_Pion_LOG_DistribuerMoyens;
    if( strMission == "Pion LOG TrierBlesses" ) return eMission_Pion_LOG_TrierBlesses;
    if( strMission == "Pion LOG TraiterBlesses" ) return eMission_Pion_LOG_TraiterBlesses;
    if( strMission == "Pion LOG TrierEtTraiterBlesses" ) return eMission_Pion_LOG_TrierEtTraiterBlesses;
    if( strMission == "Pion LOG SoignerBlesses" ) return eMission_Pion_LOG_SoignerBlesses;
    if( strMission == "Pion LOG PrendreEnCompteReactionsMentales" ) return eMission_Pion_LOG_PrendreEnCompteReactionsMentales;
    if( strMission == "Pion LOG DecontaminerBlesses" ) return eMission_Pion_LOG_DecontaminerBlesses;
    if( strMission == "Pion LOG Reparer" ) return eMission_Pion_LOG_Reparer;
    if( strMission == "Pion LOG Evacuer" ) return eMission_Pion_LOG_Evacuer;
    if( strMission == "Pion LOG Convoyer" ) return eMission_Pion_LOG_Convoyer;
    if( strMission == "Pion LOG DistribuerRessources" ) return eMission_Pion_LOG_DistribuerRessources;
    if( strMission == "Pion LOG ConstituerConvoi" ) return eMission_Pion_LOG_ConstituerConvoi;
    if( strMission == "Pion LOG Livrer" ) return eMission_Pion_LOG_Livrer;
    if( strMission == "Pion LOG PreparerBascule" ) return eMission_Pion_LOG_PreparerBascule;
    if( strMission == "Pion LOG AppuyerMouvement" ) return eMission_Pion_LOG_AppuyerMouvement;
    if( strMission == "Pion LOG ReconnaitreItineraire" ) return eMission_Pion_LOG_ReconnaitreItineraire;
    if( strMission == "Pion LOG TransporterUniteChars" ) return eMission_Pion_LOG_TransporterUniteChars;
    if( strMission == "Pion LOG ReconnaitreZoneContaminee" ) return eMission_Pion_LOG_ReconnaitreZoneContaminee;
    if( strMission == "Pion LOG AppuyerMouvementDansZone" ) return eMission_Pion_LOG_AppuyerMouvementDansZone;
    if( strMission == "Pion LOG MettreEnOeuvreZoneStationnement" ) return eMission_Pion_LOG_MettreEnOeuvreZoneStationnement;
    if( strMission == "Pion LOG AppuyerFranchissementSurUnPoint" ) return eMission_Pion_LOG_AppuyerFranchissementSurUnPoint;
    if( strMission == "Pion LOG BloquerZone" ) return eMission_Pion_LOG_BloquerZone;
    if( strMission == "Pion LOG ReconnaitreZoneDeDeploiement" ) return eMission_Pion_LOG_ReconnaitreZoneDeDeploiement;
    if( strMission == "Pion LOG ArmerPIA" ) return eMission_Pion_LOG_ArmerPIA;
    if( strMission == "Pion LOG Surveiller" ) return eMission_Pion_LOG_Surveiller;
    if( strMission == "Pion Test Heliporter" ) return eMission_Pion_Test_Heliporter;
    if( strMission == "Pion Test MoveTo" ) return eMission_Pion_Test_MoveTo;
    if( strMission == "Pion Test CreateObject" ) return eMission_Pion_Test_CreateObject;
    if( strMission == "Pion Test DestroyObject" ) return eMission_Pion_Test_DestroyObject;
    if( strMission == "Pion Test CreateBypass" ) return eMission_Pion_Test_CreateBypass;
    if( strMission == "Pion Test Fire" ) return eMission_Pion_Test_Fire;
    if( strMission == "Pion Test ChangePosture" ) return eMission_Pion_Test_ChangePosture;
    if( strMission == "Pion Test PathFind" ) return eMission_Pion_Test_PathFind;
    if( strMission == "Pion Test FindPosition" ) return eMission_Pion_Test_FindPosition;
    if( strMission == "Pion Test Reinforce" ) return eMission_Pion_Test_Reinforce;
    if( strMission == "Pion PlastronEnnemi" ) return eMission_Pion_PlastronEnnemi;
    if( strMission == "Pion FaireMouvement" ) return eMission_Pion_FaireMouvement;
    if( strMission == "Pion Suivre" ) return eMission_Pion_Suivre;
    if( strMission == "Pion SeFaireDecontaminer" ) return eMission_Pion_SeFaireDecontaminer;
    if( strMission == "Pion Franchir" ) return eMission_Pion_Franchir;
    if( strMission == "Pion Decrocher" ) return eMission_Pion_Decrocher;
    if( strMission == "Pion Stationner" ) return eMission_Pion_Stationner;
    if( strMission == "Pion SeFaireTransporter" ) return eMission_Pion_SeFaireTransporter;
    if( strMission == "Pion Generique" ) return eMission_Pion_Generique;
    if( strMission == "Pion MILICE Provoquer" ) return eMission_Pion_MILICE_Provoquer;
    if( strMission == "Pion MILICE PrendreEtTenir" ) return eMission_Pion_MILICE_PrendreEtTenir;
    if( strMission == "Pion MILICE DetruireEmbuscade" ) return eMission_Pion_MILICE_DetruireEmbuscade;
    if( strMission == "Pion JOINT MARINE Transporter" ) return eMission_Pion_JOINT_MARINE_Transporter;
    if( strMission == "Pion JOINT AIR EffectuerRaid" ) return eMission_Pion_JOINT_AIR_EffectuerRaid;
    if( strMission == "Pion NBC AnimerUnPlotDeDecontamination" ) return eMission_Pion_NBC_AnimerUnPlotDeDecontamination;
    if( strMission == "Pion NBC ReconnaitreItineraire" ) return eMission_Pion_NBC_ReconnaitreItineraire;
    if( strMission == "Pion NBC ReconnaitreZone" ) return eMission_Pion_NBC_ReconnaitreZone;
    if( strMission == "Pion NBC DecontaminerZone" ) return eMission_Pion_NBC_DecontaminerZone;
    if( strMission == "Pion NBC ReconnaitreUnSiteROTA" ) return eMission_Pion_NBC_ReconnaitreUnSiteROTA;
    if( strMission == "Pion RENS ROHUM RenseignerSur" ) return eMission_Pion_RENS_ROHUM_RenseignerSur;
    if( strMission == "Pion RENS ROHUM SExfiltrer" ) return eMission_Pion_RENS_ROHUM_SExfiltrer;
    if( strMission == "Pion RENS ROHUM OrienterGuider" ) return eMission_Pion_RENS_ROHUM_OrienterGuider;
    if( strMission == "Pion RENS ROEM Appuyer" ) return eMission_Pion_RENS_ROEM_Appuyer;
    if( strMission == "Pion RENS ROEM RenseignerSur" ) return eMission_Pion_RENS_ROEM_RenseignerSur;
    if( strMission == "Pion RENS ROEM MettreEnOeuvre" ) return eMission_Pion_RENS_ROEM_MettreEnOeuvre;
    if( strMission == "Pion RENS RADINT RenseignerSur" ) return eMission_Pion_RENS_RADINT_RenseignerSur;
    if( strMission == "Pion RENS RADINT MettreEnOeuvre" ) return eMission_Pion_RENS_RADINT_MettreEnOeuvre;
    if( strMission == "Pion RENS ROIM MettreEnOeuvre" ) return eMission_Pion_RENS_ROIM_MettreEnOeuvre;
    if( strMission == "Pion RENS ROIM SDTI RenseignerSur" ) return eMission_Pion_RENS_ROIM_SDTI_RenseignerSur;
    if( strMission == "Pion RENS ROIM CL289 RenseignerSur" ) return eMission_Pion_RENS_ROIM_CL289_RenseignerSur;
    if( strMission == "Pion TRANS EtablirGererLiaison" ) return eMission_Pion_TRANS_EtablirGererLiaison;

    return (unsigned int)( -1 );
}

// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::FragOrderToString
// Created: AGR 2005-5-11 - 10:0:21
// -----------------------------------------------------------------------------
QString MOS_EnumConverter::FragOrderToString( unsigned int nFragOrder )
{
    QObject a;
    switch( nFragOrder )
    {
        case eOrdreConduite_MettreTenueNBC : return a.tr( "MettreTenueNBC" );
        case eOrdreConduite_EnleverTenueNBC : return a.tr( "EnleverTenueNBC" );
        case eOrdreConduite_PasserEnSilenceRadio : return a.tr( "PasserEnSilenceRadio" );
        case eOrdreConduite_ArreterSilenceRadio : return a.tr( "ArreterSilenceRadio" );
        case eOrdreConduite_PasserEnSilenceRadar : return a.tr( "PasserEnSilenceRadar" );
        case eOrdreConduite_ArreterSilenceRadar : return a.tr( "ArreterSilenceRadar" );
        case eOrdreConduite_AttendreSePoster : return a.tr( "AttendreSePoster" );
        case eOrdreConduite_Poursuivre : return a.tr( "Poursuivre" );
        case eOrdreConduite_Interrompre : return a.tr( "Interrompre" );
        case eOrdreConduite_ChangerReglesEngagement : return a.tr( "ChangerReglesEngagement" );
        case eOrdreConduite_Deboucher : return a.tr( "Deboucher" );
        case eOrdreConduite_Decrocher : return a.tr( "Decrocher" );
        case eOrdreConduite_AssignerPositionEmbarquement : return a.tr( "AssignerPositionEmbarquement" );
        case eOrdreConduite_AcquerirObjectif : return a.tr( "AcquerirObjectif" );
        case eOrdreConduite_Brouiller : return a.tr( "Brouiller" );
        case eOrdreConduite_Pion_ChangerDePosition : return a.tr( "Pion ChangerDePosition" );
        case eOrdreConduite_Pion_Contourner : return a.tr( "Pion Contourner" );
        case eOrdreConduite_Pion_EnvoyerVehicule : return a.tr( "Pion EnvoyerVehicule" );
        case eOrdreConduite_Pion_ActiverObstacle : return a.tr( "Pion ActiverObstacle" );
        case eOrdreConduite_Pion_AppliquerFeux : return a.tr( "Pion AppliquerFeux" );
        case eOrdreConduite_Pion_ExecuterTir : return a.tr( "Pion ExecuterTir" );
        case eOrdreConduite_Pion_Emettre : return a.tr( "Pion Emettre" );
        case eOrdreConduite_Pion_StopperEmission : return a.tr( "Pion StopperEmission" );
        case eOrdreConduite_Pion_RenforcerEnVSRAM : return a.tr( "Pion RenforcerEnVSRAM" );
        case eOrdreConduite_Pion_TransfererVSRAM : return a.tr( "Pion TransfererVSRAM" );
        case eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM : return a.tr( "Pion ReprendreAuxOrdresVSRAM" );
        case eOrdreConduite_ModifierTempsBordeeMaintenance : return a.tr( "ModifierTempsBordeeMaintenance" );
        case eOrdreConduite_ModifierTempsBordeeSante : return a.tr( "ModifierTempsBordeeSante" );
        case eOrdreConduite_ModifierPrioritesReparations : return a.tr( "ModifierPrioritesReparations" );
        case eOrdreConduite_ModifierPrioritesBlesses : return a.tr( "ModifierPrioritesBlesses" );
        case eOrdreConduite_ModifierPrioritesTactiquesReparations : return a.tr( "ModifierPrioritesTactiquesReparations" );
        case eOrdreConduite_ModifierPrioritesTactiquesBlesses : return a.tr( "ModifierPrioritesTactiquesBlesses" );
        case eOrdreConduite_Automate_RealiserVariantement : return a.tr( "Automate RealiserVariantement" );
        case eOrdreConduite_Automate_DesequiperSiteFranchissement : return a.tr( "Automate DesequiperSiteFranchissement" );
        case eOrdreConduite_Automate_ReagirFaceAEni : return a.tr( "Automate ReagirFaceAEni" );
        case eOrdreConduite_Automate_AffecterPionAObstacle : return a.tr( "Automate AffecterPionAObstacle" );
        case eOrdreConduite_Automate_ReconnaitreZoneImplantation : return a.tr( "Automate ReconnaitreZoneImplantation" );
        case eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement : return a.tr( "Automate TC2 GererMaterielAvantDeplacement" );
        case eOrdreConduite_Automate_ActiverObstacle : return a.tr( "Automate ActiverObstacle" );
        case eOrdreConduite_Automate_TransfererRenforts : return a.tr( "Automate TransfererRenforts" );

        default:
            return a.tr( "Unknown" );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_EnumConverter::FragOrderToId
// Created: AGR 2005-5-11 - 10:0:21
// -----------------------------------------------------------------------------
unsigned int MOS_EnumConverter::FragOrderToId( const std::string& strFragOrder )
{
    if( strFragOrder == "MettreTenueNBC" ) return eOrdreConduite_MettreTenueNBC;
    if( strFragOrder == "EnleverTenueNBC" ) return eOrdreConduite_EnleverTenueNBC;
    if( strFragOrder == "PasserEnSilenceRadio" ) return eOrdreConduite_PasserEnSilenceRadio;
    if( strFragOrder == "ArreterSilenceRadio" ) return eOrdreConduite_ArreterSilenceRadio;
    if( strFragOrder == "PasserEnSilenceRadar" ) return eOrdreConduite_PasserEnSilenceRadar;
    if( strFragOrder == "ArreterSilenceRadar" ) return eOrdreConduite_ArreterSilenceRadar;
    if( strFragOrder == "AttendreSePoster" ) return eOrdreConduite_AttendreSePoster;
    if( strFragOrder == "Poursuivre" ) return eOrdreConduite_Poursuivre;
    if( strFragOrder == "Interrompre" ) return eOrdreConduite_Interrompre;
    if( strFragOrder == "ChangerReglesEngagement" ) return eOrdreConduite_ChangerReglesEngagement;
    if( strFragOrder == "Deboucher" ) return eOrdreConduite_Deboucher;
    if( strFragOrder == "Decrocher" ) return eOrdreConduite_Decrocher;
    if( strFragOrder == "AssignerPositionEmbarquement" ) return eOrdreConduite_AssignerPositionEmbarquement;
    if( strFragOrder == "AcquerirObjectif" ) return eOrdreConduite_AcquerirObjectif;
    if( strFragOrder == "Brouiller" ) return eOrdreConduite_Brouiller;
    if( strFragOrder == "Pion ChangerDePosition" ) return eOrdreConduite_Pion_ChangerDePosition;
    if( strFragOrder == "Pion Contourner" ) return eOrdreConduite_Pion_Contourner;
    if( strFragOrder == "Pion EnvoyerVehicule" ) return eOrdreConduite_Pion_EnvoyerVehicule;
    if( strFragOrder == "Pion ActiverObstacle" ) return eOrdreConduite_Pion_ActiverObstacle;
    if( strFragOrder == "Pion AppliquerFeux" ) return eOrdreConduite_Pion_AppliquerFeux;
    if( strFragOrder == "Pion ExecuterTir" ) return eOrdreConduite_Pion_ExecuterTir;
    if( strFragOrder == "Pion Emettre" ) return eOrdreConduite_Pion_Emettre;
    if( strFragOrder == "Pion StopperEmission" ) return eOrdreConduite_Pion_StopperEmission;
    if( strFragOrder == "Pion RenforcerEnVSRAM" ) return eOrdreConduite_Pion_RenforcerEnVSRAM;
    if( strFragOrder == "Pion TransfererVSRAM" ) return eOrdreConduite_Pion_TransfererVSRAM;
    if( strFragOrder == "Pion ReprendreAuxOrdresVSRAM" ) return eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM;
    if( strFragOrder == "ModifierTempsBordeeMaintenance" ) return eOrdreConduite_ModifierTempsBordeeMaintenance;
    if( strFragOrder == "ModifierTempsBordeeSante" ) return eOrdreConduite_ModifierTempsBordeeSante;
    if( strFragOrder == "ModifierPrioritesReparations" ) return eOrdreConduite_ModifierPrioritesReparations;
    if( strFragOrder == "ModifierPrioritesBlesses" ) return eOrdreConduite_ModifierPrioritesBlesses;
    if( strFragOrder == "ModifierPrioritesTactiquesReparations" ) return eOrdreConduite_ModifierPrioritesTactiquesReparations;
    if( strFragOrder == "ModifierPrioritesTactiquesBlesses" ) return eOrdreConduite_ModifierPrioritesTactiquesBlesses;
    if( strFragOrder == "Automate RealiserVariantement" ) return eOrdreConduite_Automate_RealiserVariantement;
    if( strFragOrder == "Automate DesequiperSiteFranchissement" ) return eOrdreConduite_Automate_DesequiperSiteFranchissement;
    if( strFragOrder == "Automate ReagirFaceAEni" ) return eOrdreConduite_Automate_ReagirFaceAEni;
    if( strFragOrder == "Automate AffecterPionAObstacle" ) return eOrdreConduite_Automate_AffecterPionAObstacle;
    if( strFragOrder == "Automate ReconnaitreZoneImplantation" ) return eOrdreConduite_Automate_ReconnaitreZoneImplantation;
    if( strFragOrder == "Automate TC2 GererMaterielAvantDeplacement" ) return eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement;
    if( strFragOrder == "Automate ActiverObstacle" ) return eOrdreConduite_Automate_ActiverObstacle;
    if( strFragOrder == "Automate TransfererRenforts" ) return eOrdreConduite_Automate_TransfererRenforts;

    return (unsigned int)( -1 );
}
