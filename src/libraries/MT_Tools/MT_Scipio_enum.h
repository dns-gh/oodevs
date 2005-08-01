//*****************************************************************************
//
// $Created: FBD 02-12-16 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Scipio_enum.h $
// $Author: Nld $
// $Modtime: 17/01/05 16:26 $
// $Revision: 29 $
// $Workfile: MT_Scipio_enum.h $
//
//*****************************************************************************

#ifndef __MT_Scipio_enum_h_
#define __MT_Scipio_enum_h_

/*enum E_AgentTypeID
{
    eAgentTypeAutomateInfanterie = 0,
    eAgentTypePionInfanterie     = 1,
    eAgentTypeAutomateBlinde     = 2,
    eAgentTypePionBlinde         = 3,
    eAgentTypeAutomateGenie      = 4,
    eAgentTypePionGenie          = 5,
    eAgentTypeAutomate3D         = 6,
    eAgentTypePion3D             = 7,
    eAgentTypeAutomateLogTC2     = 8,
    eAgentTypeAutomateLogBSD     = 9,
    eAgentTypePionLog            = 10,
    eAgentTypeAutomateALAT       = 11,
    eAgentTypePionALAT           = 12,
    eAgentTypeAutomateRens       = 13,
    eAgentTypePionRens           = 14,
    eAgentTypeAutomateNBC        = 15,
    eAgentTypePionNBC            = 16,

    eNbrAgentType
};*/

enum E_TerrainObjectTypeID
{
    eTypeTerrainNothing            = 0x00000001,
	eTypeTerrainNoway              = 0x00000002,
	eTypeTerrainForest             = 0x00000004,
	eTypeTerrainForestBig          = 0x00000008,
	eTypeTerrainRoadChemin         = 0x00000010,
    eTypeTerrainRoadAutoroute      = 0x00000020,
    eTypeTerrainRoadNational       = 0x00000040,
    eTypeTerrainRoadDepartemental  = 0x00000080,
	eTypeTerrainRiverRuisseau      = 0x00000100,
	eTypeTerrainRiver              = 0x00000200,
	eTypeTerrainRiverBig           = 0x00000400,
	eTypeTerrainFleuve             = 0x00000800,
	eTypeTerrainBridge             = 0x00001000,
    eTypeTerrainBridgeBig          = 0x00002000,
	eTypeTerrainLake               = 0x00004000,
    eTypeTerrainRailRoad           = 0x00008000,
	eTypeTerrainCrest              = 0x00010000,
	eTypeTerrainUrban              = 0x00020000,
 	eTypeTerrainBorder             = 0x00040000,
	eTypeTerrainXXXXX              = 0x00080000,
	eTypeTerrainCrossRoad          = 0x00100000,
    eTypeTerrainDynaObject         = 0x00200000, //$$$ A GICLER ?
	eTypeTerrainCoast              = 0x00400000,
	eTypeTerrainTunnel             = 0x00800000,
	eTypeTerrainTunnelBig          = 0x01000000,
	eTypeTerrainPlantation         = 0x02000000,
	eTypeTerrainSwamp              = 0x04000000,
	eTypeTerrainInterchange        = 0x08000000,
	eTypeTerrainGround             = 0x10000000,
	eTypeTerrainFalaise            = 0x20000000,
	eTypeTerrainDune               = 0x40000000,
   
    eTypeTerrainPreview            = 0x80000000,

    // Attention : le laisser en dernier
	eFlagTypeNumber                 = 0xffffffff
};


enum E_ShapeTypeID
{
    eTypeShapeNode      = 0x00000001,
	eTypeShapeSegment   = 0x00000002,
	eTypeShapeTri       = 0x00000003
}; 

