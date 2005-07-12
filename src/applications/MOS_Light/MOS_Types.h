//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Types.h $
// $Author: Nld $
// $Modtime: 11/05/05 9:58 $
// $Revision: 84 $
// $Workfile: MOS_Types.h $
//
//*****************************************************************************

#ifndef __MOS_Types_h_
#define __MOS_Types_h_

#include "MT_Tools/MT_Assert.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Vector3D.h"
#include "MT_Tools/MT_Line.h"
#include "MT_Tools/MT_Rect.h"

#include <qgl.h>

class MOS_Node;
class MOS_Tri;
class MOS_Line;
class MOS_Object;

//-------------------------------------------------------------------------
/** @name Debug mode*/
//-------------------------------------------------------------------------
//@{
#ifdef _DEBUG
#   ifndef MOS_DEBUG
#       define MOS_DEBUG
#   endif 
#endif 
//@}

//-----------------------------------------------------------------------------
// Machine type: simple client-server structure
//-----------------------------------------------------------------------------
enum SCP_MachineType
{
    eAgentServer,
    eMOSServer
};

enum SCP_ConnMagicCookie
{
    eConnMagicMOSServerAgentServer = 1
};

enum E_TypeArme
{
    eTypeArmeUnknown,
  /*  eTypeArmeInfanterie,
    eTypeArmeBlinde,
    eTypeArmeGenie,
    eTypeArme3D,
    eTypeArmeAlat,
    eTypeArmeLog,
    eTypeArmeRens,
    eTypeArmeNBC*/
};

enum E_TypeCamp
{
    eTypeCampAmi = 0,
    eTypeCampEnnemi,  
    eTypeCampNeutre,  
    eTypeCampInconnu, 
};


enum E_TypeAutomate
{
    eTypeAutomateCompagnieINF,
    eTypeAutomateEscadronXL,
    eTypeAutomatePelotonER,
    eTypeAutomateEscadronAMX,
    eTypeAutomateCompagnieGEN,
    eTypeAutomateCompagnieGENAPPUI,
    eTypeAutomateCompagnieART,
    eTypeAutomatePCSoutienSanitaire,
};



// Missions
enum E_MissionID
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

        eNbrPionMission
};

// Ordres de conduite
enum E_OrderConduiteID
{
        eOrdreConduite_MettreTenueNBC,
        eOrdreConduite_EnleverTenueNBC,
        eOrdreConduite_PasserEnSilenceRadio,
        eOrdreConduite_ArreterSilenceRadio,
        eOrdreConduite_PasserEnSilenceRadar,
        eOrdreConduite_ArreterSilenceRadar,
        eOrdreConduite_AttendreSePoster,
        eOrdreConduite_Poursuivre,
        eOrdreConduite_Interrompre,
        eOrdreConduite_ChangerReglesEngagement,
        eOrdreConduite_Deboucher,
        eOrdreConduite_Decrocher,
        eOrdreConduite_AssignerPositionEmbarquement,
        eOrdreConduite_AcquerirObjectif,
        eOrdreConduite_Brouiller,
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

    eNbrOrderConduite
};



#define MOS_DEFAULT_PORT_AS_MOS 10000

typedef uint32  MIL_AgentID;
typedef uint32  MIL_MOSContextID;
typedef uint32  MIL_SensorCapacityID;
typedef char    MOS_Position[15];

typedef uint32  MIL_LimaID;
typedef uint32  MIL_LimitID;
typedef uint8   MIL_LimaFunctionID;

#define MIL_NULL_MOS_AGENT_ID   (MIL_AgentID)-1
#define MIL_NULL_LIMIT_ID       (MIL_LimitID)-1

#define MT_PI 3.14159265358979323 //$$$ A deplacer


typedef std::vector< MOS_Node* >           T_NodePtrVector;
typedef const T_NodePtrVector              CT_NodePtrVector;
typedef T_NodePtrVector::iterator          IT_NodePtrVector;
typedef T_NodePtrVector::const_iterator    CIT_NodePtrVector;

typedef std::vector< MOS_Line* >           T_MOSLinePtrVector;
typedef const T_MOSLinePtrVector              CT_MOSLinePtrVector;
typedef T_MOSLinePtrVector::iterator          IT_MOSLinePtrVector;
typedef T_MOSLinePtrVector::const_iterator    CIT_MOSLinePtrVector;


typedef std::vector< MOS_Tri* >            T_TriPtrVector;
typedef const T_TriPtrVector               CT_TriPtrVector;
typedef T_TriPtrVector::iterator           IT_TriPtrVector;
typedef T_TriPtrVector::const_iterator     CIT_TriPtrVector;

typedef std::set< MOS_Object* >             T_ObjectPtrSet;
typedef const T_ObjectPtrSet                    CT_ObjectPtrSet;
typedef T_ObjectPtrSet::iterator                IT_ObjectPtrSet;
typedef T_ObjectPtrSet::const_iterator          CIT_ObjectPtrSet;


enum E_LightingType
{
    eLightingJourSansNuage,
    eLightingJourPeuNuageux,
    eLightingJourMoyennementNuageux,
    eLightingJourAssezNuageux,
    eLightingJourTresNuageux,

    eLightingNuitPleineLune,
    eLightingNuitTroisQuartDeLune,
    eLightingNuitDemiLune,
    eLightingNuitQuartDeLune,
    eLightingNuitNouvelleLune,

    eLightingEclairant,

    eNbrLightingType
};

enum E_WeatherType
{
    eWeatherTypeNone,
    eWeatherTypeRainOrSnowStorm,
    eWeatherTypeFog,
    eWeatherTypeDrizzle,
    eWeatherTypeRain,
    eWeatherTypeSnow,
    eWeatherTypeSmoke,

    eNbrWeatherType
};

// NB : for the MOC files
namespace DIN
{
    class DIN_Input;
    class DIN_BufferedMessage;
	class DIN_Engine;
	class DIN_Link;
}

#define MOS_SIZESELECT          8.0

#endif // __MOS_Types_h_
