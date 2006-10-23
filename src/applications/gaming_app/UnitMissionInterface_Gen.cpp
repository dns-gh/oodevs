// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: UnitMissionInterface destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
UnitMissionInterface::~UnitMissionInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().mission.t )
    {
        case T_Mission_Pion_mission_pion_alat_aller_se_recompleter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_aller_se_recompleter; break;
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
        case T_Mission_Pion_mission_pion_abc_escorter_unite : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_escorter_unite; break;
        case T_Mission_Pion_mission_pion_abc_escorter_unites : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_escorter_unites; break;
        case T_Mission_Pion_mission_pion_abc_armer_point_de_controle : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_armer_point_de_controle; break;
        case T_Mission_Pion_mission_pion_abc_organiser_accueil_colonne_refugies : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_organiser_accueil_colonne_refugies; break;
        case T_Mission_Pion_mission_pion_abc_appuyer_sur_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_appuyer_sur_position; break;
        case T_Mission_Pion_mission_pion_abc_rasit_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_rasit_renseigner_sur; break;
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
        case T_Mission_Pion_mission_pion_inf_escorter_unite : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_escorter_unite; break;
        case T_Mission_Pion_mission_pion_inf_escorter_unites : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_escorter_unites; break;
        case T_Mission_Pion_mission_pion_inf_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_renseigner_sur; break;
        case T_Mission_Pion_mission_pion_inf_soutenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_soutenir; break;
        case T_Mission_Pion_mission_pion_inf_securiser : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_securiser; break;
        case T_Mission_Pion_mission_pion_inf_recueillir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_recueillir; break;
        case T_Mission_Pion_mission_pion_inf_repondre_a_aggression : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_repondre_a_aggression; break;
        case T_Mission_Pion_mission_pion_inf_appuyer_sur_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_appuyer_sur_position; break;
        case T_Mission_Pion_mission_pion_log_se_deployer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_se_deployer; break;
        case T_Mission_Pion_mission_pion_log_renforcer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_renforcer; break;
        case T_Mission_Pion_mission_pion_log_preparer_bascule : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_preparer_bascule; break;
        case T_Mission_Pion_mission_pion_log_decontaminer_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_decontaminer_blesses; break;
        case T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_trier_et_traiter_blesses; break;
        case T_Mission_Pion_mission_pion_log_trier_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_trier_blesses; break;
        case T_Mission_Pion_mission_pion_log_traiter_blesses : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_traiter_blesses; break;
        case T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_prendre_en_compte_reactions_mentales; break;
        case T_Mission_Pion_mission_pion_log_convoyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_convoyer; break;
        case T_Mission_Pion_mission_pion_log_reparer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reparer; break;
        case T_Mission_Pion_mission_pion_log_appuyer_mouvement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement; break;
        case T_Mission_Pion_mission_pion_log_reconnaitre_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_itineraire; break;
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
        case T_Mission_Pion_mission_pion_transporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_transporter; break;
        case T_Mission_Pion_mission_pion_se_faire_decontaminer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_se_faire_decontaminer; break;
        case T_Mission_Pion_mission_pion_franchir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_franchir; break;
        case T_Mission_Pion_mission_pion_decrocher : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_decrocher; break;
        case T_Mission_Pion_mission_pion_stationner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_stationner; break;
        case T_Mission_Pion_mission_pion_se_faire_transporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_se_faire_transporter; break;
        case T_Mission_Pion_mission_pion_interdire_franchissement_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_interdire_franchissement_populations; break;
        case T_Mission_Pion_mission_pion_filtrer_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_filtrer_populations; break;
        case T_Mission_Pion_mission_pion_renseigner_sur_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_renseigner_sur_populations; break;
        case T_Mission_Pion_mission_pion_canaliser_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_canaliser_populations; break;
        case T_Mission_Pion_mission_pion_controler_populations_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_controler_populations_dans_zone; break;
        case T_Mission_Pion_mission_pion_securiser_zone_contre_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_securiser_zone_contre_populations; break;
        case T_Mission_Pion_mission_pion_generique : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_generique; break;
        case T_Mission_Pion_mission_pion_milice_provoquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_provoquer; break;
        case T_Mission_Pion_mission_pion_milice_prendre_et_tenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_prendre_et_tenir; break;
        case T_Mission_Pion_mission_pion_milice_detruire_embuscade : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_detruire_embuscade; break;
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
        case T_Mission_Pion_mission_pion_asy_sinfiltrer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_sinfiltrer; break;
        case T_Mission_Pion_mission_pion_asy_commettre_attentatContrePopulation : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContrePopulation; break;
        case T_Mission_Pion_mission_pion_asy_commettre_attentatContreForcesArmees : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContreForcesArmees; break;
        case T_Mission_Pion_mission_pion_asy_commettre_attentatContreInstallation : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContreInstallation; break;
        case T_Mission_Pion_mission_pion_asy_commettre_exactions_sur_population_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_commettre_exactions_sur_population_dans_zone; break;
        case T_Mission_Pion_mission_pion_asy_renseigner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_renseigner; break;

        default:
            assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateInterface
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateInterface()
{
    switch( nMissionId_ )
    {
        case eMission_Pion_ALAT_AllerSeRecompleter : CreateMission_ALAT_AllerSeRecompleter(); break;
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
        case eMission_Pion_ABC_EscorterUnite : CreateMission_ABC_EscorterUnite(); break;
        case eMission_Pion_ABC_EscorterUnites : CreateMission_ABC_EscorterUnites(); break;
        case eMission_Pion_ABC_ArmerPointDeControle : CreateMission_ABC_ArmerPointDeControle(); break;
        case eMission_Pion_ABC_OrganiserAccueilColonneRefugies : CreateMission_ABC_OrganiserAccueilColonneRefugies(); break;
        case eMission_Pion_ABC_AppuyerSurPosition : CreateMission_ABC_AppuyerSurPosition(); break;
        case eMission_Pion_ABC_RASIT_RenseignerSur : CreateMission_ABC_RASIT_RenseignerSur(); break;
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
        case eMission_Pion_INF_EscorterUnite : CreateMission_INF_EscorterUnite(); break;
        case eMission_Pion_INF_EscorterUnites : CreateMission_INF_EscorterUnites(); break;
        case eMission_Pion_INF_RenseignerSur : CreateMission_INF_RenseignerSur(); break;
        case eMission_Pion_INF_Soutenir : CreateMission_INF_Soutenir(); break;
        case eMission_Pion_INF_Securiser : CreateMission_INF_Securiser(); break;
        case eMission_Pion_INF_Recueillir : CreateMission_INF_Recueillir(); break;
        case eMission_Pion_INF_RepondreAAggression : CreateMission_INF_RepondreAAggression(); break;
        case eMission_Pion_INF_AppuyerSurPosition : CreateMission_INF_AppuyerSurPosition(); break;
        case eMission_Pion_LOG_SeDeployer : CreateMission_LOG_SeDeployer(); break;
        case eMission_Pion_LOG_Renforcer : CreateMission_LOG_Renforcer(); break;
        case eMission_Pion_LOG_PreparerBascule : CreateMission_LOG_PreparerBascule(); break;
        case eMission_Pion_LOG_DecontaminerBlesses : CreateMission_LOG_DecontaminerBlesses(); break;
        case eMission_Pion_LOG_TrierEtTraiterBlesses : CreateMission_LOG_TrierEtTraiterBlesses(); break;
        case eMission_Pion_LOG_TrierBlesses : CreateMission_LOG_TrierBlesses(); break;
        case eMission_Pion_LOG_TraiterBlesses : CreateMission_LOG_TraiterBlesses(); break;
        case eMission_Pion_LOG_PrendreEnCompteReactionsMentales : CreateMission_LOG_PrendreEnCompteReactionsMentales(); break;
        case eMission_Pion_LOG_Convoyer : CreateMission_LOG_Convoyer(); break;
        case eMission_Pion_LOG_Reparer : CreateMission_LOG_Reparer(); break;
        case eMission_Pion_LOG_AppuyerMouvement : CreateMission_LOG_AppuyerMouvement(); break;
        case eMission_Pion_LOG_ReconnaitreItineraire : CreateMission_LOG_ReconnaitreItineraire(); break;
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
        case eMission_Pion_Transporter : CreateMission_Transporter(); break;
        case eMission_Pion_SeFaireDecontaminer : CreateMission_SeFaireDecontaminer(); break;
        case eMission_Pion_Franchir : CreateMission_Franchir(); break;
        case eMission_Pion_Decrocher : CreateMission_Decrocher(); break;
        case eMission_Pion_Stationner : CreateMission_Stationner(); break;
        case eMission_Pion_SeFaireTransporter : CreateMission_SeFaireTransporter(); break;
        case eMission_Pion_InterdireFranchissementPopulations : CreateMission_InterdireFranchissementPopulations(); break;
        case eMission_Pion_FiltrerPopulations : CreateMission_FiltrerPopulations(); break;
        case eMission_Pion_RenseignerSurPopulations : CreateMission_RenseignerSurPopulations(); break;
        case eMission_Pion_CanaliserPopulations : CreateMission_CanaliserPopulations(); break;
        case eMission_Pion_ControlerPopulationsDansZone : CreateMission_ControlerPopulationsDansZone(); break;
        case eMission_Pion_SecuriserZoneContrePopulations : CreateMission_SecuriserZoneContrePopulations(); break;
        case eMission_Pion_Generique : CreateMission_Generique(); break;
        case eMission_Pion_MILICE_Provoquer : CreateMission_MILICE_Provoquer(); break;
        case eMission_Pion_MILICE_PrendreEtTenir : CreateMission_MILICE_PrendreEtTenir(); break;
        case eMission_Pion_MILICE_DetruireEmbuscade : CreateMission_MILICE_DetruireEmbuscade(); break;
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
        case eMission_Pion_ASY_SInfiltrer : CreateMission_ASY_SInfiltrer(); break;
        case eMission_Pion_ASY_CommettreAttentatContrePopulation : CreateMission_ASY_CommettreAttentatContrePopulation(); break;
        case eMission_Pion_ASY_CommettreAttentatContreForcesArmees : CreateMission_ASY_CommettreAttentatContreForcesArmees(); break;
        case eMission_Pion_ASY_CommettreAttentatContreInstallation : CreateMission_ASY_CommettreAttentatContreInstallation(); break;
        case eMission_Pion_ASY_CommettreExactionsSurPopulationDansZone : CreateMission_ASY_CommettreExactionsSurPopulationDansZone(); break;
        case eMission_Pion_ASY_Renseigner : CreateMission_ASY_Renseigner(); break;

        default:
            assert( false );
    }

    this->CreateDefaultParameters();
    this->CreateOkCancelButtons();
}


// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_AllerSeRecompleter
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_AllerSeRecompleter()
{
    ASN1T_Mission_Pion_ALAT_AllerSeRecompleter& asnMission = *new ASN1T_Mission_Pion_ALAT_AllerSeRecompleter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_aller_se_recompleter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_aller_se_recompleter = &asnMission;
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_ReconnaitreDansLaProfondeur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_ReconnaitreDansLaProfondeur()
{
    ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& asnMission = *new ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_reconnaitre_dans_la_profondeur = &asnMission;
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreatePoint( asnMission.point_dislocation, "Point dislocation");
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_ReconnaitreContourEnnemi
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_ReconnaitreContourEnnemi()
{
    ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi& asnMission = *new ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_reconnaitre_contour_ennemi;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_reconnaitre_contour_ennemi = &asnMission;
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_DetruireNeutraliserDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_DetruireNeutraliserDansZone()
{
    ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *new ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_detruire_neutraliser_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePoint( asnMission.point_dislocation, "Point dislocation");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.neutraliser, "Neutraliser");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_DetruireAeromobile
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_DetruireAeromobile()
{
    ASN1T_Mission_Pion_ALAT_DetruireAeromobile& asnMission = *new ASN1T_Mission_Pion_ALAT_DetruireAeromobile();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_detruire_aeromobile;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_detruire_aeromobile = &asnMission;
    CreateAgentKnowledgeList( asnMission.unite_a_detruire, "Unite a detruire");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Freiner
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Freiner()
{
    ASN1T_Mission_Pion_ALAT_Freiner& asnMission = *new ASN1T_Mission_Pion_ALAT_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_freiner = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Jalonner
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Jalonner()
{
    ASN1T_Mission_Pion_ALAT_Jalonner& asnMission = *new ASN1T_Mission_Pion_ALAT_Jalonner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_jalonner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_jalonner = &asnMission;
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Escorter
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Escorter()
{
    ASN1T_Mission_Pion_ALAT_Escorter& asnMission = *new ASN1T_Mission_Pion_ALAT_Escorter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_escorter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_escorter = &asnMission;
    CreateAgentList( asnMission.unites_a_escorter, "Unites a escorter");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Heliporter
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Heliporter()
{
    ASN1T_Mission_Pion_ALAT_Heliporter& asnMission = *new ASN1T_Mission_Pion_ALAT_Heliporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_heliporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_heliporter = &asnMission;
    CreateAgentList( asnMission.unites_a_heliporter, "Unites a heliporter");
    CreatePoint( asnMission.point_debarquement, "Point debarquement");
    CreatePoint( asnMission.point_embarquement, "Point embarquement");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.avec_materiel, "Avec materiel");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_HeliporterHelitransporterPlotRavitaillement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_HeliporterHelitransporterPlotRavitaillement()
{
    ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement& asnMission = *new ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_heliporter_helitransporter_plot_ravitaillement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_heliporter_helitransporter_plot_ravitaillement = &asnMission;
    CreatePoint( asnMission.point_debarquement, "Point debarquement");
    CreatePoint( asnMission.position_regroupement, "Position regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Helitransporter
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Helitransporter()
{
    ASN1T_Mission_Pion_ALAT_Helitransporter& asnMission = *new ASN1T_Mission_Pion_ALAT_Helitransporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_helitransporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_helitransporter = &asnMission;
    CreateAgentList( asnMission.unites_a_helitransporter, "Unites a helitransporter");
    CreatePoint( asnMission.point_debarquement, "Point debarquement");
    CreatePoint( asnMission.point_embarquement, "Point embarquement");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.avec_materiel, "Avec materiel");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_IMEX
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_IMEX()
{
    ASN1T_Mission_Pion_ALAT_IMEX& asnMission = *new ASN1T_Mission_Pion_ALAT_IMEX();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_imex;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_imex = &asnMission;
    CreateAgentList( asnMission.unites_a_appuyer, "Unites a appuyer");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Eclairer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Eclairer()
{
    ASN1T_Mission_Pion_ALAT_Eclairer& asnMission = *new ASN1T_Mission_Pion_ALAT_Eclairer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_eclairer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_eclairer = &asnMission;
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Surveiller()
{
    ASN1T_Mission_Pion_ALAT_Surveiller& asnMission = *new ASN1T_Mission_Pion_ALAT_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_AppuyerDirectAuContact
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_AppuyerDirectAuContact()
{
    ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact& asnMission = *new ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_appuyer_direct_au_contact;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_appuyer_direct_au_contact = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreateAgentList( asnMission.unites_a_appuyer, "Unites a appuyer");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Couvrir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Couvrir()
{
    ASN1T_Mission_Pion_ALAT_Couvrir& asnMission = *new ASN1T_Mission_Pion_ALAT_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_couvrir = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreateAgentList( asnMission.unites_a_couvrir, "Unites a couvrir");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ALAT_Attaquer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ALAT_Attaquer()
{
    ASN1T_Mission_Pion_ALAT_Attaquer& asnMission = *new ASN1T_Mission_Pion_ALAT_Attaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_alat_attaquer = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_DefendreSite
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_DefendreSite()
{
    ASN1T_Mission_Pion_ASA_DefendreSite& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreSite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_site;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_site = &asnMission;
    CreateLocation( asnMission.site, "Site");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_DefendreSiteAPartirPosition
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_DefendreSiteAPartirPosition()
{
    ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_site_a_partir_position = &asnMission;
    CreateLocation( asnMission.site, "Site");
    CreatePoint( asnMission.point_de_deploiement, "Point de deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_DefendreZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_DefendreZone()
{
    ASN1T_Mission_Pion_ASA_DefendreZone& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_DefendreZoneAPartirPosition
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_DefendreZoneAPartirPosition()
{
    ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_zone_a_partir_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_zone_a_partir_position = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePoint( asnMission.point_de_deploiement, "Point de deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_Surveiller()
{
    ASN1T_Mission_Pion_ASA_Surveiller& asnMission = *new ASN1T_Mission_Pion_ASA_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_surveiller = &asnMission;
    CreatePoint( asnMission.point_de_deploiement, "Point de deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_DefendreUnites
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_DefendreUnites()
{
    ASN1T_Mission_Pion_ASA_DefendreUnites& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreUnites();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_unites;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_defendre_unites = &asnMission;
    CreateAgentList( asnMission.unites, "Unites");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_MISTRAL_DefendreSite
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_MISTRAL_DefendreSite()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_site;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_site = &asnMission;
    CreateLocation( asnMission.site, "Site");
    ParamComboBox< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement, "Mode deploiement");
    pSelector_mode_deploiement->AddItem( "Omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel );
    pSelector_mode_deploiement->AddItem( "Directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_MISTRAL_DefendreSiteAPartirPosition
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_MISTRAL_DefendreSiteAPartirPosition()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_site_a_partir_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_site_a_partir_position = &asnMission;
    CreateLocation( asnMission.site, "Site");
    CreatePoint( asnMission.point_de_deploiement, "Point de deploiement");
    ParamComboBox< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement, "Mode deploiement");
    pSelector_mode_deploiement->AddItem( "Omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel );
    pSelector_mode_deploiement->AddItem( "Directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_MISTRAL_DefendreZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_MISTRAL_DefendreZone()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    ParamComboBox< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement, "Mode deploiement");
    pSelector_mode_deploiement->AddItem( "Omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel );
    pSelector_mode_deploiement->AddItem( "Directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_MISTRAL_DefendreZoneAPartirPosition
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_MISTRAL_DefendreZoneAPartirPosition()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_zone_a_partir_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_zone_a_partir_position = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePoint( asnMission.point_de_deploiement, "Point de deploiement");
    ParamComboBox< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement, "Mode deploiement");
    pSelector_mode_deploiement->AddItem( "Omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel );
    pSelector_mode_deploiement->AddItem( "Directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASA_MISTRAL_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASA_MISTRAL_Surveiller()
{
    ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asa_mistral_surveiller = &asnMission;
    CreatePoint( asnMission.point_de_deploiement, "Point de deploiement");
    ParamComboBox< ASN1T_EnumModeDeploiementSectionMistral >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement, "Mode deploiement");
    pSelector_mode_deploiement->AddItem( "Omnidirectionnel", EnumModeDeploiementSectionMistral::omnidirectionnel );
    pSelector_mode_deploiement->AddItem( "Directionnel", EnumModeDeploiementSectionMistral::directionnel );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASS_AssurerMiseEnOeuvre
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASS_AssurerMiseEnOeuvre()
{
    ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre& asnMission = *new ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_assurer_mise_en_oeuvre = &asnMission;
    CreateObjectKnowledge( asnMission.zone_implantation, "Zone implantation");
    CreateBool( asnMission.presence_contre_batterie, "Presence contre batterie");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASS_AssurerMiseEnOeuvreSurPosition
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASS_AssurerMiseEnOeuvreSurPosition()
{
    ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& asnMission = *new ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre_sur_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_assurer_mise_en_oeuvre_sur_position = &asnMission;
    CreatePoint( asnMission.position_implantation, "Position implantation");
    CreateBool( asnMission.presence_contre_batterie, "Presence contre batterie");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASS_ReconnaitreZoneDeploiement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASS_ReconnaitreZoneDeploiement()
{
    ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& asnMission = *new ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_reconnaitre_zone_deploiement = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASS_AcquerirLanceursEni
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASS_AcquerirLanceursEni()
{
    ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& asnMission = *new ASN1T_Mission_Pion_ASS_AcquerirLanceursEni();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_acquerir_lanceurs_eni = &asnMission;
    CreatePolygonList( asnMission.zones_a_observer, "Zones a observer");
    CreateObjectKnowledge( asnMission.zone_implantation, "Zone implantation");
    CreateBool( asnMission.menance_roem, "Menance roem");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASS_EO_AcquerirObjectifs
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASS_EO_AcquerirObjectifs()
{
    ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_eo_acquerir_objectifs = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "Zone a observer");
    CreateAutomate( asnMission.unite_a_appuyer, "Unite a appuyer");
    CreateNatureAtlas( asnMission.categories, "Categories");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASS_EOP_AcquerirObjectifs
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASS_EOP_AcquerirObjectifs()
{
    ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_eop_acquerir_objectifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_eop_acquerir_objectifs = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "Zone a observer");
    CreatePoint( asnMission.position_installation, "Position installation");
    CreatePoint( asnMission.position_debarquement, "Position debarquement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASS_RATAC_AcquerirObjectifs
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASS_RATAC_AcquerirObjectifs()
{
    ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_ratac_acquerir_objectifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_ass_ratac_acquerir_objectifs = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "Zone a observer");
    CreateNatureAtlas( asnMission.categories, "Categories");
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_ReconnaitreAxe
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_ReconnaitreAxe()
{
    ASN1T_Mission_Pion_ABC_ReconnaitreAxe& asnMission = *new ASN1T_Mission_Pion_ABC_ReconnaitreAxe();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_reconnaitre_axe;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_axe = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Soutenir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Soutenir()
{
    ASN1T_Mission_Pion_ABC_Soutenir& asnMission = *new ASN1T_Mission_Pion_ABC_Soutenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_soutenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_soutenir = &asnMission;
    CreateAgent( asnMission.unite_a_soutenir, "Unite a soutenir");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Couvrir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Couvrir()
{
    ASN1T_Mission_Pion_ABC_Couvrir& asnMission = *new ASN1T_Mission_Pion_ABC_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_couvrir = &asnMission;
    CreatePoint( asnMission.position, "Position");
    CreateAgentList( asnMission.unites_a_couvrir, "Unites a couvrir");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_FlancGarder
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_FlancGarder()
{
    ASN1T_Mission_Pion_ABC_FlancGarder& asnMission = *new ASN1T_Mission_Pion_ABC_FlancGarder();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_flanc_garder;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_flanc_garder = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    CreateAgentList( asnMission.unites_a_couvrir, "Unites a couvrir");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Reduire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Reduire()
{
    ASN1T_Mission_Pion_ABC_Reduire& asnMission = *new ASN1T_Mission_Pion_ABC_Reduire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_reduire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reduire = &asnMission;
    CreateAgentKnowledge( asnMission.unite_a_reduire, "Unite a reduire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Appuyer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Appuyer()
{
    ASN1T_Mission_Pion_ABC_Appuyer& asnMission = *new ASN1T_Mission_Pion_ABC_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_appuyer = &asnMission;
    CreateAgent( asnMission.unite_a_appuyer, "Unite a appuyer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Fixer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Fixer()
{
    ASN1T_Mission_Pion_ABC_Fixer& asnMission = *new ASN1T_Mission_Pion_ABC_Fixer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_fixer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_fixer = &asnMission;
    CreateAgentKnowledge( asnMission.unite_a_fixer, "Unite a fixer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_RenseignerSur()
{
    ASN1T_Mission_Pion_ABC_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_ABC_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_renseigner_sur = &asnMission;
    CreatePoint( asnMission.point_a_renseigner, "Point a renseigner");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_ControlerZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_ControlerZone()
{
    ASN1T_Mission_Pion_ABC_ControlerZone& asnMission = *new ASN1T_Mission_Pion_ABC_ControlerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_controler_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_controler_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Surveiller()
{
    ASN1T_Mission_Pion_ABC_Surveiller& asnMission = *new ASN1T_Mission_Pion_ABC_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_surveiller = &asnMission;
    CreatePoint( asnMission.position, "Position");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Barrer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Barrer()
{
    ASN1T_Mission_Pion_ABC_Barrer& asnMission = *new ASN1T_Mission_Pion_ABC_Barrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_barrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_barrer = &asnMission;
    CreatePoint( asnMission.position_installation, "Position installation");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Attaquer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Attaquer()
{
    ASN1T_Mission_Pion_ABC_Attaquer& asnMission = *new ASN1T_Mission_Pion_ABC_Attaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_attaquer = &asnMission;
    CreatePath( asnMission.itineraire_assaut, "Itineraire assaut");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_ReconnaitrePoint
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_ReconnaitrePoint()
{
    ASN1T_Mission_Pion_ABC_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Pion_ABC_ReconnaitrePoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_reconnaitre_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_point = &asnMission;
    CreatePoint( asnMission.point_a_reconnaitre, "Point a reconnaitre");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Eclairer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Eclairer()
{
    ASN1T_Mission_Pion_ABC_Eclairer& asnMission = *new ASN1T_Mission_Pion_ABC_Eclairer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_eclairer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_eclairer = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Jalonner
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Jalonner()
{
    ASN1T_Mission_Pion_ABC_Jalonner& asnMission = *new ASN1T_Mission_Pion_ABC_Jalonner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_jalonner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_jalonner = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_ContreAttaquer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_ContreAttaquer()
{
    ASN1T_Mission_Pion_ABC_ContreAttaquer& asnMission = *new ASN1T_Mission_Pion_ABC_ContreAttaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_contre_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_contre_attaquer = &asnMission;
    CreatePath( asnMission.itineraire_assaut, "Itineraire assaut");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_SemparerZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_SemparerZone()
{
    ASN1T_Mission_Pion_ABC_SemparerZone& asnMission = *new ASN1T_Mission_Pion_ABC_SemparerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_semparer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_semparer_zone = &asnMission;
    CreateLocation( asnMission.zone_a_prendre, "Zone a prendre");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_ReleverUnite
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_ReleverUnite()
{
    ASN1T_Mission_Pion_ABC_ReleverUnite& asnMission = *new ASN1T_Mission_Pion_ABC_ReleverUnite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_relever_unite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_relever_unite = &asnMission;
    CreateAgent( asnMission.unite_a_relever, "Unite a relever");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_ArmerPIA
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_ArmerPIA()
{
    ASN1T_Mission_Pion_ABC_ArmerPIA& asnMission = *new ASN1T_Mission_Pion_ABC_ArmerPIA();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_armer_pia;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_armer_pia = &asnMission;
    CreatePoint( asnMission.pia, "Pia");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_OuvrirItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_OuvrirItineraire()
{
    ASN1T_Mission_Pion_ABC_OuvrirItineraire& asnMission = *new ASN1T_Mission_Pion_ABC_OuvrirItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_ouvrir_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_ouvrir_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_Freiner
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_Freiner()
{
    ASN1T_Mission_Pion_ABC_Freiner& asnMission = *new ASN1T_Mission_Pion_ABC_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_freiner = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_EscorterUnite
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_EscorterUnite()
{
    ASN1T_Mission_Pion_ABC_EscorterUnite& asnMission = *new ASN1T_Mission_Pion_ABC_EscorterUnite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_escorter_unite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_escorter_unite = &asnMission;
    CreateAgent( asnMission.unite_a_escorter, "Unite a escorter");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_EscorterUnites
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_EscorterUnites()
{
    ASN1T_Mission_Pion_ABC_EscorterUnites& asnMission = *new ASN1T_Mission_Pion_ABC_EscorterUnites();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_escorter_unites;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_escorter_unites = &asnMission;
    CreateAutomate( asnMission.automate_a_escorter, "Automate a escorter");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_ArmerPointDeControle
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_ArmerPointDeControle()
{
    ASN1T_Mission_Pion_ABC_ArmerPointDeControle& asnMission = *new ASN1T_Mission_Pion_ABC_ArmerPointDeControle();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_armer_point_de_controle;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_armer_point_de_controle = &asnMission;
    CreatePoint( asnMission.point_de_controle, "Point de controle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_OrganiserAccueilColonneRefugies
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_OrganiserAccueilColonneRefugies()
{
    ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies& asnMission = *new ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_organiser_accueil_colonne_refugies;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_organiser_accueil_colonne_refugies = &asnMission;
    CreatePoint( asnMission.position_installation, "Position installation");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_AppuyerSurPosition
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_AppuyerSurPosition()
{
    ASN1T_Mission_Pion_ABC_AppuyerSurPosition& asnMission = *new ASN1T_Mission_Pion_ABC_AppuyerSurPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_appuyer_sur_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_appuyer_sur_position = &asnMission;
    CreatePoint( asnMission.position, "Position");
    CreateAgentList( asnMission.unites_a_appuyer, "Unites a appuyer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ABC_RASIT_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ABC_RASIT_RenseignerSur()
{
    ASN1T_Mission_Pion_ABC_RASIT_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_ABC_RASIT_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_rasit_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_abc_rasit_renseigner_sur = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserFosseAC
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserFosseAC()
{
    ASN1T_Mission_Pion_GEN_RealiserFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserFosseAC();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_fosse_ac;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_fosse_ac = &asnMission;
    CreateLocation( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    ParamComboBox< ASN1T_EnumTypeFosseAC >* pSelector_type = &CreateVarList( asnMission.type, "Type");
    pSelector_type->AddItem( "Simple", EnumTypeFosseAC::simple );
    pSelector_type->AddItem( "Compose", EnumTypeFosseAC::compose );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserAbattisMine
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserAbattisMine()
{
    ASN1T_Mission_Pion_GEN_RealiserAbattisMine& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserAbattisMine();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_abattis_mine;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_abattis_mine = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    ParamComboBox< ASN1T_EnumMissionGenUrgence >* pSelector_urgence = &CreateVarList( asnMission.urgence, "Urgence");
    pSelector_urgence->AddItem( "Serie minimum", EnumMissionGenUrgence::serie_minimum );
    pSelector_urgence->AddItem( "Serie complementaire", EnumMissionGenUrgence::serie_complementaire );
    ParamComboBox< ASN1T_EnumMissionGenSousTypeObstacle >* pSelector_preliminaire = &CreateVarList( asnMission.preliminaire, "Preliminaire");
    pSelector_preliminaire->AddItem( "Preliminaire", EnumMissionGenSousTypeObstacle::preliminaire );
    pSelector_preliminaire->AddItem( "De manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserPointMine
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserPointMine()
{
    ASN1T_Mission_Pion_GEN_RealiserPointMine& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPointMine();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_point_mine;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_point_mine = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserBarricade
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserBarricade()
{
    ASN1T_Mission_Pion_GEN_RealiserBarricade& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserBarricade();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_barricade;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_barricade = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    ParamComboBox< ASN1T_EnumMissionGenUrgence >* pSelector_urgence = &CreateVarList( asnMission.urgence, "Urgence");
    pSelector_urgence->AddItem( "Serie minimum", EnumMissionGenUrgence::serie_minimum );
    pSelector_urgence->AddItem( "Serie complementaire", EnumMissionGenUrgence::serie_complementaire );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserEboulement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserEboulement()
{
    ASN1T_Mission_Pion_GEN_RealiserEboulement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserEboulement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_eboulement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_eboulement = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    ParamComboBox< ASN1T_EnumMissionGenSousTypeObstacle >* pSelector_preliminaire = &CreateVarList( asnMission.preliminaire, "Preliminaire");
    pSelector_preliminaire->AddItem( "Preliminaire", EnumMissionGenSousTypeObstacle::preliminaire );
    pSelector_preliminaire->AddItem( "De manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserBouchonMine
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserBouchonMine()
{
    ASN1T_Mission_Pion_GEN_RealiserBouchonMine& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserBouchonMine();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_bouchon_mine;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_bouchon_mine = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    ParamComboBox< ASN1T_EnumMissionGenUrgence >* pSelector_urgence = &CreateVarList( asnMission.urgence, "Urgence");
    pSelector_urgence->AddItem( "Serie minimum", EnumMissionGenUrgence::serie_minimum );
    pSelector_urgence->AddItem( "Serie complementaire", EnumMissionGenUrgence::serie_complementaire );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_DetruireRoute
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_DetruireRoute()
{
    ASN1T_Mission_Pion_GEN_DetruireRoute& asnMission = *new ASN1T_Mission_Pion_GEN_DetruireRoute();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_detruire_route;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_detruire_route = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    ParamComboBox< ASN1T_EnumMissionGenSousTypeObstacle >* pSelector_preliminaire = &CreateVarList( asnMission.preliminaire, "Preliminaire");
    pSelector_preliminaire->AddItem( "Preliminaire", EnumMissionGenSousTypeObstacle::preliminaire );
    pSelector_preliminaire->AddItem( "De manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_DetruirePont
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_DetruirePont()
{
    ASN1T_Mission_Pion_GEN_DetruirePont& asnMission = *new ASN1T_Mission_Pion_GEN_DetruirePont();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_detruire_pont;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_detruire_pont = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    ParamComboBox< ASN1T_EnumMissionGenSousTypeObstacle >* pSelector_preliminaire = &CreateVarList( asnMission.preliminaire, "Preliminaire");
    pSelector_preliminaire->AddItem( "Preliminaire", EnumMissionGenSousTypeObstacle::preliminaire );
    pSelector_preliminaire->AddItem( "De manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_SecuriserItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_SecuriserItineraire()
{
    ASN1T_Mission_Pion_GEN_SecuriserItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_SecuriserItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_securiser_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_securiser_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_DegagerAbattisMine
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_DegagerAbattisMine()
{
    ASN1T_Mission_Pion_GEN_DegagerAbattisMine& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerAbattisMine();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_abattis_mine;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_abattis_mine = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_DegagerEboulement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_DegagerEboulement()
{
    ASN1T_Mission_Pion_GEN_DegagerEboulement& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerEboulement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_eboulement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_eboulement = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_DegagerRues
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_DegagerRues()
{
    ASN1T_Mission_Pion_GEN_DegagerRues& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerRues();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_rues;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_degager_rues = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_ComblerEntonnoir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_ComblerEntonnoir()
{
    ASN1T_Mission_Pion_GEN_ComblerEntonnoir& asnMission = *new ASN1T_Mission_Pion_GEN_ComblerEntonnoir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_combler_entonnoir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_combler_entonnoir = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_ComblerFosseAC
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_ComblerFosseAC()
{
    ASN1T_Mission_Pion_GEN_ComblerFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_ComblerFosseAC();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_combler_fosse_ac;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_combler_fosse_ac = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_CreerContournement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_CreerContournement()
{
    ASN1T_Mission_Pion_GEN_CreerContournement& asnMission = *new ASN1T_Mission_Pion_GEN_CreerContournement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_contournement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_contournement = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserZoneMineeLineaire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserZoneMineeLineaire()
{
    ASN1T_Mission_Pion_GEN_RealiserZoneMineeLineaire& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZoneMineeLineaire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_minee_lineaire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_lineaire = &asnMission;
    CreateLocation( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    CreateNumeric( asnMission.densite, "Densite");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_OuvrirTrouee
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_OuvrirTrouee()
{
    ASN1T_Mission_Pion_GEN_OuvrirTrouee& asnMission = *new ASN1T_Mission_Pion_GEN_OuvrirTrouee();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_ouvrir_trouee;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_ouvrir_trouee = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_Renforcer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_Renforcer()
{
    ASN1T_Mission_Pion_GEN_Renforcer& asnMission = *new ASN1T_Mission_Pion_GEN_Renforcer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_renforcer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_renforcer = &asnMission;
    CreatePoint( asnMission.lieu_renforcement, "Lieu renforcement");
    CreateAgent( asnMission.unite, "Unite");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_FranchirEntonnoir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_FranchirEntonnoir()
{
    ASN1T_Mission_Pion_GEN_FranchirEntonnoir& asnMission = *new ASN1T_Mission_Pion_GEN_FranchirEntonnoir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_franchir_entonnoir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_franchir_entonnoir = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_FranchirFosseAC
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_FranchirFosseAC()
{
    ASN1T_Mission_Pion_GEN_FranchirFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_FranchirFosseAC();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_franchir_fosse_ac;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_franchir_fosse_ac = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserZoneMineeParDispersion
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserZoneMineeParDispersion()
{
    ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_par_dispersion = &asnMission;
    CreateLocation( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    CreateNumeric( asnMission.densite, "Densite");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserPostesTir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserPostesTir()
{
    ASN1T_Mission_Pion_GEN_RealiserPostesTir& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPostesTir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_postes_tir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_postes_tir = &asnMission;
    CreatePoint( asnMission.pos_objet, "Pos objet");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserTravauxProtection
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserTravauxProtection()
{
    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxProtection();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_travaux_protection;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_protection = &asnMission;
    CreatePoint( asnMission.pos_objet, "Pos objet");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RetablirItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RetablirItineraire()
{
    ASN1T_Mission_Pion_GEN_RetablirItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_RetablirItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_retablir_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_retablir_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_EquiperExploiter
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_EquiperExploiter()
{
    ASN1T_Mission_Pion_GEN_EquiperExploiter& asnMission = *new ASN1T_Mission_Pion_GEN_EquiperExploiter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_equiper_exploiter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_equiper_exploiter = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "Site franchissement");
    //NOTHING
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    ParamComboBox< ASN1T_EnumGenTypePontage >* pSelector_type_pontage = &CreateVarList( asnMission.type_pontage, "Type pontage");
    pSelector_type_pontage->AddItem( "Continu", EnumGenTypePontage::continu );
    pSelector_type_pontage->AddItem( "Discontinu", EnumGenTypePontage::discontinu );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_ExecuterVariantement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_ExecuterVariantement()
{
    ASN1T_Mission_Pion_GEN_ExecuterVariantement& asnMission = *new ASN1T_Mission_Pion_GEN_ExecuterVariantement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_executer_variantement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_executer_variantement = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "Site franchissement");
    //NOTHING
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    ParamComboBox< ASN1T_EnumGenTypePontage >* pSelector_type_pontage = &CreateVarList( asnMission.type_pontage, "Type pontage");
    pSelector_type_pontage->AddItem( "Continu", EnumGenTypePontage::continu );
    pSelector_type_pontage->AddItem( "Discontinu", EnumGenTypePontage::discontinu );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_ReconnaitreSiteFranchissement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_ReconnaitreSiteFranchissement()
{
    ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement& asnMission = *new ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reconnaitre_site_franchissement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_site_franchissement = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "Site franchissement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_ReconnaitreItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_ReconnaitreItineraire()
{
    ASN1T_Mission_Pion_GEN_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_ReconnaitreItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reconnaitre_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_DeminerSiteFranchissement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_DeminerSiteFranchissement()
{
    ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement& asnMission = *new ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_deminer_site_franchissement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_deminer_site_franchissement = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "Site franchissement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_ReconnaitreZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_ReconnaitreZone()
{
    ASN1T_Mission_Pion_GEN_ReconnaitreZone& asnMission = *new ASN1T_Mission_Pion_GEN_ReconnaitreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reconnaitre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reconnaitre_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_CreerPlateForme
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_CreerPlateForme()
{
    ASN1T_Mission_Pion_GEN_CreerPlateForme& asnMission = *new ASN1T_Mission_Pion_GEN_CreerPlateForme();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_plate_forme;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_plate_forme = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_CreerPiste
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_CreerPiste()
{
    ASN1T_Mission_Pion_GEN_CreerPiste& asnMission = *new ASN1T_Mission_Pion_GEN_CreerPiste();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_piste;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_piste = &asnMission;
    CreateLocation( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_CreerAirePoser
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_CreerAirePoser()
{
    ASN1T_Mission_Pion_GEN_CreerAirePoser& asnMission = *new ASN1T_Mission_Pion_GEN_CreerAirePoser();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_aire_poser;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_creer_aire_poser = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_NeutraliserEnginsExplosifs
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_NeutraliserEnginsExplosifs()
{
    ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs& asnMission = *new ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_neutraliser_engins_explosifs = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_ActiverObstacle
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_ActiverObstacle()
{
    ASN1T_Mission_Pion_GEN_ActiverObstacle& asnMission = *new ASN1T_Mission_Pion_GEN_ActiverObstacle();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_activer_obstacle;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_activer_obstacle = &asnMission;
    CreateObjectKnowledge( asnMission.obstacle, "Obstacle");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_AmeliorerMobilite
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_AmeliorerMobilite()
{
    ASN1T_Mission_Pion_GEN_AmeliorerMobilite& asnMission = *new ASN1T_Mission_Pion_GEN_AmeliorerMobilite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_ameliorer_mobilite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_ameliorer_mobilite = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserAireLogistique
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserAireLogistique()
{
    ASN1T_Mission_Pion_GEN_RealiserAireLogistique& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserAireLogistique();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_aire_logistique;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_aire_logistique = &asnMission;
    CreatePoint( asnMission.position_obstacle, "Position obstacle");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_DepolluerZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_DepolluerZone()
{
    ASN1T_Mission_Pion_GEN_DepolluerZone& asnMission = *new ASN1T_Mission_Pion_GEN_DepolluerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_depolluer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_depolluer_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_AmenagerTerrainPoser
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_AmenagerTerrainPoser()
{
    ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser& asnMission = *new ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_amenager_terrain_poser;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_amenager_terrain_poser = &asnMission;
    CreatePoint( asnMission.pos_obstacle, "Pos obstacle");
    //NOTHING
    ParamComboBox< ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser_type_objet >* pSelector_type_objet = &CreateVarList( asnMission.type_objet, "Type objet");
    pSelector_type_objet->AddItem( "Aire poser", Mission_Pion_GEN_AmenagerTerrainPoser_type_objet::aire_poser );
    pSelector_type_objet->AddItem( "Terrain largage", Mission_Pion_GEN_AmenagerTerrainPoser_type_objet::terrain_largage );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserPlateFormeDeploiement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserPlateFormeDeploiement()
{
    ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_plate_forme_deploiement = &asnMission;
    CreatePoint( asnMission.position_obstacle, "Position obstacle");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_ReprendreMissionConstruction
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_ReprendreMissionConstruction()
{
    ASN1T_Mission_Pion_GEN_ReprendreMissionConstruction& asnMission = *new ASN1T_Mission_Pion_GEN_ReprendreMissionConstruction();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reprendre_mission_construction;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_reprendre_mission_construction = &asnMission;
    CreateObjectKnowledge( asnMission.travaux, "Travaux");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserZonePoserHelicoptere
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserZonePoserHelicoptere()
{
    ASN1T_Mission_Pion_GEN_RealiserZonePoserHelicoptere& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZonePoserHelicoptere();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_poser_helicoptere;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_poser_helicoptere = &asnMission;
    CreatePoint( asnMission.zone_poser, "Zone poser");
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_AppuyerDebarquementTroupesGU
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_AppuyerDebarquementTroupesGU()
{
    ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU& asnMission = *new ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_appuyer_debarquement_troupes_gu = &asnMission;
    CreateLocation( asnMission.zone_appui, "Zone appui");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserCampPrisonniers
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserCampPrisonniers()
{
    ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_prisonniers = &asnMission;
    CreateLocation( asnMission.camp, "Camp");
    CreateAutomate( asnMission.tc2, "Tc2");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_GEN_RealiserCampRefugies
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_GEN_RealiserCampRefugies()
{
    ASN1T_Mission_Pion_GEN_RealiserCampRefugies& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserCampRefugies();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_camp_refugies;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_refugies = &asnMission;
    CreateLocation( asnMission.camp, "Camp");
    CreateAutomate( asnMission.tc2, "Tc2");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_ReconnaitreAxe
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_ReconnaitreAxe()
{
    ASN1T_Mission_Pion_INF_ReconnaitreAxe& asnMission = *new ASN1T_Mission_Pion_INF_ReconnaitreAxe();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_reconnaitre_axe;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reconnaitre_axe = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Couvrir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Couvrir()
{
    ASN1T_Mission_Pion_INF_Couvrir& asnMission = *new ASN1T_Mission_Pion_INF_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_couvrir = &asnMission;
    CreatePoint( asnMission.point_couverture, "Point couverture");
    CreateAgent( asnMission.cible, "Cible");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Reduire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Reduire()
{
    ASN1T_Mission_Pion_INF_Reduire& asnMission = *new ASN1T_Mission_Pion_INF_Reduire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_reduire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reduire = &asnMission;
    CreateAgentKnowledge( asnMission.unite_a_reduire, "Unite a reduire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Appuyer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Appuyer()
{
    ASN1T_Mission_Pion_INF_Appuyer& asnMission = *new ASN1T_Mission_Pion_INF_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_appuyer = &asnMission;
    CreateAgent( asnMission.unite_a_appuyer, "Unite a appuyer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Fixer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Fixer()
{
    ASN1T_Mission_Pion_INF_Fixer& asnMission = *new ASN1T_Mission_Pion_INF_Fixer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_fixer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_fixer = &asnMission;
    CreateAgentKnowledge( asnMission.ennemi, "Ennemi");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_ControlerSecteur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_ControlerSecteur()
{
    ASN1T_Mission_Pion_INF_ControlerSecteur& asnMission = *new ASN1T_Mission_Pion_INF_ControlerSecteur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_controler_secteur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_controler_secteur = &asnMission;
    CreateLocation( asnMission.secteur, "Secteur");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_SurveillerSecteur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_SurveillerSecteur()
{
    ASN1T_Mission_Pion_INF_SurveillerSecteur& asnMission = *new ASN1T_Mission_Pion_INF_SurveillerSecteur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_surveiller_secteur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_surveiller_secteur = &asnMission;
    CreatePoint( asnMission.position_installation, "Position installation");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Barrer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Barrer()
{
    ASN1T_Mission_Pion_INF_Barrer& asnMission = *new ASN1T_Mission_Pion_INF_Barrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_barrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_barrer = &asnMission;
    CreatePoint( asnMission.position_a_barrer, "Position a barrer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_ReconnaitrePoint
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_ReconnaitrePoint()
{
    ASN1T_Mission_Pion_INF_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Pion_INF_ReconnaitrePoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_reconnaitre_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_reconnaitre_point = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    CreatePoint( asnMission.point_a_reconnaitre, "Point a reconnaitre");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Eclairer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Eclairer()
{
    ASN1T_Mission_Pion_INF_Eclairer& asnMission = *new ASN1T_Mission_Pion_INF_Eclairer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_eclairer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_eclairer = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Interdire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Interdire()
{
    ASN1T_Mission_Pion_INF_Interdire& asnMission = *new ASN1T_Mission_Pion_INF_Interdire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_interdire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_interdire = &asnMission;
    CreatePoint( asnMission.point_a_interdire, "Point a interdire");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Semparer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Semparer()
{
    ASN1T_Mission_Pion_INF_Semparer& asnMission = *new ASN1T_Mission_Pion_INF_Semparer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_semparer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_semparer = &asnMission;
    CreatePoint( asnMission.objectif, "Objectif");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_PrendrePreciserContact
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_PrendrePreciserContact()
{
    ASN1T_Mission_Pion_INF_PrendrePreciserContact& asnMission = *new ASN1T_Mission_Pion_INF_PrendrePreciserContact();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_prendre_preciser_contact;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_prendre_preciser_contact = &asnMission;
    CreateAgentKnowledge( asnMission.ennemi, "Ennemi");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Defendre
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Defendre()
{
    ASN1T_Mission_Pion_INF_Defendre& asnMission = *new ASN1T_Mission_Pion_INF_Defendre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_defendre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_defendre = &asnMission;
    CreatePoint( asnMission.point_a_defendre, "Point a defendre");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_DetruireEmbuscade
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_DetruireEmbuscade()
{
    ASN1T_Mission_Pion_INF_DetruireEmbuscade& asnMission = *new ASN1T_Mission_Pion_INF_DetruireEmbuscade();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_detruire_embuscade;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_detruire_embuscade = &asnMission;
    CreatePoint( asnMission.position_embuscade, "Position embuscade");
    CreatePoint( asnMission.position_regroupement, "Position regroupement");
    CreateAgentKnowledge( asnMission.ennemi, "Ennemi");
    ParamComboBox< ASN1T_EnumObjectType >* pSelector_nature_obstacle = &CreateVarList( asnMission.nature_obstacle, "Nature obstacle");
    pSelector_nature_obstacle->AddItem( "Bouchon mines", EnumObjectType::bouchon_mines );
    pSelector_nature_obstacle->AddItem( "Zone minee lineaire", EnumObjectType::zone_minee_lineaire );
    pSelector_nature_obstacle->AddItem( "Zone minee par dispersion", EnumObjectType::zone_minee_par_dispersion );
    pSelector_nature_obstacle->AddItem( "Fosse anti char", EnumObjectType::fosse_anti_char );
    pSelector_nature_obstacle->AddItem( "Abattis", EnumObjectType::abattis );
    pSelector_nature_obstacle->AddItem( "Barricade", EnumObjectType::barricade );
    pSelector_nature_obstacle->AddItem( "Eboulement", EnumObjectType::eboulement );
    pSelector_nature_obstacle->AddItem( "Destruction route", EnumObjectType::destruction_route );
    pSelector_nature_obstacle->AddItem( "Destruction pont", EnumObjectType::destruction_pont );
    pSelector_nature_obstacle->AddItem( "Pont flottant continu", EnumObjectType::pont_flottant_continu );
    pSelector_nature_obstacle->AddItem( "Poste tir", EnumObjectType::poste_tir );
    pSelector_nature_obstacle->AddItem( "Zone protegee", EnumObjectType::zone_protegee );
    pSelector_nature_obstacle->AddItem( "Zone implantation canon", EnumObjectType::zone_implantation_canon );
    pSelector_nature_obstacle->AddItem( "Zone implantation cobra", EnumObjectType::zone_implantation_cobra );
    pSelector_nature_obstacle->AddItem( "Zone implantation lrm", EnumObjectType::zone_implantation_lrm );
    pSelector_nature_obstacle->AddItem( "Site franchissement", EnumObjectType::site_franchissement );
    pSelector_nature_obstacle->AddItem( "Nuage nbc", EnumObjectType::nuage_nbc );
    pSelector_nature_obstacle->AddItem( "Plot ravitaillement", EnumObjectType::plot_ravitaillement );
    pSelector_nature_obstacle->AddItem( "Site decontamination", EnumObjectType::site_decontamination );
    pSelector_nature_obstacle->AddItem( "Zone brouillage brod", EnumObjectType::zone_brouillage_brod );
    pSelector_nature_obstacle->AddItem( "Rota", EnumObjectType::rota );
    pSelector_nature_obstacle->AddItem( "Zone nbc", EnumObjectType::zone_nbc );
    pSelector_nature_obstacle->AddItem( "Zone brouillage bromure", EnumObjectType::zone_brouillage_bromure );
    pSelector_nature_obstacle->AddItem( "Aire poser", EnumObjectType::aire_poser );
    pSelector_nature_obstacle->AddItem( "Piste", EnumObjectType::piste );
    pSelector_nature_obstacle->AddItem( "Plateforme", EnumObjectType::plateforme );
    pSelector_nature_obstacle->AddItem( "Zone mobilite amelioree", EnumObjectType::zone_mobilite_amelioree );
    pSelector_nature_obstacle->AddItem( "Zone poser helicoptere", EnumObjectType::zone_poser_helicoptere );
    pSelector_nature_obstacle->AddItem( "Aire logistique", EnumObjectType::aire_logistique );
    pSelector_nature_obstacle->AddItem( "Itineraire logistique", EnumObjectType::itineraire_logistique );
    pSelector_nature_obstacle->AddItem( "Camp prisonniers", EnumObjectType::camp_prisonniers );
    pSelector_nature_obstacle->AddItem( "Camp refugies", EnumObjectType::camp_refugies );
    pSelector_nature_obstacle->AddItem( "Poste controle", EnumObjectType::poste_controle );
    pSelector_nature_obstacle->AddItem( "Terrain largage", EnumObjectType::terrain_largage );
    pSelector_nature_obstacle->AddItem( "Zone interdite mouvement", EnumObjectType::zone_interdite_mouvement );
    pSelector_nature_obstacle->AddItem( "Zone interdite tir", EnumObjectType::zone_interdite_tir );
    pSelector_nature_obstacle->AddItem( "Zone implantation mortier", EnumObjectType::zone_implantation_mortier );
    pSelector_nature_obstacle->AddItem( "Pont flottant discontinu", EnumObjectType::pont_flottant_discontinu );
    pSelector_nature_obstacle->AddItem( "Installation", EnumObjectType::installation );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_ContreAttaquer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_ContreAttaquer()
{
    ASN1T_Mission_Pion_INF_ContreAttaquer& asnMission = *new ASN1T_Mission_Pion_INF_ContreAttaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_contre_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_contre_attaquer = &asnMission;
    CreateAgentKnowledge( asnMission.ennemi, "Ennemi");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Freiner
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Freiner()
{
    ASN1T_Mission_Pion_INF_Freiner& asnMission = *new ASN1T_Mission_Pion_INF_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_freiner = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_FlancGarder
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_FlancGarder()
{
    ASN1T_Mission_Pion_INF_FlancGarder& asnMission = *new ASN1T_Mission_Pion_INF_FlancGarder();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_flanc_garder;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_flanc_garder = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    CreateAgent( asnMission.unite_a_flancgarder, "Unite a flancgarder");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_OrganiserUnCheckPoint
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_OrganiserUnCheckPoint()
{
    ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& asnMission = *new ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_organiser_un_check_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_organiser_un_check_point = &asnMission;
    CreatePoint( asnMission.point_installation, "Point installation");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Harceler
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Harceler()
{
    ASN1T_Mission_Pion_INF_Harceler& asnMission = *new ASN1T_Mission_Pion_INF_Harceler();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_harceler;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_harceler = &asnMission;
    CreateLocation( asnMission.secteur, "Secteur");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    ParamComboBox< ASN1T_EnumMissionInfNatureObjectif >* pSelector_nature_objectif = &CreateVarList( asnMission.nature_objectif, "Nature objectif");
    pSelector_nature_objectif->AddItem( "Elements terrain", EnumMissionInfNatureObjectif::elements_terrain );
    pSelector_nature_objectif->AddItem( "Objets", EnumMissionInfNatureObjectif::objets );
    pSelector_nature_objectif->AddItem( "Elements mobiles", EnumMissionInfNatureObjectif::elements_mobiles );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_DetruireParCoupDeMain
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_DetruireParCoupDeMain()
{
    ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& asnMission = *new ASN1T_Mission_Pion_INF_DetruireParCoupDeMain();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_detruire_par_coup_de_main = &asnMission;
    CreatePoint( asnMission.objectif, "Objectif");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateGDH( asnMission.heure_destruction, "Heure destruction");
    ParamComboBox< ASN1T_EnumMissionInfNatureObjectif >* pSelector_nature_destruction = &CreateVarList( asnMission.nature_destruction, "Nature destruction");
    pSelector_nature_destruction->AddItem( "Elements terrain", EnumMissionInfNatureObjectif::elements_terrain );
    pSelector_nature_destruction->AddItem( "Objets", EnumMissionInfNatureObjectif::objets );
    pSelector_nature_destruction->AddItem( "Elements mobiles", EnumMissionInfNatureObjectif::elements_mobiles );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_EscorterUnite
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_EscorterUnite()
{
    ASN1T_Mission_Pion_INF_EscorterUnite& asnMission = *new ASN1T_Mission_Pion_INF_EscorterUnite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_escorter_unite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_escorter_unite = &asnMission;
    CreateAgent( asnMission.unite_a_escorter, "Unite a escorter");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_EscorterUnites
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_EscorterUnites()
{
    ASN1T_Mission_Pion_INF_EscorterUnites& asnMission = *new ASN1T_Mission_Pion_INF_EscorterUnites();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_escorter_unites;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_escorter_unites = &asnMission;
    CreateAutomate( asnMission.automate_a_escorter, "Automate a escorter");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_RenseignerSur()
{
    ASN1T_Mission_Pion_INF_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_INF_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_renseigner_sur = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Soutenir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Soutenir()
{
    ASN1T_Mission_Pion_INF_Soutenir& asnMission = *new ASN1T_Mission_Pion_INF_Soutenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_soutenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_soutenir = &asnMission;
    CreateAgent( asnMission.unite_a_soutenir, "Unite a soutenir");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Securiser
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Securiser()
{
    ASN1T_Mission_Pion_INF_Securiser& asnMission = *new ASN1T_Mission_Pion_INF_Securiser();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_securiser;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_securiser = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_Recueillir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_Recueillir()
{
    ASN1T_Mission_Pion_INF_Recueillir& asnMission = *new ASN1T_Mission_Pion_INF_Recueillir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_recueillir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_recueillir = &asnMission;
    CreatePoint( asnMission.position_installation, "Position installation");
    CreatePoint( asnMission.pia, "Pia");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_RepondreAAggression
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_RepondreAAggression()
{
    ASN1T_Mission_Pion_INF_RepondreAAggression& asnMission = *new ASN1T_Mission_Pion_INF_RepondreAAggression();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_repondre_a_aggression;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_repondre_a_aggression = &asnMission;
    CreateAgentKnowledge( asnMission.aggresseur, "Aggresseur");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_INF_AppuyerSurPosition
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_INF_AppuyerSurPosition()
{
    ASN1T_Mission_Pion_INF_AppuyerSurPosition& asnMission = *new ASN1T_Mission_Pion_INF_AppuyerSurPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_appuyer_sur_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_inf_appuyer_sur_position = &asnMission;
    CreatePoint( asnMission.position, "Position");
    CreateAgentList( asnMission.unites_a_appuyer, "Unites a appuyer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_SeDeployer
// Created: AGR
// -----------------------------------------------------------------------------
class OptionalParamFunctor_Mission_Pion_LOG_SeDeployer_position_deploiement : public OptionalParamFunctor_ABC
{
public:
    OptionalParamFunctor_Mission_Pion_LOG_SeDeployer_position_deploiement( ASN1T_Mission_Pion_LOG_SeDeployer&asnMission )
      : pAsnMission_( &asnMission ){}
    virtual void SetOptionalPresent(){
        pAsnMission_->m.position_deploiementPresent = 1;}
private:
    ASN1T_Mission_Pion_LOG_SeDeployer* pAsnMission_;
};

void UnitMissionInterface::CreateMission_LOG_SeDeployer()
{
    ASN1T_Mission_Pion_LOG_SeDeployer& asnMission = *new ASN1T_Mission_Pion_LOG_SeDeployer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_se_deployer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_se_deployer = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement", BuildOptionalParamFunctor< OptionalParamFunctor_Mission_Pion_LOG_SeDeployer_position_deploiement, ASN1T_Mission_Pion_LOG_SeDeployer>( asnMission ));
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_Renforcer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_Renforcer()
{
    ASN1T_Mission_Pion_LOG_Renforcer& asnMission = *new ASN1T_Mission_Pion_LOG_Renforcer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_renforcer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_renforcer = &asnMission;
    CreatePoint( asnMission.lieu_renforcement, "Lieu renforcement");
    CreateAutomate( asnMission.automate, "Automate");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_PreparerBascule
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_PreparerBascule()
{
    ASN1T_Mission_Pion_LOG_PreparerBascule& asnMission = *new ASN1T_Mission_Pion_LOG_PreparerBascule();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_preparer_bascule;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_preparer_bascule = &asnMission;
    CreateAutomate( asnMission.automate_pour_bascule, "Automate pour bascule");
    CreateBool( asnMission.rester_sur_place, "Rester sur place");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_DecontaminerBlesses
// Created: AGR
// -----------------------------------------------------------------------------
class OptionalParamFunctor_Mission_Pion_LOG_DecontaminerBlesses_position_deploiement : public OptionalParamFunctor_ABC
{
public:
    OptionalParamFunctor_Mission_Pion_LOG_DecontaminerBlesses_position_deploiement( ASN1T_Mission_Pion_LOG_DecontaminerBlesses&asnMission )
      : pAsnMission_( &asnMission ){}
    virtual void SetOptionalPresent(){
        pAsnMission_->m.position_deploiementPresent = 1;}
private:
    ASN1T_Mission_Pion_LOG_DecontaminerBlesses* pAsnMission_;
};

void UnitMissionInterface::CreateMission_LOG_DecontaminerBlesses()
{
    ASN1T_Mission_Pion_LOG_DecontaminerBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_DecontaminerBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_decontaminer_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_decontaminer_blesses = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement", BuildOptionalParamFunctor< OptionalParamFunctor_Mission_Pion_LOG_DecontaminerBlesses_position_deploiement, ASN1T_Mission_Pion_LOG_DecontaminerBlesses>( asnMission ));
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_TrierEtTraiterBlesses
// Created: AGR
// -----------------------------------------------------------------------------
class OptionalParamFunctor_Mission_Pion_LOG_TrierEtTraiterBlesses_position_deploiement : public OptionalParamFunctor_ABC
{
public:
    OptionalParamFunctor_Mission_Pion_LOG_TrierEtTraiterBlesses_position_deploiement( ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses&asnMission )
      : pAsnMission_( &asnMission ){}
    virtual void SetOptionalPresent(){
        pAsnMission_->m.position_deploiementPresent = 1;}
private:
    ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses* pAsnMission_;
};

void UnitMissionInterface::CreateMission_LOG_TrierEtTraiterBlesses()
{
    ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_trier_et_traiter_blesses = &asnMission;
    CreateMedicalPriorities( asnMission.blessures_traitees, "Blessures traitees");
    CreatePoint( asnMission.position_deploiement, "Position deploiement", BuildOptionalParamFunctor< OptionalParamFunctor_Mission_Pion_LOG_TrierEtTraiterBlesses_position_deploiement, ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses>( asnMission ));
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_TrierBlesses
// Created: AGR
// -----------------------------------------------------------------------------
class OptionalParamFunctor_Mission_Pion_LOG_TrierBlesses_position_deploiement : public OptionalParamFunctor_ABC
{
public:
    OptionalParamFunctor_Mission_Pion_LOG_TrierBlesses_position_deploiement( ASN1T_Mission_Pion_LOG_TrierBlesses&asnMission )
      : pAsnMission_( &asnMission ){}
    virtual void SetOptionalPresent(){
        pAsnMission_->m.position_deploiementPresent = 1;}
private:
    ASN1T_Mission_Pion_LOG_TrierBlesses* pAsnMission_;
};

void UnitMissionInterface::CreateMission_LOG_TrierBlesses()
{
    ASN1T_Mission_Pion_LOG_TrierBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TrierBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_trier_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_trier_blesses = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement", BuildOptionalParamFunctor< OptionalParamFunctor_Mission_Pion_LOG_TrierBlesses_position_deploiement, ASN1T_Mission_Pion_LOG_TrierBlesses>( asnMission ));
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_TraiterBlesses
// Created: AGR
// -----------------------------------------------------------------------------
class OptionalParamFunctor_Mission_Pion_LOG_TraiterBlesses_position_deploiement : public OptionalParamFunctor_ABC
{
public:
    OptionalParamFunctor_Mission_Pion_LOG_TraiterBlesses_position_deploiement( ASN1T_Mission_Pion_LOG_TraiterBlesses&asnMission )
      : pAsnMission_( &asnMission ){}
    virtual void SetOptionalPresent(){
        pAsnMission_->m.position_deploiementPresent = 1;}
private:
    ASN1T_Mission_Pion_LOG_TraiterBlesses* pAsnMission_;
};

void UnitMissionInterface::CreateMission_LOG_TraiterBlesses()
{
    ASN1T_Mission_Pion_LOG_TraiterBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TraiterBlesses();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_traiter_blesses;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_traiter_blesses = &asnMission;
    CreateMedicalPriorities( asnMission.blessures_traitees, "Blessures traitees");
    CreatePoint( asnMission.position_deploiement, "Position deploiement", BuildOptionalParamFunctor< OptionalParamFunctor_Mission_Pion_LOG_TraiterBlesses_position_deploiement, ASN1T_Mission_Pion_LOG_TraiterBlesses>( asnMission ));
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_PrendreEnCompteReactionsMentales
// Created: AGR
// -----------------------------------------------------------------------------
class OptionalParamFunctor_Mission_Pion_LOG_PrendreEnCompteReactionsMentales_position_deploiement : public OptionalParamFunctor_ABC
{
public:
    OptionalParamFunctor_Mission_Pion_LOG_PrendreEnCompteReactionsMentales_position_deploiement( ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales&asnMission )
      : pAsnMission_( &asnMission ){}
    virtual void SetOptionalPresent(){
        pAsnMission_->m.position_deploiementPresent = 1;}
private:
    ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales* pAsnMission_;
};

void UnitMissionInterface::CreateMission_LOG_PrendreEnCompteReactionsMentales()
{
    ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& asnMission = *new ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_prendre_en_compte_reactions_mentales = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement", BuildOptionalParamFunctor< OptionalParamFunctor_Mission_Pion_LOG_PrendreEnCompteReactionsMentales_position_deploiement, ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales>( asnMission ));
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_Convoyer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_Convoyer()
{
    ASN1T_Mission_Pion_LOG_Convoyer& asnMission = *new ASN1T_Mission_Pion_LOG_Convoyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_convoyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_convoyer = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_Reparer
// Created: AGR
// -----------------------------------------------------------------------------
class OptionalParamFunctor_Mission_Pion_LOG_Reparer_position_deploiement : public OptionalParamFunctor_ABC
{
public:
    OptionalParamFunctor_Mission_Pion_LOG_Reparer_position_deploiement( ASN1T_Mission_Pion_LOG_Reparer&asnMission )
      : pAsnMission_( &asnMission ){}
    virtual void SetOptionalPresent(){
        pAsnMission_->m.position_deploiementPresent = 1;}
private:
    ASN1T_Mission_Pion_LOG_Reparer* pAsnMission_;
};

void UnitMissionInterface::CreateMission_LOG_Reparer()
{
    ASN1T_Mission_Pion_LOG_Reparer& asnMission = *new ASN1T_Mission_Pion_LOG_Reparer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reparer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reparer = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement", BuildOptionalParamFunctor< OptionalParamFunctor_Mission_Pion_LOG_Reparer_position_deploiement, ASN1T_Mission_Pion_LOG_Reparer>( asnMission ));
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_AppuyerMouvement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_AppuyerMouvement()
{
    ASN1T_Mission_Pion_LOG_AppuyerMouvement& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerMouvement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_mouvement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement = &asnMission;
    CreateAgent( asnMission.unite_a_appuyer, "Unite a appuyer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_ReconnaitreItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_ReconnaitreItineraire()
{
    ASN1T_Mission_Pion_LOG_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Pion_LOG_ReconnaitreItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reconnaitre_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_itineraire = &asnMission;
    CreatePath( asnMission.itineraire_log, "Itineraire log");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_ReconnaitreZoneContaminee
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_ReconnaitreZoneContaminee()
{
    ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee& asnMission = *new ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reconnaitre_zone_contaminee;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_zone_contaminee = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_AppuyerMouvementDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_AppuyerMouvementDansZone()
{
    ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_mouvement_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_MettreEnOeuvreZoneStationnement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_MettreEnOeuvreZoneStationnement()
{
    ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement& asnMission = *new ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_mettre_en_oeuvre_zone_stationnement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_mettre_en_oeuvre_zone_stationnement = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_AppuyerFranchissementSurUnPoint
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_AppuyerFranchissementSurUnPoint()
{
    ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_appuyer_franchissement_sur_un_point = &asnMission;
    CreateObjectKnowledge( asnMission.pont_flottant, "Pont flottant");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_BloquerZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_BloquerZone()
{
    ASN1T_Mission_Pion_LOG_BloquerZone& asnMission = *new ASN1T_Mission_Pion_LOG_BloquerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_bloquer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_bloquer_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_ReconnaitreZoneDeDeploiement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_ReconnaitreZoneDeDeploiement()
{
    ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement& asnMission = *new ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_reconnaitre_zone_de_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_reconnaitre_zone_de_deploiement = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_ArmerPIA
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_ArmerPIA()
{
    ASN1T_Mission_Pion_LOG_ArmerPIA& asnMission = *new ASN1T_Mission_Pion_LOG_ArmerPIA();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_armer_pia;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_armer_pia = &asnMission;
    CreatePoint( asnMission.point, "Point");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_LOG_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_LOG_Surveiller()
{
    ASN1T_Mission_Pion_LOG_Surveiller& asnMission = *new ASN1T_Mission_Pion_LOG_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_log_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_Heliporter
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_Heliporter()
{
    ASN1T_Mission_Pion_Test_Heliporter& asnMission = *new ASN1T_Mission_Pion_Test_Heliporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_heliporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_heliporter = &asnMission;
    CreateAgentList( asnMission.agents, "Agents");
    CreatePoint( asnMission.point_embarquement, "Point embarquement");
    CreatePoint( asnMission.point_debarquement, "Point debarquement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_MoveTo
// Created: AGR
// -----------------------------------------------------------------------------
class OptionalParamFunctor_Mission_Pion_Test_MoveTo_vision_point : public OptionalParamFunctor_ABC
{
public:
    OptionalParamFunctor_Mission_Pion_Test_MoveTo_vision_point( ASN1T_Mission_Pion_Test_MoveTo&asnMission )
      : pAsnMission_( &asnMission ){}
    virtual void SetOptionalPresent(){
        pAsnMission_->m.vision_pointPresent = 1;}
private:
    ASN1T_Mission_Pion_Test_MoveTo* pAsnMission_;
};

void UnitMissionInterface::CreateMission_Test_MoveTo()
{
    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *new ASN1T_Mission_Pion_Test_MoveTo();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_move_to;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_move_to = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    ParamComboBox< ASN1T_Mission_Pion_Test_MoveTo_type_itineraire >* pSelector_type_itineraire = &CreateVarList( asnMission.type_itineraire, "Type itineraire");
    pSelector_type_itineraire->AddItem( "Movement", Mission_Pion_Test_MoveTo_type_itineraire::movement );
    pSelector_type_itineraire->AddItem( "Recon", Mission_Pion_Test_MoveTo_type_itineraire::recon );
    pSelector_type_itineraire->AddItem( "Infiltration", Mission_Pion_Test_MoveTo_type_itineraire::infiltration );
    pSelector_type_itineraire->AddItem( "Assault", Mission_Pion_Test_MoveTo_type_itineraire::assault );
    pSelector_type_itineraire->AddItem( "Retreat", Mission_Pion_Test_MoveTo_type_itineraire::retreat );
    pSelector_type_itineraire->AddItem( "Backup", Mission_Pion_Test_MoveTo_type_itineraire::backup );
    pSelector_type_itineraire->AddItem( "Mine clearance", Mission_Pion_Test_MoveTo_type_itineraire::mine_clearance );
    pSelector_type_itineraire->AddItem( "Logistic", Mission_Pion_Test_MoveTo_type_itineraire::logistic );
    CreateBool( asnMission.debarque, "Debarque");
    ParamComboBox< ASN1T_Mission_Pion_Test_MoveTo_verrouillage_vision >* pSelector_verrouillage_vision = &CreateVarList( asnMission.verrouillage_vision, "Verrouillage vision");
    pSelector_verrouillage_vision->AddItem( "Rien", Mission_Pion_Test_MoveTo_verrouillage_vision::rien );
    pSelector_verrouillage_vision->AddItem( "Point", Mission_Pion_Test_MoveTo_verrouillage_vision::point );
    pSelector_verrouillage_vision->AddItem( "Direction", Mission_Pion_Test_MoveTo_verrouillage_vision::direction );
    CreatePoint( asnMission.vision_point, "Vision point", BuildOptionalParamFunctor< OptionalParamFunctor_Mission_Pion_Test_MoveTo_vision_point, ASN1T_Mission_Pion_Test_MoveTo>( asnMission ));
    CreateDirection( asnMission.vision_direction, "Vision direction");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_CreateObject
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_CreateObject()
{
    ASN1T_Mission_Pion_Test_CreateObject& asnMission = *new ASN1T_Mission_Pion_Test_CreateObject();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_create_object;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_create_object = &asnMission;
    CreateLocation( asnMission.forme, "Forme");
    ParamComboBox< ASN1T_EnumObjectType >* pSelector_type = &CreateVarList( asnMission.type, "Type");
    pSelector_type->AddItem( "Bouchon mines", EnumObjectType::bouchon_mines );
    pSelector_type->AddItem( "Zone minee lineaire", EnumObjectType::zone_minee_lineaire );
    pSelector_type->AddItem( "Zone minee par dispersion", EnumObjectType::zone_minee_par_dispersion );
    pSelector_type->AddItem( "Fosse anti char", EnumObjectType::fosse_anti_char );
    pSelector_type->AddItem( "Abattis", EnumObjectType::abattis );
    pSelector_type->AddItem( "Barricade", EnumObjectType::barricade );
    pSelector_type->AddItem( "Eboulement", EnumObjectType::eboulement );
    pSelector_type->AddItem( "Destruction route", EnumObjectType::destruction_route );
    pSelector_type->AddItem( "Destruction pont", EnumObjectType::destruction_pont );
    pSelector_type->AddItem( "Pont flottant continu", EnumObjectType::pont_flottant_continu );
    pSelector_type->AddItem( "Poste tir", EnumObjectType::poste_tir );
    pSelector_type->AddItem( "Zone protegee", EnumObjectType::zone_protegee );
    pSelector_type->AddItem( "Zone implantation canon", EnumObjectType::zone_implantation_canon );
    pSelector_type->AddItem( "Zone implantation cobra", EnumObjectType::zone_implantation_cobra );
    pSelector_type->AddItem( "Zone implantation lrm", EnumObjectType::zone_implantation_lrm );
    pSelector_type->AddItem( "Site franchissement", EnumObjectType::site_franchissement );
    pSelector_type->AddItem( "Nuage nbc", EnumObjectType::nuage_nbc );
    pSelector_type->AddItem( "Plot ravitaillement", EnumObjectType::plot_ravitaillement );
    pSelector_type->AddItem( "Site decontamination", EnumObjectType::site_decontamination );
    pSelector_type->AddItem( "Zone brouillage brod", EnumObjectType::zone_brouillage_brod );
    pSelector_type->AddItem( "Rota", EnumObjectType::rota );
    pSelector_type->AddItem( "Zone nbc", EnumObjectType::zone_nbc );
    pSelector_type->AddItem( "Zone brouillage bromure", EnumObjectType::zone_brouillage_bromure );
    pSelector_type->AddItem( "Aire poser", EnumObjectType::aire_poser );
    pSelector_type->AddItem( "Piste", EnumObjectType::piste );
    pSelector_type->AddItem( "Plateforme", EnumObjectType::plateforme );
    pSelector_type->AddItem( "Zone mobilite amelioree", EnumObjectType::zone_mobilite_amelioree );
    pSelector_type->AddItem( "Zone poser helicoptere", EnumObjectType::zone_poser_helicoptere );
    pSelector_type->AddItem( "Aire logistique", EnumObjectType::aire_logistique );
    pSelector_type->AddItem( "Itineraire logistique", EnumObjectType::itineraire_logistique );
    pSelector_type->AddItem( "Camp prisonniers", EnumObjectType::camp_prisonniers );
    pSelector_type->AddItem( "Camp refugies", EnumObjectType::camp_refugies );
    pSelector_type->AddItem( "Poste controle", EnumObjectType::poste_controle );
    pSelector_type->AddItem( "Terrain largage", EnumObjectType::terrain_largage );
    pSelector_type->AddItem( "Zone interdite mouvement", EnumObjectType::zone_interdite_mouvement );
    pSelector_type->AddItem( "Zone interdite tir", EnumObjectType::zone_interdite_tir );
    pSelector_type->AddItem( "Zone implantation mortier", EnumObjectType::zone_implantation_mortier );
    pSelector_type->AddItem( "Pont flottant discontinu", EnumObjectType::pont_flottant_discontinu );
    pSelector_type->AddItem( "Installation", EnumObjectType::installation );
    CreateNumeric( asnMission.param, "Param");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_DestroyObject
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_DestroyObject()
{
    ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *new ASN1T_Mission_Pion_Test_DestroyObject();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_destroy_object;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_destroy_object = &asnMission;
    CreateObjectKnowledge( asnMission.objet, "Objet");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_CreateBypass
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_CreateBypass()
{
    ASN1T_Mission_Pion_Test_CreateBypass& asnMission = *new ASN1T_Mission_Pion_Test_CreateBypass();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_create_bypass;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_create_bypass = &asnMission;
    CreateObjectKnowledge( asnMission.objet, "Objet");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_Fire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_Fire()
{
    ASN1T_Mission_Pion_Test_Fire& asnMission = *new ASN1T_Mission_Pion_Test_Fire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_fire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_fire = &asnMission;
    CreatePoint( asnMission.point, "Point");
    CreateBool( asnMission.tir_indirect, "Tir indirect");
    CreateDotationDType( asnMission.munitions, "Munitions");
    CreateNumeric( asnMission.nb_it, "Nb it");
    CreateAgentKnowledgeList( asnMission.targets, "Targets");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_ChangePosture
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_ChangePosture()
{
    ASN1T_Mission_Pion_Test_ChangePosture& asnMission = *new ASN1T_Mission_Pion_Test_ChangePosture();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_change_posture;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_change_posture = &asnMission;
    ParamComboBox< ASN1T_Mission_Pion_Test_ChangePosture_posture >* pSelector_posture = &CreateVarList( asnMission.posture, "Posture");
    pSelector_posture->AddItem( "Mouvement", Mission_Pion_Test_ChangePosture_posture::mouvement );
    pSelector_posture->AddItem( "Arret", Mission_Pion_Test_ChangePosture_posture::arret );
    pSelector_posture->AddItem( "Poste reflexe", Mission_Pion_Test_ChangePosture_posture::poste_reflexe );
    pSelector_posture->AddItem( "Poste", Mission_Pion_Test_ChangePosture_posture::poste );
    pSelector_posture->AddItem( "Poste amenage", Mission_Pion_Test_ChangePosture_posture::poste_amenage );
    pSelector_posture->AddItem( "Poste prepare genie", Mission_Pion_Test_ChangePosture_posture::poste_prepare_genie );
    pSelector_posture->AddItem( "Automatique", Mission_Pion_Test_ChangePosture_posture::automatique );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_PathFind
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_PathFind()
{
    ASN1T_Mission_Pion_Test_PathFind& asnMission = *new ASN1T_Mission_Pion_Test_PathFind();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_path_find;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_path_find = &asnMission;
    CreateNumeric( asnMission.nb_pathfind, "Nb pathfind");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_FindPosition
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_FindPosition()
{
    ASN1T_Mission_Pion_Test_FindPosition& asnMission = *new ASN1T_Mission_Pion_Test_FindPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_find_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_find_position = &asnMission;
    CreatePoint( asnMission.point, "Point");
    CreatePoint( asnMission.retreat, "Retreat");
    CreateAgentList( asnMission.unite, "Unite");
    ParamComboBox< ASN1T_Mission_Pion_Test_FindPosition_type_objectif >* pSelector_type_objectif = &CreateVarList( asnMission.type_objectif, "Type objectif");
    pSelector_type_objectif->AddItem( "Agent", Mission_Pion_Test_FindPosition_type_objectif::agent );
    pSelector_type_objectif->AddItem( "Point", Mission_Pion_Test_FindPosition_type_objectif::point );
    pSelector_type_objectif->AddItem( "Rep point", Mission_Pion_Test_FindPosition_type_objectif::rep_point );
    pSelector_type_objectif->AddItem( "Objet", Mission_Pion_Test_FindPosition_type_objectif::objet );
    pSelector_type_objectif->AddItem( "Zone", Mission_Pion_Test_FindPosition_type_objectif::zone );
    ParamComboBox< ASN1T_Mission_Pion_Test_FindPosition_type_operation >* pSelector_type_operation = &CreateVarList( asnMission.type_operation, "Type operation");
    pSelector_type_operation->AddItem( "See", Mission_Pion_Test_FindPosition_type_operation::see );
    pSelector_type_operation->AddItem( "Shoot", Mission_Pion_Test_FindPosition_type_operation::shoot );
    pSelector_type_operation->AddItem( "Add", Mission_Pion_Test_FindPosition_type_operation::add );
    pSelector_type_operation->AddItem( "Rem", Mission_Pion_Test_FindPosition_type_operation::rem );
    pSelector_type_operation->AddItem( "Safety", Mission_Pion_Test_FindPosition_type_operation::safety );
    pSelector_type_operation->AddItem( "Border", Mission_Pion_Test_FindPosition_type_operation::border );
    pSelector_type_operation->AddItem( "Appui", Mission_Pion_Test_FindPosition_type_operation::appui );
    pSelector_type_operation->AddItem( "Support", Mission_Pion_Test_FindPosition_type_operation::support );
    pSelector_type_operation->AddItem( "Ambush", Mission_Pion_Test_FindPosition_type_operation::ambush );
    pSelector_type_operation->AddItem( "Random", Mission_Pion_Test_FindPosition_type_operation::random );
    CreateBool( asnMission.avoid, "Avoid");
    CreateBool( asnMission.reco, "Reco");
    CreateBool( asnMission.no_obstacle_avoid, "No obstacle avoid");
    CreateBool( asnMission.debordement, "Debordement");
    CreateBool( asnMission.dec_points, "Dec points");
    ParamComboBox< ASN1T_EnumUnitIdentificationLevel >* pSelector_niveau_identification = &CreateVarList( asnMission.niveau_identification, "Niveau identification");
    pSelector_niveau_identification->AddItem( "Identifiee", EnumUnitIdentificationLevel::identifiee );
    pSelector_niveau_identification->AddItem( "Reconnue", EnumUnitIdentificationLevel::reconnue );
    pSelector_niveau_identification->AddItem( "Detectee", EnumUnitIdentificationLevel::detectee );
    pSelector_niveau_identification->AddItem( "Signale", EnumUnitIdentificationLevel::signale );
    CreateDirection( asnMission.dgo, "Dgo");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Test_Reinforce
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Test_Reinforce()
{
    ASN1T_Mission_Pion_Test_Reinforce& asnMission = *new ASN1T_Mission_Pion_Test_Reinforce();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_reinforce;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_reinforce = &asnMission;
    CreateAgent( asnMission.unite, "Unite");
    CreateBool( asnMission.renforce, "Renforce");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_PlastronEnnemi
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_PlastronEnnemi()
{
    ASN1T_Mission_Pion_PlastronEnnemi& asnMission = *new ASN1T_Mission_Pion_PlastronEnnemi();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_plastron_ennemi;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_plastron_ennemi = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    CreatePoint( asnMission.point_mission, "Point mission");
    ParamComboBox< ASN1T_Mission_Pion_PlastronEnnemi_type >* pSelector_type = &CreateVarList( asnMission.type, "Type");
    pSelector_type->AddItem( "Defense statique", Mission_Pion_PlastronEnnemi_type::defense_statique );
    pSelector_type->AddItem( "Defense dynamique", Mission_Pion_PlastronEnnemi_type::defense_dynamique );
    pSelector_type->AddItem( "Esquive", Mission_Pion_PlastronEnnemi_type::esquive );
    pSelector_type->AddItem( "Reco offensive", Mission_Pion_PlastronEnnemi_type::reco_offensive );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_FaireMouvement
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_FaireMouvement()
{
    ASN1T_Mission_Pion_FaireMouvement& asnMission = *new ASN1T_Mission_Pion_FaireMouvement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_faire_mouvement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_faire_mouvement = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Suivre
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Suivre()
{
    ASN1T_Mission_Pion_Suivre& asnMission = *new ASN1T_Mission_Pion_Suivre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_suivre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_suivre = &asnMission;
    CreateAgent( asnMission.unite_a_suivre, "Unite a suivre");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Transporter
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Transporter()
{
    ASN1T_Mission_Pion_Transporter& asnMission = *new ASN1T_Mission_Pion_Transporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_transporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_transporter = &asnMission;
    CreateAgentList( asnMission.unites_a_transporter, "Unites a transporter");
    CreatePoint( asnMission.point_debarquement, "Point debarquement");
    CreatePoint( asnMission.point_embarquement, "Point embarquement");
    CreateBool( asnMission.avec_materiel, "Avec materiel");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_SeFaireDecontaminer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_SeFaireDecontaminer()
{
    ASN1T_Mission_Pion_SeFaireDecontaminer& asnMission = *new ASN1T_Mission_Pion_SeFaireDecontaminer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_se_faire_decontaminer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_se_faire_decontaminer = &asnMission;
    CreatePoint( asnMission.point_arrivee, "Point arrivee");
    CreateObjectKnowledge( asnMission.site_decontamination, "Site decontamination");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Franchir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Franchir()
{
    ASN1T_Mission_Pion_Franchir& asnMission = *new ASN1T_Mission_Pion_Franchir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_franchir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_franchir = &asnMission;
    CreateObjectKnowledge( asnMission.pont_flottant, "Pont flottant");
    CreatePoint( asnMission.point_arrivee, "Point arrivee");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Decrocher
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Decrocher()
{
    ASN1T_Mission_Pion_Decrocher& asnMission = *new ASN1T_Mission_Pion_Decrocher();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_decrocher;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_decrocher = &asnMission;
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Stationner
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Stationner()
{
    ASN1T_Mission_Pion_Stationner& asnMission = *new ASN1T_Mission_Pion_Stationner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_stationner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_stationner = &asnMission;
    CreatePoint( asnMission.point_attente, "Point attente");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_SeFaireTransporter
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_SeFaireTransporter()
{
    ASN1T_Mission_Pion_SeFaireTransporter& asnMission = *new ASN1T_Mission_Pion_SeFaireTransporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_se_faire_transporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_se_faire_transporter = &asnMission;
    CreatePoint( asnMission.point_embarquement, "Point embarquement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_InterdireFranchissementPopulations
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_InterdireFranchissementPopulations()
{
    ASN1T_Mission_Pion_InterdireFranchissementPopulations& asnMission = *new ASN1T_Mission_Pion_InterdireFranchissementPopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_interdire_franchissement_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_interdire_franchissement_populations = &asnMission;
    CreatePoint( asnMission.point, "Point");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_FiltrerPopulations
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_FiltrerPopulations()
{
    ASN1T_Mission_Pion_FiltrerPopulations& asnMission = *new ASN1T_Mission_Pion_FiltrerPopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_filtrer_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_filtrer_populations = &asnMission;
    CreatePoint( asnMission.point, "Point");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RenseignerSurPopulations
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RenseignerSurPopulations()
{
    ASN1T_Mission_Pion_RenseignerSurPopulations& asnMission = *new ASN1T_Mission_Pion_RenseignerSurPopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_renseigner_sur_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_renseigner_sur_populations = &asnMission;
    CreatePoint( asnMission.point, "Point");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_CanaliserPopulations
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_CanaliserPopulations()
{
    ASN1T_Mission_Pion_CanaliserPopulations& asnMission = *new ASN1T_Mission_Pion_CanaliserPopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_canaliser_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_canaliser_populations = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ControlerPopulationsDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ControlerPopulationsDansZone()
{
    ASN1T_Mission_Pion_ControlerPopulationsDansZone& asnMission = *new ASN1T_Mission_Pion_ControlerPopulationsDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_controler_populations_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_controler_populations_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_SecuriserZoneContrePopulations
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_SecuriserZoneContrePopulations()
{
    ASN1T_Mission_Pion_SecuriserZoneContrePopulations& asnMission = *new ASN1T_Mission_Pion_SecuriserZoneContrePopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_securiser_zone_contre_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_securiser_zone_contre_populations = &asnMission;
    CreatePoint( asnMission.point, "Point");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_Generique
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_Generique()
{
    ASN1T_Mission_Pion_Generique& asnMission = *new ASN1T_Mission_Pion_Generique();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_generique;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_generique = &asnMission;
    CreatePolygonList( asnMission.zones, "Zones");
    CreatePointList( asnMission.points, "Points");
    CreatePathList( asnMission.itineraires, "Itineraires");
    CreateAgentList( asnMission.unites, "Unites");
    CreateAutomateList( asnMission.automates, "Automates");
    CreateAgentKnowledgeList( asnMission.connaissances_agents, "Connaissances agents");
    CreateObjectKnowledgeList( asnMission.connaissances_objets, "Connaissances objets");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_MILICE_Provoquer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_MILICE_Provoquer()
{
    ASN1T_Mission_Pion_MILICE_Provoquer& asnMission = *new ASN1T_Mission_Pion_MILICE_Provoquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_milice_provoquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_provoquer = &asnMission;
    CreateAgentKnowledge( asnMission.unite_a_provoquer, "Unite a provoquer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_MILICE_PrendreEtTenir
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_MILICE_PrendreEtTenir()
{
    ASN1T_Mission_Pion_MILICE_PrendreEtTenir& asnMission = *new ASN1T_Mission_Pion_MILICE_PrendreEtTenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_milice_prendre_et_tenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_prendre_et_tenir = &asnMission;
    CreatePoint( asnMission.point_a_prendre, "Point a prendre");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_MILICE_DetruireEmbuscade
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_MILICE_DetruireEmbuscade()
{
    ASN1T_Mission_Pion_MILICE_DetruireEmbuscade& asnMission = *new ASN1T_Mission_Pion_MILICE_DetruireEmbuscade();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_milice_detruire_embuscade;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_milice_detruire_embuscade = &asnMission;
    CreatePoint( asnMission.position_embuscade, "Position embuscade");
    CreatePoint( asnMission.position_regroupement, "Position regroupement");
    ParamComboBox< ASN1T_EnumObjectType >* pSelector_nature_obstacle = &CreateVarList( asnMission.nature_obstacle, "Nature obstacle");
    pSelector_nature_obstacle->AddItem( "Bouchon mines", EnumObjectType::bouchon_mines );
    pSelector_nature_obstacle->AddItem( "Zone minee lineaire", EnumObjectType::zone_minee_lineaire );
    pSelector_nature_obstacle->AddItem( "Zone minee par dispersion", EnumObjectType::zone_minee_par_dispersion );
    pSelector_nature_obstacle->AddItem( "Fosse anti char", EnumObjectType::fosse_anti_char );
    pSelector_nature_obstacle->AddItem( "Abattis", EnumObjectType::abattis );
    pSelector_nature_obstacle->AddItem( "Barricade", EnumObjectType::barricade );
    pSelector_nature_obstacle->AddItem( "Eboulement", EnumObjectType::eboulement );
    pSelector_nature_obstacle->AddItem( "Destruction route", EnumObjectType::destruction_route );
    pSelector_nature_obstacle->AddItem( "Destruction pont", EnumObjectType::destruction_pont );
    pSelector_nature_obstacle->AddItem( "Pont flottant continu", EnumObjectType::pont_flottant_continu );
    pSelector_nature_obstacle->AddItem( "Poste tir", EnumObjectType::poste_tir );
    pSelector_nature_obstacle->AddItem( "Zone protegee", EnumObjectType::zone_protegee );
    pSelector_nature_obstacle->AddItem( "Zone implantation canon", EnumObjectType::zone_implantation_canon );
    pSelector_nature_obstacle->AddItem( "Zone implantation cobra", EnumObjectType::zone_implantation_cobra );
    pSelector_nature_obstacle->AddItem( "Zone implantation lrm", EnumObjectType::zone_implantation_lrm );
    pSelector_nature_obstacle->AddItem( "Site franchissement", EnumObjectType::site_franchissement );
    pSelector_nature_obstacle->AddItem( "Nuage nbc", EnumObjectType::nuage_nbc );
    pSelector_nature_obstacle->AddItem( "Plot ravitaillement", EnumObjectType::plot_ravitaillement );
    pSelector_nature_obstacle->AddItem( "Site decontamination", EnumObjectType::site_decontamination );
    pSelector_nature_obstacle->AddItem( "Zone brouillage brod", EnumObjectType::zone_brouillage_brod );
    pSelector_nature_obstacle->AddItem( "Rota", EnumObjectType::rota );
    pSelector_nature_obstacle->AddItem( "Zone nbc", EnumObjectType::zone_nbc );
    pSelector_nature_obstacle->AddItem( "Zone brouillage bromure", EnumObjectType::zone_brouillage_bromure );
    pSelector_nature_obstacle->AddItem( "Aire poser", EnumObjectType::aire_poser );
    pSelector_nature_obstacle->AddItem( "Piste", EnumObjectType::piste );
    pSelector_nature_obstacle->AddItem( "Plateforme", EnumObjectType::plateforme );
    pSelector_nature_obstacle->AddItem( "Zone mobilite amelioree", EnumObjectType::zone_mobilite_amelioree );
    pSelector_nature_obstacle->AddItem( "Zone poser helicoptere", EnumObjectType::zone_poser_helicoptere );
    pSelector_nature_obstacle->AddItem( "Aire logistique", EnumObjectType::aire_logistique );
    pSelector_nature_obstacle->AddItem( "Itineraire logistique", EnumObjectType::itineraire_logistique );
    pSelector_nature_obstacle->AddItem( "Camp prisonniers", EnumObjectType::camp_prisonniers );
    pSelector_nature_obstacle->AddItem( "Camp refugies", EnumObjectType::camp_refugies );
    pSelector_nature_obstacle->AddItem( "Poste controle", EnumObjectType::poste_controle );
    pSelector_nature_obstacle->AddItem( "Terrain largage", EnumObjectType::terrain_largage );
    pSelector_nature_obstacle->AddItem( "Zone interdite mouvement", EnumObjectType::zone_interdite_mouvement );
    pSelector_nature_obstacle->AddItem( "Zone interdite tir", EnumObjectType::zone_interdite_tir );
    pSelector_nature_obstacle->AddItem( "Zone implantation mortier", EnumObjectType::zone_implantation_mortier );
    pSelector_nature_obstacle->AddItem( "Pont flottant discontinu", EnumObjectType::pont_flottant_discontinu );
    pSelector_nature_obstacle->AddItem( "Installation", EnumObjectType::installation );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_JOINT_AIR_EffectuerRaid
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_JOINT_AIR_EffectuerRaid()
{
    ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid& asnMission = *new ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_joint_air_effectuer_raid;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_joint_air_effectuer_raid = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_NBC_AnimerUnPlotDeDecontamination
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_NBC_AnimerUnPlotDeDecontamination()
{
    ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination& asnMission = *new ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_animer_un_plot_de_decontamination;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_animer_un_plot_de_decontamination = &asnMission;
    CreatePoint( asnMission.position_site_decontamination, "Position site decontamination");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_NBC_ReconnaitreItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_NBC_ReconnaitreItineraire()
{
    ASN1T_Mission_Pion_NBC_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_reconnaitre_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_NBC_ReconnaitreZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_NBC_ReconnaitreZone()
{
    ASN1T_Mission_Pion_NBC_ReconnaitreZone& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_reconnaitre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_NBC_DecontaminerZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_NBC_DecontaminerZone()
{
    ASN1T_Mission_Pion_NBC_DecontaminerZone& asnMission = *new ASN1T_Mission_Pion_NBC_DecontaminerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_decontaminer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_decontaminer_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_NBC_ReconnaitreUnSiteROTA
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_NBC_ReconnaitreUnSiteROTA()
{
    ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_un_site_rota = &asnMission;
    CreateObjectKnowledge( asnMission.site_ROTA, "Site ROTA");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROHUM_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROHUM_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_ROHUM_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROHUM_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_rohum_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_renseigner_sur = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "Zone a observer");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROHUM_SExfiltrer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROHUM_SExfiltrer()
{
    ASN1T_Mission_Pion_RENS_ROHUM_SExfiltrer& asnMission = *new ASN1T_Mission_Pion_RENS_ROHUM_SExfiltrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_rohum_sexfiltrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_sexfiltrer = &asnMission;
    CreatePoint( asnMission.point_de_regroupement, "Point de regroupement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROHUM_OrienterGuider
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROHUM_OrienterGuider()
{
    ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider& asnMission = *new ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_rohum_orienter_guider;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_rohum_orienter_guider = &asnMission;
    CreateAgent( asnMission.ami, "Ami");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROEM_Appuyer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROEM_Appuyer()
{
    ASN1T_Mission_Pion_RENS_ROEM_Appuyer& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_appuyer = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROEM_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROEM_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_ROEM_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_renseigner_sur = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROEM_MettreEnOeuvre
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROEM_MettreEnOeuvre()
{
    ASN1T_Mission_Pion_RENS_ROEM_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_MettreEnOeuvre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_mettre_en_oeuvre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roem_mettre_en_oeuvre = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_RADINT_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_RADINT_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_radint_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_radint_renseigner_sur = &asnMission;
    CreatePolygonList( asnMission.zones_de_recherche, "Zones de recherche");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_RADINT_MettreEnOeuvre
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_RADINT_MettreEnOeuvre()
{
    ASN1T_Mission_Pion_RENS_RADINT_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_RADINT_MettreEnOeuvre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_radint_mettre_en_oeuvre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_radint_mettre_en_oeuvre = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROIM_MettreEnOeuvre
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROIM_MettreEnOeuvre()
{
    ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roim_mettre_en_oeuvre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_mettre_en_oeuvre = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROIM_SDTI_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROIM_SDTI_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roim_sdti_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_sdti_renseigner_sur = &asnMission;
    CreatePath( asnMission.ficelle_de_vole, "Ficelle de vole");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_RENS_ROIM_CL289_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_RENS_ROIM_CL289_RenseignerSur()
{
    ASN1T_Mission_Pion_RENS_ROIM_CL289_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROIM_CL289_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roim_cl289_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_rens_roim_cl289_renseigner_sur = &asnMission;
    CreatePath( asnMission.ficelle_de_vole, "Ficelle de vole");
    CreatePolygonList( asnMission.zones_de_recherche, "Zones de recherche");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_TRANS_EtablirGererLiaison
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_TRANS_EtablirGererLiaison()
{
    ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& asnMission = *new ASN1T_Mission_Pion_TRANS_EtablirGererLiaison();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_trans_etablir_gerer_liaison = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASY_SInfiltrer
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASY_SInfiltrer()
{
    ASN1T_Mission_Pion_ASY_SInfiltrer& asnMission = *new ASN1T_Mission_Pion_ASY_SInfiltrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_sinfiltrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_sinfiltrer = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASY_CommettreAttentatContrePopulation
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASY_CommettreAttentatContrePopulation()
{
    ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_commettre_attentatContrePopulation;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContrePopulation = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    CreatePoint( asnMission.point_repli, "Point repli");
    CreatePopulationKnowledge( asnMission.population_cible, "Population cible");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASY_CommettreAttentatContreForcesArmees
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASY_CommettreAttentatContreForcesArmees()
{
    ASN1T_Mission_Pion_ASY_CommettreAttentatContreForcesArmees& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentatContreForcesArmees();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_commettre_attentatContreForcesArmees;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContreForcesArmees = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    CreatePoint( asnMission.point_repli, "Point repli");
    ParamComboBox< ASN1T_Mission_Pion_ASY_CommettreAttentatContreForcesArmees_type_attentat_forces_armees >* pSelector_type_attentat_forces_armees = &CreateVarList( asnMission.type_attentat_forces_armees, "Type attentat forces armees");
    pSelector_type_attentat_forces_armees->AddItem( "Assasinat officier", Mission_Pion_ASY_CommettreAttentatContreForcesArmees_type_attentat_forces_armees::assasinat_officier );
    pSelector_type_attentat_forces_armees->AddItem( "Destruction composante majeure", Mission_Pion_ASY_CommettreAttentatContreForcesArmees_type_attentat_forces_armees::destruction_composante_majeure );
    CreateAgentKnowledge( asnMission.unite_cible, "Unite cible");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASY_CommettreAttentatContreInstallation
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASY_CommettreAttentatContreInstallation()
{
    ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_commettre_attentatContreInstallation;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContreInstallation = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
    CreatePoint( asnMission.point_repli, "Point repli");
    CreateObjectKnowledge( asnMission.installation_cible, "Installation cible");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASY_CommettreExactionsSurPopulationDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASY_CommettreExactionsSurPopulationDansZone()
{
    ASN1T_Mission_Pion_ASY_CommettreExactionsSurPopulationDansZone& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreExactionsSurPopulationDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_commettre_exactions_sur_population_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_commettre_exactions_sur_population_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateMission_ASY_Renseigner
// Created: AGR
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateMission_ASY_Renseigner()
{
    ASN1T_Mission_Pion_ASY_Renseigner& asnMission = *new ASN1T_Mission_Pion_ASY_Renseigner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_renseigner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_asy_renseigner = &asnMission;
    CreateAutomate( asnMission.destinataire, "Destinataire");
}