/*
// Missions
enum E_PionMissionID
{
    // Common
    eMission_Pion_PlastronEnnemi,
    eMission_Pion_SeDeplacer,
    eMission_Pion_Suivre,

    // ABC
    eMission_Pion_ABC_Appuyer,
    eMission_Pion_ABC_ArmerPIA,
    eMission_Pion_ABC_Attaquer,
    eMission_Pion_ABC_Barrer,
    eMission_Pion_ABC_ContreAttaquer,
    eMission_Pion_ABC_ControlerSecteur,
    eMission_Pion_ABC_Couvrir,
    eMission_Pion_ABC_Eclairer,
    eMission_Pion_ABC_Fixer,
    eMission_Pion_ABC_Jalonner,
    eMission_Pion_ABC_OuvrirItineraire,
    eMission_Pion_ABC_ReconnaitreAxe,
    eMission_Pion_ABC_ReconnaitrePoint,
    eMission_Pion_ABC_Reduire,
    eMission_Pion_ABC_ReleverUnite,
    eMission_Pion_ABC_RenseignerSur,
    eMission_Pion_ABC_SemparerZone,
    eMission_Pion_ABC_Soutenir,
    eMission_Pion_ABC_SurveillerSecteur,
    eMission_Pion_ABC_Freiner,
          
    // INF
    eMission_Pion_INF_Appuyer,
    eMission_Pion_INF_Barrer,
    eMission_Pion_INF_ContreAttaquer,
    eMission_Pion_INF_ControlerSecteur,
    eMission_Pion_INF_Couvrir,
    eMission_Pion_INF_Defendre,
    eMission_Pion_INF_DetruireEmbuscade,
    eMission_Pion_INF_Eclairer,
    eMission_Pion_INF_Fixer,
    eMission_Pion_INF_Interdire,
    eMission_Pion_INF_PrendrePreciserContact,
    eMission_Pion_INF_ReconnaitreAxe,
    eMission_Pion_INF_ReconnaitrePoint,
    eMission_Pion_INF_Reduire,
    eMission_Pion_INF_Semparer,
    eMission_Pion_INF_SurveillerSecteur,
    eMission_Pion_INF_Freiner,
    
    // GEN
    eMission_Pion_GEN_ComblerEntonnoir,
    eMission_Pion_GEN_ComblerFosseAC,
    eMission_Pion_GEN_CreerContournement,
    eMission_Pion_GEN_DegagerAbattisMine,
    eMission_Pion_GEN_DegagerEboulement,
    eMission_Pion_GEN_DegagerRues,
    eMission_Pion_GEN_DetruirePont,
    eMission_Pion_GEN_DetruireRoute,
    eMission_Pion_GEN_OuvrirTrouee,    
    eMission_Pion_GEN_RealiserAbattisMine,
    eMission_Pion_GEN_RealiserBarricade,
    eMission_Pion_GEN_RealiserBouchonMine,
    eMission_Pion_GEN_RealiserEboulement,
    eMission_Pion_GEN_RealiserFosseAC,
    eMission_Pion_GEN_RealiserPointMine,
    eMission_Pion_GEN_RealiserZoneMineeLineaire,
    eMission_Pion_GEN_Renforcer,
    eMission_Pion_GEN_SecuriserItineraire,
    eMission_Pion_GEN_FranchirEntonnoir,
    eMission_Pion_GEN_FranchirFosseAC,
    eMission_Pion_GEN_RealiserZoneMineeParDispersion,
    eMission_Pion_GEN_RealiserPostesTir,
    eMission_Pion_GEN_RealiserTravauxProtection,
    eMission_Pion_GEN_RetablirItineraire,
    eMission_Pion_GEN_EquiperExploiterDiscontinu,
    eMission_Pion_GEN_EquiperExploiterContinu,
    eMission_Pion_GEN_ExecuterVariantementDiscontinu,
    eMission_Pion_GEN_ExecuterVariantementContinu,
    eMission_Pion_GEN_ReconnaitreSiteFranchissement,
    eMission_Pion_GEN_ReconnaitreItineraire,
    eMission_Pion_GEN_ElargirTrouee,
    eMission_Pion_GEN_DeminerSiteFranchissement,
    eMission_Pion_GEN_ReconnaitreZone,
    
    // 3D - ASS
    eMission_Pion_ASS_AcquerirLanceursEniCOBRA,
    eMission_Pion_ASS_AcquerirObjectifs,
    eMission_Pion_ASS_AppliquerFeux,
    eMission_Pion_ASS_AssurerMiseEnOeuvre,
    eMission_Pion_ASS_AssurerMiseEnOeuvreLRM,
    eMission_Pion_ASS_ReconnaitreZoneDeploiement,
    eMission_Pion_ASS_ReconnaitreZoneDeploiementCOBRA,
    eMission_Pion_ASS_ReconnaitreZoneDeploiementLRM,
    eMission_Pion_ASS_SurveillerRATAC,

    // 3D - ASA
    eMission_Pion_ASA_DefendreSite,
    eMission_Pion_ASA_DefendreZone,
    eMission_Pion_ASA_DefendreUnite,
    eMission_Pion_ASA_Surveiller,
 
    // 3D - ALAT
    eMission_Pion_ALAT_ReconnaitreDansProfondeur,
    eMission_Pion_ALAT_DetruireDansZone,
    eMission_Pion_ALAT_Freiner,
    eMission_Pion_ALAT_Jalonner,
    eMission_Pion_ALAT_Heliporter,
    eMission_Pion_ALAT_Helitransporter,
    eMission_Pion_ALAT_ReconnaitreSurContourEnnemi,
    eMission_Pion_ALAT_DetruireUnite,
    eMission_Pion_ALAT_Escorter,
    eMission_Pion_ALAT_Eclairer,
    eMission_Pion_ALAT_ReconnaitrePoint,
    eMission_Pion_ALAT_Surveiller,
    eMission_Pion_ALAT_Relever,

    // RENS
    eMission_Pion_RENS_RenseignerSur,
    eMission_Pion_RENS_SExfiltrer,

    // NBC
    eMission_Pion_NBC_Decontaminer,
    eMission_Pion_NBC_ReconnaitreItineraire,
    eMission_Pion_NBC_ReconnaitrePoint,

    // BSD
    eMission_Pion_BSD_DistribuerMoyens,
    eMission_Pion_BSD_SeDeployer,
    eMission_Pion_BSD_TrierBlesses,
    eMission_Pion_BSD_TrierEtTraiterUE,
    eMission_Pion_BSD_TraiterUE,
    eMission_Pion_BSD_DistribuerRessources,
    eMission_Pion_BSD_Reparer,

    // Test
    eMission_Pion_Test_MoveTo,              
    eMission_Pion_Test_CreateObject,
    eMission_Pion_Test_DestroyObject,
    eMission_Pion_Test_CreateBypass,
    eMission_Pion_Test_Fire,
    eMission_Pion_Test_ChangePosture,
    eMission_Pion_Test_PathFind,
    eMission_Pion_Test_FindPosition,
    eMission_Pion_Test_Reinforce, 

    eNbrPionMission
};
*
enum E_AutomateMissionID
{
    // Common
    eMission_Automate_SeDeplacer,

    // ABC
    eMission_Automate_ABC_Appuyer,
    eMission_Automate_ABC_Attaquer,
    eMission_Automate_ABC_ContreAttaquer,
    eMission_Automate_ABC_Controler,
    eMission_Automate_ABC_Couvrir,
    eMission_Automate_ABC_DonnerCoupArret,
    eMission_Automate_ABC_Eclairer,
    eMission_Automate_ABC_EffectuerRaid,
    eMission_Automate_ABC_Fixer,
    eMission_Automate_ABC_Freiner,
    eMission_Automate_ABC_Interdire,
    eMission_Automate_ABC_Jalonner,
    eMission_Automate_ABC_PrendrePreciserContact,
    eMission_Automate_ABC_Reconnaitre,
    eMission_Automate_ABC_ReconnaitrePoint,
    eMission_Automate_ABC_Reduire,
    eMission_Automate_ABC_ReleverEnDepassant,
    eMission_Automate_ABC_Semparer,
    eMission_Automate_ABC_SurveillerItineraire,
    eMission_Automate_ABC_Tenir,
    eMission_Automate_ABC_Soutenir,
    eMission_Automate_ABC_SurveillerZone,
    eMission_Automate_ABC_FlancGarder,
    eMission_Automate_ABC_RenseignerSurUnAxe,
    eMission_Automate_ABC_RenseignerSur,


    // INF
    eMission_Automate_INF_AttaquerEnTerrainDifficile,
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
    eMission_Automate_INF_BarrerDirection,
    eMission_Automate_INF_DefendreFerme,
    eMission_Automate_INF_Tenir,

    // GEN                                               
    eMission_Automate_GEN_Renforcer,
    eMission_Automate_GEN_RealiserSystemeObstacles,
    eMission_Automate_GEN_AmenagerAireStationnement,
    eMission_Automate_GEN_RetablirItineraires,
    eMission_Automate_GEN_FranchirDeViveForce,
    eMission_Automate_GEN_FaireFranchir,

    // LOG : SANTE
    eMission_Automate_BSD_SeDeployer,
    eMission_Automate_BSD_MettreEnOeuvreZone, 
    eMission_Automate_TC2_SeDeployer,

    // ASS
    eMission_Automate_ASS_AssurerMiseEnOeuvre,
    eMission_Automate_ASS_AssurerMiseEnOeuvreLRM,
    eMission_Automate_ASS_AcquerirObjectifs,
    eMission_Automate_ASS_SurveillerRATAC,
    eMission_Automate_ASS_AcquerirLanceursEniCOBRA,
    eMission_Automate_ASS_SuivreDesObjectifsRATAC,
    
    // ASA
    eMission_Automate_ASA_Surveiller,
    eMission_Automate_ASA_DefendreSite,
    eMission_Automate_ASA_DefendreZone,
    eMission_Automate_ASA_DefendreSiteMISTRAL,
    eMission_Automate_ASA_DefendreZoneMISTRAL,
    eMission_Automate_ASA_DefendreUnite,
    

    // ALAT
    eMission_Automate_ALAT_Heliporter,

    // RENS
    eMission_Automate_RENS_RenseignerSur,
    eMission_Automate_RENS_SExfiltrer,

    // NBC
    eMission_Automate_NBC_ReconnaitreSurDeuxAxes,
    eMission_Automate_NBC_ReconnaitreSurDeuxZones,
    eMission_Automate_NBC_Decontaminer,

    // 
    // Tests
    // 
    eMission_Automate_Test_MoveTo,

    eNbrAutomateMission
};*/
/*
// Ordres de conduite
enum E_OrderConduiteID
{
    eOrdreConduite_AttendreSePoster,
    eOrdreConduite_Poursuivre,
    eOrdreConduite_Interrompre,
    eOrdreConduite_MettreTenueNBC,
    eOrdreConduite_EnleverTenueNBC,
    eOrdreConduite_Pion_Contourner,
	eOrdreConduite_Pion_Fixer,
	eOrdreConduite_Pion_Detruire,
	eOrdreConduite_Pion_Decrocher,
	eOrdreConduite_Pion_RompreLeContact,
    eOrdreConduite_Pion_Deboucher, 
    eOrdreConduite_Pion_ActiverObstacle, 
    eOrdreConduite_Pion_ChangerDePosition, 
    eOrdreConduite_Pion_ExecuterTir,
    eOrdreConduite_Pion_AppliquerFeux,
    eOrdreConduite_Pion_PrendreContact,
    eOrdreConduite_Pion_PreciserContact,
    eOrdreConduite_Pion_Emettre,
    eOrdreConduite_Pion_RenforcerTC2EnVSRAM,
    eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM,
    eOrdreConduite_Pion_TirEO,
    eOrdreConduite_Automate_AppliquerFeux,
    eOrdreConduite_Automate_ChangerDePosition,
    eOrdreConduite_Automate_ChangerDePositionPour,
    eOrdreConduite_Automate_ActiverSecteur,
    eOrdreConduite_Automate_Renforcer,
    eOrdreConduite_Automate_ReprendreAuxOrdres,
    eOrdreConduite_Automate_RealiserVariantement,
    eOrdreConduite_Automate_DesequiperSiteFranchissement,
    eOrdreConduite_Automate_ReagirFaceAEni,
    eOrdreConduite_Automate_AffecterPionAObstacle,
    eOrdreConduite_Automate_TirEO,
    eOrdreConduite_Automate_ReconnaitreZoneImplantation,
    eOrdreConduite_Automate_ExecuterTir,
    eOrdreConduite_Automate_AcquerirObjectifs,
    eOrdreConduite_Automate_Emettre,
    eOrdreConduite_Automate_AcquerirLanceursEniCOBRA,

    eNbrOrderConduite
};
*/

