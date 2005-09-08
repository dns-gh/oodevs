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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 10:24 $
// $Revision: 8 $
// $Workfile: AGR_MOS_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_MissionPion.h"

#include "MOS_LimaMission.h"
#include "MOS_LimitMission.h"
#include "MOS_MTH_Itineraire1Pos.h"
#include "MOS_MTH_ItinerairePos.h"
#include "MOS_AgentListView.h"
#include "MOS_AgentMission.h"
#include "MOS_ValueList.h"
#include "MOS_MTH_SegmentPos.h"
#include "MOS_MTH_DirectionPos.h"
#include "MOS_MTH_AgentKnowledgeList.h"
#include "MOS_MTH_ObjectKnowledgeList.h"
#include "MOS_MTH_GDH.h"
#include "MOS_ASN_Messages.h"
#include "MOS_ASN_Types.h"
#include "MOS_PointListViewItem.h"
#include "MOS_MTH_Localisation.h"
#include "MOS_MTH_ListLocalisation_ListView.h"
#include "MOS_MTH_NatureAtlas.h"
#include "MOS_MTH_ListPoint.h"
#include "MOS_MTH_PointList_ListView.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion constructor
// Created:  
//-----------------------------------------------------------------------------
MOS_MissionPion::MOS_MissionPion( E_MissionID nTypeMission, QWidget* pParent )
    : MOS_Mission_ABC( nTypeMission, pParent )
{
    pASNMsgOrder_ = new MOS_ASN_MsgPionOrder();
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPion destructor
// Created:  
//-----------------------------------------------------------------------------
MOS_MissionPion::~MOS_MissionPion()
{
    delete pASNMsgOrder_;
}

//=============================================================================
// INITIALIZATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::Initialize
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::Initialize()
{
    MOS_Mission_ABC::Initialize();
   

    //--------------------
    // Lima / limit
    pLimaLimitWidget_ = new QTabWidget( this );
    pTabWidget_->addTab( pLimaLimitWidget_, "Limas / Limites" );
    pLimaLimitLayout_ = new QVBoxLayout( pLimaLimitWidget_ );

    CreateLimitMission( pASNMsgOrder_->GetAsnMsg().oid_limite_gauche, pASNMsgOrder_->GetAsnMsg().oid_limite_droite, pLimaLimitWidget_, pLimaLimitLayout_, "Limits" );
    CreateLimaMission ( pASNMsgOrder_->GetAsnMsg().oid_limas, pLimaLimitWidget_, pLimaLimitLayout_, "Limas" ); 

    //--------------------
    // Common
    pCommonWidget_ = new QTabWidget( this );
    pTabWidget_->addTab( pCommonWidget_, "Common" );
    pCommonLayout_ = new QVBoxLayout( pCommonWidget_  );


    // Direction dangereuse
    CreateDirection( pASNMsgOrder_->GetAsnMsg().direction_dangereuse, pCommonWidget_, pCommonLayout_, "Danger", true );

    CreateMissionTools();

    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::Terminate()
{                                
    MOS_Mission_ABC::Terminate();
    
    switch( pASNMsgOrder_->GetAsnMsg().mission.t )
    {
        case T_Mission_Pion_mission_pion_alat_evacuation_sanitaire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_evacuation_sanitaire; break;
        case T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_reconnaitre_dans_la_profondeur; break;
        case T_Mission_Pion_mission_pion_alat_reconnaitre_contour_ennemi : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_reconnaitre_contour_ennemi; break;
        case T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_detruire_neutraliser_dans_zone; break;
        case T_Mission_Pion_mission_pion_alat_detruire_aeromobile : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_detruire_aeromobile; break;
        case T_Mission_Pion_mission_pion_alat_freiner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_freiner; break;
        case T_Mission_Pion_mission_pion_alat_jalonner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_jalonner; break;
        case T_Mission_Pion_mission_pion_alat_escorter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_escorter; break;
        case T_Mission_Pion_mission_pion_alat_heliporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_heliporter; break;
        case T_Mission_Pion_mission_pion_alat_heliporter_helitransporter_plot_ravitaillement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_heliporter_helitransporter_plot_ravitaillement; break;
        case T_Mission_Pion_mission_pion_alat_helitransporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_helitransporter; break;
        case T_Mission_Pion_mission_pion_alat_effectuer_recherche_et_sauvetage : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_effectuer_recherche_et_sauvetage; break;
        case T_Mission_Pion_mission_pion_alat_imex : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_imex; break;
        case T_Mission_Pion_mission_pion_alat_eclairer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_eclairer; break;
        case T_Mission_Pion_mission_pion_alat_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_surveiller; break;
        case T_Mission_Pion_mission_pion_alat_appuyer_direct_au_contact : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_appuyer_direct_au_contact; break;
        case T_Mission_Pion_mission_pion_alat_couvrir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_couvrir; break;
        case T_Mission_Pion_mission_pion_alat_attaquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_attaquer; break;
        case T_Mission_Pion_mission_pion_asa_defendre_site : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_site; break;
        case T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_site_a_partir_position; break;
        case T_Mission_Pion_mission_pion_asa_defendre_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_zone; break;
        case T_Mission_Pion_mission_pion_asa_defendre_zone_a_partir_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_zone_a_partir_position; break;
        case T_Mission_Pion_mission_pion_asa_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_surveiller; break;
        case T_Mission_Pion_mission_pion_asa_defendre_unites : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_unites; break;
        case T_Mission_Pion_mission_pion_asa_mistral_defendre_site : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_site; break;
        case T_Mission_Pion_mission_pion_asa_mistral_defendre_site_a_partir_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_site_a_partir_position; break;
        case T_Mission_Pion_mission_pion_asa_mistral_defendre_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_zone; break;
        case T_Mission_Pion_mission_pion_asa_mistral_defendre_zone_a_partir_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_zone_a_partir_position; break;
        case T_Mission_Pion_mission_pion_asa_mistral_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_surveiller; break;
        case T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_assurer_mise_en_oeuvre; break;
        case T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre_sur_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_assurer_mise_en_oeuvre_sur_position; break;
        case T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_reconnaitre_zone_deploiement; break;
        case T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_acquerir_lanceurs_eni; break;
        case T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_eo_acquerir_objectifs; break;
        case T_Mission_Pion_mission_pion_ass_eop_acquerir_objectifs : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_eop_acquerir_objectifs; break;
        case T_Mission_Pion_mission_pion_ass_ratac_acquerir_objectifs : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_ratac_acquerir_objectifs; break;
        case T_Mission_Pion_mission_pion_abc_reconnaitre_axe : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_axe; break;
        case T_Mission_Pion_mission_pion_abc_soutenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_soutenir; break;
        case T_Mission_Pion_mission_pion_abc_couvrir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_couvrir; break;
        case T_Mission_Pion_mission_pion_abc_flanc_garder : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_flanc_garder; break;
        case T_Mission_Pion_mission_pion_abc_reduire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reduire; break;
        case T_Mission_Pion_mission_pion_abc_appuyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_appuyer; break;
        case T_Mission_Pion_mission_pion_abc_fixer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_fixer; break;
        case T_Mission_Pion_mission_pion_abc_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_renseigner_sur; break;
        case T_Mission_Pion_mission_pion_abc_controler_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_controler_zone; break;
        case T_Mission_Pion_mission_pion_abc_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_surveiller; break;
        case T_Mission_Pion_mission_pion_abc_barrer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_barrer; break;
        case T_Mission_Pion_mission_pion_abc_attaquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_attaquer; break;
        case T_Mission_Pion_mission_pion_abc_reconnaitre_point : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_point; break;
        case T_Mission_Pion_mission_pion_abc_eclairer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_eclairer; break;
        case T_Mission_Pion_mission_pion_abc_jalonner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_jalonner; break;
        case T_Mission_Pion_mission_pion_abc_contre_attaquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_contre_attaquer; break;
        case T_Mission_Pion_mission_pion_abc_semparer_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_semparer_zone; break;
        case T_Mission_Pion_mission_pion_abc_relever_unite : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_relever_unite; break;
        case T_Mission_Pion_mission_pion_abc_armer_pia : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_armer_pia; break;
        case T_Mission_Pion_mission_pion_abc_ouvrir_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_ouvrir_itineraire; break;
        case T_Mission_Pion_mission_pion_abc_freiner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_freiner; break;
        case T_Mission_Pion_mission_pion_abc_escorter_un_convoi : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_escorter_un_convoi; break;
        case T_Mission_Pion_mission_pion_abc_armer_point_de_controle : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_armer_point_de_controle; break;
        case T_Mission_Pion_mission_pion_abc_organiser_accueil_colonne_refugies : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_organiser_accueil_colonne_refugies; break;
        case T_Mission_Pion_mission_pion_gen_realiser_fosse_ac : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_fosse_ac; break;
        case T_Mission_Pion_mission_pion_gen_realiser_abattis_mine : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_abattis_mine; break;
        case T_Mission_Pion_mission_pion_gen_realiser_point_mine : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_point_mine; break;
        case T_Mission_Pion_mission_pion_gen_realiser_barricade : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_barricade; break;
        case T_Mission_Pion_mission_pion_gen_realiser_eboulement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_eboulement; break;
        case T_Mission_Pion_mission_pion_gen_realiser_bouchon_mine : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_bouchon_mine; break;
        case T_Mission_Pion_mission_pion_gen_detruire_route : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_detruire_route; break;
        case T_Mission_Pion_mission_pion_gen_detruire_pont : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_detruire_pont; break;
        case T_Mission_Pion_mission_pion_gen_securiser_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_securiser_itineraire; break;
        case T_Mission_Pion_mission_pion_gen_degager_abattis_mine : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_abattis_mine; break;
        case T_Mission_Pion_mission_pion_gen_degager_eboulement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_eboulement; break;
        case T_Mission_Pion_mission_pion_gen_degager_rues : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_rues; break;
        case T_Mission_Pion_mission_pion_gen_combler_entonnoir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_combler_entonnoir; break;
        case T_Mission_Pion_mission_pion_gen_combler_fosse_ac : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_combler_fosse_ac; break;
        case T_Mission_Pion_mission_pion_gen_creer_contournement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_contournement; break;
        case T_Mission_Pion_mission_pion_gen_realiser_zone_minee_lineaire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_lineaire; break;
        case T_Mission_Pion_mission_pion_gen_ouvrir_trouee : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_ouvrir_trouee; break;
        case T_Mission_Pion_mission_pion_gen_renforcer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_renforcer; break;
        case T_Mission_Pion_mission_pion_gen_franchir_entonnoir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_franchir_entonnoir; break;
        case T_Mission_Pion_mission_pion_gen_franchir_fosse_ac : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_franchir_fosse_ac; break;
        case T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_par_dispersion; break;
        case T_Mission_Pion_mission_pion_gen_realiser_postes_tir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_postes_tir; break;
        case T_Mission_Pion_mission_pion_gen_realiser_travaux_protection : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_protection; break;
        case T_Mission_Pion_mission_pion_gen_retablir_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_retablir_itineraire; break;
        case T_Mission_Pion_mission_pion_gen_equiper_exploiter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_equiper_exploiter; break;
        case T_Mission_Pion_mission_pion_gen_executer_variantement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_executer_variantement; break;
        case T_Mission_Pion_mission_pion_gen_reconnaitre_site_franchissement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_site_franchissement; break;
        case T_Mission_Pion_mission_pion_gen_reconnaitre_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_itineraire; break;
        case T_Mission_Pion_mission_pion_gen_deminer_site_franchissement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_deminer_site_franchissement; break;
        case T_Mission_Pion_mission_pion_gen_reconnaitre_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_zone; break;
        case T_Mission_Pion_mission_pion_gen_creer_plate_forme : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_plate_forme; break;
        case T_Mission_Pion_mission_pion_gen_creer_piste : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_piste; break;
        case T_Mission_Pion_mission_pion_gen_creer_aire_poser : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_aire_poser; break;
        case T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_neutraliser_engins_explosifs; break;
        case T_Mission_Pion_mission_pion_gen_activer_obstacle : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_activer_obstacle; break;
        case T_Mission_Pion_mission_pion_gen_ameliorer_mobilite : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_ameliorer_mobilite; break;
        case T_Mission_Pion_mission_pion_gen_realiser_aire_logistique : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_aire_logistique; break;
        case T_Mission_Pion_mission_pion_gen_depolluer_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_depolluer_zone; break;
        case T_Mission_Pion_mission_pion_gen_amenager_terrain_poser : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_amenager_terrain_poser; break;
        case T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_plate_forme_deploiement; break;
        case T_Mission_Pion_mission_pion_gen_reprendre_mission_construction : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reprendre_mission_construction; break;
        case T_Mission_Pion_mission_pion_gen_realiser_zone_poser_helicoptere : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_poser_helicoptere; break;
        case T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_appuyer_debarquement_troupes_gu; break;
        case T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_prisonniers; break;
        case T_Mission_Pion_mission_pion_gen_realiser_camp_refugies : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_refugies; break;
        case T_Mission_Pion_mission_pion_inf_reconnaitre_axe : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reconnaitre_axe; break;
        case T_Mission_Pion_mission_pion_inf_couvrir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_couvrir; break;
        case T_Mission_Pion_mission_pion_inf_reduire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reduire; break;
        case T_Mission_Pion_mission_pion_inf_appuyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_appuyer; break;
        case T_Mission_Pion_mission_pion_inf_fixer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_fixer; break;
        case T_Mission_Pion_mission_pion_inf_controler_secteur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_controler_secteur; break;
        case T_Mission_Pion_mission_pion_inf_surveiller_secteur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_surveiller_secteur; break;
        case T_Mission_Pion_mission_pion_inf_barrer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_barrer; break;
        case T_Mission_Pion_mission_pion_inf_reconnaitre_point : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reconnaitre_point; break;
        case T_Mission_Pion_mission_pion_inf_eclairer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_eclairer; break;
        case T_Mission_Pion_mission_pion_inf_interdire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_interdire; break;
        case T_Mission_Pion_mission_pion_inf_semparer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_semparer; break;
        case T_Mission_Pion_mission_pion_inf_prendre_preciser_contact : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_prendre_preciser_contact; break;
        case T_Mission_Pion_mission_pion_inf_defendre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_defendre; break;
        case T_Mission_Pion_mission_pion_inf_detruire_embuscade : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_detruire_embuscade; break;
        case T_Mission_Pion_mission_pion_inf_contre_attaquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_contre_attaquer; break;
        case T_Mission_Pion_mission_pion_inf_freiner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_freiner; break;
        case T_Mission_Pion_mission_pion_inf_flanc_garder : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_flanc_garder; break;
        case T_Mission_Pion_mission_pion_inf_organiser_un_check_point : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_organiser_un_check_point; break;
        case T_Mission_Pion_mission_pion_inf_harceler : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_harceler; break;
        case T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_detruire_par_coup_de_main; break;
        case T_Mission_Pion_mission_pion_inf_escorter_un_convoi : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_escorter_un_convoi; break;
        case T_Mission_Pion_mission_pion_inf_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_renseigner_sur; break;
        case T_Mission_Pion_mission_pion_inf_soutenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_soutenir; break;
        case T_Mission_Pion_mission_pion_inf_securiser : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_securiser; break;
        case T_Mission_Pion_mission_pion_inf_recueillir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_recueillir; break;
        case T_Mission_Pion_mission_pion_inf_repondre_a_aggression : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_repondre_a_aggression; break;
        case T_Mission_Pion_mission_pion_log_se_deployer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_se_deployer; break;
        case T_Mission_Pion_mission_pion_log_distribuer_moyens : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_distribuer_moyens; break;
        case T_Mission_Pion_mission_pion_log_trier_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_trier_blesses; break;
        case T_Mission_Pion_mission_pion_log_traiter_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_traiter_blesses; break;
        case T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_trier_et_traiter_blesses; break;
        case T_Mission_Pion_mission_pion_log_soigner_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_soigner_blesses; break;
        case T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_prendre_en_compte_reactions_mentales; break;
        case T_Mission_Pion_mission_pion_log_decontaminer_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_decontaminer_blesses; break;
        case T_Mission_Pion_mission_pion_log_reparer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reparer; break;
        case T_Mission_Pion_mission_pion_log_evacuer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_evacuer; break;
        case T_Mission_Pion_mission_pion_log_convoyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_convoyer; break;
        case T_Mission_Pion_mission_pion_log_distribuer_ressources : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_distribuer_ressources; break;
        case T_Mission_Pion_mission_pion_log_constituer_convoi : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_constituer_convoi; break;
        case T_Mission_Pion_mission_pion_log_livrer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_livrer; break;
        case T_Mission_Pion_mission_pion_log_preparer_bascule : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_preparer_bascule; break;
        case T_Mission_Pion_mission_pion_log_appuyer_mouvement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement; break;
        case T_Mission_Pion_mission_pion_log_reconnaitre_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_itineraire; break;
        case T_Mission_Pion_mission_pion_log_transporter_unite_chars : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_transporter_unite_chars; break;
        case T_Mission_Pion_mission_pion_log_reconnaitre_zone_contaminee : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_zone_contaminee; break;
        case T_Mission_Pion_mission_pion_log_appuyer_mouvement_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement_dans_zone; break;
        case T_Mission_Pion_mission_pion_log_mettre_en_oeuvre_zone_stationnement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_mettre_en_oeuvre_zone_stationnement; break;
        case T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_franchissement_sur_un_point; break;
        case T_Mission_Pion_mission_pion_log_bloquer_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_bloquer_zone; break;
        case T_Mission_Pion_mission_pion_log_reconnaitre_zone_de_deploiement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_zone_de_deploiement; break;
        case T_Mission_Pion_mission_pion_log_armer_pia : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_armer_pia; break;
        case T_Mission_Pion_mission_pion_log_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_surveiller; break;
        case T_Mission_Pion_mission_pion_test_heliporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_heliporter; break;
        case T_Mission_Pion_mission_pion_test_move_to : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_move_to; break;
        case T_Mission_Pion_mission_pion_test_create_object : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_create_object; break;
        case T_Mission_Pion_mission_pion_test_destroy_object : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_destroy_object; break;
        case T_Mission_Pion_mission_pion_test_create_bypass : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_create_bypass; break;
        case T_Mission_Pion_mission_pion_test_fire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_fire; break;
        case T_Mission_Pion_mission_pion_test_change_posture : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_change_posture; break;
        case T_Mission_Pion_mission_pion_test_path_find : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_path_find; break;
        case T_Mission_Pion_mission_pion_test_find_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_find_position; break;
        case T_Mission_Pion_mission_pion_test_reinforce : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_reinforce; break;
        case T_Mission_Pion_mission_pion_plastron_ennemi : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_plastron_ennemi; break;
        case T_Mission_Pion_mission_pion_faire_mouvement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_faire_mouvement; break;
        case T_Mission_Pion_mission_pion_suivre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_suivre; break;
        case T_Mission_Pion_mission_pion_se_faire_decontaminer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_se_faire_decontaminer; break;
        case T_Mission_Pion_mission_pion_franchir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_franchir; break;
        case T_Mission_Pion_mission_pion_decrocher : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_decrocher; break;
        case T_Mission_Pion_mission_pion_stationner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_stationner; break;
        case T_Mission_Pion_mission_pion_se_faire_transporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_se_faire_transporter; break;
        case T_Mission_Pion_mission_pion_generique : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_generique; break;
        case T_Mission_Pion_mission_pion_milice_provoquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_provoquer; break;
        case T_Mission_Pion_mission_pion_milice_prendre_et_tenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_prendre_et_tenir; break;
        case T_Mission_Pion_mission_pion_milice_detruire_embuscade : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_detruire_embuscade; break;
        case T_Mission_Pion_mission_pion_joint_marine_transporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_joint_marine_transporter; break;
        case T_Mission_Pion_mission_pion_joint_air_effectuer_raid : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_joint_air_effectuer_raid; break;
        case T_Mission_Pion_mission_pion_nbc_animer_un_plot_de_decontamination : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_animer_un_plot_de_decontamination; break;
        case T_Mission_Pion_mission_pion_nbc_reconnaitre_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_itineraire; break;
        case T_Mission_Pion_mission_pion_nbc_reconnaitre_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_zone; break;
        case T_Mission_Pion_mission_pion_nbc_decontaminer_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_decontaminer_zone; break;
        case T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_un_site_rota; break;
        case T_Mission_Pion_mission_pion_rens_rohum_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_renseigner_sur; break;
        case T_Mission_Pion_mission_pion_rens_rohum_sexfiltrer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_sexfiltrer; break;
        case T_Mission_Pion_mission_pion_rens_rohum_orienter_guider : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_orienter_guider; break;
        case T_Mission_Pion_mission_pion_rens_roem_appuyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_appuyer; break;
        case T_Mission_Pion_mission_pion_rens_roem_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_renseigner_sur; break;
        case T_Mission_Pion_mission_pion_rens_roem_mettre_en_oeuvre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_mettre_en_oeuvre; break;
        case T_Mission_Pion_mission_pion_rens_radint_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_radint_renseigner_sur; break;
        case T_Mission_Pion_mission_pion_rens_radint_mettre_en_oeuvre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_radint_mettre_en_oeuvre; break;
        case T_Mission_Pion_mission_pion_rens_roim_mettre_en_oeuvre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_mettre_en_oeuvre; break;
        case T_Mission_Pion_mission_pion_rens_roim_sdti_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_sdti_renseigner_sur; break;
        case T_Mission_Pion_mission_pion_rens_roim_cl289_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_cl289_renseigner_sur; break;
        case T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_trans_etablir_gerer_liaison; break;

        default:
            assert( false );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::SetAgent
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::SetAgent( MOS_Agent* pAgent )
{
    MOS_Mission_ABC::SetAgent( pAgent );
    if( pAgent )
    {
        pASNMsgOrder_->GetAsnMsg().order_id = (uint)GetAgent();
        pASNMsgOrder_->GetAsnMsg().oid_unite_executante = pAgent->GetAgentID();

    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::CreateMissionTools
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::CreateMissionTools()
{
    switch( nTypeMission_ )
    {

        case eMission_Pion_ALAT_EvacuationSanitaire : CreateMission_ALAT_EvacuationSanitaire(); break;
        case eMission_Pion_ALAT_ReconnaitreDansLaProfondeur : CreateMission_ALAT_ReconnaitreDansLaProfondeur(); break;
        case eMission_Pion_ALAT_ReconnaitreContourEnnemi : CreateMission_ALAT_ReconnaitreContourEnnemi(); break;
        case eMission_Pion_ALAT_DetruireNeutraliserDansZone : CreateMission_ALAT_DetruireNeutraliserDansZone(); break;
        case eMission_Pion_ALAT_DetruireAeromobile : CreateMission_ALAT_DetruireAeromobile(); break;
        case eMission_Pion_ALAT_Freiner : CreateMission_ALAT_Freiner(); break;
        case eMission_Pion_ALAT_Jalonner : CreateMission_ALAT_Jalonner(); break;
        case eMission_Pion_ALAT_Escorter : CreateMission_ALAT_Escorter(); break;
        case eMission_Pion_ALAT_Heliporter : CreateMission_ALAT_Heliporter(); break;
        case eMission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement : CreateMission_ALAT_HeliporterHelitransporterPlotRavitaillement(); break;
        case eMission_Pion_ALAT_Helitransporter : CreateMission_ALAT_Helitransporter(); break;
        case eMission_Pion_ALAT_EffectuerRechercheEtSauvetage : CreateMission_ALAT_EffectuerRechercheEtSauvetage(); break;
        case eMission_Pion_ALAT_IMEX : CreateMission_ALAT_IMEX(); break;
        case eMission_Pion_ALAT_Eclairer : CreateMission_ALAT_Eclairer(); break;
        case eMission_Pion_ALAT_Surveiller : CreateMission_ALAT_Surveiller(); break;
        case eMission_Pion_ALAT_AppuyerDirectAuContact : CreateMission_ALAT_AppuyerDirectAuContact(); break;
        case eMission_Pion_ALAT_Couvrir : CreateMission_ALAT_Couvrir(); break;
        case eMission_Pion_ALAT_Attaquer : CreateMission_ALAT_Attaquer(); break;
        case eMission_Pion_ASA_DefendreSite : CreateMission_ASA_DefendreSite(); break;
        case eMission_Pion_ASA_DefendreSiteAPartirPosition : CreateMission_ASA_DefendreSiteAPartirPosition(); break;
        case eMission_Pion_ASA_DefendreZone : CreateMission_ASA_DefendreZone(); break;
        case eMission_Pion_ASA_DefendreZoneAPartirPosition : CreateMission_ASA_DefendreZoneAPartirPosition(); break;
        case eMission_Pion_ASA_Surveiller : CreateMission_ASA_Surveiller(); break;
        case eMission_Pion_ASA_DefendreUnites : CreateMission_ASA_DefendreUnites(); break;
        case eMission_Pion_ASA_MISTRAL_DefendreSite : CreateMission_ASA_MISTRAL_DefendreSite(); break;
        case eMission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition : CreateMission_ASA_MISTRAL_DefendreSiteAPartirPosition(); break;
        case eMission_Pion_ASA_MISTRAL_DefendreZone : CreateMission_ASA_MISTRAL_DefendreZone(); break;
        case eMission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition : CreateMission_ASA_MISTRAL_DefendreZoneAPartirPosition(); break;
        case eMission_Pion_ASA_MISTRAL_Surveiller : CreateMission_ASA_MISTRAL_Surveiller(); break;
        case eMission_Pion_ASS_AssurerMiseEnOeuvre : CreateMission_ASS_AssurerMiseEnOeuvre(); break;
        case eMission_Pion_ASS_AssurerMiseEnOeuvreSurPosition : CreateMission_ASS_AssurerMiseEnOeuvreSurPosition(); break;
        case eMission_Pion_ASS_ReconnaitreZoneDeploiement : CreateMission_ASS_ReconnaitreZoneDeploiement(); break;
        case eMission_Pion_ASS_AcquerirLanceursEni : CreateMission_ASS_AcquerirLanceursEni(); break;
        case eMission_Pion_ASS_EO_AcquerirObjectifs : CreateMission_ASS_EO_AcquerirObjectifs(); break;
        case eMission_Pion_ASS_EOP_AcquerirObjectifs : CreateMission_ASS_EOP_AcquerirObjectifs(); break;
        case eMission_Pion_ASS_RATAC_AcquerirObjectifs : CreateMission_ASS_RATAC_AcquerirObjectifs(); break;
        case eMission_Pion_ABC_ReconnaitreAxe : CreateMission_ABC_ReconnaitreAxe(); break;
        case eMission_Pion_ABC_Soutenir : CreateMission_ABC_Soutenir(); break;
        case eMission_Pion_ABC_Couvrir : CreateMission_ABC_Couvrir(); break;
        case eMission_Pion_ABC_FlancGarder : CreateMission_ABC_FlancGarder(); break;
        case eMission_Pion_ABC_Reduire : CreateMission_ABC_Reduire(); break;
        case eMission_Pion_ABC_Appuyer : CreateMission_ABC_Appuyer(); break;
        case eMission_Pion_ABC_Fixer : CreateMission_ABC_Fixer(); break;
        case eMission_Pion_ABC_RenseignerSur : CreateMission_ABC_RenseignerSur(); break;
        case eMission_Pion_ABC_ControlerZone : CreateMission_ABC_ControlerZone(); break;
        case eMission_Pion_ABC_Surveiller : CreateMission_ABC_Surveiller(); break;
        case eMission_Pion_ABC_Barrer : CreateMission_ABC_Barrer(); break;
        case eMission_Pion_ABC_Attaquer : CreateMission_ABC_Attaquer(); break;
        case eMission_Pion_ABC_ReconnaitrePoint : CreateMission_ABC_ReconnaitrePoint(); break;
        case eMission_Pion_ABC_Eclairer : CreateMission_ABC_Eclairer(); break;
        case eMission_Pion_ABC_Jalonner : CreateMission_ABC_Jalonner(); break;
        case eMission_Pion_ABC_ContreAttaquer : CreateMission_ABC_ContreAttaquer(); break;
        case eMission_Pion_ABC_SemparerZone : CreateMission_ABC_SemparerZone(); break;
        case eMission_Pion_ABC_ReleverUnite : CreateMission_ABC_ReleverUnite(); break;
        case eMission_Pion_ABC_ArmerPIA : CreateMission_ABC_ArmerPIA(); break;
        case eMission_Pion_ABC_OuvrirItineraire : CreateMission_ABC_OuvrirItineraire(); break;
        case eMission_Pion_ABC_Freiner : CreateMission_ABC_Freiner(); break;
        case eMission_Pion_ABC_EscorterUnConvoi : CreateMission_ABC_EscorterUnConvoi(); break;
        case eMission_Pion_ABC_ArmerPointDeControle : CreateMission_ABC_ArmerPointDeControle(); break;
        case eMission_Pion_ABC_OrganiserAccueilColonneRefugies : CreateMission_ABC_OrganiserAccueilColonneRefugies(); break;
        case eMission_Pion_GEN_RealiserFosseAC : CreateMission_GEN_RealiserFosseAC(); break;
        case eMission_Pion_GEN_RealiserAbattisMine : CreateMission_GEN_RealiserAbattisMine(); break;
        case eMission_Pion_GEN_RealiserPointMine : CreateMission_GEN_RealiserPointMine(); break;
        case eMission_Pion_GEN_RealiserBarricade : CreateMission_GEN_RealiserBarricade(); break;
        case eMission_Pion_GEN_RealiserEboulement : CreateMission_GEN_RealiserEboulement(); break;
        case eMission_Pion_GEN_RealiserBouchonMine : CreateMission_GEN_RealiserBouchonMine(); break;
        case eMission_Pion_GEN_DetruireRoute : CreateMission_GEN_DetruireRoute(); break;
        case eMission_Pion_GEN_DetruirePont : CreateMission_GEN_DetruirePont(); break;
        case eMission_Pion_GEN_SecuriserItineraire : CreateMission_GEN_SecuriserItineraire(); break;
        case eMission_Pion_GEN_DegagerAbattisMine : CreateMission_GEN_DegagerAbattisMine(); break;
        case eMission_Pion_GEN_DegagerEboulement : CreateMission_GEN_DegagerEboulement(); break;
        case eMission_Pion_GEN_DegagerRues : CreateMission_GEN_DegagerRues(); break;
        case eMission_Pion_GEN_ComblerEntonnoir : CreateMission_GEN_ComblerEntonnoir(); break;
        case eMission_Pion_GEN_ComblerFosseAC : CreateMission_GEN_ComblerFosseAC(); break;
        case eMission_Pion_GEN_CreerContournement : CreateMission_GEN_CreerContournement(); break;
        case eMission_Pion_GEN_RealiserZoneMineeLineaire : CreateMission_GEN_RealiserZoneMineeLineaire(); break;
        case eMission_Pion_GEN_OuvrirTrouee : CreateMission_GEN_OuvrirTrouee(); break;
        case eMission_Pion_GEN_Renforcer : CreateMission_GEN_Renforcer(); break;
        case eMission_Pion_GEN_FranchirEntonnoir : CreateMission_GEN_FranchirEntonnoir(); break;
        case eMission_Pion_GEN_FranchirFosseAC : CreateMission_GEN_FranchirFosseAC(); break;
        case eMission_Pion_GEN_RealiserZoneMineeParDispersion : CreateMission_GEN_RealiserZoneMineeParDispersion(); break;
        case eMission_Pion_GEN_RealiserPostesTir : CreateMission_GEN_RealiserPostesTir(); break;
        case eMission_Pion_GEN_RealiserTravauxProtection : CreateMission_GEN_RealiserTravauxProtection(); break;
        case eMission_Pion_GEN_RetablirItineraire : CreateMission_GEN_RetablirItineraire(); break;
        case eMission_Pion_GEN_EquiperExploiter : CreateMission_GEN_EquiperExploiter(); break;
        case eMission_Pion_GEN_ExecuterVariantement : CreateMission_GEN_ExecuterVariantement(); break;
        case eMission_Pion_GEN_ReconnaitreSiteFranchissement : CreateMission_GEN_ReconnaitreSiteFranchissement(); break;
        case eMission_Pion_GEN_ReconnaitreItineraire : CreateMission_GEN_ReconnaitreItineraire(); break;
        case eMission_Pion_GEN_DeminerSiteFranchissement : CreateMission_GEN_DeminerSiteFranchissement(); break;
        case eMission_Pion_GEN_ReconnaitreZone : CreateMission_GEN_ReconnaitreZone(); break;
        case eMission_Pion_GEN_CreerPlateForme : CreateMission_GEN_CreerPlateForme(); break;
        case eMission_Pion_GEN_CreerPiste : CreateMission_GEN_CreerPiste(); break;
        case eMission_Pion_GEN_CreerAirePoser : CreateMission_GEN_CreerAirePoser(); break;
        case eMission_Pion_GEN_NeutraliserEnginsExplosifs : CreateMission_GEN_NeutraliserEnginsExplosifs(); break;
        case eMission_Pion_GEN_ActiverObstacle : CreateMission_GEN_ActiverObstacle(); break;
        case eMission_Pion_GEN_AmeliorerMobilite : CreateMission_GEN_AmeliorerMobilite(); break;
        case eMission_Pion_GEN_RealiserAireLogistique : CreateMission_GEN_RealiserAireLogistique(); break;
        case eMission_Pion_GEN_DepolluerZone : CreateMission_GEN_DepolluerZone(); break;
        case eMission_Pion_GEN_AmenagerTerrainPoser : CreateMission_GEN_AmenagerTerrainPoser(); break;
        case eMission_Pion_GEN_RealiserPlateFormeDeploiement : CreateMission_GEN_RealiserPlateFormeDeploiement(); break;
        case eMission_Pion_GEN_ReprendreMissionConstruction : CreateMission_GEN_ReprendreMissionConstruction(); break;
        case eMission_Pion_GEN_RealiserZonePoserHelicoptere : CreateMission_GEN_RealiserZonePoserHelicoptere(); break;
        case eMission_Pion_GEN_AppuyerDebarquementTroupesGU : CreateMission_GEN_AppuyerDebarquementTroupesGU(); break;
        case eMission_Pion_GEN_RealiserCampPrisonniers : CreateMission_GEN_RealiserCampPrisonniers(); break;
        case eMission_Pion_GEN_RealiserCampRefugies : CreateMission_GEN_RealiserCampRefugies(); break;
        case eMission_Pion_INF_ReconnaitreAxe : CreateMission_INF_ReconnaitreAxe(); break;
        case eMission_Pion_INF_Couvrir : CreateMission_INF_Couvrir(); break;
        case eMission_Pion_INF_Reduire : CreateMission_INF_Reduire(); break;
        case eMission_Pion_INF_Appuyer : CreateMission_INF_Appuyer(); break;
        case eMission_Pion_INF_Fixer : CreateMission_INF_Fixer(); break;
        case eMission_Pion_INF_ControlerSecteur : CreateMission_INF_ControlerSecteur(); break;
        case eMission_Pion_INF_SurveillerSecteur : CreateMission_INF_SurveillerSecteur(); break;
        case eMission_Pion_INF_Barrer : CreateMission_INF_Barrer(); break;
        case eMission_Pion_INF_ReconnaitrePoint : CreateMission_INF_ReconnaitrePoint(); break;
        case eMission_Pion_INF_Eclairer : CreateMission_INF_Eclairer(); break;
        case eMission_Pion_INF_Interdire : CreateMission_INF_Interdire(); break;
        case eMission_Pion_INF_Semparer : CreateMission_INF_Semparer(); break;
        case eMission_Pion_INF_PrendrePreciserContact : CreateMission_INF_PrendrePreciserContact(); break;
        case eMission_Pion_INF_Defendre : CreateMission_INF_Defendre(); break;
        case eMission_Pion_INF_DetruireEmbuscade : CreateMission_INF_DetruireEmbuscade(); break;
        case eMission_Pion_INF_ContreAttaquer : CreateMission_INF_ContreAttaquer(); break;
        case eMission_Pion_INF_Freiner : CreateMission_INF_Freiner(); break;
        case eMission_Pion_INF_FlancGarder : CreateMission_INF_FlancGarder(); break;
        case eMission_Pion_INF_OrganiserUnCheckPoint : CreateMission_INF_OrganiserUnCheckPoint(); break;
        case eMission_Pion_INF_Harceler : CreateMission_INF_Harceler(); break;
        case eMission_Pion_INF_DetruireParCoupDeMain : CreateMission_INF_DetruireParCoupDeMain(); break;
        case eMission_Pion_INF_EscorterUnConvoi : CreateMission_INF_EscorterUnConvoi(); break;
        case eMission_Pion_INF_RenseignerSur : CreateMission_INF_RenseignerSur(); break;
        case eMission_Pion_INF_Soutenir : CreateMission_INF_Soutenir(); break;
        case eMission_Pion_INF_Securiser : CreateMission_INF_Securiser(); break;
        case eMission_Pion_INF_Recueillir : CreateMission_INF_Recueillir(); break;
        case eMission_Pion_INF_RepondreAAggression : CreateMission_INF_RepondreAAggression(); break;
        case eMission_Pion_LOG_SeDeployer : CreateMission_LOG_SeDeployer(); break;
        case eMission_Pion_LOG_DistribuerMoyens : CreateMission_LOG_DistribuerMoyens(); break;
        case eMission_Pion_LOG_TrierBlesses : CreateMission_LOG_TrierBlesses(); break;
        case eMission_Pion_LOG_TraiterBlesses : CreateMission_LOG_TraiterBlesses(); break;
        case eMission_Pion_LOG_TrierEtTraiterBlesses : CreateMission_LOG_TrierEtTraiterBlesses(); break;
        case eMission_Pion_LOG_SoignerBlesses : CreateMission_LOG_SoignerBlesses(); break;
        case eMission_Pion_LOG_PrendreEnCompteReactionsMentales : CreateMission_LOG_PrendreEnCompteReactionsMentales(); break;
        case eMission_Pion_LOG_DecontaminerBlesses : CreateMission_LOG_DecontaminerBlesses(); break;
        case eMission_Pion_LOG_Reparer : CreateMission_LOG_Reparer(); break;
        case eMission_Pion_LOG_Evacuer : CreateMission_LOG_Evacuer(); break;
        case eMission_Pion_LOG_Convoyer : CreateMission_LOG_Convoyer(); break;
        case eMission_Pion_LOG_DistribuerRessources : CreateMission_LOG_DistribuerRessources(); break;
        case eMission_Pion_LOG_ConstituerConvoi : CreateMission_LOG_ConstituerConvoi(); break;
        case eMission_Pion_LOG_Livrer : CreateMission_LOG_Livrer(); break;
        case eMission_Pion_LOG_PreparerBascule : CreateMission_LOG_PreparerBascule(); break;
        case eMission_Pion_LOG_AppuyerMouvement : CreateMission_LOG_AppuyerMouvement(); break;
        case eMission_Pion_LOG_ReconnaitreItineraire : CreateMission_LOG_ReconnaitreItineraire(); break;
        case eMission_Pion_LOG_TransporterUniteChars : CreateMission_LOG_TransporterUniteChars(); break;
        case eMission_Pion_LOG_ReconnaitreZoneContaminee : CreateMission_LOG_ReconnaitreZoneContaminee(); break;
        case eMission_Pion_LOG_AppuyerMouvementDansZone : CreateMission_LOG_AppuyerMouvementDansZone(); break;
        case eMission_Pion_LOG_MettreEnOeuvreZoneStationnement : CreateMission_LOG_MettreEnOeuvreZoneStationnement(); break;
        case eMission_Pion_LOG_AppuyerFranchissementSurUnPoint : CreateMission_LOG_AppuyerFranchissementSurUnPoint(); break;
        case eMission_Pion_LOG_BloquerZone : CreateMission_LOG_BloquerZone(); break;
        case eMission_Pion_LOG_ReconnaitreZoneDeDeploiement : CreateMission_LOG_ReconnaitreZoneDeDeploiement(); break;
        case eMission_Pion_LOG_ArmerPIA : CreateMission_LOG_ArmerPIA(); break;
        case eMission_Pion_LOG_Surveiller : CreateMission_LOG_Surveiller(); break;
        case eMission_Pion_Test_Heliporter : CreateMission_Test_Heliporter(); break;
        case eMission_Pion_Test_MoveTo : CreateMission_Test_MoveTo(); break;
        case eMission_Pion_Test_CreateObject : CreateMission_Test_CreateObject(); break;
        case eMission_Pion_Test_DestroyObject : CreateMission_Test_DestroyObject(); break;
        case eMission_Pion_Test_CreateBypass : CreateMission_Test_CreateBypass(); break;
        case eMission_Pion_Test_Fire : CreateMission_Test_Fire(); break;
        case eMission_Pion_Test_ChangePosture : CreateMission_Test_ChangePosture(); break;
        case eMission_Pion_Test_PathFind : CreateMission_Test_PathFind(); break;
        case eMission_Pion_Test_FindPosition : CreateMission_Test_FindPosition(); break;
        case eMission_Pion_Test_Reinforce : CreateMission_Test_Reinforce(); break;
        case eMission_Pion_PlastronEnnemi : CreateMission_PlastronEnnemi(); break;
        case eMission_Pion_FaireMouvement : CreateMission_FaireMouvement(); break;
        case eMission_Pion_Suivre : CreateMission_Suivre(); break;
        case eMission_Pion_SeFaireDecontaminer : CreateMission_SeFaireDecontaminer(); break;
        case eMission_Pion_Franchir : CreateMission_Franchir(); break;
        case eMission_Pion_Decrocher : CreateMission_Decrocher(); break;
        case eMission_Pion_Stationner : CreateMission_Stationner(); break;
        case eMission_Pion_SeFaireTransporter : CreateMission_SeFaireTransporter(); break;
        case eMission_Pion_Generique : CreateMission_Generique(); break;
        case eMission_Pion_MILICE_Provoquer : CreateMission_MILICE_Provoquer(); break;
        case eMission_Pion_MILICE_PrendreEtTenir : CreateMission_MILICE_PrendreEtTenir(); break;
        case eMission_Pion_MILICE_DetruireEmbuscade : CreateMission_MILICE_DetruireEmbuscade(); break;
        case eMission_Pion_JOINT_MARINE_Transporter : CreateMission_JOINT_MARINE_Transporter(); break;
        case eMission_Pion_JOINT_AIR_EffectuerRaid : CreateMission_JOINT_AIR_EffectuerRaid(); break;
        case eMission_Pion_NBC_AnimerUnPlotDeDecontamination : CreateMission_NBC_AnimerUnPlotDeDecontamination(); break;
        case eMission_Pion_NBC_ReconnaitreItineraire : CreateMission_NBC_ReconnaitreItineraire(); break;
        case eMission_Pion_NBC_ReconnaitreZone : CreateMission_NBC_ReconnaitreZone(); break;
        case eMission_Pion_NBC_DecontaminerZone : CreateMission_NBC_DecontaminerZone(); break;
        case eMission_Pion_NBC_ReconnaitreUnSiteROTA : CreateMission_NBC_ReconnaitreUnSiteROTA(); break;
        case eMission_Pion_RENS_ROHUM_RenseignerSur : CreateMission_RENS_ROHUM_RenseignerSur(); break;
        case eMission_Pion_RENS_ROHUM_SExfiltrer : CreateMission_RENS_ROHUM_SExfiltrer(); break;
        case eMission_Pion_RENS_ROHUM_OrienterGuider : CreateMission_RENS_ROHUM_OrienterGuider(); break;
        case eMission_Pion_RENS_ROEM_Appuyer : CreateMission_RENS_ROEM_Appuyer(); break;
        case eMission_Pion_RENS_ROEM_RenseignerSur : CreateMission_RENS_ROEM_RenseignerSur(); break;
        case eMission_Pion_RENS_ROEM_MettreEnOeuvre : CreateMission_RENS_ROEM_MettreEnOeuvre(); break;
        case eMission_Pion_RENS_RADINT_RenseignerSur : CreateMission_RENS_RADINT_RenseignerSur(); break;
        case eMission_Pion_RENS_RADINT_MettreEnOeuvre : CreateMission_RENS_RADINT_MettreEnOeuvre(); break;
        case eMission_Pion_RENS_ROIM_MettreEnOeuvre : CreateMission_RENS_ROIM_MettreEnOeuvre(); break;
        case eMission_Pion_RENS_ROIM_SDTI_RenseignerSur : CreateMission_RENS_ROIM_SDTI_RenseignerSur(); break;
        case eMission_Pion_RENS_ROIM_CL289_RenseignerSur : CreateMission_RENS_ROIM_CL289_RenseignerSur(); break;
        case eMission_Pion_TRANS_EtablirGererLiaison : CreateMission_TRANS_EtablirGererLiaison(); break;


        default:
            assert( false );
    }
}


//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::WriteOrder
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::SendMsg( std::string& sParam )
{
    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
    {
        sParam += "------------------------------------------------|";
        (*itMissionTool)->WriteMsg( sParam );
    }

    if( pASNMsgOrder_->GetAsnMsg().oid_limite_gauche != MIL_NULL_LIMIT_ID )
        pASNMsgOrder_->GetAsnMsg().m.oid_limite_gauchePresent  = 1;
    else
        pASNMsgOrder_->GetAsnMsg().m.oid_limite_gauchePresent  = 0;

    if( pASNMsgOrder_->GetAsnMsg().oid_limite_droite != MIL_NULL_LIMIT_ID )
        pASNMsgOrder_->GetAsnMsg().m.oid_limite_droitePresent  = 1;
    else
        pASNMsgOrder_->GetAsnMsg().m.oid_limite_droitePresent  = 0;

      
    pASNMsgOrder_->Send( 45 );
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::FillRandomParameters()
{
    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->FillRandomParameters();
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::FillRandomParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint i = 0;
    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
    {
        (*itMissionTool)->ReloadParameters( archive );
        ++i;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MissionPion::ResaveParameters
// Created: 
//-----------------------------------------------------------------------------
void MOS_MissionPion::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint i = 0;
    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
    {
        (*itMissionTool)->ResaveParameters( archive );
        ++i;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPion::IsAutomateMission
/** @return 
*/
// Created: 
// -----------------------------------------------------------------------------
bool MOS_MissionPion::IsAutomateMission() const
{
    return false;
}

/*
//-----------------------------------------------------------------------------
// Name: void MOS_MissionPion::CreateMission_Test_MoveTo
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
void MOS_MissionPion::CreateMission_Test_MoveTo()
{
    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *new ASN1T_Mission_Pion_Test_MoveTo();
    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_move_to;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_move_to  = &asnMission;

    MOS_MTH_ItinerairePos& itineraire = CreatePath( asnMission.itineraire, "Itineraire" );
    itineraire.SetFirstWidget( true );

    MOS_ValueList* pPathFind = &CreateValueList( "Mode", 2, 2 );
    pPathFind->CreateValue( asnMission.mode_short      , "short" , MOSToolValueCheck | MOSToolValueModif );
    pPathFind->CreateValue( asnMission.mode_speed      , "speed" , MOSToolValueCheck | MOSToolValueModif );

    MOS_ValueList* pPathIgnore = &CreateValueList( "Ignore", 2, 2 );
    pPathIgnore->CreateValue( asnMission.ign_road          , "road"     , MOSToolValueCheck | MOSToolValueModif );
    pPathIgnore->CreateValue( asnMission.ign_crossroad     , "crossroad", MOSToolValueCheck | MOSToolValueModif );
    pPathIgnore->CreateValue( asnMission.ign_forest        , "forest"   , MOSToolValueCheck | MOSToolValueModif );
    pPathIgnore->CreateValue( asnMission.ign_urban         , "urban"    , MOSToolValueCheck | MOSToolValueModif );
    pPathIgnore->CreateValue( asnMission.ign_river         , "river"    , MOSToolValueCheck | MOSToolValueModif );

    MOS_ValueList* pPathInfos = &CreateValueList( "Infos", 3, 2 );
    pPathInfos->CreateValue( asnMission.inf_object        , "object"     , MOSToolValueCheck | MOSToolValueModif );
    pPathInfos->CreateValue( asnMission.inf_eni_avoid     , "eni avoid"  , MOSToolValueCheck | MOSToolValueModif );
    pPathInfos->CreateValue( asnMission.inf_eni_attack    , "eni attack" , MOSToolValueCheck | MOSToolValueModif );
    pPathInfos->CreateValue( asnMission.inf_fuseau        , "fuseau"     , MOSToolValueCheck | MOSToolValueModif );

    CreatePoint( asnMission.point_dir_locked, "Point/direction lockée" );

    MOS_ValueButtonList<ASN1T_Mission_Pion_Test_MoveTo_lock>*  pLockSelector = &CreateVarList( asnMission.lock, "Lock", 1, 3, false );
    pLockSelector->CreateValue( "Rien",       Mission_Pion_Test_MoveTo_lock::rien, true );
    pLockSelector->CreateValue( "Point",      Mission_Pion_Test_MoveTo_lock::point );
    pLockSelector->CreateValue( "Direction",  Mission_Pion_Test_MoveTo_lock::direction );

    MOS_ValueList* pTmp = &CreateValueList( "Misc", 2, 2 );
    pTmp->CreateValue( asnMission.mode_surete, "surete"  , MOSToolValueCheck | MOSToolValueModif );
    pTmp->CreateValue( asnMission.debarque   , "debarque", MOSToolValueCheck | MOSToolValueModif );
}
*/

// =============================================================================
// Missions
// =============================================================================
void MOS_MissionPion::CreateMission_ALAT_EvacuationSanitaire()
{
    ASN1T_Mission_Pion_ALAT_EvacuationSanitaire& asnMission = *new ASN1T_Mission_Pion_ALAT_EvacuationSanitaire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_evacuation_sanitaire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_evacuation_sanitaire = &asnMission;
    CreatePoint( asnMission.point_debarquement, "point_debarquement" );
    CreateLocation( asnMission.zone_extraction, "zone_extraction" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
}

void MOS_MissionPion::CreateMission_ALAT_ReconnaitreDansLaProfondeur()
{
    ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& asnMission = *new ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_reconnaitre_dans_la_profondeur = &asnMission;
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
    CreatePoint( asnMission.point_dislocation, "point_dislocation" );
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_ALAT_ReconnaitreContourEnnemi()
{
    ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi& asnMission = *new ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_reconnaitre_contour_ennemi;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_reconnaitre_contour_ennemi = &asnMission;
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_DetruireNeutraliserDansZone()
{
    ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *new ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_detruire_neutraliser_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
    CreatePoint( asnMission.point_dislocation, "point_dislocation" );
    CreateNatureAtlas( asnMission.cibles_prioritaires, "cibles_prioritaires" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
    MOS_ValueList* pParameters_neutraliser= &CreateValueList( "", 1, 2 );
    pParameters_neutraliser->CreateValue( asnMission.neutraliser, "neutraliser", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_ALAT_DetruireAeromobile()
{
    ASN1T_Mission_Pion_ALAT_DetruireAeromobile& asnMission = *new ASN1T_Mission_Pion_ALAT_DetruireAeromobile();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_detruire_aeromobile;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_detruire_aeromobile = &asnMission;
    CreateAgentKnowledgeList( asnMission.unite_a_detruire, "unite_a_detruire" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_Freiner()
{
    ASN1T_Mission_Pion_ALAT_Freiner& asnMission = *new ASN1T_Mission_Pion_ALAT_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_freiner = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "cibles_prioritaires" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_Jalonner()
{
    ASN1T_Mission_Pion_ALAT_Jalonner& asnMission = *new ASN1T_Mission_Pion_ALAT_Jalonner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_jalonner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_jalonner = &asnMission;
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateNatureAtlas( asnMission.cibles_prioritaires, "cibles_prioritaires" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_Escorter()
{
    ASN1T_Mission_Pion_ALAT_Escorter& asnMission = *new ASN1T_Mission_Pion_ALAT_Escorter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_escorter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_escorter = &asnMission;
    CreateAgentList( asnMission.unites_a_escorter, "unites_a_escorter" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateNatureAtlas( asnMission.cibles_prioritaires, "cibles_prioritaires" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_Heliporter()
{
    ASN1T_Mission_Pion_ALAT_Heliporter& asnMission = *new ASN1T_Mission_Pion_ALAT_Heliporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_heliporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_heliporter = &asnMission;
    CreateAgentList( asnMission.unites_a_heliporter, "unites_a_heliporter" );
    CreatePoint( asnMission.point_debarquement, "point_debarquement" );
    CreatePoint( asnMission.point_embarquement, "point_embarquement" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueList* pParameters_attendre_unite= &CreateValueList( "", 1, 2 );
    pParameters_attendre_unite->CreateValue( asnMission.attendre_unite, "attendre_unite", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueList* pParameters_avec_materiel= &CreateValueList( "", 1, 2 );
    pParameters_avec_materiel->CreateValue( asnMission.avec_materiel, "avec_materiel", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_ALAT_HeliporterHelitransporterPlotRavitaillement()
{
    ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement& asnMission = *new ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_heliporter_helitransporter_plot_ravitaillement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_heliporter_helitransporter_plot_ravitaillement = &asnMission;
    CreatePoint( asnMission.point_debarquement, "point_debarquement" );
    CreatePoint( asnMission.position_regroupement, "position_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
}

void MOS_MissionPion::CreateMission_ALAT_Helitransporter()
{
    ASN1T_Mission_Pion_ALAT_Helitransporter& asnMission = *new ASN1T_Mission_Pion_ALAT_Helitransporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_helitransporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_helitransporter = &asnMission;
    CreateAgentList( asnMission.unites_a_helitransporter, "unites_a_helitransporter" );
    CreatePoint( asnMission.point_debarquement, "point_debarquement" );
    CreatePoint( asnMission.point_embarquement, "point_embarquement" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueList* pParameters_avec_materiel= &CreateValueList( "", 1, 2 );
    pParameters_avec_materiel->CreateValue( asnMission.avec_materiel, "avec_materiel", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_ALAT_EffectuerRechercheEtSauvetage()
{
    ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage& asnMission = *new ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_effectuer_recherche_et_sauvetage;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_effectuer_recherche_et_sauvetage = &asnMission;
    CreateAgentList( asnMission.unites_a_secourir, "unites_a_secourir" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
}

void MOS_MissionPion::CreateMission_ALAT_IMEX()
{
    ASN1T_Mission_Pion_ALAT_IMEX& asnMission = *new ASN1T_Mission_Pion_ALAT_IMEX();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_imex;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_imex = &asnMission;
    CreateAgentList( asnMission.unites_a_appuyer, "unites_a_appuyer" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
}

void MOS_MissionPion::CreateMission_ALAT_Eclairer()
{
    ASN1T_Mission_Pion_ALAT_Eclairer& asnMission = *new ASN1T_Mission_Pion_ALAT_Eclairer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_eclairer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_eclairer = &asnMission;
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_Surveiller()
{
    ASN1T_Mission_Pion_ALAT_Surveiller& asnMission = *new ASN1T_Mission_Pion_ALAT_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_AppuyerDirectAuContact()
{
    ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact& asnMission = *new ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_appuyer_direct_au_contact;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_appuyer_direct_au_contact = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "cibles_prioritaires" );
    CreateAgentList( asnMission.unites_a_appuyer, "unites_a_appuyer" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_Couvrir()
{
    ASN1T_Mission_Pion_ALAT_Couvrir& asnMission = *new ASN1T_Mission_Pion_ALAT_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_couvrir = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "cibles_prioritaires" );
    CreateAgentList( asnMission.unites_a_couvrir, "unites_a_couvrir" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ALAT_Attaquer()
{
    ASN1T_Mission_Pion_ALAT_Attaquer& asnMission = *new ASN1T_Mission_Pion_ALAT_Attaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_attaquer = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "cibles_prioritaires" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "plots_ravitaillement" );
    MOS_ValueList* pParameters_ravitaillement_debut_mission= &CreateValueList( "", 1, 2 );
    pParameters_ravitaillement_debut_mission->CreateValue( asnMission.ravitaillement_debut_mission, "ravitaillement_debut_mission", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action,"portee_action", 2, 3, false );
    pSelector_portee_action->CreateValue( "courte_portee", EnumMissionALAT_PorteeAction::courte_portee, true );
    pSelector_portee_action->CreateValue( "moyenne_portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->CreateValue( "longue_portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->CreateValue( "sans_munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    MOS_ValueButtonList< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission,"ambiance_mission", 2, 3, false );
    pSelector_ambiance_mission->CreateValue( "aa", EnumMissionALAT_AmbianceMission::aa, true );
    pSelector_ambiance_mission->CreateValue( "as", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->CreateValue( "polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

void MOS_MissionPion::CreateMission_ASA_DefendreSite()
{
    ASN1T_Mission_Pion_ASA_DefendreSite& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreSite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_site;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_site = &asnMission;
    CreateLocation( asnMission.site, "site" );
}

void MOS_MissionPion::CreateMission_ASA_DefendreSiteAPartirPosition()
{
    ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_site_a_partir_position = &asnMission;
    CreateLocation( asnMission.site, "site" );
    CreatePoint( asnMission.point_de_deploiement, "point_de_deploiement" );
}

void MOS_MissionPion::CreateMission_ASA_DefendreZone()
{
    ASN1T_Mission_Pion_ASA_DefendreZone& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_ASA_DefendreZoneAPartirPosition()
{
    ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_zone_a_partir_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_zone_a_partir_position = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
    CreatePoint( asnMission.point_de_deploiement, "point_de_deploiement" );
}

void MOS_MissionPion::CreateMission_ASA_Surveiller()
{
    ASN1T_Mission_Pion_ASA_Surveiller& asnMission = *new ASN1T_Mission_Pion_ASA_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_surveiller = &asnMission;
    CreatePoint( asnMission.point_de_deploiement, "point_de_deploiement" );
}

void MOS_MissionPion::CreateMission_ASA_DefendreUnites()
{
    ASN1T_Mission_Pion_ASA_DefendreUnites& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreUnites();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_unites;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_unites = &asnMission;
    CreateAgentList( asnMission.unites, "unites" );
}

void MOS_MissionPion::CreateMission_ASA_MISTRAL_DefendreSite()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_site;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_site = &asnMission;
    CreateLocation( asnMission.site, "site" );
    MOS_ValueButtonList< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement,"mode_deploiement", 2, 3, false );
    pSelector_mode_deploiement->CreateValue( "omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel, true );
    pSelector_mode_deploiement->CreateValue( "directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

void MOS_MissionPion::CreateMission_ASA_MISTRAL_DefendreSiteAPartirPosition()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_site_a_partir_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_site_a_partir_position = &asnMission;
    CreateLocation( asnMission.site, "site" );
    CreatePoint( asnMission.point_de_deploiement, "point_de_deploiement" );
    MOS_ValueButtonList< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement,"mode_deploiement", 2, 3, false );
    pSelector_mode_deploiement->CreateValue( "omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel, true );
    pSelector_mode_deploiement->CreateValue( "directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

void MOS_MissionPion::CreateMission_ASA_MISTRAL_DefendreZone()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
    MOS_ValueButtonList< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement,"mode_deploiement", 2, 3, false );
    pSelector_mode_deploiement->CreateValue( "omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel, true );
    pSelector_mode_deploiement->CreateValue( "directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

void MOS_MissionPion::CreateMission_ASA_MISTRAL_DefendreZoneAPartirPosition()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_zone_a_partir_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_zone_a_partir_position = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
    CreatePoint( asnMission.point_de_deploiement, "point_de_deploiement" );
    MOS_ValueButtonList< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement,"mode_deploiement", 2, 3, false );
    pSelector_mode_deploiement->CreateValue( "omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel, true );
    pSelector_mode_deploiement->CreateValue( "directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

void MOS_MissionPion::CreateMission_ASA_MISTRAL_Surveiller()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_surveiller = &asnMission;
    CreatePoint( asnMission.point_de_deploiement, "point_de_deploiement" );
    MOS_ValueButtonList< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement,"mode_deploiement", 2, 3, false );
    pSelector_mode_deploiement->CreateValue( "omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel, true );
    pSelector_mode_deploiement->CreateValue( "directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

void MOS_MissionPion::CreateMission_ASS_AssurerMiseEnOeuvre()
{
    ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre& asnMission = *new ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_assurer_mise_en_oeuvre = &asnMission;
    CreateObjectKnowledge( asnMission.zone_implantation, "zone_implantation" );
    MOS_ValueList* pParameters_presence_contre_batterie= &CreateValueList( "", 1, 2 );
    pParameters_presence_contre_batterie->CreateValue( asnMission.presence_contre_batterie, "presence_contre_batterie", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_ASS_AssurerMiseEnOeuvreSurPosition()
{
    ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& asnMission = *new ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre_sur_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_assurer_mise_en_oeuvre_sur_position = &asnMission;
    CreatePoint( asnMission.position_implantation, "position_implantation" );
    MOS_ValueList* pParameters_presence_contre_batterie= &CreateValueList( "", 1, 2 );
    pParameters_presence_contre_batterie->CreateValue( asnMission.presence_contre_batterie, "presence_contre_batterie", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_ASS_ReconnaitreZoneDeploiement()
{
    ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& asnMission = *new ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_reconnaitre_zone_deploiement = &asnMission;
    CreatePoint( asnMission.position_deploiement, "position_deploiement" );
}

void MOS_MissionPion::CreateMission_ASS_AcquerirLanceursEni()
{
    ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& asnMission = *new ASN1T_Mission_Pion_ASS_AcquerirLanceursEni();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_acquerir_lanceurs_eni = &asnMission;
    CreatePolygonList( asnMission.zones_a_observer, "zones_a_observer" );
    CreateObjectKnowledge( asnMission.zone_implantation, "zone_implantation" );
    MOS_ValueList* pParameters_menance_roem= &CreateValueList( "", 1, 2 );
    pParameters_menance_roem->CreateValue( asnMission.menance_roem, "menance_roem", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_ASS_EO_AcquerirObjectifs()
{
    ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_eo_acquerir_objectifs = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "zone_a_observer" );
    CreateAgent( asnMission.unite_a_appuyer, "unite_a_appuyer" );
    CreateNatureAtlas( asnMission.categories, "categories" );
}

void MOS_MissionPion::CreateMission_ASS_EOP_AcquerirObjectifs()
{
    ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_eop_acquerir_objectifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_eop_acquerir_objectifs = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "zone_a_observer" );
    CreatePoint( asnMission.position_installation, "position_installation" );
    CreatePoint( asnMission.position_debarquement, "position_debarquement" );
}

void MOS_MissionPion::CreateMission_ASS_RATAC_AcquerirObjectifs()
{
    ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_ratac_acquerir_objectifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_ratac_acquerir_objectifs = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "zone_a_observer" );
    CreateNatureAtlas( asnMission.categories, "categories" );
    CreatePoint( asnMission.position_deploiement, "position_deploiement" );
}

void MOS_MissionPion::CreateMission_ABC_ReconnaitreAxe()
{
    ASN1T_Mission_Pion_ABC_ReconnaitreAxe& asnMission = *new ASN1T_Mission_Pion_ABC_ReconnaitreAxe();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_reconnaitre_axe;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_axe = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_ABC_Soutenir()
{
    ASN1T_Mission_Pion_ABC_Soutenir& asnMission = *new ASN1T_Mission_Pion_ABC_Soutenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_soutenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_soutenir = &asnMission;
    CreateAgent( asnMission.unite_a_soutenir, "unite_a_soutenir" );
}

void MOS_MissionPion::CreateMission_ABC_Couvrir()
{
    ASN1T_Mission_Pion_ABC_Couvrir& asnMission = *new ASN1T_Mission_Pion_ABC_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_couvrir = &asnMission;
    CreatePoint( asnMission.position, "position" );
    CreateAgentList( asnMission.unites_a_couvrir, "unites_a_couvrir" );
}

void MOS_MissionPion::CreateMission_ABC_FlancGarder()
{
    ASN1T_Mission_Pion_ABC_FlancGarder& asnMission = *new ASN1T_Mission_Pion_ABC_FlancGarder();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_flanc_garder;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_flanc_garder = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
    CreateAgentList( asnMission.unites_a_couvrir, "unites_a_couvrir" );
}

void MOS_MissionPion::CreateMission_ABC_Reduire()
{
    ASN1T_Mission_Pion_ABC_Reduire& asnMission = *new ASN1T_Mission_Pion_ABC_Reduire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_reduire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reduire = &asnMission;
    CreateAgentKnowledge( asnMission.unite_a_reduire, "unite_a_reduire" );
}

void MOS_MissionPion::CreateMission_ABC_Appuyer()
{
    ASN1T_Mission_Pion_ABC_Appuyer& asnMission = *new ASN1T_Mission_Pion_ABC_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_appuyer = &asnMission;
    CreateAgent( asnMission.unite_a_appuyer, "unite_a_appuyer" );
}

void MOS_MissionPion::CreateMission_ABC_Fixer()
{
    ASN1T_Mission_Pion_ABC_Fixer& asnMission = *new ASN1T_Mission_Pion_ABC_Fixer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_fixer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_fixer = &asnMission;
    CreateAgentKnowledge( asnMission.unite_a_fixer, "unite_a_fixer" );
}

void MOS_MissionPion::CreateMission_ABC_RenseignerSur()
{
    ASN1T_Mission_Pion_ABC_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_ABC_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_renseigner_sur = &asnMission;
    CreatePoint( asnMission.point_a_renseigner, "point_a_renseigner" );
}

void MOS_MissionPion::CreateMission_ABC_ControlerZone()
{
    ASN1T_Mission_Pion_ABC_ControlerZone& asnMission = *new ASN1T_Mission_Pion_ABC_ControlerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_controler_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_controler_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_ABC_Surveiller()
{
    ASN1T_Mission_Pion_ABC_Surveiller& asnMission = *new ASN1T_Mission_Pion_ABC_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_surveiller = &asnMission;
    CreatePoint( asnMission.position, "position" );
}

void MOS_MissionPion::CreateMission_ABC_Barrer()
{
    ASN1T_Mission_Pion_ABC_Barrer& asnMission = *new ASN1T_Mission_Pion_ABC_Barrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_barrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_barrer = &asnMission;
    CreatePoint( asnMission.position_installation, "position_installation" );
}

void MOS_MissionPion::CreateMission_ABC_Attaquer()
{
    ASN1T_Mission_Pion_ABC_Attaquer& asnMission = *new ASN1T_Mission_Pion_ABC_Attaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_attaquer = &asnMission;
    CreatePath( asnMission.itineraire_assaut, "itineraire_assaut" );
}

void MOS_MissionPion::CreateMission_ABC_ReconnaitrePoint()
{
    ASN1T_Mission_Pion_ABC_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Pion_ABC_ReconnaitrePoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_reconnaitre_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_point = &asnMission;
    CreatePoint( asnMission.point_a_reconnaitre, "point_a_reconnaitre" );
}

void MOS_MissionPion::CreateMission_ABC_Eclairer()
{
    ASN1T_Mission_Pion_ABC_Eclairer& asnMission = *new ASN1T_Mission_Pion_ABC_Eclairer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_eclairer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_eclairer = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_ABC_Jalonner()
{
    ASN1T_Mission_Pion_ABC_Jalonner& asnMission = *new ASN1T_Mission_Pion_ABC_Jalonner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_jalonner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_jalonner = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_ABC_ContreAttaquer()
{
    ASN1T_Mission_Pion_ABC_ContreAttaquer& asnMission = *new ASN1T_Mission_Pion_ABC_ContreAttaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_contre_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_contre_attaquer = &asnMission;
    CreatePath( asnMission.itineraire_assaut, "itineraire_assaut" );
}

void MOS_MissionPion::CreateMission_ABC_SemparerZone()
{
    ASN1T_Mission_Pion_ABC_SemparerZone& asnMission = *new ASN1T_Mission_Pion_ABC_SemparerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_semparer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_semparer_zone = &asnMission;
    CreateLocation( asnMission.zone_a_prendre, "zone_a_prendre" );
}

void MOS_MissionPion::CreateMission_ABC_ReleverUnite()
{
    ASN1T_Mission_Pion_ABC_ReleverUnite& asnMission = *new ASN1T_Mission_Pion_ABC_ReleverUnite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_relever_unite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_relever_unite = &asnMission;
    CreateAgent( asnMission.unite_a_relever, "unite_a_relever" );
}

void MOS_MissionPion::CreateMission_ABC_ArmerPIA()
{
    ASN1T_Mission_Pion_ABC_ArmerPIA& asnMission = *new ASN1T_Mission_Pion_ABC_ArmerPIA();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_armer_pia;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_armer_pia = &asnMission;
    CreatePoint( asnMission.pia, "pia" );
}

void MOS_MissionPion::CreateMission_ABC_OuvrirItineraire()
{
    ASN1T_Mission_Pion_ABC_OuvrirItineraire& asnMission = *new ASN1T_Mission_Pion_ABC_OuvrirItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_ouvrir_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_ouvrir_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_ABC_Freiner()
{
    ASN1T_Mission_Pion_ABC_Freiner& asnMission = *new ASN1T_Mission_Pion_ABC_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_freiner = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_ABC_EscorterUnConvoi()
{
    ASN1T_Mission_Pion_ABC_EscorterUnConvoi& asnMission = *new ASN1T_Mission_Pion_ABC_EscorterUnConvoi();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_escorter_un_convoi;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_escorter_un_convoi = &asnMission;
    CreateAgent( asnMission.convoi_a_escorter, "convoi_a_escorter" );
}

void MOS_MissionPion::CreateMission_ABC_ArmerPointDeControle()
{
    ASN1T_Mission_Pion_ABC_ArmerPointDeControle& asnMission = *new ASN1T_Mission_Pion_ABC_ArmerPointDeControle();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_armer_point_de_controle;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_armer_point_de_controle = &asnMission;
    CreatePoint( asnMission.point_de_controle, "point_de_controle" );
}

void MOS_MissionPion::CreateMission_ABC_OrganiserAccueilColonneRefugies()
{
    ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies& asnMission = *new ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_organiser_accueil_colonne_refugies;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_organiser_accueil_colonne_refugies = &asnMission;
    CreatePoint( asnMission.position_installation, "position_installation" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserFosseAC()
{
    ASN1T_Mission_Pion_GEN_RealiserFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserFosseAC();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_fosse_ac;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_fosse_ac = &asnMission;
    CreateLocation( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueButtonList< ASN1T_EnumTypeFosseAC >* pSelector_type = &CreateVarList( asnMission.type,"type", 2, 3, false );
    pSelector_type->CreateValue( "simple", EnumTypeFosseAC::simple, true );
    pSelector_type->CreateValue( "compose", EnumTypeFosseAC::compose );
}

void MOS_MissionPion::CreateMission_GEN_RealiserAbattisMine()
{
    ASN1T_Mission_Pion_GEN_RealiserAbattisMine& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserAbattisMine();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_abattis_mine;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_abattis_mine = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueButtonList< ASN1T_EnumMissionGenUrgence >* pSelector_urgence = &CreateVarList( asnMission.urgence,"urgence", 2, 3, false );
    pSelector_urgence->CreateValue( "serie_minimum", EnumMissionGenUrgence::serie_minimum, true );
    pSelector_urgence->CreateValue( "serie_complementaire", EnumMissionGenUrgence::serie_complementaire );
    MOS_ValueButtonList< ASN1T_EnumMissionGenSousTypeObstacle >* pSelector_preliminaire = &CreateVarList( asnMission.preliminaire,"preliminaire", 2, 3, false );
    pSelector_preliminaire->CreateValue( "preliminaire", EnumMissionGenSousTypeObstacle::preliminaire, true );
    pSelector_preliminaire->CreateValue( "de_manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
}

void MOS_MissionPion::CreateMission_GEN_RealiserPointMine()
{
    ASN1T_Mission_Pion_GEN_RealiserPointMine& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPointMine();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_point_mine;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_point_mine = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserBarricade()
{
    ASN1T_Mission_Pion_GEN_RealiserBarricade& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserBarricade();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_barricade;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_barricade = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueButtonList< ASN1T_EnumMissionGenUrgence >* pSelector_urgence = &CreateVarList( asnMission.urgence,"urgence", 2, 3, false );
    pSelector_urgence->CreateValue( "serie_minimum", EnumMissionGenUrgence::serie_minimum, true );
    pSelector_urgence->CreateValue( "serie_complementaire", EnumMissionGenUrgence::serie_complementaire );
}

void MOS_MissionPion::CreateMission_GEN_RealiserEboulement()
{
    ASN1T_Mission_Pion_GEN_RealiserEboulement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserEboulement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_eboulement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_eboulement = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueButtonList< ASN1T_EnumMissionGenSousTypeObstacle >* pSelector_preliminaire = &CreateVarList( asnMission.preliminaire,"preliminaire", 2, 3, false );
    pSelector_preliminaire->CreateValue( "preliminaire", EnumMissionGenSousTypeObstacle::preliminaire, true );
    pSelector_preliminaire->CreateValue( "de_manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
}

void MOS_MissionPion::CreateMission_GEN_RealiserBouchonMine()
{
    ASN1T_Mission_Pion_GEN_RealiserBouchonMine& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserBouchonMine();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_bouchon_mine;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_bouchon_mine = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueButtonList< ASN1T_EnumMissionGenUrgence >* pSelector_urgence = &CreateVarList( asnMission.urgence,"urgence", 2, 3, false );
    pSelector_urgence->CreateValue( "serie_minimum", EnumMissionGenUrgence::serie_minimum, true );
    pSelector_urgence->CreateValue( "serie_complementaire", EnumMissionGenUrgence::serie_complementaire );
}

void MOS_MissionPion::CreateMission_GEN_DetruireRoute()
{
    ASN1T_Mission_Pion_GEN_DetruireRoute& asnMission = *new ASN1T_Mission_Pion_GEN_DetruireRoute();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_detruire_route;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_detruire_route = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueButtonList< ASN1T_EnumMissionGenSousTypeObstacle >* pSelector_preliminaire = &CreateVarList( asnMission.preliminaire,"preliminaire", 2, 3, false );
    pSelector_preliminaire->CreateValue( "preliminaire", EnumMissionGenSousTypeObstacle::preliminaire, true );
    pSelector_preliminaire->CreateValue( "de_manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
}

void MOS_MissionPion::CreateMission_GEN_DetruirePont()
{
    ASN1T_Mission_Pion_GEN_DetruirePont& asnMission = *new ASN1T_Mission_Pion_GEN_DetruirePont();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_detruire_pont;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_detruire_pont = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueButtonList< ASN1T_EnumMissionGenSousTypeObstacle >* pSelector_preliminaire = &CreateVarList( asnMission.preliminaire,"preliminaire", 2, 3, false );
    pSelector_preliminaire->CreateValue( "preliminaire", EnumMissionGenSousTypeObstacle::preliminaire, true );
    pSelector_preliminaire->CreateValue( "de_manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
}

void MOS_MissionPion::CreateMission_GEN_SecuriserItineraire()
{
    ASN1T_Mission_Pion_GEN_SecuriserItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_SecuriserItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_securiser_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_securiser_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_GEN_DegagerAbattisMine()
{
    ASN1T_Mission_Pion_GEN_DegagerAbattisMine& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerAbattisMine();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_abattis_mine;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_abattis_mine = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_DegagerEboulement()
{
    ASN1T_Mission_Pion_GEN_DegagerEboulement& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerEboulement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_eboulement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_eboulement = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_DegagerRues()
{
    ASN1T_Mission_Pion_GEN_DegagerRues& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerRues();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_rues;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_rues = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_ComblerEntonnoir()
{
    ASN1T_Mission_Pion_GEN_ComblerEntonnoir& asnMission = *new ASN1T_Mission_Pion_GEN_ComblerEntonnoir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_combler_entonnoir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_combler_entonnoir = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_ComblerFosseAC()
{
    ASN1T_Mission_Pion_GEN_ComblerFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_ComblerFosseAC();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_combler_fosse_ac;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_combler_fosse_ac = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_CreerContournement()
{
    ASN1T_Mission_Pion_GEN_CreerContournement& asnMission = *new ASN1T_Mission_Pion_GEN_CreerContournement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_contournement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_contournement = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserZoneMineeLineaire()
{
    ASN1T_Mission_Pion_GEN_RealiserZoneMineeLineaire& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZoneMineeLineaire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_minee_lineaire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_lineaire = &asnMission;
    CreateLocation( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueList* pParameters_densite= &CreateValueList( "", 1, 2 );
    pParameters_densite->CreateValue( asnMission.densite, "densite", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_GEN_OuvrirTrouee()
{
    ASN1T_Mission_Pion_GEN_OuvrirTrouee& asnMission = *new ASN1T_Mission_Pion_GEN_OuvrirTrouee();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_ouvrir_trouee;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_ouvrir_trouee = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_Renforcer()
{
    ASN1T_Mission_Pion_GEN_Renforcer& asnMission = *new ASN1T_Mission_Pion_GEN_Renforcer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_renforcer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_renforcer = &asnMission;
    CreatePoint( asnMission.lieu_renforcement, "lieu_renforcement" );
    CreateAgent( asnMission.unite, "unite" );
}

void MOS_MissionPion::CreateMission_GEN_FranchirEntonnoir()
{
    ASN1T_Mission_Pion_GEN_FranchirEntonnoir& asnMission = *new ASN1T_Mission_Pion_GEN_FranchirEntonnoir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_franchir_entonnoir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_franchir_entonnoir = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_FranchirFosseAC()
{
    ASN1T_Mission_Pion_GEN_FranchirFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_FranchirFosseAC();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_franchir_fosse_ac;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_franchir_fosse_ac = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserZoneMineeParDispersion()
{
    ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_par_dispersion = &asnMission;
    CreateLocation( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueList* pParameters_densite= &CreateValueList( "", 1, 2 );
    pParameters_densite->CreateValue( asnMission.densite, "densite", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_GEN_RealiserPostesTir()
{
    ASN1T_Mission_Pion_GEN_RealiserPostesTir& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPostesTir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_postes_tir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_postes_tir = &asnMission;
    CreatePoint( asnMission.pos_objet, "pos_objet" );
    CreateID( asnMission.oid_objet_planifie, "oid_objet_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserTravauxProtection()
{
    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxProtection();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_travaux_protection;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_protection = &asnMission;
    CreatePoint( asnMission.pos_objet, "pos_objet" );
    CreateID( asnMission.oid_objet_planifie, "oid_objet_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_RetablirItineraire()
{
    ASN1T_Mission_Pion_GEN_RetablirItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_RetablirItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_retablir_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_retablir_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_GEN_EquiperExploiter()
{
    ASN1T_Mission_Pion_GEN_EquiperExploiter& asnMission = *new ASN1T_Mission_Pion_GEN_EquiperExploiter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_equiper_exploiter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_equiper_exploiter = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "site_franchissement" );
    CreateID( asnMission.oid_objet_planifie, "oid_objet_planifie" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    MOS_ValueButtonList< ASN1T_EnumGenTypePontage >* pSelector_type_pontage = &CreateVarList( asnMission.type_pontage,"type_pontage", 2, 3, false );
    pSelector_type_pontage->CreateValue( "continu", EnumGenTypePontage::continu, true );
    pSelector_type_pontage->CreateValue( "discontinu", EnumGenTypePontage::discontinu );
}

void MOS_MissionPion::CreateMission_GEN_ExecuterVariantement()
{
    ASN1T_Mission_Pion_GEN_ExecuterVariantement& asnMission = *new ASN1T_Mission_Pion_GEN_ExecuterVariantement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_executer_variantement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_executer_variantement = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "site_franchissement" );
    CreateID( asnMission.oid_objet_planifie, "oid_objet_planifie" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    MOS_ValueButtonList< ASN1T_EnumGenTypePontage >* pSelector_type_pontage = &CreateVarList( asnMission.type_pontage,"type_pontage", 2, 3, false );
    pSelector_type_pontage->CreateValue( "continu", EnumGenTypePontage::continu, true );
    pSelector_type_pontage->CreateValue( "discontinu", EnumGenTypePontage::discontinu );
}

void MOS_MissionPion::CreateMission_GEN_ReconnaitreSiteFranchissement()
{
    ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement& asnMission = *new ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reconnaitre_site_franchissement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_site_franchissement = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "site_franchissement" );
}

void MOS_MissionPion::CreateMission_GEN_ReconnaitreItineraire()
{
    ASN1T_Mission_Pion_GEN_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_ReconnaitreItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reconnaitre_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_GEN_DeminerSiteFranchissement()
{
    ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement& asnMission = *new ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_deminer_site_franchissement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_deminer_site_franchissement = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "site_franchissement" );
}

void MOS_MissionPion::CreateMission_GEN_ReconnaitreZone()
{
    ASN1T_Mission_Pion_GEN_ReconnaitreZone& asnMission = *new ASN1T_Mission_Pion_GEN_ReconnaitreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reconnaitre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_GEN_CreerPlateForme()
{
    ASN1T_Mission_Pion_GEN_CreerPlateForme& asnMission = *new ASN1T_Mission_Pion_GEN_CreerPlateForme();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_plate_forme;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_plate_forme = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_CreerPiste()
{
    ASN1T_Mission_Pion_GEN_CreerPiste& asnMission = *new ASN1T_Mission_Pion_GEN_CreerPiste();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_piste;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_piste = &asnMission;
    CreateLocation( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_CreerAirePoser()
{
    ASN1T_Mission_Pion_GEN_CreerAirePoser& asnMission = *new ASN1T_Mission_Pion_GEN_CreerAirePoser();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_aire_poser;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_aire_poser = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_NeutraliserEnginsExplosifs()
{
    ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs& asnMission = *new ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_neutraliser_engins_explosifs = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_ActiverObstacle()
{
    ASN1T_Mission_Pion_GEN_ActiverObstacle& asnMission = *new ASN1T_Mission_Pion_GEN_ActiverObstacle();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_activer_obstacle;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_activer_obstacle = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "obstacle" );
}

void MOS_MissionPion::CreateMission_GEN_AmeliorerMobilite()
{
    ASN1T_Mission_Pion_GEN_AmeliorerMobilite& asnMission = *new ASN1T_Mission_Pion_GEN_AmeliorerMobilite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_ameliorer_mobilite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_ameliorer_mobilite = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserAireLogistique()
{
    ASN1T_Mission_Pion_GEN_RealiserAireLogistique& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserAireLogistique();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_aire_logistique;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_aire_logistique = &asnMission;
    CreatePoint( asnMission.position_obstacle, "position_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_DepolluerZone()
{
    ASN1T_Mission_Pion_GEN_DepolluerZone& asnMission = *new ASN1T_Mission_Pion_GEN_DepolluerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_depolluer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_depolluer_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_GEN_AmenagerTerrainPoser()
{
    ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser& asnMission = *new ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_amenager_terrain_poser;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_amenager_terrain_poser = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "pos_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
    MOS_ValueButtonList< ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser_type_objet >* pSelector_type_objet = &CreateVarList( asnMission.type_objet,"type_objet", 2, 3, false );
    pSelector_type_objet->CreateValue( "aire_poser", Mission_Pion_GEN_AmenagerTerrainPoser_type_objet::aire_poser, true );
    pSelector_type_objet->CreateValue( "terrain_largage", Mission_Pion_GEN_AmenagerTerrainPoser_type_objet::terrain_largage );
}

void MOS_MissionPion::CreateMission_GEN_RealiserPlateFormeDeploiement()
{
    ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_plate_forme_deploiement = &asnMission;
    CreatePoint( asnMission.position_obstacle, "position_obstacle" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_ReprendreMissionConstruction()
{
    ASN1T_Mission_Pion_GEN_ReprendreMissionConstruction& asnMission = *new ASN1T_Mission_Pion_GEN_ReprendreMissionConstruction();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reprendre_mission_construction;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reprendre_mission_construction = &asnMission;
    CreateObjectKnowledge( asnMission.travaux, "travaux" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserZonePoserHelicoptere()
{
    ASN1T_Mission_Pion_GEN_RealiserZonePoserHelicoptere& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZonePoserHelicoptere();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_poser_helicoptere;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_poser_helicoptere = &asnMission;
    CreatePoint( asnMission.zone_poser, "zone_poser" );
    CreateID( asnMission.oid_obstacle_planifie, "oid_obstacle_planifie" );
}

void MOS_MissionPion::CreateMission_GEN_AppuyerDebarquementTroupesGU()
{
    ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU& asnMission = *new ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_appuyer_debarquement_troupes_gu = &asnMission;
    CreateLocation( asnMission.zone_appui, "zone_appui" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserCampPrisonniers()
{
    ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_prisonniers = &asnMission;
    CreateLocation( asnMission.camp, "camp" );
    CreateAgent( asnMission.tc2, "tc2" );
}

void MOS_MissionPion::CreateMission_GEN_RealiserCampRefugies()
{
    ASN1T_Mission_Pion_GEN_RealiserCampRefugies& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserCampRefugies();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_camp_refugies;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_refugies = &asnMission;
    CreateLocation( asnMission.camp, "camp" );
    CreateAgent( asnMission.tc2, "tc2" );
}

void MOS_MissionPion::CreateMission_INF_ReconnaitreAxe()
{
    ASN1T_Mission_Pion_INF_ReconnaitreAxe& asnMission = *new ASN1T_Mission_Pion_INF_ReconnaitreAxe();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_reconnaitre_axe;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reconnaitre_axe = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_INF_Couvrir()
{
    ASN1T_Mission_Pion_INF_Couvrir& asnMission = *new ASN1T_Mission_Pion_INF_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_couvrir = &asnMission;
    CreatePoint( asnMission.point_couverture, "point_couverture" );
    CreateAgent( asnMission.cible, "cible" );
}

void MOS_MissionPion::CreateMission_INF_Reduire()
{
    ASN1T_Mission_Pion_INF_Reduire& asnMission = *new ASN1T_Mission_Pion_INF_Reduire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_reduire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reduire = &asnMission;
    CreateAgentKnowledge( asnMission.unite_a_reduire, "unite_a_reduire" );
}

void MOS_MissionPion::CreateMission_INF_Appuyer()
{
    ASN1T_Mission_Pion_INF_Appuyer& asnMission = *new ASN1T_Mission_Pion_INF_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_appuyer = &asnMission;
    CreateAgent( asnMission.unite_a_appuyer, "unite_a_appuyer" );
}

void MOS_MissionPion::CreateMission_INF_Fixer()
{
    ASN1T_Mission_Pion_INF_Fixer& asnMission = *new ASN1T_Mission_Pion_INF_Fixer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_fixer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_fixer = &asnMission;
    CreateAgentKnowledge( asnMission.ennemi, "ennemi" );
}

void MOS_MissionPion::CreateMission_INF_ControlerSecteur()
{
    ASN1T_Mission_Pion_INF_ControlerSecteur& asnMission = *new ASN1T_Mission_Pion_INF_ControlerSecteur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_controler_secteur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_controler_secteur = &asnMission;
    CreateLocation( asnMission.secteur, "secteur" );
    MOS_ValueList* pParameters_preparer_terrain= &CreateValueList( "", 1, 2 );
    pParameters_preparer_terrain->CreateValue( asnMission.preparer_terrain, "preparer_terrain", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_INF_SurveillerSecteur()
{
    ASN1T_Mission_Pion_INF_SurveillerSecteur& asnMission = *new ASN1T_Mission_Pion_INF_SurveillerSecteur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_surveiller_secteur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_surveiller_secteur = &asnMission;
    CreatePoint( asnMission.position_installation, "position_installation" );
}

void MOS_MissionPion::CreateMission_INF_Barrer()
{
    ASN1T_Mission_Pion_INF_Barrer& asnMission = *new ASN1T_Mission_Pion_INF_Barrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_barrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_barrer = &asnMission;
    CreatePoint( asnMission.position_a_barrer, "position_a_barrer" );
}

void MOS_MissionPion::CreateMission_INF_ReconnaitrePoint()
{
    ASN1T_Mission_Pion_INF_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Pion_INF_ReconnaitrePoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_reconnaitre_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reconnaitre_point = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
    CreatePoint( asnMission.point_a_reconnaitre, "point_a_reconnaitre" );
}

void MOS_MissionPion::CreateMission_INF_Eclairer()
{
    ASN1T_Mission_Pion_INF_Eclairer& asnMission = *new ASN1T_Mission_Pion_INF_Eclairer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_eclairer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_eclairer = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_INF_Interdire()
{
    ASN1T_Mission_Pion_INF_Interdire& asnMission = *new ASN1T_Mission_Pion_INF_Interdire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_interdire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_interdire = &asnMission;
    CreatePoint( asnMission.point_a_interdire, "point_a_interdire" );
    MOS_ValueList* pParameters_preparer_terrain= &CreateValueList( "", 1, 2 );
    pParameters_preparer_terrain->CreateValue( asnMission.preparer_terrain, "preparer_terrain", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_INF_Semparer()
{
    ASN1T_Mission_Pion_INF_Semparer& asnMission = *new ASN1T_Mission_Pion_INF_Semparer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_semparer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_semparer = &asnMission;
    CreatePoint( asnMission.objectif, "objectif" );
}

void MOS_MissionPion::CreateMission_INF_PrendrePreciserContact()
{
    ASN1T_Mission_Pion_INF_PrendrePreciserContact& asnMission = *new ASN1T_Mission_Pion_INF_PrendrePreciserContact();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_prendre_preciser_contact;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_prendre_preciser_contact = &asnMission;
    CreateAgentKnowledge( asnMission.ennemi, "ennemi" );
}

void MOS_MissionPion::CreateMission_INF_Defendre()
{
    ASN1T_Mission_Pion_INF_Defendre& asnMission = *new ASN1T_Mission_Pion_INF_Defendre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_defendre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_defendre = &asnMission;
    CreatePoint( asnMission.point_a_defendre, "point_a_defendre" );
    MOS_ValueList* pParameters_preparer_terrain= &CreateValueList( "", 1, 2 );
    pParameters_preparer_terrain->CreateValue( asnMission.preparer_terrain, "preparer_terrain", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_INF_DetruireEmbuscade()
{
    ASN1T_Mission_Pion_INF_DetruireEmbuscade& asnMission = *new ASN1T_Mission_Pion_INF_DetruireEmbuscade();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_detruire_embuscade;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_detruire_embuscade = &asnMission;
    CreatePoint( asnMission.position_embuscade, "position_embuscade" );
    CreatePoint( asnMission.position_regroupement, "position_regroupement" );
    CreateAgentKnowledge( asnMission.ennemi, "ennemi" );
    MOS_ValueButtonList< ASN1T_EnumObjectType >* pSelector_nature_obstacle = &CreateVarList( asnMission.nature_obstacle,"nature_obstacle", 2, 3, false );
    pSelector_nature_obstacle->CreateValue( "bouchon_mines", EnumObjectType::bouchon_mines, true );
    pSelector_nature_obstacle->CreateValue( "zone_minee_lineaire", EnumObjectType::zone_minee_lineaire );
    pSelector_nature_obstacle->CreateValue( "zone_minee_par_dispersion", EnumObjectType::zone_minee_par_dispersion );
    pSelector_nature_obstacle->CreateValue( "fosse_anti_char", EnumObjectType::fosse_anti_char );
    pSelector_nature_obstacle->CreateValue( "abattis", EnumObjectType::abattis );
    pSelector_nature_obstacle->CreateValue( "barricade", EnumObjectType::barricade );
    pSelector_nature_obstacle->CreateValue( "eboulement", EnumObjectType::eboulement );
    pSelector_nature_obstacle->CreateValue( "destruction_route", EnumObjectType::destruction_route );
    pSelector_nature_obstacle->CreateValue( "destruction_pont", EnumObjectType::destruction_pont );
    pSelector_nature_obstacle->CreateValue( "pont_flottant_continu", EnumObjectType::pont_flottant_continu );
    pSelector_nature_obstacle->CreateValue( "poste_tir", EnumObjectType::poste_tir );
    pSelector_nature_obstacle->CreateValue( "zone_protegee", EnumObjectType::zone_protegee );
    pSelector_nature_obstacle->CreateValue( "zone_implantation_canon", EnumObjectType::zone_implantation_canon );
    pSelector_nature_obstacle->CreateValue( "zone_implantation_cobra", EnumObjectType::zone_implantation_cobra );
    pSelector_nature_obstacle->CreateValue( "zone_implantation_lrm", EnumObjectType::zone_implantation_lrm );
    pSelector_nature_obstacle->CreateValue( "site_franchissement", EnumObjectType::site_franchissement );
    pSelector_nature_obstacle->CreateValue( "nuage_nbc", EnumObjectType::nuage_nbc );
    pSelector_nature_obstacle->CreateValue( "plot_ravitaillement", EnumObjectType::plot_ravitaillement );
    pSelector_nature_obstacle->CreateValue( "site_decontamination", EnumObjectType::site_decontamination );
    pSelector_nature_obstacle->CreateValue( "zone_brouillage_brod", EnumObjectType::zone_brouillage_brod );
    pSelector_nature_obstacle->CreateValue( "rota", EnumObjectType::rota );
    pSelector_nature_obstacle->CreateValue( "zone_nbc", EnumObjectType::zone_nbc );
    pSelector_nature_obstacle->CreateValue( "zone_brouillage_bromure", EnumObjectType::zone_brouillage_bromure );
    pSelector_nature_obstacle->CreateValue( "aire_poser", EnumObjectType::aire_poser );
    pSelector_nature_obstacle->CreateValue( "piste", EnumObjectType::piste );
    pSelector_nature_obstacle->CreateValue( "plateforme", EnumObjectType::plateforme );
    pSelector_nature_obstacle->CreateValue( "zone_mobilite_amelioree", EnumObjectType::zone_mobilite_amelioree );
    pSelector_nature_obstacle->CreateValue( "zone_poser_helicoptere", EnumObjectType::zone_poser_helicoptere );
    pSelector_nature_obstacle->CreateValue( "aire_logistique", EnumObjectType::aire_logistique );
    pSelector_nature_obstacle->CreateValue( "itineraire_logistique", EnumObjectType::itineraire_logistique );
    pSelector_nature_obstacle->CreateValue( "camp_prisonniers", EnumObjectType::camp_prisonniers );
    pSelector_nature_obstacle->CreateValue( "camp_refugies", EnumObjectType::camp_refugies );
    pSelector_nature_obstacle->CreateValue( "poste_controle", EnumObjectType::poste_controle );
    pSelector_nature_obstacle->CreateValue( "terrain_largage", EnumObjectType::terrain_largage );
    pSelector_nature_obstacle->CreateValue( "zone_interdite_mouvement", EnumObjectType::zone_interdite_mouvement );
    pSelector_nature_obstacle->CreateValue( "zone_interdite_tir", EnumObjectType::zone_interdite_tir );
    pSelector_nature_obstacle->CreateValue( "zone_implantation_mortier", EnumObjectType::zone_implantation_mortier );
    pSelector_nature_obstacle->CreateValue( "pont_flottant_discontinu", EnumObjectType::pont_flottant_discontinu );
}

void MOS_MissionPion::CreateMission_INF_ContreAttaquer()
{
    ASN1T_Mission_Pion_INF_ContreAttaquer& asnMission = *new ASN1T_Mission_Pion_INF_ContreAttaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_contre_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_contre_attaquer = &asnMission;
    CreateAgentKnowledge( asnMission.ennemi, "ennemi" );
}

void MOS_MissionPion::CreateMission_INF_Freiner()
{
    ASN1T_Mission_Pion_INF_Freiner& asnMission = *new ASN1T_Mission_Pion_INF_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_freiner = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_INF_FlancGarder()
{
    ASN1T_Mission_Pion_INF_FlancGarder& asnMission = *new ASN1T_Mission_Pion_INF_FlancGarder();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_flanc_garder;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_flanc_garder = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
    CreateAgent( asnMission.unite_a_flancgarder, "unite_a_flancgarder" );
}

void MOS_MissionPion::CreateMission_INF_OrganiserUnCheckPoint()
{
    ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& asnMission = *new ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_organiser_un_check_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_organiser_un_check_point = &asnMission;
    CreatePoint( asnMission.point_installation, "point_installation" );
}

void MOS_MissionPion::CreateMission_INF_Harceler()
{
    ASN1T_Mission_Pion_INF_Harceler& asnMission = *new ASN1T_Mission_Pion_INF_Harceler();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_harceler;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_harceler = &asnMission;
    CreateLocation( asnMission.secteur, "secteur" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    MOS_ValueButtonList< ASN1T_EnumMissionInfNatureObjectif >* pSelector_nature_objectif = &CreateVarList( asnMission.nature_objectif,"nature_objectif", 2, 3, false );
    pSelector_nature_objectif->CreateValue( "elements_terrain", EnumMissionInfNatureObjectif::elements_terrain, true );
    pSelector_nature_objectif->CreateValue( "objets", EnumMissionInfNatureObjectif::objets );
    pSelector_nature_objectif->CreateValue( "elements_mobiles", EnumMissionInfNatureObjectif::elements_mobiles );
}

void MOS_MissionPion::CreateMission_INF_DetruireParCoupDeMain()
{
    ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& asnMission = *new ASN1T_Mission_Pion_INF_DetruireParCoupDeMain();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_detruire_par_coup_de_main = &asnMission;
    CreatePoint( asnMission.objectif, "objectif" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
    CreateGDH( asnMission.heure_destruction, "heure_destruction", MOSToolValueTitle | MOSToolValueTime | MOSToolValueSlide | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumMissionInfNatureObjectif >* pSelector_nature_destruction = &CreateVarList( asnMission.nature_destruction,"nature_destruction", 2, 3, false );
    pSelector_nature_destruction->CreateValue( "elements_terrain", EnumMissionInfNatureObjectif::elements_terrain, true );
    pSelector_nature_destruction->CreateValue( "objets", EnumMissionInfNatureObjectif::objets );
    pSelector_nature_destruction->CreateValue( "elements_mobiles", EnumMissionInfNatureObjectif::elements_mobiles );
}

void MOS_MissionPion::CreateMission_INF_EscorterUnConvoi()
{
    ASN1T_Mission_Pion_INF_EscorterUnConvoi& asnMission = *new ASN1T_Mission_Pion_INF_EscorterUnConvoi();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_escorter_un_convoi;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_escorter_un_convoi = &asnMission;
    CreateAgent( asnMission.convoi_a_escorter, "convoi_a_escorter" );
}

void MOS_MissionPion::CreateMission_INF_RenseignerSur()
{
    ASN1T_Mission_Pion_INF_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_INF_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_renseigner_sur = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_INF_Soutenir()
{
    ASN1T_Mission_Pion_INF_Soutenir& asnMission = *new ASN1T_Mission_Pion_INF_Soutenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_soutenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_soutenir = &asnMission;
    CreateAgent( asnMission.unite_a_soutenir, "unite_a_soutenir" );
}

void MOS_MissionPion::CreateMission_INF_Securiser()
{
    ASN1T_Mission_Pion_INF_Securiser& asnMission = *new ASN1T_Mission_Pion_INF_Securiser();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_securiser;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_securiser = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_INF_Recueillir()
{
    ASN1T_Mission_Pion_INF_Recueillir& asnMission = *new ASN1T_Mission_Pion_INF_Recueillir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_recueillir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_recueillir = &asnMission;
    CreatePoint( asnMission.position_installation, "position_installation" );
    CreatePoint( asnMission.pia, "pia" );
}

void MOS_MissionPion::CreateMission_INF_RepondreAAggression()
{
    ASN1T_Mission_Pion_INF_RepondreAAggression& asnMission = *new ASN1T_Mission_Pion_INF_RepondreAAggression();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_repondre_a_aggression;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_repondre_a_aggression = &asnMission;
    CreateAgentKnowledge( asnMission.aggresseur, "aggresseur" );
}

void MOS_MissionPion::CreateMission_LOG_SeDeployer()
{
    ASN1T_Mission_Pion_LOG_SeDeployer& asnMission = *new ASN1T_Mission_Pion_LOG_SeDeployer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_se_deployer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_se_deployer = &asnMission;
    CreatePoint( asnMission.position_deploiement, "position_deploiement" );
}

void MOS_MissionPion::CreateMission_LOG_DistribuerMoyens()
{
    ASN1T_Mission_Pion_LOG_DistribuerMoyens& asnMission = *new ASN1T_Mission_Pion_LOG_DistribuerMoyens();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_distribuer_moyens;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_distribuer_moyens = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_TrierBlesses()
{
    ASN1T_Mission_Pion_LOG_TrierBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TrierBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_trier_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_trier_blesses = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_TraiterBlesses()
{
    ASN1T_Mission_Pion_LOG_TraiterBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TraiterBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_traiter_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_traiter_blesses = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_TrierEtTraiterBlesses()
{
    ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_trier_et_traiter_blesses = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_SoignerBlesses()
{
    ASN1T_Mission_Pion_LOG_SoignerBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_SoignerBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_soigner_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_soigner_blesses = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_PrendreEnCompteReactionsMentales()
{
    ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& asnMission = *new ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_prendre_en_compte_reactions_mentales = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_DecontaminerBlesses()
{
    ASN1T_Mission_Pion_LOG_DecontaminerBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_DecontaminerBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_decontaminer_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_decontaminer_blesses = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_Reparer()
{
    ASN1T_Mission_Pion_LOG_Reparer& asnMission = *new ASN1T_Mission_Pion_LOG_Reparer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reparer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reparer = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_Evacuer()
{
    ASN1T_Mission_Pion_LOG_Evacuer& asnMission = *new ASN1T_Mission_Pion_LOG_Evacuer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_evacuer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_evacuer = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_Convoyer()
{
    ASN1T_Mission_Pion_LOG_Convoyer& asnMission = *new ASN1T_Mission_Pion_LOG_Convoyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_convoyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_convoyer = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_DistribuerRessources()
{
    ASN1T_Mission_Pion_LOG_DistribuerRessources& asnMission = *new ASN1T_Mission_Pion_LOG_DistribuerRessources();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_distribuer_ressources;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_distribuer_ressources = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_ConstituerConvoi()
{
    ASN1T_Mission_Pion_LOG_ConstituerConvoi& asnMission = *new ASN1T_Mission_Pion_LOG_ConstituerConvoi();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_constituer_convoi;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_constituer_convoi = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_Livrer()
{
    ASN1T_Mission_Pion_LOG_Livrer& asnMission = *new ASN1T_Mission_Pion_LOG_Livrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_livrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_livrer = &asnMission;
}

void MOS_MissionPion::CreateMission_LOG_PreparerBascule()
{
    ASN1T_Mission_Pion_LOG_PreparerBascule& asnMission = *new ASN1T_Mission_Pion_LOG_PreparerBascule();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_preparer_bascule;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_preparer_bascule = &asnMission;
    CreateAgent( asnMission.automate_pour_bascule, "automate_pour_bascule" );
}

void MOS_MissionPion::CreateMission_LOG_AppuyerMouvement()
{
    ASN1T_Mission_Pion_LOG_AppuyerMouvement& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerMouvement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_mouvement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement = &asnMission;
    CreateAgent( asnMission.unite_a_appuyer, "unite_a_appuyer" );
}

void MOS_MissionPion::CreateMission_LOG_ReconnaitreItineraire()
{
    ASN1T_Mission_Pion_LOG_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Pion_LOG_ReconnaitreItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reconnaitre_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_itineraire = &asnMission;
    CreatePath( asnMission.itineraire_log, "itineraire_log" );
}

void MOS_MissionPion::CreateMission_LOG_TransporterUniteChars()
{
    ASN1T_Mission_Pion_LOG_TransporterUniteChars& asnMission = *new ASN1T_Mission_Pion_LOG_TransporterUniteChars();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_transporter_unite_chars;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_transporter_unite_chars = &asnMission;
    CreateAgentList( asnMission.unites_a_transporter, "unites_a_transporter" );
    CreatePoint( asnMission.point_rendez_vous, "point_rendez_vous" );
    CreatePoint( asnMission.point_destination, "point_destination" );
}

void MOS_MissionPion::CreateMission_LOG_ReconnaitreZoneContaminee()
{
    ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee& asnMission = *new ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reconnaitre_zone_contaminee;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_zone_contaminee = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_LOG_AppuyerMouvementDansZone()
{
    ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_mouvement_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_LOG_MettreEnOeuvreZoneStationnement()
{
    ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement& asnMission = *new ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_mettre_en_oeuvre_zone_stationnement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_mettre_en_oeuvre_zone_stationnement = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_LOG_AppuyerFranchissementSurUnPoint()
{
    ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_franchissement_sur_un_point = &asnMission;
    CreateObjectKnowledge( asnMission.pont_flottant, "pont_flottant" );
}

void MOS_MissionPion::CreateMission_LOG_BloquerZone()
{
    ASN1T_Mission_Pion_LOG_BloquerZone& asnMission = *new ASN1T_Mission_Pion_LOG_BloquerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_bloquer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_bloquer_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_LOG_ReconnaitreZoneDeDeploiement()
{
    ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement& asnMission = *new ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reconnaitre_zone_de_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_zone_de_deploiement = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_LOG_ArmerPIA()
{
    ASN1T_Mission_Pion_LOG_ArmerPIA& asnMission = *new ASN1T_Mission_Pion_LOG_ArmerPIA();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_armer_pia;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_armer_pia = &asnMission;
    CreatePoint( asnMission.point, "point" );
}

void MOS_MissionPion::CreateMission_LOG_Surveiller()
{
    ASN1T_Mission_Pion_LOG_Surveiller& asnMission = *new ASN1T_Mission_Pion_LOG_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_Test_Heliporter()
{
    ASN1T_Mission_Pion_Test_Heliporter& asnMission = *new ASN1T_Mission_Pion_Test_Heliporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_heliporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_heliporter = &asnMission;
    CreateAgentList( asnMission.agents, "agents" );
    CreatePoint( asnMission.point_embarquement, "point_embarquement" );
    CreatePoint( asnMission.point_debarquement, "point_debarquement" );
}

void MOS_MissionPion::CreateMission_Test_MoveTo()
{
    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *new ASN1T_Mission_Pion_Test_MoveTo();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_move_to;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_move_to = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
    MOS_ValueButtonList< ASN1T_Mission_Pion_Test_MoveTo_type_itineraire >* pSelector_type_itineraire = &CreateVarList( asnMission.type_itineraire,"type_itineraire", 2, 3, false );
    pSelector_type_itineraire->CreateValue( "movement", Mission_Pion_Test_MoveTo_type_itineraire::movement, true );
    pSelector_type_itineraire->CreateValue( "recon", Mission_Pion_Test_MoveTo_type_itineraire::recon );
    pSelector_type_itineraire->CreateValue( "infiltration", Mission_Pion_Test_MoveTo_type_itineraire::infiltration );
    pSelector_type_itineraire->CreateValue( "assault", Mission_Pion_Test_MoveTo_type_itineraire::assault );
    pSelector_type_itineraire->CreateValue( "retreat", Mission_Pion_Test_MoveTo_type_itineraire::retreat );
    pSelector_type_itineraire->CreateValue( "backup", Mission_Pion_Test_MoveTo_type_itineraire::backup );
    pSelector_type_itineraire->CreateValue( "mine_clearance", Mission_Pion_Test_MoveTo_type_itineraire::mine_clearance );
    MOS_ValueList* pParameters_debarque= &CreateValueList( "", 1, 2 );
    pParameters_debarque->CreateValue( asnMission.debarque, "debarque", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision >* pSelector_verrouillage_vision = &CreateVarList( asnMission.verrouillage_vision,"verrouillage_vision", 2, 3, false );
    pSelector_verrouillage_vision->CreateValue( "rien", Mission_Pion_Test_MoveTo_verrouillage_vision::rien, true );
    pSelector_verrouillage_vision->CreateValue( "point", Mission_Pion_Test_MoveTo_verrouillage_vision::point );
    pSelector_verrouillage_vision->CreateValue( "direction", Mission_Pion_Test_MoveTo_verrouillage_vision::direction );
    CreatePoint( asnMission.vision_point, "vision_point" );
    CreateDirection( asnMission.vision_direction, "vision_direction" );
}

void MOS_MissionPion::CreateMission_Test_CreateObject()
{
    ASN1T_Mission_Pion_Test_CreateObject& asnMission = *new ASN1T_Mission_Pion_Test_CreateObject();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_create_object;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_create_object = &asnMission;
    CreateLocation( asnMission.forme, "forme" );
    MOS_ValueButtonList< ASN1T_EnumObjectType >* pSelector_type = &CreateVarList( asnMission.type,"type", 2, 3, false );
    pSelector_type->CreateValue( "bouchon_mines", EnumObjectType::bouchon_mines, true );
    pSelector_type->CreateValue( "zone_minee_lineaire", EnumObjectType::zone_minee_lineaire );
    pSelector_type->CreateValue( "zone_minee_par_dispersion", EnumObjectType::zone_minee_par_dispersion );
    pSelector_type->CreateValue( "fosse_anti_char", EnumObjectType::fosse_anti_char );
    pSelector_type->CreateValue( "abattis", EnumObjectType::abattis );
    pSelector_type->CreateValue( "barricade", EnumObjectType::barricade );
    pSelector_type->CreateValue( "eboulement", EnumObjectType::eboulement );
    pSelector_type->CreateValue( "destruction_route", EnumObjectType::destruction_route );
    pSelector_type->CreateValue( "destruction_pont", EnumObjectType::destruction_pont );
    pSelector_type->CreateValue( "pont_flottant_continu", EnumObjectType::pont_flottant_continu );
    pSelector_type->CreateValue( "poste_tir", EnumObjectType::poste_tir );
    pSelector_type->CreateValue( "zone_protegee", EnumObjectType::zone_protegee );
    pSelector_type->CreateValue( "zone_implantation_canon", EnumObjectType::zone_implantation_canon );
    pSelector_type->CreateValue( "zone_implantation_cobra", EnumObjectType::zone_implantation_cobra );
    pSelector_type->CreateValue( "zone_implantation_lrm", EnumObjectType::zone_implantation_lrm );
    pSelector_type->CreateValue( "site_franchissement", EnumObjectType::site_franchissement );
    pSelector_type->CreateValue( "nuage_nbc", EnumObjectType::nuage_nbc );
    pSelector_type->CreateValue( "plot_ravitaillement", EnumObjectType::plot_ravitaillement );
    pSelector_type->CreateValue( "site_decontamination", EnumObjectType::site_decontamination );
    pSelector_type->CreateValue( "zone_brouillage_brod", EnumObjectType::zone_brouillage_brod );
    pSelector_type->CreateValue( "rota", EnumObjectType::rota );
    pSelector_type->CreateValue( "zone_nbc", EnumObjectType::zone_nbc );
    pSelector_type->CreateValue( "zone_brouillage_bromure", EnumObjectType::zone_brouillage_bromure );
    pSelector_type->CreateValue( "aire_poser", EnumObjectType::aire_poser );
    pSelector_type->CreateValue( "piste", EnumObjectType::piste );
    pSelector_type->CreateValue( "plateforme", EnumObjectType::plateforme );
    pSelector_type->CreateValue( "zone_mobilite_amelioree", EnumObjectType::zone_mobilite_amelioree );
    pSelector_type->CreateValue( "zone_poser_helicoptere", EnumObjectType::zone_poser_helicoptere );
    pSelector_type->CreateValue( "aire_logistique", EnumObjectType::aire_logistique );
    pSelector_type->CreateValue( "itineraire_logistique", EnumObjectType::itineraire_logistique );
    pSelector_type->CreateValue( "camp_prisonniers", EnumObjectType::camp_prisonniers );
    pSelector_type->CreateValue( "camp_refugies", EnumObjectType::camp_refugies );
    pSelector_type->CreateValue( "poste_controle", EnumObjectType::poste_controle );
    pSelector_type->CreateValue( "terrain_largage", EnumObjectType::terrain_largage );
    pSelector_type->CreateValue( "zone_interdite_mouvement", EnumObjectType::zone_interdite_mouvement );
    pSelector_type->CreateValue( "zone_interdite_tir", EnumObjectType::zone_interdite_tir );
    pSelector_type->CreateValue( "zone_implantation_mortier", EnumObjectType::zone_implantation_mortier );
    pSelector_type->CreateValue( "pont_flottant_discontinu", EnumObjectType::pont_flottant_discontinu );
    MOS_ValueList* pParameters_param= &CreateValueList( "", 1, 2 );
    pParameters_param->CreateValue( asnMission.param, "param", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_Test_DestroyObject()
{
    ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *new ASN1T_Mission_Pion_Test_DestroyObject();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_destroy_object;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_destroy_object = &asnMission;
    CreateObjectKnowledge( asnMission.objet, "objet" );
}

void MOS_MissionPion::CreateMission_Test_CreateBypass()
{
    ASN1T_Mission_Pion_Test_CreateBypass& asnMission = *new ASN1T_Mission_Pion_Test_CreateBypass();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_create_bypass;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_create_bypass = &asnMission;
    CreateObjectKnowledge( asnMission.objet, "objet" );
}

void MOS_MissionPion::CreateMission_Test_Fire()
{
    ASN1T_Mission_Pion_Test_Fire& asnMission = *new ASN1T_Mission_Pion_Test_Fire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_fire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_fire = &asnMission;
    CreatePoint( asnMission.point, "point" );
    MOS_ValueList* pParameters_tir_indirect= &CreateValueList( "", 1, 2 );
    pParameters_tir_indirect->CreateValue( asnMission.tir_indirect, "tir_indirect", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_Mission_Pion_Test_Fire_munitions >* pSelector_munitions = &CreateVarList( asnMission.munitions,"munitions", 2, 3, false );
    pSelector_munitions->CreateValue( "mun_obus_explosif", Mission_Pion_Test_Fire_munitions::mun_obus_explosif, true );
    pSelector_munitions->CreateValue( "mun_obus_grenade", Mission_Pion_Test_Fire_munitions::mun_obus_grenade );
    pSelector_munitions->CreateValue( "mun_obus_aced", Mission_Pion_Test_Fire_munitions::mun_obus_aced );
    pSelector_munitions->CreateValue( "mun_obus_fumigene", Mission_Pion_Test_Fire_munitions::mun_obus_fumigene );
    pSelector_munitions->CreateValue( "mun_obus_eclairant", Mission_Pion_Test_Fire_munitions::mun_obus_eclairant );
    pSelector_munitions->CreateValue( "mun_obus_mine", Mission_Pion_Test_Fire_munitions::mun_obus_mine );
    MOS_ValueList* pParameters_nb_obus= &CreateValueList( "", 1, 2 );
    pParameters_nb_obus->CreateValue( asnMission.nb_obus, "nb_obus", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
    CreateAgentKnowledgeList( asnMission.targets, "targets" );
}

void MOS_MissionPion::CreateMission_Test_ChangePosture()
{
    ASN1T_Mission_Pion_Test_ChangePosture& asnMission = *new ASN1T_Mission_Pion_Test_ChangePosture();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_change_posture;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_change_posture = &asnMission;
    MOS_ValueButtonList< ASN1T_Mission_Pion_Test_ChangePosture_posture >* pSelector_posture = &CreateVarList( asnMission.posture,"posture", 2, 3, false );
    pSelector_posture->CreateValue( "mouvement", Mission_Pion_Test_ChangePosture_posture::mouvement, true );
    pSelector_posture->CreateValue( "arret", Mission_Pion_Test_ChangePosture_posture::arret );
    pSelector_posture->CreateValue( "poste_reflexe", Mission_Pion_Test_ChangePosture_posture::poste_reflexe );
    pSelector_posture->CreateValue( "poste", Mission_Pion_Test_ChangePosture_posture::poste );
    pSelector_posture->CreateValue( "poste_amenage", Mission_Pion_Test_ChangePosture_posture::poste_amenage );
    pSelector_posture->CreateValue( "poste_prepare_genie", Mission_Pion_Test_ChangePosture_posture::poste_prepare_genie );
    pSelector_posture->CreateValue( "automatique", Mission_Pion_Test_ChangePosture_posture::automatique );
}

void MOS_MissionPion::CreateMission_Test_PathFind()
{
    ASN1T_Mission_Pion_Test_PathFind& asnMission = *new ASN1T_Mission_Pion_Test_PathFind();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_path_find;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_path_find = &asnMission;
    MOS_ValueList* pParameters_nb_pathfind= &CreateValueList( "", 1, 2 );
    pParameters_nb_pathfind->CreateValue( asnMission.nb_pathfind, "nb_pathfind", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_Test_FindPosition()
{
    ASN1T_Mission_Pion_Test_FindPosition& asnMission = *new ASN1T_Mission_Pion_Test_FindPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_find_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_find_position = &asnMission;
    CreatePoint( asnMission.point, "point" );
    CreatePoint( asnMission.retreat, "retreat" );
    CreateAgentList( asnMission.unite, "unite" );
    MOS_ValueButtonList< ASN1T_Mission_Pion_Test_FindPosition_type_objectif >* pSelector_type_objectif = &CreateVarList( asnMission.type_objectif,"type_objectif", 2, 3, false );
    pSelector_type_objectif->CreateValue( "agent", Mission_Pion_Test_FindPosition_type_objectif::agent, true );
    pSelector_type_objectif->CreateValue( "point", Mission_Pion_Test_FindPosition_type_objectif::point );
    pSelector_type_objectif->CreateValue( "rep_point", Mission_Pion_Test_FindPosition_type_objectif::rep_point );
    pSelector_type_objectif->CreateValue( "objet", Mission_Pion_Test_FindPosition_type_objectif::objet );
    pSelector_type_objectif->CreateValue( "zone", Mission_Pion_Test_FindPosition_type_objectif::zone );
    MOS_ValueButtonList< ASN1T_Mission_Pion_Test_FindPosition_type_operation >* pSelector_type_operation = &CreateVarList( asnMission.type_operation,"type_operation", 2, 3, false );
    pSelector_type_operation->CreateValue( "see", Mission_Pion_Test_FindPosition_type_operation::see, true );
    pSelector_type_operation->CreateValue( "shoot", Mission_Pion_Test_FindPosition_type_operation::shoot );
    pSelector_type_operation->CreateValue( "add", Mission_Pion_Test_FindPosition_type_operation::add );
    pSelector_type_operation->CreateValue( "rem", Mission_Pion_Test_FindPosition_type_operation::rem );
    pSelector_type_operation->CreateValue( "safety", Mission_Pion_Test_FindPosition_type_operation::safety );
    pSelector_type_operation->CreateValue( "border", Mission_Pion_Test_FindPosition_type_operation::border );
    pSelector_type_operation->CreateValue( "appui", Mission_Pion_Test_FindPosition_type_operation::appui );
    pSelector_type_operation->CreateValue( "support", Mission_Pion_Test_FindPosition_type_operation::support );
    pSelector_type_operation->CreateValue( "ambush", Mission_Pion_Test_FindPosition_type_operation::ambush );
    pSelector_type_operation->CreateValue( "random", Mission_Pion_Test_FindPosition_type_operation::random );
    MOS_ValueList* pParameters_avoid= &CreateValueList( "", 1, 2 );
    pParameters_avoid->CreateValue( asnMission.avoid, "avoid", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueList* pParameters_reco= &CreateValueList( "", 1, 2 );
    pParameters_reco->CreateValue( asnMission.reco, "reco", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueList* pParameters_no_obstacle_avoid= &CreateValueList( "", 1, 2 );
    pParameters_no_obstacle_avoid->CreateValue( asnMission.no_obstacle_avoid, "no_obstacle_avoid", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueList* pParameters_debordement= &CreateValueList( "", 1, 2 );
    pParameters_debordement->CreateValue( asnMission.debordement, "debordement", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueList* pParameters_dec_points= &CreateValueList( "", 1, 2 );
    pParameters_dec_points->CreateValue( asnMission.dec_points, "dec_points", MOSToolValueCheck | MOSToolValueModif );
    MOS_ValueButtonList< ASN1T_EnumUnitIdentificationLevel >* pSelector_niveau_identification = &CreateVarList( asnMission.niveau_identification,"niveau_identification", 2, 3, false );
    pSelector_niveau_identification->CreateValue( "identifiee", EnumUnitIdentificationLevel::identifiee, true );
    pSelector_niveau_identification->CreateValue( "reconnue", EnumUnitIdentificationLevel::reconnue );
    pSelector_niveau_identification->CreateValue( "detectee", EnumUnitIdentificationLevel::detectee );
    pSelector_niveau_identification->CreateValue( "signale", EnumUnitIdentificationLevel::signale );
    CreateDirection( asnMission.dgo, "dgo" );
}

void MOS_MissionPion::CreateMission_Test_Reinforce()
{
    ASN1T_Mission_Pion_Test_Reinforce& asnMission = *new ASN1T_Mission_Pion_Test_Reinforce();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_reinforce;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_reinforce = &asnMission;
    CreateAgent( asnMission.unite, "unite" );
    MOS_ValueList* pParameters_renforce= &CreateValueList( "", 1, 2 );
    pParameters_renforce->CreateValue( asnMission.renforce, "renforce", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_PlastronEnnemi()
{
    ASN1T_Mission_Pion_PlastronEnnemi& asnMission = *new ASN1T_Mission_Pion_PlastronEnnemi();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_plastron_ennemi;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_plastron_ennemi = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
    CreatePoint( asnMission.point_mission, "point_mission" );
    MOS_ValueButtonList< ASN1T_Mission_Pion_PlastronEnnemi_type >* pSelector_type = &CreateVarList( asnMission.type,"type", 2, 3, false );
    pSelector_type->CreateValue( "defense_statique", Mission_Pion_PlastronEnnemi_type::defense_statique, true );
    pSelector_type->CreateValue( "defense_dynamique", Mission_Pion_PlastronEnnemi_type::defense_dynamique );
    pSelector_type->CreateValue( "esquive", Mission_Pion_PlastronEnnemi_type::esquive );
    pSelector_type->CreateValue( "reco_offensive", Mission_Pion_PlastronEnnemi_type::reco_offensive );
}

void MOS_MissionPion::CreateMission_FaireMouvement()
{
    ASN1T_Mission_Pion_FaireMouvement& asnMission = *new ASN1T_Mission_Pion_FaireMouvement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_faire_mouvement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_faire_mouvement = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_Suivre()
{
    ASN1T_Mission_Pion_Suivre& asnMission = *new ASN1T_Mission_Pion_Suivre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_suivre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_suivre = &asnMission;
    CreateAgent( asnMission.unite_a_suivre, "unite_a_suivre" );
}

void MOS_MissionPion::CreateMission_SeFaireDecontaminer()
{
    ASN1T_Mission_Pion_SeFaireDecontaminer& asnMission = *new ASN1T_Mission_Pion_SeFaireDecontaminer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_se_faire_decontaminer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_se_faire_decontaminer = &asnMission;
    CreatePoint( asnMission.point_arrivee, "point_arrivee" );
    CreateObjectKnowledge( asnMission.site_decontamination, "site_decontamination" );
}

void MOS_MissionPion::CreateMission_Franchir()
{
    ASN1T_Mission_Pion_Franchir& asnMission = *new ASN1T_Mission_Pion_Franchir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_franchir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_franchir = &asnMission;
    CreateObjectKnowledge( asnMission.pont_flottant, "pont_flottant" );
    CreatePoint( asnMission.point_arrivee, "point_arrivee" );
}

void MOS_MissionPion::CreateMission_Decrocher()
{
    ASN1T_Mission_Pion_Decrocher& asnMission = *new ASN1T_Mission_Pion_Decrocher();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_decrocher;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_decrocher = &asnMission;
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
}

void MOS_MissionPion::CreateMission_Stationner()
{
    ASN1T_Mission_Pion_Stationner& asnMission = *new ASN1T_Mission_Pion_Stationner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_stationner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_stationner = &asnMission;
    CreatePoint( asnMission.point_attente, "point_attente" );
}

void MOS_MissionPion::CreateMission_SeFaireTransporter()
{
    ASN1T_Mission_Pion_SeFaireTransporter& asnMission = *new ASN1T_Mission_Pion_SeFaireTransporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_se_faire_transporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_se_faire_transporter = &asnMission;
    CreatePoint( asnMission.point_embarquement, "point_embarquement" );
}

void MOS_MissionPion::CreateMission_Generique()
{
    ASN1T_Mission_Pion_Generique& asnMission = *new ASN1T_Mission_Pion_Generique();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_generique;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_generique = &asnMission;
    CreatePolygonList( asnMission.zones, "zones" );
    CreatePointList( asnMission.points, "points" );
    CreatePathList( asnMission.itineraires, "itineraires" );
    CreateAgentList( asnMission.unites, "unites" );
    CreateAutomateList( asnMission.automates, "automates" );
    CreateAgentKnowledgeList( asnMission.connaissances_agents, "connaissances_agents" );
    CreateObjectKnowledgeList( asnMission.connaissances_objets, "connaissances_objets" );
}

void MOS_MissionPion::CreateMission_MILICE_Provoquer()
{
    ASN1T_Mission_Pion_MILICE_Provoquer& asnMission = *new ASN1T_Mission_Pion_MILICE_Provoquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_milice_provoquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_provoquer = &asnMission;
    CreateAgentKnowledge( asnMission.unite_a_provoquer, "unite_a_provoquer" );
}

void MOS_MissionPion::CreateMission_MILICE_PrendreEtTenir()
{
    ASN1T_Mission_Pion_MILICE_PrendreEtTenir& asnMission = *new ASN1T_Mission_Pion_MILICE_PrendreEtTenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_milice_prendre_et_tenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_prendre_et_tenir = &asnMission;
    CreatePoint( asnMission.point_a_prendre, "point_a_prendre" );
}

void MOS_MissionPion::CreateMission_MILICE_DetruireEmbuscade()
{
    ASN1T_Mission_Pion_MILICE_DetruireEmbuscade& asnMission = *new ASN1T_Mission_Pion_MILICE_DetruireEmbuscade();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_milice_detruire_embuscade;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_detruire_embuscade = &asnMission;
    CreatePoint( asnMission.position_embuscade, "position_embuscade" );
    CreatePoint( asnMission.position_regroupement, "position_regroupement" );
    MOS_ValueButtonList< ASN1T_EnumObjectType >* pSelector_nature_obstacle = &CreateVarList( asnMission.nature_obstacle,"nature_obstacle", 2, 3, false );
    pSelector_nature_obstacle->CreateValue( "bouchon_mines", EnumObjectType::bouchon_mines, true );
    pSelector_nature_obstacle->CreateValue( "zone_minee_lineaire", EnumObjectType::zone_minee_lineaire );
    pSelector_nature_obstacle->CreateValue( "zone_minee_par_dispersion", EnumObjectType::zone_minee_par_dispersion );
    pSelector_nature_obstacle->CreateValue( "fosse_anti_char", EnumObjectType::fosse_anti_char );
    pSelector_nature_obstacle->CreateValue( "abattis", EnumObjectType::abattis );
    pSelector_nature_obstacle->CreateValue( "barricade", EnumObjectType::barricade );
    pSelector_nature_obstacle->CreateValue( "eboulement", EnumObjectType::eboulement );
    pSelector_nature_obstacle->CreateValue( "destruction_route", EnumObjectType::destruction_route );
    pSelector_nature_obstacle->CreateValue( "destruction_pont", EnumObjectType::destruction_pont );
    pSelector_nature_obstacle->CreateValue( "pont_flottant_continu", EnumObjectType::pont_flottant_continu );
    pSelector_nature_obstacle->CreateValue( "poste_tir", EnumObjectType::poste_tir );
    pSelector_nature_obstacle->CreateValue( "zone_protegee", EnumObjectType::zone_protegee );
    pSelector_nature_obstacle->CreateValue( "zone_implantation_canon", EnumObjectType::zone_implantation_canon );
    pSelector_nature_obstacle->CreateValue( "zone_implantation_cobra", EnumObjectType::zone_implantation_cobra );
    pSelector_nature_obstacle->CreateValue( "zone_implantation_lrm", EnumObjectType::zone_implantation_lrm );
    pSelector_nature_obstacle->CreateValue( "site_franchissement", EnumObjectType::site_franchissement );
    pSelector_nature_obstacle->CreateValue( "nuage_nbc", EnumObjectType::nuage_nbc );
    pSelector_nature_obstacle->CreateValue( "plot_ravitaillement", EnumObjectType::plot_ravitaillement );
    pSelector_nature_obstacle->CreateValue( "site_decontamination", EnumObjectType::site_decontamination );
    pSelector_nature_obstacle->CreateValue( "zone_brouillage_brod", EnumObjectType::zone_brouillage_brod );
    pSelector_nature_obstacle->CreateValue( "rota", EnumObjectType::rota );
    pSelector_nature_obstacle->CreateValue( "zone_nbc", EnumObjectType::zone_nbc );
    pSelector_nature_obstacle->CreateValue( "zone_brouillage_bromure", EnumObjectType::zone_brouillage_bromure );
    pSelector_nature_obstacle->CreateValue( "aire_poser", EnumObjectType::aire_poser );
    pSelector_nature_obstacle->CreateValue( "piste", EnumObjectType::piste );
    pSelector_nature_obstacle->CreateValue( "plateforme", EnumObjectType::plateforme );
    pSelector_nature_obstacle->CreateValue( "zone_mobilite_amelioree", EnumObjectType::zone_mobilite_amelioree );
    pSelector_nature_obstacle->CreateValue( "zone_poser_helicoptere", EnumObjectType::zone_poser_helicoptere );
    pSelector_nature_obstacle->CreateValue( "aire_logistique", EnumObjectType::aire_logistique );
    pSelector_nature_obstacle->CreateValue( "itineraire_logistique", EnumObjectType::itineraire_logistique );
    pSelector_nature_obstacle->CreateValue( "camp_prisonniers", EnumObjectType::camp_prisonniers );
    pSelector_nature_obstacle->CreateValue( "camp_refugies", EnumObjectType::camp_refugies );
    pSelector_nature_obstacle->CreateValue( "poste_controle", EnumObjectType::poste_controle );
    pSelector_nature_obstacle->CreateValue( "terrain_largage", EnumObjectType::terrain_largage );
    pSelector_nature_obstacle->CreateValue( "zone_interdite_mouvement", EnumObjectType::zone_interdite_mouvement );
    pSelector_nature_obstacle->CreateValue( "zone_interdite_tir", EnumObjectType::zone_interdite_tir );
    pSelector_nature_obstacle->CreateValue( "zone_implantation_mortier", EnumObjectType::zone_implantation_mortier );
    pSelector_nature_obstacle->CreateValue( "pont_flottant_discontinu", EnumObjectType::pont_flottant_discontinu );
}

void MOS_MissionPion::CreateMission_JOINT_MARINE_Transporter()
{
    ASN1T_Mission_Pion_JOINT_MARINE_Transporter& asnMission = *new ASN1T_Mission_Pion_JOINT_MARINE_Transporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_joint_marine_transporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_joint_marine_transporter = &asnMission;
    CreateAgentList( asnMission.unites_a_transporter, "unites_a_transporter" );
    CreatePoint( asnMission.point_debarquement, "point_debarquement" );
    CreatePoint( asnMission.point_embarquement, "point_embarquement" );
    MOS_ValueList* pParameters_avec_materiel= &CreateValueList( "", 1, 2 );
    pParameters_avec_materiel->CreateValue( asnMission.avec_materiel, "avec_materiel", MOSToolValueCheck | MOSToolValueModif );
}

void MOS_MissionPion::CreateMission_JOINT_AIR_EffectuerRaid()
{
    ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid& asnMission = *new ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_joint_air_effectuer_raid;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_joint_air_effectuer_raid = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_NBC_AnimerUnPlotDeDecontamination()
{
    ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination& asnMission = *new ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_animer_un_plot_de_decontamination;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_animer_un_plot_de_decontamination = &asnMission;
    CreatePoint( asnMission.position_site_decontamination, "position_site_decontamination" );
}

void MOS_MissionPion::CreateMission_NBC_ReconnaitreItineraire()
{
    ASN1T_Mission_Pion_NBC_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_reconnaitre_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "itineraire" );
}

void MOS_MissionPion::CreateMission_NBC_ReconnaitreZone()
{
    ASN1T_Mission_Pion_NBC_ReconnaitreZone& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_reconnaitre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_NBC_DecontaminerZone()
{
    ASN1T_Mission_Pion_NBC_DecontaminerZone& asnMission = *new ASN1T_Mission_Pion_NBC_DecontaminerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_decontaminer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_decontaminer_zone = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_NBC_ReconnaitreUnSiteROTA()
{
    ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_un_site_rota = &asnMission;
    CreateObjectKnowledge( asnMission.site_ROTA, "site_ROTA" );
}

void MOS_MissionPion::CreateMission_RENS_ROHUM_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_ROHUM_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROHUM_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_rohum_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_renseigner_sur = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "zone_a_observer" );
}

void MOS_MissionPion::CreateMission_RENS_ROHUM_SExfiltrer()
{
    ASN1T_Mission_Pion_RENS_ROHUM_SExfiltrer& asnMission = *new ASN1T_Mission_Pion_RENS_ROHUM_SExfiltrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_rohum_sexfiltrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_sexfiltrer = &asnMission;
    CreatePoint( asnMission.point_de_regroupement, "point_de_regroupement" );
}

void MOS_MissionPion::CreateMission_RENS_ROHUM_OrienterGuider()
{
    ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider& asnMission = *new ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_rohum_orienter_guider;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_orienter_guider = &asnMission;
    CreateAgent( asnMission.ami, "ami" );
}

void MOS_MissionPion::CreateMission_RENS_ROEM_Appuyer()
{
    ASN1T_Mission_Pion_RENS_ROEM_Appuyer& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_appuyer = &asnMission;
    CreatePoint( asnMission.position_deploiement, "position_deploiement" );
    CreatePoint( asnMission.point_regroupement, "point_regroupement" );
}

void MOS_MissionPion::CreateMission_RENS_ROEM_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_ROEM_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_renseigner_sur = &asnMission;
    CreateLocation( asnMission.zone, "zone" );
}

void MOS_MissionPion::CreateMission_RENS_ROEM_MettreEnOeuvre()
{
    ASN1T_Mission_Pion_RENS_ROEM_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_MettreEnOeuvre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_mettre_en_oeuvre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_mettre_en_oeuvre = &asnMission;
    CreatePoint( asnMission.position_deploiement, "position_deploiement" );
}

void MOS_MissionPion::CreateMission_RENS_RADINT_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_radint_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_radint_renseigner_sur = &asnMission;
    CreatePolygonList( asnMission.zones_de_recherche, "zones_de_recherche" );
}

void MOS_MissionPion::CreateMission_RENS_RADINT_MettreEnOeuvre()
{
    ASN1T_Mission_Pion_RENS_RADINT_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_RADINT_MettreEnOeuvre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_radint_mettre_en_oeuvre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_radint_mettre_en_oeuvre = &asnMission;
    CreatePoint( asnMission.position_deploiement, "position_deploiement" );
}

void MOS_MissionPion::CreateMission_RENS_ROIM_MettreEnOeuvre()
{
    ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roim_mettre_en_oeuvre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_mettre_en_oeuvre = &asnMission;
    CreatePoint( asnMission.position_deploiement, "position_deploiement" );
}

void MOS_MissionPion::CreateMission_RENS_ROIM_SDTI_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roim_sdti_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_sdti_renseigner_sur = &asnMission;
    CreatePath( asnMission.ficelle_de_vole, "ficelle_de_vole" );
}

void MOS_MissionPion::CreateMission_RENS_ROIM_CL289_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_ROIM_CL289_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROIM_CL289_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roim_cl289_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_cl289_renseigner_sur = &asnMission;
    CreatePath( asnMission.ficelle_de_vole, "ficelle_de_vole" );
    CreatePolygonList( asnMission.zones_de_recherche, "zones_de_recherche" );
}

void MOS_MissionPion::CreateMission_TRANS_EtablirGererLiaison()
{
    ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& asnMission = *new ASN1T_Mission_Pion_TRANS_EtablirGererLiaison();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_trans_etablir_gerer_liaison = &asnMission;
    CreatePoint( asnMission.position_deploiement, "position_deploiement" );
}



// -----------------------------------------------------------------------------
// Name: MOS_MissionPion::GetName()
// Created: SBO 2005-08-01
// -----------------------------------------------------------------------------
std::string MOS_MissionPion::GetName() const
{
    return ENT_Tr::ConvertFromUnitMission( ( E_UnitMission )nTypeMission_ );
}
