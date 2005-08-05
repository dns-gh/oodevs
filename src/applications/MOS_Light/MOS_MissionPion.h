// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: 2005-08-05 - 14:24:58 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_MissionPion_Skeleton.h $
// $Author: Nld $
// $Modtime: 18/10/04 13:04 $
// $Revision: 3 $
// $Workfile: AGR_MOS_MissionPion_Skeleton.h $
//
// *****************************************************************************

#ifndef __MOS_MissionPion_h_
#define __MOS_MissionPion_h_

#include "MOS_Types.h"

#include "MOS_Mission_ABC.h"

class MOS_ASN_MsgPionOrder;

//=============================================================================
// Created:  NLD 2005-08-05 - 14:24:58
//=============================================================================
class MOS_MissionPion : public MOS_Mission_ABC
{
    MT_COPYNOTALLOWED( MOS_MissionPion )

public:
     MOS_MissionPion( E_MissionID nTypeMission, QWidget* pParent = 0 );
    ~MOS_MissionPion();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Terminate ();

    virtual void SetAgent( MOS_Agent* pAgent );
    virtual bool IsAutomateMission() const;
    virtual std::string  GetName          () const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Exit*/
    //-------------------------------------------------------------------------
    //@{
    void Cancel();
    void Validate();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void CreateMissionTools();
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}


    //-------------------------------------------------------------------------
    /** @name Missions*/
    //-------------------------------------------------------------------------
    //@{
     void CreateMission_ALAT_EvacuationSanitaire();
    void CreateMission_ALAT_ReconnaitreDansLaProfondeur();
    void CreateMission_ALAT_ReconnaitreContourEnnemi();
    void CreateMission_ALAT_DetruireNeutraliserDansZone();
    void CreateMission_ALAT_DetruireAeromobile();
    void CreateMission_ALAT_Freiner();
    void CreateMission_ALAT_Jalonner();
    void CreateMission_ALAT_Escorter();
    void CreateMission_ALAT_Heliporter();
    void CreateMission_ALAT_HeliporterHelitransporterPlotRavitaillement();
    void CreateMission_ALAT_Helitransporter();
    void CreateMission_ALAT_EffectuerRechercheEtSauvetage();
    void CreateMission_ALAT_IMEX();
    void CreateMission_ALAT_Eclairer();
    void CreateMission_ALAT_Surveiller();
    void CreateMission_ALAT_AppuyerDirectAuContact();
    void CreateMission_ALAT_Couvrir();
    void CreateMission_ALAT_Attaquer();
    void CreateMission_ASA_DefendreSite();
    void CreateMission_ASA_DefendreSiteAPartirPosition();
    void CreateMission_ASA_DefendreZone();
    void CreateMission_ASA_DefendreZoneAPartirPosition();
    void CreateMission_ASA_Surveiller();
    void CreateMission_ASA_DefendreUnites();
    void CreateMission_ASA_MISTRAL_DefendreSite();
    void CreateMission_ASA_MISTRAL_DefendreSiteAPartirPosition();
    void CreateMission_ASA_MISTRAL_DefendreZone();
    void CreateMission_ASA_MISTRAL_DefendreZoneAPartirPosition();
    void CreateMission_ASA_MISTRAL_Surveiller();
    void CreateMission_ASS_AssurerMiseEnOeuvre();
    void CreateMission_ASS_AssurerMiseEnOeuvreSurPosition();
    void CreateMission_ASS_ReconnaitreZoneDeploiement();
    void CreateMission_ASS_AcquerirLanceursEni();
    void CreateMission_ASS_EO_AcquerirObjectifs();
    void CreateMission_ASS_EOP_AcquerirObjectifs();
    void CreateMission_ASS_RATAC_AcquerirObjectifs();
    void CreateMission_ABC_ReconnaitreAxe();
    void CreateMission_ABC_Soutenir();
    void CreateMission_ABC_Couvrir();
    void CreateMission_ABC_FlancGarder();
    void CreateMission_ABC_Reduire();
    void CreateMission_ABC_Appuyer();
    void CreateMission_ABC_Fixer();
    void CreateMission_ABC_RenseignerSur();
    void CreateMission_ABC_ControlerZone();
    void CreateMission_ABC_Surveiller();
    void CreateMission_ABC_Barrer();
    void CreateMission_ABC_Attaquer();
    void CreateMission_ABC_ReconnaitrePoint();
    void CreateMission_ABC_Eclairer();
    void CreateMission_ABC_Jalonner();
    void CreateMission_ABC_ContreAttaquer();
    void CreateMission_ABC_SemparerZone();
    void CreateMission_ABC_ReleverUnite();
    void CreateMission_ABC_ArmerPIA();
    void CreateMission_ABC_OuvrirItineraire();
    void CreateMission_ABC_Freiner();
    void CreateMission_ABC_EscorterUnConvoi();
    void CreateMission_ABC_ArmerPointDeControle();
    void CreateMission_ABC_OrganiserAccueilColonneRefugies();
    void CreateMission_GEN_RealiserFosseAC();
    void CreateMission_GEN_RealiserAbattisMine();
    void CreateMission_GEN_RealiserPointMine();
    void CreateMission_GEN_RealiserBarricade();
    void CreateMission_GEN_RealiserEboulement();
    void CreateMission_GEN_RealiserBouchonMine();
    void CreateMission_GEN_DetruireRoute();
    void CreateMission_GEN_DetruirePont();
    void CreateMission_GEN_SecuriserItineraire();
    void CreateMission_GEN_DegagerAbattisMine();
    void CreateMission_GEN_DegagerEboulement();
    void CreateMission_GEN_DegagerRues();
    void CreateMission_GEN_ComblerEntonnoir();
    void CreateMission_GEN_ComblerFosseAC();
    void CreateMission_GEN_CreerContournement();
    void CreateMission_GEN_RealiserZoneMineeLineaire();
    void CreateMission_GEN_OuvrirTrouee();
    void CreateMission_GEN_Renforcer();
    void CreateMission_GEN_FranchirEntonnoir();
    void CreateMission_GEN_FranchirFosseAC();
    void CreateMission_GEN_RealiserZoneMineeParDispersion();
    void CreateMission_GEN_RealiserPostesTir();
    void CreateMission_GEN_RealiserTravauxProtection();
    void CreateMission_GEN_RetablirItineraire();
    void CreateMission_GEN_EquiperExploiter();
    void CreateMission_GEN_ExecuterVariantement();
    void CreateMission_GEN_ReconnaitreSiteFranchissement();
    void CreateMission_GEN_ReconnaitreItineraire();
    void CreateMission_GEN_DeminerSiteFranchissement();
    void CreateMission_GEN_ReconnaitreZone();
    void CreateMission_GEN_CreerPlateForme();
    void CreateMission_GEN_CreerPiste();
    void CreateMission_GEN_CreerAirePoser();
    void CreateMission_GEN_NeutraliserEnginsExplosifs();
    void CreateMission_GEN_ActiverObstacle();
    void CreateMission_GEN_AmeliorerMobilite();
    void CreateMission_GEN_RealiserAireLogistique();
    void CreateMission_GEN_DepolluerZone();
    void CreateMission_GEN_AmenagerTerrainPoser();
    void CreateMission_GEN_RealiserPlateFormeDeploiement();
    void CreateMission_GEN_ReprendreMissionConstruction();
    void CreateMission_GEN_RealiserZonePoserHelicoptere();
    void CreateMission_GEN_AppuyerDebarquementTroupesGU();
    void CreateMission_GEN_RealiserCampPrisonniers();
    void CreateMission_GEN_RealiserCampRefugies();
    void CreateMission_INF_ReconnaitreAxe();
    void CreateMission_INF_Couvrir();
    void CreateMission_INF_Reduire();
    void CreateMission_INF_Appuyer();
    void CreateMission_INF_Fixer();
    void CreateMission_INF_ControlerSecteur();
    void CreateMission_INF_SurveillerSecteur();
    void CreateMission_INF_Barrer();
    void CreateMission_INF_ReconnaitrePoint();
    void CreateMission_INF_Eclairer();
    void CreateMission_INF_Interdire();
    void CreateMission_INF_Semparer();
    void CreateMission_INF_PrendrePreciserContact();
    void CreateMission_INF_Defendre();
    void CreateMission_INF_DetruireEmbuscade();
    void CreateMission_INF_ContreAttaquer();
    void CreateMission_INF_Freiner();
    void CreateMission_INF_FlancGarder();
    void CreateMission_INF_OrganiserUnCheckPoint();
    void CreateMission_INF_Harceler();
    void CreateMission_INF_DetruireParCoupDeMain();
    void CreateMission_INF_EscorterUnConvoi();
    void CreateMission_INF_RenseignerSur();
    void CreateMission_INF_Soutenir();
    void CreateMission_INF_Securiser();
    void CreateMission_INF_Recueillir();
    void CreateMission_INF_RepondreAAggression();
    void CreateMission_LOG_SeDeployer();
    void CreateMission_LOG_DistribuerMoyens();
    void CreateMission_LOG_TrierBlesses();
    void CreateMission_LOG_TraiterBlesses();
    void CreateMission_LOG_TrierEtTraiterBlesses();
    void CreateMission_LOG_SoignerBlesses();
    void CreateMission_LOG_PrendreEnCompteReactionsMentales();
    void CreateMission_LOG_DecontaminerBlesses();
    void CreateMission_LOG_Reparer();
    void CreateMission_LOG_Evacuer();
    void CreateMission_LOG_Convoyer();
    void CreateMission_LOG_DistribuerRessources();
    void CreateMission_LOG_ConstituerConvoi();
    void CreateMission_LOG_Livrer();
    void CreateMission_LOG_PreparerBascule();
    void CreateMission_LOG_AppuyerMouvement();
    void CreateMission_LOG_ReconnaitreItineraire();
    void CreateMission_LOG_TransporterUniteChars();
    void CreateMission_LOG_ReconnaitreZoneContaminee();
    void CreateMission_LOG_AppuyerMouvementDansZone();
    void CreateMission_LOG_MettreEnOeuvreZoneStationnement();
    void CreateMission_LOG_AppuyerFranchissementSurUnPoint();
    void CreateMission_LOG_BloquerZone();
    void CreateMission_LOG_ReconnaitreZoneDeDeploiement();
    void CreateMission_LOG_ArmerPIA();
    void CreateMission_LOG_Surveiller();
    void CreateMission_Test_Heliporter();
    void CreateMission_Test_MoveTo();
    void CreateMission_Test_CreateObject();
    void CreateMission_Test_DestroyObject();
    void CreateMission_Test_CreateBypass();
    void CreateMission_Test_Fire();
    void CreateMission_Test_ChangePosture();
    void CreateMission_Test_PathFind();
    void CreateMission_Test_FindPosition();
    void CreateMission_Test_Reinforce();
    void CreateMission_PlastronEnnemi();
    void CreateMission_FaireMouvement();
    void CreateMission_Suivre();
    void CreateMission_SeFaireDecontaminer();
    void CreateMission_Franchir();
    void CreateMission_Decrocher();
    void CreateMission_Stationner();
    void CreateMission_SeFaireTransporter();
    void CreateMission_Generique();
    void CreateMission_MILICE_Provoquer();
    void CreateMission_MILICE_PrendreEtTenir();
    void CreateMission_MILICE_DetruireEmbuscade();
    void CreateMission_JOINT_MARINE_Transporter();
    void CreateMission_JOINT_AIR_EffectuerRaid();
    void CreateMission_NBC_AnimerUnPlotDeDecontamination();
    void CreateMission_NBC_ReconnaitreItineraire();
    void CreateMission_NBC_ReconnaitreZone();
    void CreateMission_NBC_DecontaminerZone();
    void CreateMission_NBC_ReconnaitreUnSiteROTA();
    void CreateMission_RENS_ROHUM_RenseignerSur();
    void CreateMission_RENS_ROHUM_SExfiltrer();
    void CreateMission_RENS_ROHUM_OrienterGuider();
    void CreateMission_RENS_ROEM_Appuyer();
    void CreateMission_RENS_ROEM_RenseignerSur();
    void CreateMission_RENS_ROEM_MettreEnOeuvre();
    void CreateMission_RENS_RADINT_RenseignerSur();
    void CreateMission_RENS_RADINT_MettreEnOeuvre();
    void CreateMission_RENS_ROIM_MettreEnOeuvre();
    void CreateMission_RENS_ROIM_SDTI_RenseignerSur();
    void CreateMission_RENS_ROIM_CL289_RenseignerSur();
    void CreateMission_TRANS_EtablirGererLiaison();

    //@}

    //-------------------------------------------------------------------------
    /** @name Network */
    //-------------------------------------------------------------------------
    //@{
    virtual void SendMsg( std::string& sParam );
    //@}

private:
    MOS_ASN_MsgPionOrder*   pASNMsgOrder_;

    QTabWidget*     pLimaLimitWidget_;
    QVBoxLayout*    pLimaLimitLayout_;
    QTabWidget*     pCommonWidget_;
    QVBoxLayout*    pCommonLayout_;
};


#endif // __MOS_MissionPion_h_