enum E_NatureWeaponType
{
    eNatureWeapon_None,
    eNatureWeapon_Antichar,
    eNatureWeapon_Administration,
    eNatureWeapon_DefenseAerienne,
    eNatureWeapon_CentreSoutienOperationsAeriennes,
    eNatureWeapon_Blinde,
    eNatureWeapon_Aviation,
    eNatureWeapon_Cavalerie,
    eNatureWeapon_Genie,
    eNatureWeapon_Deminage,
    eNatureWeapon_SoutienArtillerie,
    eNatureWeapon_Artillerie,
    eNatureWeapon_Infanterie,
    eNatureWeapon_GuerreInformation,
    eNatureWeapon_ForcesSecuriteInterne,
    eNatureWeapon_ExecutionLoi,
    eNatureWeapon_SoutienAtterrissage,
    eNatureWeapon_Logistique,
    eNatureWeapon_Reparation,
    eNatureWeapon_Manoeuvre,
    eNatureWeapon_Medical,
    eNatureWeapon_RenseignementMilitaire,
    eNatureWeapon_Missile,
    eNatureWeapon_Nbc,
    eNatureWeapon_Inconnue,
    eNatureWeapon_NonSpecifiee,
    eNatureWeapon_Reconnaissance,
    eNatureWeapon_Transmissions,
    eNatureWeapon_Ravitaillement,
    eNatureWeapon_TacticalAirControlParty,
    eNatureWeapon_Transport
};

