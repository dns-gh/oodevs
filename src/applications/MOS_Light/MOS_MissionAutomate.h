// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_MissionAutomate_Skeleton.h $
// $Author: Nld $
// $Modtime: 18/10/04 13:04 $
// $Revision: 3 $
// $Workfile: AGR_MOS_MissionAutomate_Skeleton.h $
//
// *****************************************************************************

#ifndef __MOS_MissionAutomate_h_
#define __MOS_MissionAutomate_h_

#include "MOS_Types.h"

#include "MOS_Mission_ABC.h"

class MOS_ASN_MsgAutomateOrder;

//=============================================================================
// Created:  2005-08-19 - 11:30:56
//=============================================================================
class MOS_MissionAutomate : public MOS_Mission_ABC
{
    MT_COPYNOTALLOWED( MOS_MissionAutomate )

public:
     MOS_MissionAutomate( E_MissionID nTypeMission, QWidget* pParent = 0 );
    ~MOS_MissionAutomate();

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
    //@}

    //-------------------------------------------------------------------------
    /** @name Missions*/
    //-------------------------------------------------------------------------
    //@{
    void CreateMission_ABC_ArmerPointDeControle();
    void CreateMission_ABC_OrganiserAccueilColonneRefugies();
    void CreateMission_ABC_ProtegerPC();
    void CreateMission_ABC_Appuyer();
    void CreateMission_ABC_Attaquer();
    void CreateMission_ABC_ContreAttaquer();
    void CreateMission_ABC_Controler();
    void CreateMission_ABC_Couvrir();
    void CreateMission_ABC_DonnerCoupArret();
    void CreateMission_ABC_Eclairer();
    void CreateMission_ABC_EffectuerRaid();
    void CreateMission_ABC_Interdire();
    void CreateMission_ABC_Jalonner();
    void CreateMission_ABC_PrendrePreciserContact();
    void CreateMission_ABC_Reconnaitre();
    void CreateMission_ABC_ReconnaitrePoint();
    void CreateMission_ABC_Reduire();
    void CreateMission_ABC_ReleverEnDepassant();
    void CreateMission_ABC_Semparer();
    void CreateMission_ABC_Tenir();
    void CreateMission_ABC_Soutenir();
    void CreateMission_ABC_Surveiller();
    void CreateMission_ABC_FlancGarder();
    void CreateMission_ABC_RenseignerSur();
    void CreateMission_ABC_Fixer();
    void CreateMission_ABC_Freiner();
    void CreateMission_ABC_SurveillerItineraire();
    void CreateMission_ABC_RenseignerSurUnAxe();
    void CreateMission_ABC_RecueillirUnite();
    void CreateMission_ABC_EscorterUnConvoi();
    void CreateMission_ALAT_Heliporter();
    void CreateMission_ALAT_Jalonner();
    void CreateMission_ALAT_Helitransporter();
    void CreateMission_ALAT_ExtrairePersonnel();
    void CreateMission_ALAT_Appuyer();
    void CreateMission_ALAT_Couvrir();
    void CreateMission_ALAT_Surveiller();
    void CreateMission_ALAT_Reconnaitre();
    void CreateMission_ALAT_Freiner();
    void CreateMission_ALAT_Escorter();
    void CreateMission_ALAT_Attaquer();
    void CreateMission_ALAT_RecueillirFormationAeromobile();
    void CreateMission_ALAT_AttaquerFormationAeromobile();
    void CreateMission_ALAT_DonnerCoupArret();
    void CreateMission_ALAT_DetruireNeutraliserDansProfondeur();
    void CreateMission_ALAT_DetruireNeutraliserDansZone();
    void CreateMission_ALAT_EffectuerRechercheEtSauvetage();
    void CreateMission_ASA_DefendreZone();
    void CreateMission_ASA_DefendreSite();
    void CreateMission_ASA_Surveiller();
    void CreateMission_ASA_DefendreItineraire();
    void CreateMission_ASA_MISTRAL_DefendreZone();
    void CreateMission_ASA_MISTRAL_DefendreSite();
    void CreateMission_ASA_MISTRAL_Surveiller();
    void CreateMission_ASA_HAWK_DefendreZone();
    void CreateMission_ASA_HAWK_DefendreSite();
    void CreateMission_ASA_HAWK_Surveiller();
    void CreateMission_ASA_DefendreUnites();
    void CreateMission_ASS_SeDeployer();
    void CreateMission_ASS_ReconnaitreZonesDeploiement();
    void CreateMission_ASS_AcquerirObjectifs();
    void CreateMission_GEN_SeDeployer();
    void CreateMission_GEN_FaireFranchir();
    void CreateMission_GEN_RealiserSystemeObstacles();
    void CreateMission_GEN_FranchirDeViveForce();
    void CreateMission_GEN_RetablirItineraires();
    void CreateMission_GEN_AmenagerAireStationnement();
    void CreateMission_GEN_AmenagerTerrainPoserSommaire();
    void CreateMission_GEN_RealiserZonePoser();
    void CreateMission_GEN_ReconnaitreZoneDeploiement();
    void CreateMission_GEN_RealiserCampPrisonniers();
    void CreateMission_GEN_RealiserCampRefugies();
    void CreateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial();
    void CreateMission_GEN_RealiserTravauxSommairesAuProfitPopulation();
    void CreateMission_GEN_AppuyerDebarquementTroupes();
    void CreateMission_GEN_OuvrirAxesProgression();
    void CreateMission_INF_AttaquerSouplesse();
    void CreateMission_INF_Appuyer();
    void CreateMission_INF_Controler();
    void CreateMission_INF_Couvrir();
    void CreateMission_INF_DonnerCoupArret();
    void CreateMission_INF_Eclairer();
    void CreateMission_INF_FlancGarder();
    void CreateMission_INF_Freiner();
    void CreateMission_INF_Harceler();
    void CreateMission_INF_Interdire();
    void CreateMission_INF_MenerDefenseUsure();
    void CreateMission_INF_PrendrePreciserMaintenirContact();
    void CreateMission_INF_ReconnaissanceOffensive();
    void CreateMission_INF_RecueillirUnite();
    void CreateMission_INF_Reduire();
    void CreateMission_INF_Relever();
    void CreateMission_INF_RompreContact();
    void CreateMission_INF_Surveiller();
    void CreateMission_INF_Semparer();
    void CreateMission_INF_Fixer();
    void CreateMission_INF_AttaquerEnTerrainDifficile();
    void CreateMission_INF_BarrerDirection();
    void CreateMission_INF_DefendreFerme();
    void CreateMission_INF_Tenir();
    void CreateMission_INF_AppuyerUnFreinage();
    void CreateMission_INF_AppuyerUneAttaque();
    void CreateMission_INF_Soutenir();
    void CreateMission_LOG_SeDeployer();
    void CreateMission_LOG_AppuyerMouvement();
    void CreateMission_LOG_ReconnaitreItineraire();
    void CreateMission_LOG_Surveiller();
    void CreateMission_LOG_TransporterFormationBlindee();
    void CreateMission_LOG_AppuyerMouvementDansZone();
    void CreateMission_LOG_MettreEnOeuvreZoneStationnement();
    void CreateMission_LOG_AppuyerFranchissementDansZone();
    void CreateMission_LOG_BloquerZone();
    void CreateMission_LOG_ReconnaitreZoneContaminee();
    void CreateMission_LOG_ReconnaitreZoneDeDeploiement();
    void CreateMission_LOG_ArmerPIAs();
    void CreateMission_Test_MoveTo();
    void CreateMission_Decrocher();
    void CreateMission_ReleverSurPlace();
    void CreateMission_FaireMouvement();
    void CreateMission_SeFaireDecontaminer();
    void CreateMission_Franchir();
    void CreateMission_Stationner();
    void CreateMission_SeFaireTransporter();
    void CreateMission_SeRecompleter();
    void CreateMission_SeRendre();
    void CreateMission_FaireMouvementVersCampRefugies();
    void CreateMission_Generique();
    void CreateMission_NBC_ReconnaitreUnAxe();
    void CreateMission_NBC_ReconnaitreUneZone();
    void CreateMission_NBC_ArmerUnSiteDeDecontamination();
    void CreateMission_NBC_DecontaminerUneZone();
    void CreateMission_NBC_ReconnaitreDesSitesROTA();
    void CreateMission_RENS_ROHUM_RenseignerSur();
    void CreateMission_RENS_ROHUM_OrienterGuider();
    void CreateMission_RENS_ROHUM_SExfiltrer();
    void CreateMission_RENS_ROIM_CL289_RenseignerSur();
    void CreateMission_RENS_ROIM_SDTI_RenseignerSur();
    void CreateMission_RENS_ROEM_Appuyer();
    void CreateMission_RENS_ROEM_RenseignerSur();
    void CreateMission_RENS_RADINT_RenseignerSur();
    void CreateMission_TRANS_Commander();

    //@}

    

    //-------------------------------------------------------------------------
    /** @name Network */
    //-------------------------------------------------------------------------
    //@{
    virtual void SendMsg( std::string& sParam );
    virtual void FillRandomParameters() ;
    virtual void ReloadParameters( MT_InputArchive_ABC& archive );
    virtual void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}

private:
    MOS_ASN_MsgAutomateOrder*   pASNMsgOrder_;

    QTabWidget*     pLimaLimitWidget_;
    QVBoxLayout*    pLimaLimitLayout_;
    QTabWidget*     pCommonWidget_;
    QVBoxLayout*    pCommonLayout_;
};


#endif // __MOS_MissionAutomate_h_

