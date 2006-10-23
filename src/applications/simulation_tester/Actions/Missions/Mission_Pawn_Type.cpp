// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Type.h"

#include "Pawn/Mission_Pawn_ALAT_AllerSeRecompleter.cpp"
#include "Pawn/Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur.cpp"
#include "Pawn/Mission_Pawn_ALAT_ReconnaitreContourEnnemi.cpp"
#include "Pawn/Mission_Pawn_ALAT_DetruireNeutraliserDansZone.cpp"
#include "Pawn/Mission_Pawn_ALAT_DetruireAeromobile.cpp"
#include "Pawn/Mission_Pawn_ALAT_Freiner.cpp"
#include "Pawn/Mission_Pawn_ALAT_Jalonner.cpp"
#include "Pawn/Mission_Pawn_ALAT_Escorter.cpp"
#include "Pawn/Mission_Pawn_ALAT_Heliporter.cpp"
#include "Pawn/Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement.cpp"
#include "Pawn/Mission_Pawn_ALAT_Helitransporter.cpp"
#include "Pawn/Mission_Pawn_ALAT_IMEX.cpp"
#include "Pawn/Mission_Pawn_ALAT_Eclairer.cpp"
#include "Pawn/Mission_Pawn_ALAT_Surveiller.cpp"
#include "Pawn/Mission_Pawn_ALAT_AppuyerDirectAuContact.cpp"
#include "Pawn/Mission_Pawn_ALAT_Couvrir.cpp"
#include "Pawn/Mission_Pawn_ALAT_Attaquer.cpp"
#include "Pawn/Mission_Pawn_ASA_DefendreSite.cpp"
#include "Pawn/Mission_Pawn_ASA_DefendreSiteAPartirPosition.cpp"
#include "Pawn/Mission_Pawn_ASA_DefendreZone.cpp"
#include "Pawn/Mission_Pawn_ASA_DefendreZoneAPartirPosition.cpp"
#include "Pawn/Mission_Pawn_ASA_Surveiller.cpp"
#include "Pawn/Mission_Pawn_ASA_DefendreUnites.cpp"
#include "Pawn/Mission_Pawn_ASA_MISTRAL_DefendreSite.cpp"
#include "Pawn/Mission_Pawn_ASA_MISTRAL_DefendreSiteAPartirPosition.cpp"
#include "Pawn/Mission_Pawn_ASA_MISTRAL_DefendreZone.cpp"
#include "Pawn/Mission_Pawn_ASA_MISTRAL_DefendreZoneAPartirPosition.cpp"
#include "Pawn/Mission_Pawn_ASA_MISTRAL_Surveiller.cpp"
#include "Pawn/Mission_Pawn_ASS_AssurerMiseEnOeuvre.cpp"
#include "Pawn/Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition.cpp"
#include "Pawn/Mission_Pawn_ASS_ReconnaitreZoneDeploiement.cpp"
#include "Pawn/Mission_Pawn_ASS_AcquerirLanceursEni.cpp"
#include "Pawn/Mission_Pawn_ASS_EO_AcquerirObjectifs.cpp"
#include "Pawn/Mission_Pawn_ASS_EOP_AcquerirObjectifs.cpp"
#include "Pawn/Mission_Pawn_ASS_RATAC_AcquerirObjectifs.cpp"
#include "Pawn/Mission_Pawn_ABC_ReconnaitreAxe.cpp"
#include "Pawn/Mission_Pawn_ABC_Soutenir.cpp"
#include "Pawn/Mission_Pawn_ABC_Couvrir.cpp"
#include "Pawn/Mission_Pawn_ABC_FlancGarder.cpp"
#include "Pawn/Mission_Pawn_ABC_Reduire.cpp"
#include "Pawn/Mission_Pawn_ABC_Appuyer.cpp"
#include "Pawn/Mission_Pawn_ABC_Fixer.cpp"
#include "Pawn/Mission_Pawn_ABC_RenseignerSur.cpp"
#include "Pawn/Mission_Pawn_ABC_ControlerZone.cpp"
#include "Pawn/Mission_Pawn_ABC_Surveiller.cpp"
#include "Pawn/Mission_Pawn_ABC_Barrer.cpp"
#include "Pawn/Mission_Pawn_ABC_Attaquer.cpp"
#include "Pawn/Mission_Pawn_ABC_ReconnaitrePoint.cpp"
#include "Pawn/Mission_Pawn_ABC_Eclairer.cpp"
#include "Pawn/Mission_Pawn_ABC_Jalonner.cpp"
#include "Pawn/Mission_Pawn_ABC_ContreAttaquer.cpp"
#include "Pawn/Mission_Pawn_ABC_SemparerZone.cpp"
#include "Pawn/Mission_Pawn_ABC_ReleverUnite.cpp"
#include "Pawn/Mission_Pawn_ABC_ArmerPIA.cpp"
#include "Pawn/Mission_Pawn_ABC_OuvrirItineraire.cpp"
#include "Pawn/Mission_Pawn_ABC_Freiner.cpp"
#include "Pawn/Mission_Pawn_ABC_EscorterUnite.cpp"
#include "Pawn/Mission_Pawn_ABC_EscorterUnites.cpp"
#include "Pawn/Mission_Pawn_ABC_ArmerPointDeControle.cpp"
#include "Pawn/Mission_Pawn_ABC_OrganiserAccueilColonneRefugies.cpp"
#include "Pawn/Mission_Pawn_ABC_AppuyerSurPosition.cpp"
#include "Pawn/Mission_Pawn_ABC_RASIT_RenseignerSur.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserFosseAC.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserAbattisMine.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserPointMine.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserBarricade.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserEboulement.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserBouchonMine.cpp"
#include "Pawn/Mission_Pawn_GEN_DetruireRoute.cpp"
#include "Pawn/Mission_Pawn_GEN_DetruirePont.cpp"
#include "Pawn/Mission_Pawn_GEN_SecuriserItineraire.cpp"
#include "Pawn/Mission_Pawn_GEN_DegagerAbattisMine.cpp"
#include "Pawn/Mission_Pawn_GEN_DegagerEboulement.cpp"
#include "Pawn/Mission_Pawn_GEN_DegagerRues.cpp"
#include "Pawn/Mission_Pawn_GEN_ComblerEntonnoir.cpp"
#include "Pawn/Mission_Pawn_GEN_ComblerFosseAC.cpp"
#include "Pawn/Mission_Pawn_GEN_CreerContournement.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserZoneMineeLineaire.cpp"
#include "Pawn/Mission_Pawn_GEN_OuvrirTrouee.cpp"
#include "Pawn/Mission_Pawn_GEN_Renforcer.cpp"
#include "Pawn/Mission_Pawn_GEN_FranchirEntonnoir.cpp"
#include "Pawn/Mission_Pawn_GEN_FranchirFosseAC.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserZoneMineeParDispersion.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserPostesTir.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserTravauxProtection.cpp"
#include "Pawn/Mission_Pawn_GEN_RetablirItineraire.cpp"
#include "Pawn/Mission_Pawn_GEN_EquiperExploiter.cpp"
#include "Pawn/Mission_Pawn_GEN_ExecuterVariantement.cpp"
#include "Pawn/Mission_Pawn_GEN_ReconnaitreSiteFranchissement.cpp"
#include "Pawn/Mission_Pawn_GEN_ReconnaitreItineraire.cpp"
#include "Pawn/Mission_Pawn_GEN_DeminerSiteFranchissement.cpp"
#include "Pawn/Mission_Pawn_GEN_ReconnaitreZone.cpp"
#include "Pawn/Mission_Pawn_GEN_CreerPlateForme.cpp"
#include "Pawn/Mission_Pawn_GEN_CreerPiste.cpp"
#include "Pawn/Mission_Pawn_GEN_CreerAirePoser.cpp"
#include "Pawn/Mission_Pawn_GEN_NeutraliserEnginsExplosifs.cpp"
#include "Pawn/Mission_Pawn_GEN_ActiverObstacle.cpp"
#include "Pawn/Mission_Pawn_GEN_AmeliorerMobilite.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserAireLogistique.cpp"
#include "Pawn/Mission_Pawn_GEN_DepolluerZone.cpp"
#include "Pawn/Mission_Pawn_GEN_AmenagerTerrainPoser.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserPlateFormeDeploiement.cpp"
#include "Pawn/Mission_Pawn_GEN_ReprendreMissionConstruction.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserZonePoserHelicoptere.cpp"
#include "Pawn/Mission_Pawn_GEN_AppuyerDebarquementTroupesGU.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserCampPrisonniers.cpp"
#include "Pawn/Mission_Pawn_GEN_RealiserCampRefugies.cpp"
#include "Pawn/Mission_Pawn_INF_ReconnaitreAxe.cpp"
#include "Pawn/Mission_Pawn_INF_Couvrir.cpp"
#include "Pawn/Mission_Pawn_INF_Reduire.cpp"
#include "Pawn/Mission_Pawn_INF_Appuyer.cpp"
#include "Pawn/Mission_Pawn_INF_Fixer.cpp"
#include "Pawn/Mission_Pawn_INF_ControlerSecteur.cpp"
#include "Pawn/Mission_Pawn_INF_SurveillerSecteur.cpp"
#include "Pawn/Mission_Pawn_INF_Barrer.cpp"
#include "Pawn/Mission_Pawn_INF_ReconnaitrePoint.cpp"
#include "Pawn/Mission_Pawn_INF_Eclairer.cpp"
#include "Pawn/Mission_Pawn_INF_Interdire.cpp"
#include "Pawn/Mission_Pawn_INF_Semparer.cpp"
#include "Pawn/Mission_Pawn_INF_PrendrePreciserContact.cpp"
#include "Pawn/Mission_Pawn_INF_Defendre.cpp"
#include "Pawn/Mission_Pawn_INF_DetruireEmbuscade.cpp"
#include "Pawn/Mission_Pawn_INF_ContreAttaquer.cpp"
#include "Pawn/Mission_Pawn_INF_Freiner.cpp"
#include "Pawn/Mission_Pawn_INF_FlancGarder.cpp"
#include "Pawn/Mission_Pawn_INF_OrganiserUnCheckPoint.cpp"
#include "Pawn/Mission_Pawn_INF_Harceler.cpp"
#include "Pawn/Mission_Pawn_INF_DetruireParCoupDeMain.cpp"
#include "Pawn/Mission_Pawn_INF_EscorterUnite.cpp"
#include "Pawn/Mission_Pawn_INF_EscorterUnites.cpp"
#include "Pawn/Mission_Pawn_INF_RenseignerSur.cpp"
#include "Pawn/Mission_Pawn_INF_Soutenir.cpp"
#include "Pawn/Mission_Pawn_INF_Securiser.cpp"
#include "Pawn/Mission_Pawn_INF_Recueillir.cpp"
#include "Pawn/Mission_Pawn_INF_RepondreAAggression.cpp"
#include "Pawn/Mission_Pawn_INF_AppuyerSurPosition.cpp"
#include "Pawn/Mission_Pawn_LOG_SeDeployer.cpp"
#include "Pawn/Mission_Pawn_LOG_Renforcer.cpp"
#include "Pawn/Mission_Pawn_LOG_PreparerBascule.cpp"
#include "Pawn/Mission_Pawn_LOG_DecontaminerBlesses.cpp"
#include "Pawn/Mission_Pawn_LOG_TrierEtTraiterBlesses.cpp"
#include "Pawn/Mission_Pawn_LOG_TrierBlesses.cpp"
#include "Pawn/Mission_Pawn_LOG_TraiterBlesses.cpp"
#include "Pawn/Mission_Pawn_LOG_PrendreEnCompteReactionsMentales.cpp"
#include "Pawn/Mission_Pawn_LOG_Convoyer.cpp"
#include "Pawn/Mission_Pawn_LOG_Reparer.cpp"
#include "Pawn/Mission_Pawn_LOG_AppuyerMouvement.cpp"
#include "Pawn/Mission_Pawn_LOG_ReconnaitreItineraire.cpp"
#include "Pawn/Mission_Pawn_LOG_ReconnaitreZoneContaminee.cpp"
#include "Pawn/Mission_Pawn_LOG_AppuyerMouvementDansZone.cpp"
#include "Pawn/Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement.cpp"
#include "Pawn/Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint.cpp"
#include "Pawn/Mission_Pawn_LOG_BloquerZone.cpp"
#include "Pawn/Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement.cpp"
#include "Pawn/Mission_Pawn_LOG_ArmerPIA.cpp"
#include "Pawn/Mission_Pawn_LOG_Surveiller.cpp"
#include "Pawn/Mission_Pawn_Test_Heliporter.cpp"
#include "Pawn/Mission_Pawn_Test_MoveTo.cpp"
#include "Pawn/Mission_Pawn_Test_CreateObject.cpp"
#include "Pawn/Mission_Pawn_Test_DestroyObject.cpp"
#include "Pawn/Mission_Pawn_Test_CreateBypass.cpp"
#include "Pawn/Mission_Pawn_Test_Fire.cpp"
#include "Pawn/Mission_Pawn_Test_ChangePosture.cpp"
#include "Pawn/Mission_Pawn_Test_PathFind.cpp"
#include "Pawn/Mission_Pawn_Test_FindPosition.cpp"
#include "Pawn/Mission_Pawn_Test_Reinforce.cpp"
#include "Pawn/Mission_Pawn_PlastronEnnemi.cpp"
#include "Pawn/Mission_Pawn_FaireMouvement.cpp"
#include "Pawn/Mission_Pawn_Suivre.cpp"
#include "Pawn/Mission_Pawn_Transporter.cpp"
#include "Pawn/Mission_Pawn_SeFaireDecontaminer.cpp"
#include "Pawn/Mission_Pawn_Franchir.cpp"
#include "Pawn/Mission_Pawn_Decrocher.cpp"
#include "Pawn/Mission_Pawn_Stationner.cpp"
#include "Pawn/Mission_Pawn_SeFaireTransporter.cpp"
#include "Pawn/Mission_Pawn_InterdireFranchissementPopulations.cpp"
#include "Pawn/Mission_Pawn_FiltrerPopulations.cpp"
#include "Pawn/Mission_Pawn_RenseignerSurPopulations.cpp"
#include "Pawn/Mission_Pawn_CanaliserPopulations.cpp"
#include "Pawn/Mission_Pawn_ControlerPopulationsDansZone.cpp"
#include "Pawn/Mission_Pawn_SecuriserZoneContrePopulations.cpp"
#include "Pawn/Mission_Pawn_Generique.cpp"
#include "Pawn/Mission_Pawn_MILICE_Provoquer.cpp"
#include "Pawn/Mission_Pawn_MILICE_PrendreEtTenir.cpp"
#include "Pawn/Mission_Pawn_MILICE_DetruireEmbuscade.cpp"
#include "Pawn/Mission_Pawn_JOINT_AIR_EffectuerRaid.cpp"
#include "Pawn/Mission_Pawn_NBC_AnimerUnPlotDeDecontamination.cpp"
#include "Pawn/Mission_Pawn_NBC_ReconnaitreItineraire.cpp"
#include "Pawn/Mission_Pawn_NBC_ReconnaitreZone.cpp"
#include "Pawn/Mission_Pawn_NBC_DecontaminerZone.cpp"
#include "Pawn/Mission_Pawn_NBC_ReconnaitreUnSiteROTA.cpp"
#include "Pawn/Mission_Pawn_RENS_ROHUM_RenseignerSur.cpp"
#include "Pawn/Mission_Pawn_RENS_ROHUM_SExfiltrer.cpp"
#include "Pawn/Mission_Pawn_RENS_ROHUM_OrienterGuider.cpp"
#include "Pawn/Mission_Pawn_RENS_ROEM_Appuyer.cpp"
#include "Pawn/Mission_Pawn_RENS_ROEM_RenseignerSur.cpp"
#include "Pawn/Mission_Pawn_RENS_ROEM_MettreEnOeuvre.cpp"
#include "Pawn/Mission_Pawn_RENS_RADINT_RenseignerSur.cpp"
#include "Pawn/Mission_Pawn_RENS_RADINT_MettreEnOeuvre.cpp"
#include "Pawn/Mission_Pawn_RENS_ROIM_MettreEnOeuvre.cpp"
#include "Pawn/Mission_Pawn_RENS_ROIM_SDTI_RenseignerSur.cpp"
#include "Pawn/Mission_Pawn_RENS_ROIM_CL289_RenseignerSur.cpp"
#include "Pawn/Mission_Pawn_TRANS_EtablirGererLiaison.cpp"
#include "Pawn/Mission_Pawn_ASY_SInfiltrer.cpp"
#include "Pawn/Mission_Pawn_ASY_CommettreAttentatContrePopulation.cpp"
#include "Pawn/Mission_Pawn_ASY_CommettreAttentatContreForcesArmees.cpp"
#include "Pawn/Mission_Pawn_ASY_CommettreAttentatContreInstallation.cpp"
#include "Pawn/Mission_Pawn_ASY_CommettreExactionsSurPopulationDansZone.cpp"
#include "Pawn/Mission_Pawn_ASY_Renseigner.cpp"