enum E_NatureSpecializationType
{
    eNatureSpec_None,
    eNatureSpec_Aerien,
    eNatureSpec_AttaqueAerienne,
    eNatureSpec_Aeroporte,
    eNatureSpec_Blinde,
    eNatureSpec_Compose,
    eNatureSpec_Construction,
    eNatureSpec_Dentiste,
    eNatureSpec_GuerreElectronique,
    eNatureSpec_Finance,
    eNatureSpec_VoilureFixe,
    eNatureSpec_Terrestre,
    eNatureSpec_Helicoptere,
    eNatureSpec_Jag,
    eNatureSpec_Mecanise,
    eNatureSpec_PoliceMilitaire,
    eNatureSpec_Motorise,
    eNatureSpec_Naval,
    eNatureSpec_ServiceDuPersonnel,
    eNatureSpec_Courrier,
    eNatureSpec_Psychologique,
    eNatureSpec_CheminDeFer,
    eNatureSpec_Religieux,
    eNatureSpec_Riverine,
    eNatureSpec_SecurityPolice,
    eNatureSpec_PatrouilleCotiere,
    eNatureSpec_Chirurgie,
    eNatureSpec_Veterinaire,
    eNatureSpec_DecollageAtterissageVerticalCourt
};

enum E_NatureQualifierType
{
    eNatureQualifier_None,
    eNatureQualifier_Lourd,
    eNatureQualifier_Leger,
    eNatureQualifier_Moyen
};

enum E_NatureCategoryType
{
    eNatureCategory_None,
    eNatureCategory_Combat,
    eNatureCategory_SoutienLogistique,
    eNatureCategory_CombatSupport,
    eNatureCategory_Inconnu
};

enum E_NatureMobilityType
{
    eNatureMobility_None,
    eNatureMobility_Aeroporte,
    eNatureMobility_AerienCompose,
    eNatureMobility_AerienVoilureFixe,
    eNatureMobility_AerienPlusLegerQueAir,
    eNatureMobility_AerienAilesRotatives,
    eNatureMobility_AerienAmphibie,
    eNatureMobility_AnimalMonte,
    eNatureMobility_Demonte,
    eNatureMobility_TerrestreCheminDeFer,
    eNatureMobility_TerrestreAutoPropulse,
    eNatureMobility_TerrestreChenille,
    eNatureMobility_TerrestreRemorque,
    eNatureMobility_TerrestreRoues,
    eNatureMobility_MaritimeSousMarin,
    eNatureMobility_MaritimeSurface,
    eNatureMobility_Inconnu
};

enum E_NatureLevelType
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
    eNatureLevel_xxxxx
};

enum E_NatureAtlasType
{
    eNatureAtlas_Blinde         = 0x8000, // NB : valeurs imposés par l'ASN
    eNatureAtlas_ASS            = 0x4000,
    eNatureAtlas_PC             = 0x2000,
    eNatureAtlas_Log            = 0x1000,
    eNatureAtlas_Mortier        = 0x0800,
    eNatureAtlas_LanceRoquette  = 0x0400,
    eNatureAtlas_Vehicule       = 0x0200,
    eNatureAtlas_PersonnelAPied = 0x0100,
    eNatureAtlas_Helicoptere    = 0x0080,
    eNatureAtlas_Indefini       = 0x0040,
    eNatureAtlas_None           = 0x0020
};