using namespace TEST;

Mission_Pawn_Type::T_MissionPawnTypeMap Mission_Pawn_Type::missions_;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Pawn_Type::Mission_Pawn_Type( const std::string& strName, T_MissionAllocator missionAllocator )
    : strName_          ( strName )
    , missionAllocator_ ( missionAllocator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Pawn_Type::~Mission_Pawn_Type()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::Initialize
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Pawn_Type::Initialize()
{
    Register< Mission_Pawn_ALAT_AllerSeRecompleter >( "Pion ALAT AllerSeRecompleter" );
    Register< Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur >( "Pion ALAT ReconnaitreDansLaProfondeur" );
    Register< Mission_Pawn_ALAT_ReconnaitreContourEnnemi >( "Pion ALAT ReconnaitreContourEnnemi" );
    Register< Mission_Pawn_ALAT_DetruireNeutraliserDansZone >( "Pion ALAT DetruireNeutraliserDansZone" );
    Register< Mission_Pawn_ALAT_DetruireAeromobile >( "Pion ALAT DetruireAeromobile" );
    Register< Mission_Pawn_ALAT_Freiner >( "Pion ALAT Freiner" );
    Register< Mission_Pawn_ALAT_Jalonner >( "Pion ALAT Jalonner" );
    Register< Mission_Pawn_ALAT_Escorter >( "Pion ALAT Escorter" );
    Register< Mission_Pawn_ALAT_Heliporter >( "Pion ALAT Heliporter" );
    Register< Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement >( "Pion ALAT HeliporterHelitransporterPlotRavitaillement" );
    Register< Mission_Pawn_ALAT_Helitransporter >( "Pion ALAT Helitransporter" );
    Register< Mission_Pawn_ALAT_IMEX >( "Pion ALAT IMEX" );
    Register< Mission_Pawn_ALAT_Eclairer >( "Pion ALAT Eclairer" );
    Register< Mission_Pawn_ALAT_Surveiller >( "Pion ALAT Surveiller" );
    Register< Mission_Pawn_ALAT_AppuyerDirectAuContact >( "Pion ALAT AppuyerDirectAuContact" );
    Register< Mission_Pawn_ALAT_Couvrir >( "Pion ALAT Couvrir" );
    Register< Mission_Pawn_ALAT_Attaquer >( "Pion ALAT Attaquer" );
    Register< Mission_Pawn_ASA_DefendreSite >( "Pion ASA DefendreSite" );
    Register< Mission_Pawn_ASA_DefendreSiteAPartirPosition >( "Pion ASA DefendreSiteAPartirPosition" );
    Register< Mission_Pawn_ASA_DefendreZone >( "Pion ASA DefendreZone" );
    Register< Mission_Pawn_ASA_DefendreZoneAPartirPosition >( "Pion ASA DefendreZoneAPartirPosition" );
    Register< Mission_Pawn_ASA_Surveiller >( "Pion ASA Surveiller" );
    Register< Mission_Pawn_ASA_DefendreUnites >( "Pion ASA DefendreUnites" );
    Register< Mission_Pawn_ASA_MISTRAL_DefendreSite >( "Pion ASA MISTRAL DefendreSite" );
    Register< Mission_Pawn_ASA_MISTRAL_DefendreSiteAPartirPosition >( "Pion ASA MISTRAL DefendreSiteAPartirPosition" );
    Register< Mission_Pawn_ASA_MISTRAL_DefendreZone >( "Pion ASA MISTRAL DefendreZone" );
    Register< Mission_Pawn_ASA_MISTRAL_DefendreZoneAPartirPosition >( "Pion ASA MISTRAL DefendreZoneAPartirPosition" );
    Register< Mission_Pawn_ASA_MISTRAL_Surveiller >( "Pion ASA MISTRAL Surveiller" );
    Register< Mission_Pawn_ASS_AssurerMiseEnOeuvre >( "Pion ASS AssurerMiseEnOeuvre" );
    Register< Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition >( "Pion ASS AssurerMiseEnOeuvreSurPosition" );
    Register< Mission_Pawn_ASS_ReconnaitreZoneDeploiement >( "Pion ASS ReconnaitreZoneDeploiement" );
    Register< Mission_Pawn_ASS_AcquerirLanceursEni >( "Pion ASS AcquerirLanceursEni" );
    Register< Mission_Pawn_ASS_EO_AcquerirObjectifs >( "Pion ASS EO AcquerirObjectifs" );
    Register< Mission_Pawn_ASS_EOP_AcquerirObjectifs >( "Pion ASS EOP AcquerirObjectifs" );
    Register< Mission_Pawn_ASS_RATAC_AcquerirObjectifs >( "Pion ASS RATAC AcquerirObjectifs" );
    Register< Mission_Pawn_ABC_ReconnaitreAxe >( "Pion ABC ReconnaitreAxe" );
    Register< Mission_Pawn_ABC_Soutenir >( "Pion ABC Soutenir" );
    Register< Mission_Pawn_ABC_Couvrir >( "Pion ABC Couvrir" );
    Register< Mission_Pawn_ABC_FlancGarder >( "Pion ABC FlancGarder" );
    Register< Mission_Pawn_ABC_Reduire >( "Pion ABC Reduire" );
    Register< Mission_Pawn_ABC_Appuyer >( "Pion ABC Appuyer" );
    Register< Mission_Pawn_ABC_Fixer >( "Pion ABC Fixer" );
    Register< Mission_Pawn_ABC_RenseignerSur >( "Pion ABC RenseignerSur" );
    Register< Mission_Pawn_ABC_ControlerZone >( "Pion ABC ControlerZone" );
    Register< Mission_Pawn_ABC_Surveiller >( "Pion ABC Surveiller" );
    Register< Mission_Pawn_ABC_Barrer >( "Pion ABC Barrer" );
    Register< Mission_Pawn_ABC_Attaquer >( "Pion ABC Attaquer" );
    Register< Mission_Pawn_ABC_ReconnaitrePoint >( "Pion ABC ReconnaitrePoint" );
    Register< Mission_Pawn_ABC_Eclairer >( "Pion ABC Eclairer" );
    Register< Mission_Pawn_ABC_Jalonner >( "Pion ABC Jalonner" );
    Register< Mission_Pawn_ABC_ContreAttaquer >( "Pion ABC ContreAttaquer" );
    Register< Mission_Pawn_ABC_SemparerZone >( "Pion ABC SemparerZone" );
    Register< Mission_Pawn_ABC_ReleverUnite >( "Pion ABC ReleverUnite" );
    Register< Mission_Pawn_ABC_ArmerPIA >( "Pion ABC ArmerPIA" );
    Register< Mission_Pawn_ABC_OuvrirItineraire >( "Pion ABC OuvrirItineraire" );
    Register< Mission_Pawn_ABC_Freiner >( "Pion ABC Freiner" );
    Register< Mission_Pawn_ABC_EscorterUnite >( "Pion ABC EscorterUnite" );
    Register< Mission_Pawn_ABC_EscorterUnites >( "Pion ABC EscorterUnites" );
    Register< Mission_Pawn_ABC_ArmerPointDeControle >( "Pion ABC ArmerPointDeControle" );
    Register< Mission_Pawn_ABC_OrganiserAccueilColonneRefugies >( "Pion ABC OrganiserAccueilColonneRefugies" );
    Register< Mission_Pawn_ABC_AppuyerSurPosition >( "Pion ABC AppuyerSurPosition" );
    Register< Mission_Pawn_ABC_RASIT_RenseignerSur >( "Pion ABC RASIT RenseignerSur" );
    Register< Mission_Pawn_GEN_RealiserFosseAC >( "Pion GEN RealiserFosseAC" );
    Register< Mission_Pawn_GEN_RealiserAbattisMine >( "Pion GEN RealiserAbattisMine" );
    Register< Mission_Pawn_GEN_RealiserPointMine >( "Pion GEN RealiserPointMine" );
    Register< Mission_Pawn_GEN_RealiserBarricade >( "Pion GEN RealiserBarricade" );
    Register< Mission_Pawn_GEN_RealiserEboulement >( "Pion GEN RealiserEboulement" );
    Register< Mission_Pawn_GEN_RealiserBouchonMine >( "Pion GEN RealiserBouchonMine" );
    Register< Mission_Pawn_GEN_DetruireRoute >( "Pion GEN DetruireRoute" );
    Register< Mission_Pawn_GEN_DetruirePont >( "Pion GEN DetruirePont" );
    Register< Mission_Pawn_GEN_SecuriserItineraire >( "Pion GEN SecuriserItineraire" );
    Register< Mission_Pawn_GEN_DegagerAbattisMine >( "Pion GEN DegagerAbattisMine" );
    Register< Mission_Pawn_GEN_DegagerEboulement >( "Pion GEN DegagerEboulement" );
    Register< Mission_Pawn_GEN_DegagerRues >( "Pion GEN DegagerRues" );
    Register< Mission_Pawn_GEN_ComblerEntonnoir >( "Pion GEN ComblerEntonnoir" );
    Register< Mission_Pawn_GEN_ComblerFosseAC >( "Pion GEN ComblerFosseAC" );
    Register< Mission_Pawn_GEN_CreerContournement >( "Pion GEN CreerContournement" );
    Register< Mission_Pawn_GEN_RealiserZoneMineeLineaire >( "Pion GEN RealiserZoneMineeLineaire" );
    Register< Mission_Pawn_GEN_OuvrirTrouee >( "Pion GEN OuvrirTrouee" );
    Register< Mission_Pawn_GEN_Renforcer >( "Pion GEN Renforcer" );
    Register< Mission_Pawn_GEN_FranchirEntonnoir >( "Pion GEN FranchirEntonnoir" );
    Register< Mission_Pawn_GEN_FranchirFosseAC >( "Pion GEN FranchirFosseAC" );
    Register< Mission_Pawn_GEN_RealiserZoneMineeParDispersion >( "Pion GEN RealiserZoneMineeParDispersion" );
    Register< Mission_Pawn_GEN_RealiserPostesTir >( "Pion GEN RealiserPostesTir" );
    Register< Mission_Pawn_GEN_RealiserTravauxProtection >( "Pion GEN RealiserTravauxProtection" );
    Register< Mission_Pawn_GEN_RetablirItineraire >( "Pion GEN RetablirItineraire" );
    Register< Mission_Pawn_GEN_EquiperExploiter >( "Pion GEN EquiperExploiter" );
    Register< Mission_Pawn_GEN_ExecuterVariantement >( "Pion GEN ExecuterVariantement" );
    Register< Mission_Pawn_GEN_ReconnaitreSiteFranchissement >( "Pion GEN ReconnaitreSiteFranchissement" );
    Register< Mission_Pawn_GEN_ReconnaitreItineraire >( "Pion GEN ReconnaitreItineraire" );
    Register< Mission_Pawn_GEN_DeminerSiteFranchissement >( "Pion GEN DeminerSiteFranchissement" );
    Register< Mission_Pawn_GEN_ReconnaitreZone >( "Pion GEN ReconnaitreZone" );
    Register< Mission_Pawn_GEN_CreerPlateForme >( "Pion GEN CreerPlateForme" );
    Register< Mission_Pawn_GEN_CreerPiste >( "Pion GEN CreerPiste" );
    Register< Mission_Pawn_GEN_CreerAirePoser >( "Pion GEN CreerAirePoser" );
    Register< Mission_Pawn_GEN_NeutraliserEnginsExplosifs >( "Pion GEN NeutraliserEnginsExplosifs" );
    Register< Mission_Pawn_GEN_ActiverObstacle >( "Pion GEN ActiverObstacle" );
    Register< Mission_Pawn_GEN_AmeliorerMobilite >( "Pion GEN AmeliorerMobilite" );
    Register< Mission_Pawn_GEN_RealiserAireLogistique >( "Pion GEN RealiserAireLogistique" );
    Register< Mission_Pawn_GEN_DepolluerZone >( "Pion GEN DepolluerZone" );
    Register< Mission_Pawn_GEN_AmenagerTerrainPoser >( "Pion GEN AmenagerTerrainPoser" );
    Register< Mission_Pawn_GEN_RealiserPlateFormeDeploiement >( "Pion GEN RealiserPlateFormeDeploiement" );
    Register< Mission_Pawn_GEN_ReprendreMissionConstruction >( "Pion GEN ReprendreMissionConstruction" );
    Register< Mission_Pawn_GEN_RealiserZonePoserHelicoptere >( "Pion GEN RealiserZonePoserHelicoptere" );
    Register< Mission_Pawn_GEN_AppuyerDebarquementTroupesGU >( "Pion GEN AppuyerDebarquementTroupesGU" );
    Register< Mission_Pawn_GEN_RealiserCampPrisonniers >( "Pion GEN RealiserCampPrisonniers" );
    Register< Mission_Pawn_GEN_RealiserCampRefugies >( "Pion GEN RealiserCampRefugies" );
    Register< Mission_Pawn_INF_ReconnaitreAxe >( "Pion INF ReconnaitreAxe" );
    Register< Mission_Pawn_INF_Couvrir >( "Pion INF Couvrir" );
    Register< Mission_Pawn_INF_Reduire >( "Pion INF Reduire" );
    Register< Mission_Pawn_INF_Appuyer >( "Pion INF Appuyer" );
    Register< Mission_Pawn_INF_Fixer >( "Pion INF Fixer" );
    Register< Mission_Pawn_INF_ControlerSecteur >( "Pion INF ControlerSecteur" );
    Register< Mission_Pawn_INF_SurveillerSecteur >( "Pion INF SurveillerSecteur" );
    Register< Mission_Pawn_INF_Barrer >( "Pion INF Barrer" );
    Register< Mission_Pawn_INF_ReconnaitrePoint >( "Pion INF ReconnaitrePoint" );
    Register< Mission_Pawn_INF_Eclairer >( "Pion INF Eclairer" );
    Register< Mission_Pawn_INF_Interdire >( "Pion INF Interdire" );
    Register< Mission_Pawn_INF_Semparer >( "Pion INF Semparer" );
    Register< Mission_Pawn_INF_PrendrePreciserContact >( "Pion INF PrendrePreciserContact" );
    Register< Mission_Pawn_INF_Defendre >( "Pion INF Defendre" );
    Register< Mission_Pawn_INF_DetruireEmbuscade >( "Pion INF DetruireEmbuscade" );
    Register< Mission_Pawn_INF_ContreAttaquer >( "Pion INF ContreAttaquer" );
    Register< Mission_Pawn_INF_Freiner >( "Pion INF Freiner" );
    Register< Mission_Pawn_INF_FlancGarder >( "Pion INF FlancGarder" );
    Register< Mission_Pawn_INF_OrganiserUnCheckPoint >( "Pion INF OrganiserUnCheckPoint" );
    Register< Mission_Pawn_INF_Harceler >( "Pion INF Harceler" );
    Register< Mission_Pawn_INF_DetruireParCoupDeMain >( "Pion INF DetruireParCoupDeMain" );
    Register< Mission_Pawn_INF_EscorterUnite >( "Pion INF EscorterUnite" );
    Register< Mission_Pawn_INF_EscorterUnites >( "Pion INF EscorterUnites" );
    Register< Mission_Pawn_INF_RenseignerSur >( "Pion INF RenseignerSur" );
    Register< Mission_Pawn_INF_Soutenir >( "Pion INF Soutenir" );
    Register< Mission_Pawn_INF_Securiser >( "Pion INF Securiser" );
    Register< Mission_Pawn_INF_Recueillir >( "Pion INF Recueillir" );
    Register< Mission_Pawn_INF_RepondreAAggression >( "Pion INF RepondreAAggression" );
    Register< Mission_Pawn_INF_AppuyerSurPosition >( "Pion INF AppuyerSurPosition" );
    Register< Mission_Pawn_LOG_SeDeployer >( "Pion LOG SeDeployer" );
    Register< Mission_Pawn_LOG_Renforcer >( "Pion LOG Renforcer" );
    Register< Mission_Pawn_LOG_PreparerBascule >( "Pion LOG PreparerBascule" );
    Register< Mission_Pawn_LOG_DecontaminerBlesses >( "Pion LOG DecontaminerBlesses" );
    Register< Mission_Pawn_LOG_TrierEtTraiterBlesses >( "Pion LOG TrierEtTraiterBlesses" );
    Register< Mission_Pawn_LOG_TrierBlesses >( "Pion LOG TrierBlesses" );
    Register< Mission_Pawn_LOG_TraiterBlesses >( "Pion LOG TraiterBlesses" );
    Register< Mission_Pawn_LOG_PrendreEnCompteReactionsMentales >( "Pion LOG PrendreEnCompteReactionsMentales" );
    Register< Mission_Pawn_LOG_Convoyer >( "Pion LOG Convoyer" );
    Register< Mission_Pawn_LOG_Reparer >( "Pion LOG Reparer" );
    Register< Mission_Pawn_LOG_AppuyerMouvement >( "Pion LOG AppuyerMouvement" );
    Register< Mission_Pawn_LOG_ReconnaitreItineraire >( "Pion LOG ReconnaitreItineraire" );
    Register< Mission_Pawn_LOG_ReconnaitreZoneContaminee >( "Pion LOG ReconnaitreZoneContaminee" );
    Register< Mission_Pawn_LOG_AppuyerMouvementDansZone >( "Pion LOG AppuyerMouvementDansZone" );
    Register< Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement >( "Pion LOG MettreEnOeuvreZoneStationnement" );
    Register< Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint >( "Pion LOG AppuyerFranchissementSurUnPoint" );
    Register< Mission_Pawn_LOG_BloquerZone >( "Pion LOG BloquerZone" );
    Register< Mission_Pawn_LOG_ReconnaitreZoneDeDeploiement >( "Pion LOG ReconnaitreZoneDeDeploiement" );
    Register< Mission_Pawn_LOG_ArmerPIA >( "Pion LOG ArmerPIA" );
    Register< Mission_Pawn_LOG_Surveiller >( "Pion LOG Surveiller" );
    Register< Mission_Pawn_Test_Heliporter >( "Pion Test Heliporter" );
    Register< Mission_Pawn_Test_MoveTo >( "Pion Test MoveTo" );
    Register< Mission_Pawn_Test_CreateObject >( "Pion Test CreateObject" );
    Register< Mission_Pawn_Test_DestroyObject >( "Pion Test DestroyObject" );
    Register< Mission_Pawn_Test_CreateBypass >( "Pion Test CreateBypass" );
    Register< Mission_Pawn_Test_Fire >( "Pion Test Fire" );
    Register< Mission_Pawn_Test_ChangePosture >( "Pion Test ChangePosture" );
    Register< Mission_Pawn_Test_PathFind >( "Pion Test PathFind" );
    Register< Mission_Pawn_Test_FindPosition >( "Pion Test FindPosition" );
    Register< Mission_Pawn_Test_Reinforce >( "Pion Test Reinforce" );
    Register< Mission_Pawn_PlastronEnnemi >( "Pion PlastronEnnemi" );
    Register< Mission_Pawn_FaireMouvement >( "Pion FaireMouvement" );
    Register< Mission_Pawn_Suivre >( "Pion Suivre" );
    Register< Mission_Pawn_Transporter >( "Pion Transporter" );
    Register< Mission_Pawn_SeFaireDecontaminer >( "Pion SeFaireDecontaminer" );
    Register< Mission_Pawn_Franchir >( "Pion Franchir" );
    Register< Mission_Pawn_Decrocher >( "Pion Decrocher" );
    Register< Mission_Pawn_Stationner >( "Pion Stationner" );
    Register< Mission_Pawn_SeFaireTransporter >( "Pion SeFaireTransporter" );
    Register< Mission_Pawn_InterdireFranchissementPopulations >( "Pion InterdireFranchissementPopulations" );
    Register< Mission_Pawn_FiltrerPopulations >( "Pion FiltrerPopulations" );
    Register< Mission_Pawn_RenseignerSurPopulations >( "Pion RenseignerSurPopulations" );
    Register< Mission_Pawn_CanaliserPopulations >( "Pion CanaliserPopulations" );
    Register< Mission_Pawn_ControlerPopulationsDansZone >( "Pion ControlerPopulationsDansZone" );
    Register< Mission_Pawn_SecuriserZoneContrePopulations >( "Pion SecuriserZoneContrePopulations" );
    Register< Mission_Pawn_Generique >( "Pion Generique" );
    Register< Mission_Pawn_MILICE_Provoquer >( "Pion MILICE Provoquer" );
    Register< Mission_Pawn_MILICE_PrendreEtTenir >( "Pion MILICE PrendreEtTenir" );
    Register< Mission_Pawn_MILICE_DetruireEmbuscade >( "Pion MILICE DetruireEmbuscade" );
    Register< Mission_Pawn_JOINT_AIR_EffectuerRaid >( "Pion JOINT AIR EffectuerRaid" );
    Register< Mission_Pawn_NBC_AnimerUnPlotDeDecontamination >( "Pion NBC AnimerUnPlotDeDecontamination" );
    Register< Mission_Pawn_NBC_ReconnaitreItineraire >( "Pion NBC ReconnaitreItineraire" );
    Register< Mission_Pawn_NBC_ReconnaitreZone >( "Pion NBC ReconnaitreZone" );
    Register< Mission_Pawn_NBC_DecontaminerZone >( "Pion NBC DecontaminerZone" );
    Register< Mission_Pawn_NBC_ReconnaitreUnSiteROTA >( "Pion NBC ReconnaitreUnSiteROTA" );
    Register< Mission_Pawn_RENS_ROHUM_RenseignerSur >( "Pion RENS ROHUM RenseignerSur" );
    Register< Mission_Pawn_RENS_ROHUM_SExfiltrer >( "Pion RENS ROHUM SExfiltrer" );
    Register< Mission_Pawn_RENS_ROHUM_OrienterGuider >( "Pion RENS ROHUM OrienterGuider" );
    Register< Mission_Pawn_RENS_ROEM_Appuyer >( "Pion RENS ROEM Appuyer" );
    Register< Mission_Pawn_RENS_ROEM_RenseignerSur >( "Pion RENS ROEM RenseignerSur" );
    Register< Mission_Pawn_RENS_ROEM_MettreEnOeuvre >( "Pion RENS ROEM MettreEnOeuvre" );
    Register< Mission_Pawn_RENS_RADINT_RenseignerSur >( "Pion RENS RADINT RenseignerSur" );
    Register< Mission_Pawn_RENS_RADINT_MettreEnOeuvre >( "Pion RENS RADINT MettreEnOeuvre" );
    Register< Mission_Pawn_RENS_ROIM_MettreEnOeuvre >( "Pion RENS ROIM MettreEnOeuvre" );
    Register< Mission_Pawn_RENS_ROIM_SDTI_RenseignerSur >( "Pion RENS ROIM SDTI RenseignerSur" );
    Register< Mission_Pawn_RENS_ROIM_CL289_RenseignerSur >( "Pion RENS ROIM CL289 RenseignerSur" );
    Register< Mission_Pawn_TRANS_EtablirGererLiaison >( "Pion TRANS EtablirGererLiaison" );
    Register< Mission_Pawn_ASY_SInfiltrer >( "Pion ASY SInfiltrer" );
    Register< Mission_Pawn_ASY_CommettreAttentatContrePopulation >( "Pion ASY CommettreAttentatContrePopulation" );
    Register< Mission_Pawn_ASY_CommettreAttentatContreForcesArmees >( "Pion ASY CommettreAttentatContreForcesArmees" );
    Register< Mission_Pawn_ASY_CommettreAttentatContreInstallation >( "Pion ASY CommettreAttentatContreInstallation" );
    Register< Mission_Pawn_ASY_CommettreExactionsSurPopulationDansZone >( "Pion ASY CommettreExactionsSurPopulationDansZone" );
    Register< Mission_Pawn_ASY_Renseigner >( "Pion ASY Renseigner" );

}
    
// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::Terminate
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Pawn_Type::Terminate()
{
    for( CIT_MissionPawnTypeMap it = missions_.begin(); it != missions_.end(); ++it )
        delete it->second;
    missions_.clear();
}