enum E_NatureCapaciteMissionType
{
    eNatureCapaciteMission_CivilianLawEnforcement,
    eNatureCapaciteMission_CommandOperations,
    eNatureCapaciteMission_CouvertureAerienne,
    eNatureCapaciteMission_JointIntelligence,
    eNatureCapaciteMission_MoraleWelfareRecreation,
    eNatureCapaciteMission_ReglageElectronique,
    eNatureCapaciteMission_Remplacement,
    eNatureCapaciteMission_ServiceMortuaire,
    eNatureCapaciteMission_SignalsIntelligence,
    eNatureCapaciteMission_TheatreMissileDefence,
    eNatureCapaciteMission_Utilitaire,
    eNatureCapaciteMission_Aéroporte,
    eNatureCapaciteMission_AffairesPubliques,
    eNatureCapaciteMission_AffairesPubliques_Communication,
    eNatureCapaciteMission_AffairesPubliques_JointInformation,
    eNatureCapaciteMission_Alpin,
    eNatureCapaciteMission_Amphibie,
    eNatureCapaciteMission_Arctique,
    eNatureCapaciteMission_ArtillerieReconnaissance,
    eNatureCapaciteMission_Attaque,
    eNatureCapaciteMission_AttaqueAerienne,
    eNatureCapaciteMission_C2,
    eNatureCapaciteMission_CheminDeFer,
    eNatureCapaciteMission_Ciblage,
    eNatureCapaciteMission_ContreEspionnage,
    eNatureCapaciteMission_Eclairage,
    eNatureCapaciteMission_Entretien,
    eNatureCapaciteMission_EntretienMateriel,
    eNatureCapaciteMission_EntretienMissile,
    eNatureCapaciteMission_EntretienOptoElectronique,
    eNatureCapaciteMission_Espionnage,
    eNatureCapaciteMission_EspionnageMilitaireAerialExploitation,
    eNatureCapaciteMission_EspionnageMilitaireOperation,
    eNatureCapaciteMission_EspionnageMilitaireTacticalExploitation,
    eNatureCapaciteMission_EvacuationSanitaire,
    eNatureCapaciteMission_Finance,
    eNatureCapaciteMission_GenieCombat,
    eNatureCapaciteMission_GenieConstruction,
    eNatureCapaciteMission_GenieConstructionNavale,
    eNatureCapaciteMission_GuerreElectronique,
    eNatureCapaciteMission_GuerreElectroniqueInterception,
    eNatureCapaciteMission_GuerreElectroniqueBrouillage,
    eNatureCapaciteMission_GuerreElectroniqueDétection,
    eNatureCapaciteMission_Interrogation,
    eNatureCapaciteMission_MainDOeuvre,
    eNatureCapaciteMission_MaintienDeLaPaix,
    eNatureCapaciteMission_Marine,
    eNatureCapaciteMission_MineDeContreMesure,
    eNatureCapaciteMission_NBCBiologique,
    eNatureCapaciteMission_NBCChimique,
    eNatureCapaciteMission_NBCChimiqueFumee,
    eNatureCapaciteMission_NBCDecontamination,
    eNatureCapaciteMission_NBCDecontaminationChimique,
    eNatureCapaciteMission_NBCNucleaire,
    eNatureCapaciteMission_NonRenseigne,
    eNatureCapaciteMission_Observation,
    eNatureCapaciteMission_ObservationCapteur,
    eNatureCapaciteMission_ObservationLonguePortee,
    eNatureCapaciteMission_ObservationMeteo,
    eNatureCapaciteMission_ObservationModuleTerrestre,
    eNatureCapaciteMission_PurificationDeLEau,
    eNatureCapaciteMission_Ravitaillement_ClasseI,
    eNatureCapaciteMission_Ravitaillement_ClasseII,
    eNatureCapaciteMission_Ravitaillement_ClasseIIIAviation,
    eNatureCapaciteMission_Ravitaillement_ClasseIII,
    eNatureCapaciteMission_Ravitaillement_ClasseIV,
    eNatureCapaciteMission_Ravitaillement_ClasseV,
    eNatureCapaciteMission_Ravitaillement_Eau,
    eNatureCapaciteMission_RavitaillementBlanchisserieBain,
    eNatureCapaciteMission_RechercheEtSauvetage,
    eNatureCapaciteMission_Reconnaissance,
    eNatureCapaciteMission_ReconnaissanceAerienneTactique,
    eNatureCapaciteMission_Recuperation,
    eNatureCapaciteMission_ServiceDuPersonnel,
    eNatureCapaciteMission_ServiceJuridique,
    eNatureCapaciteMission_SupportAerienRapproche,
    eNatureCapaciteMission_Transmissions_ForwardCommunications,
    eNatureCapaciteMission_Transmissions_NodeCentre,
    eNatureCapaciteMission_Transmissions_NodeLargeExtension,
    eNatureCapaciteMission_Transmissions_NodeSmallExtension,
    eNatureCapaciteMission_Transmissions_RadioRelay,
    eNatureCapaciteMission_Transmissions_RadioTacticalSatellite,
    eNatureCapaciteMission_Transmissions_RadioTeletype,
    eNatureCapaciteMission_Transmissions_Support,
    eNatureCapaciteMission_Transmissions_Radio,
    eNatureCapaciteMission_Transport_AllocationDesMouvements,
    eNatureCapaciteMission_Transport_APOD_APOE,
    eNatureCapaciteMission_Transport_SPOD_SPOE,
    eNatureCapaciteMission_TransportMissile,
    eNatureCapaciteMission_VerrouillageDeCible,
    eNatureCapaciteMission_VerrouillageDeCible_Flash,
    eNatureCapaciteMission_VerrouillageDeCible_Radar,
    eNatureCapaciteMission_VerrouillageDeCible_Sonore,
};

enum E_ForceRatioState
{
    eForceRatioStateNone,
    eForceRatioStateNeutral,
    eForceRatioStateFavorable,
    eForceRatioStateUnfavorable,
};
enum E_RulesOfEngagementState 
{
    eRoeStateNone,
    eRoeStateFreeFire,
    eRoeStateRestrictedFire,
    eRoeStateFireByOrder
};
enum E_CloseCombatState 
{
    eCloseCombatStateNone,
    eCloseCombatStateDodge,
    eCloseCombatStateFixed,
    eCloseCombatStateProceedMission
};

enum E_OperationalState
{
    eOpStateFullyDestroyed,
    eOpStateTacticallyDestroyed,
    eOpStateOperational,
};


/*enum E_ObjectTypeID
{
    // Objets provoquant des attritions
    eObjectTypeBouchonMines             = 0,
    eObjectTypeZoneMineeLineaire        = 1,
    eObjectTypeZoneMineeParDispersion   = 2,

    eNbrAttritionObjectType             = 3,

    // Objets sans attritions
    eObjectTypeFosseAntiChar            = 3,
    eObjectTypeAbatti                   = 4,
    eObjectTypeBarricade                = 5,
    eObjectTypeEboulement               = 6,
    eObjectTypeDestructionRoute         = 7,
    eObjectTypeDestructionPont          = 8,
    eObjectTypePontFlottant             = 9,
    eObjectTypePosteTir                 = 10,
    eObjectTypeZoneProtegee             = 11,
    eObjectTypeZoneImplantationCanon    = 12,
    eObjectTypeZoneImplantationCOBRA    = 13,
    eObjectTypeZoneImplantationLRM      = 14,
    eObjectTypeSiteFranchissement       = 15,
    eObjectTypeNuageNBC                 = 16,
    eObjectTypePlotDecontamination      = 17,
    eObjectTypePlotRavitaillement       = 18,

    eNbrObjectType                      = 19
};*/
#endif // __MT_Scipio_enum_h_