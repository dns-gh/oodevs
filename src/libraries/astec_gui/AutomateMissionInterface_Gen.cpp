// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
AutomateMissionInterface::~AutomateMissionInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().mission.t )
    {
        case T_Mission_Automate_mission_automate_abc_armer_point_de_controle : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_armer_point_de_controle; break;
        case T_Mission_Automate_mission_automate_abc_organiser_accueil_colonne_refugies : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_organiser_accueil_colonne_refugies; break;
        case T_Mission_Automate_mission_automate_abc_proteger_pc : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_proteger_pc; break;
        case T_Mission_Automate_mission_automate_abc_appuyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_appuyer; break;
        case T_Mission_Automate_mission_automate_abc_attaquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_attaquer; break;
        case T_Mission_Automate_mission_automate_abc_contre_attaquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_contre_attaquer; break;
        case T_Mission_Automate_mission_automate_abc_controler : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_controler; break;
        case T_Mission_Automate_mission_automate_abc_couvrir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_couvrir; break;
        case T_Mission_Automate_mission_automate_abc_donner_coup_arret : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_donner_coup_arret; break;
        case T_Mission_Automate_mission_automate_abc_eclairer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_eclairer; break;
        case T_Mission_Automate_mission_automate_abc_effectuer_raid : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_effectuer_raid; break;
        case T_Mission_Automate_mission_automate_abc_interdire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_interdire; break;
        case T_Mission_Automate_mission_automate_abc_jalonner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_jalonner; break;
        case T_Mission_Automate_mission_automate_abc_prendre_preciser_contact : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_prendre_preciser_contact; break;
        case T_Mission_Automate_mission_automate_abc_reconnaitre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_reconnaitre; break;
        case T_Mission_Automate_mission_automate_abc_reconnaitre_point : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_reconnaitre_point; break;
        case T_Mission_Automate_mission_automate_abc_reduire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_reduire; break;
        case T_Mission_Automate_mission_automate_abc_relever_en_depassant : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_relever_en_depassant; break;
        case T_Mission_Automate_mission_automate_abc_semparer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_semparer; break;
        case T_Mission_Automate_mission_automate_abc_tenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_tenir; break;
        case T_Mission_Automate_mission_automate_abc_soutenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_soutenir; break;
        case T_Mission_Automate_mission_automate_abc_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_surveiller; break;
        case T_Mission_Automate_mission_automate_abc_flanc_garder : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_flanc_garder; break;
        case T_Mission_Automate_mission_automate_abc_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_renseigner_sur; break;
        case T_Mission_Automate_mission_automate_abc_fixer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_fixer; break;
        case T_Mission_Automate_mission_automate_abc_freiner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_freiner; break;
        case T_Mission_Automate_mission_automate_abc_surveiller_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_surveiller_itineraire; break;
        case T_Mission_Automate_mission_automate_abc_renseigner_sur_un_axe : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_renseigner_sur_un_axe; break;
        case T_Mission_Automate_mission_automate_abc_recueillir_unite : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_recueillir_unite; break;
        case T_Mission_Automate_mission_automate_abc_escorter_unite : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_escorter_unite; break;
        case T_Mission_Automate_mission_automate_abc_escorter_unites : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_escorter_unites; break;
        case T_Mission_Automate_mission_automate_abc_appuyer_sur_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_appuyer_sur_position; break;
        case T_Mission_Automate_mission_automate_alat_heliporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_heliporter; break;
        case T_Mission_Automate_mission_automate_alat_jalonner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_jalonner; break;
        case T_Mission_Automate_mission_automate_alat_helitransporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_helitransporter; break;
        case T_Mission_Automate_mission_automate_alat_extraire_personnel : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_extraire_personnel; break;
        case T_Mission_Automate_mission_automate_alat_appuyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_appuyer; break;
        case T_Mission_Automate_mission_automate_alat_couvrir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_couvrir; break;
        case T_Mission_Automate_mission_automate_alat_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_surveiller; break;
        case T_Mission_Automate_mission_automate_alat_reconnaitre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_reconnaitre; break;
        case T_Mission_Automate_mission_automate_alat_freiner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_freiner; break;
        case T_Mission_Automate_mission_automate_alat_escorter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_escorter; break;
        case T_Mission_Automate_mission_automate_alat_attaquer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_attaquer; break;
        case T_Mission_Automate_mission_automate_alat_recueillir_formation_aeromobile : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_recueillir_formation_aeromobile; break;
        case T_Mission_Automate_mission_automate_alat_attaquer_formation_aeromobile : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_attaquer_formation_aeromobile; break;
        case T_Mission_Automate_mission_automate_alat_donner_coup_arret : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_donner_coup_arret; break;
        case T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_profondeur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_detruire_neutraliser_dans_profondeur; break;
        case T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_detruire_neutraliser_dans_zone; break;
        case T_Mission_Automate_mission_automate_alat_effectuer_recherche_et_sauvetage : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_effectuer_recherche_et_sauvetage; break;
        case T_Mission_Automate_mission_automate_asa_defendre_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_defendre_zone; break;
        case T_Mission_Automate_mission_automate_asa_defendre_site : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_defendre_site; break;
        case T_Mission_Automate_mission_automate_asa_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_surveiller; break;
        case T_Mission_Automate_mission_automate_asa_defendre_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_defendre_itineraire; break;
        case T_Mission_Automate_mission_automate_asa_mistral_defendre_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_mistral_defendre_zone; break;
        case T_Mission_Automate_mission_automate_asa_mistral_defendre_site : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_mistral_defendre_site; break;
        case T_Mission_Automate_mission_automate_asa_mistral_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_mistral_surveiller; break;
        case T_Mission_Automate_mission_automate_asa_hawk_defendre_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_hawk_defendre_zone; break;
        case T_Mission_Automate_mission_automate_asa_hawk_defendre_site : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_hawk_defendre_site; break;
        case T_Mission_Automate_mission_automate_asa_hawk_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_hawk_surveiller; break;
        case T_Mission_Automate_mission_automate_asa_defendre_unites : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_defendre_unites; break;
        case T_Mission_Automate_mission_automate_ass_se_deployer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_ass_se_deployer; break;
        case T_Mission_Automate_mission_automate_ass_reconnaitre_zones_deploiement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_ass_reconnaitre_zones_deploiement; break;
        case T_Mission_Automate_mission_automate_ass_acquerir_objectifs : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_ass_acquerir_objectifs; break;
        case T_Mission_Automate_mission_automate_gen_se_deployer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_se_deployer; break;
        case T_Mission_Automate_mission_automate_gen_faire_franchir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_faire_franchir; break;
        case T_Mission_Automate_mission_automate_gen_realiser_systeme_obstacles : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_systeme_obstacles; break;
        case T_Mission_Automate_mission_automate_gen_franchir_de_vive_force : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_franchir_de_vive_force; break;
        case T_Mission_Automate_mission_automate_gen_retablir_itineraires : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_retablir_itineraires; break;
        case T_Mission_Automate_mission_automate_gen_amenager_aire_stationnement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_amenager_aire_stationnement; break;
        case T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_amenager_terrain_poser_sommaire; break;
        case T_Mission_Automate_mission_automate_gen_realiser_zone_poser : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_zone_poser; break;
        case T_Mission_Automate_mission_automate_gen_reconnaitre_zone_deploiement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_reconnaitre_zone_deploiement; break;
        case T_Mission_Automate_mission_automate_gen_realiser_camp_prisonniers : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_camp_prisonniers; break;
        case T_Mission_Automate_mission_automate_gen_realiser_camp_refugies : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_camp_refugies; break;
        case T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial; break;
        case T_Mission_Automate_mission_automate_gen_realiser_travaux_sommaires_au_profit_population : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_travaux_sommaires_au_profit_population; break;
        case T_Mission_Automate_mission_automate_gen_appuyer_debarquement_troupes : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_appuyer_debarquement_troupes; break;
        case T_Mission_Automate_mission_automate_gen_ouvrir_axes_progression : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_ouvrir_axes_progression; break;
        case T_Mission_Automate_mission_automate_inf_attaquer_souplesse : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_attaquer_souplesse; break;
        case T_Mission_Automate_mission_automate_inf_appuyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_appuyer; break;
        case T_Mission_Automate_mission_automate_inf_controler : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_controler; break;
        case T_Mission_Automate_mission_automate_inf_couvrir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_couvrir; break;
        case T_Mission_Automate_mission_automate_inf_donner_coup_arret : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_donner_coup_arret; break;
        case T_Mission_Automate_mission_automate_inf_eclairer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_eclairer; break;
        case T_Mission_Automate_mission_automate_inf_flanc_garder : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_flanc_garder; break;
        case T_Mission_Automate_mission_automate_inf_freiner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_freiner; break;
        case T_Mission_Automate_mission_automate_inf_harceler : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_harceler; break;
        case T_Mission_Automate_mission_automate_inf_interdire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_interdire; break;
        case T_Mission_Automate_mission_automate_inf_mener_defense_usure : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_mener_defense_usure; break;
        case T_Mission_Automate_mission_automate_inf_prendre_preciser_maintenir_contact : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_prendre_preciser_maintenir_contact; break;
        case T_Mission_Automate_mission_automate_inf_reconnaissance_offensive : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_reconnaissance_offensive; break;
        case T_Mission_Automate_mission_automate_inf_recueillir_unite : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_recueillir_unite; break;
        case T_Mission_Automate_mission_automate_inf_reduire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_reduire; break;
        case T_Mission_Automate_mission_automate_inf_relever : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_relever; break;
        case T_Mission_Automate_mission_automate_inf_rompre_contact : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_rompre_contact; break;
        case T_Mission_Automate_mission_automate_inf_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_surveiller; break;
        case T_Mission_Automate_mission_automate_inf_semparer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_semparer; break;
        case T_Mission_Automate_mission_automate_inf_fixer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_fixer; break;
        case T_Mission_Automate_mission_automate_inf_attaquer_en_terrain_difficile : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_attaquer_en_terrain_difficile; break;
        case T_Mission_Automate_mission_automate_inf_barrer_direction : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_barrer_direction; break;
        case T_Mission_Automate_mission_automate_inf_defendre_ferme : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_defendre_ferme; break;
        case T_Mission_Automate_mission_automate_inf_tenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_tenir; break;
        case T_Mission_Automate_mission_automate_inf_appuyer_un_freinage : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_appuyer_un_freinage; break;
        case T_Mission_Automate_mission_automate_inf_appuyer_une_attaque : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_appuyer_une_attaque; break;
        case T_Mission_Automate_mission_automate_inf_soutenir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_soutenir; break;
        case T_Mission_Automate_mission_automate_inf_escorter_unite : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_escorter_unite; break;
        case T_Mission_Automate_mission_automate_inf_escorter_unites : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_escorter_unites; break;
        case T_Mission_Automate_mission_automate_inf_appuyer_sur_position : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_appuyer_sur_position; break;
        case T_Mission_Automate_mission_automate_log_se_deployer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_se_deployer; break;
        case T_Mission_Automate_mission_automate_log_appuyer_mouvement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_appuyer_mouvement; break;
        case T_Mission_Automate_mission_automate_log_reconnaitre_itineraire : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_reconnaitre_itineraire; break;
        case T_Mission_Automate_mission_automate_log_surveiller : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_surveiller; break;
        case T_Mission_Automate_mission_automate_log_transporter_unites : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_transporter_unites; break;
        case T_Mission_Automate_mission_automate_log_appuyer_mouvement_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_appuyer_mouvement_dans_zone; break;
        case T_Mission_Automate_mission_automate_log_mettre_en_oeuvre_zone_stationnement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_mettre_en_oeuvre_zone_stationnement; break;
        case T_Mission_Automate_mission_automate_log_appuyer_franchissement_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_appuyer_franchissement_dans_zone; break;
        case T_Mission_Automate_mission_automate_log_bloquer_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_bloquer_zone; break;
        case T_Mission_Automate_mission_automate_log_reconnaitre_zone_contaminee : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_reconnaitre_zone_contaminee; break;
        case T_Mission_Automate_mission_automate_log_reconnaitre_zone_de_deploiement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_reconnaitre_zone_de_deploiement; break;
        case T_Mission_Automate_mission_automate_log_armer_pias : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_armer_pias; break;
        case T_Mission_Automate_mission_automate_test_move_to : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_test_move_to; break;
        case T_Mission_Automate_mission_automate_decrocher : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_decrocher; break;
        case T_Mission_Automate_mission_automate_relever_sur_place : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_relever_sur_place; break;
        case T_Mission_Automate_mission_automate_faire_mouvement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_faire_mouvement; break;
        case T_Mission_Automate_mission_automate_se_faire_decontaminer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_se_faire_decontaminer; break;
        case T_Mission_Automate_mission_automate_franchir : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_franchir; break;
        case T_Mission_Automate_mission_automate_stationner : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_stationner; break;
        case T_Mission_Automate_mission_automate_se_faire_transporter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_se_faire_transporter; break;
        case T_Mission_Automate_mission_automate_se_recompleter : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_se_recompleter; break;
        case T_Mission_Automate_mission_automate_se_rendre : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_se_rendre; break;
        case T_Mission_Automate_mission_automate_faire_mouvement_vers_camp_refugies : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_faire_mouvement_vers_camp_refugies; break;
        case T_Mission_Automate_mission_automate_interdire_franchissement_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_interdire_franchissement_populations; break;
        case T_Mission_Automate_mission_automate_filtrer_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_filtrer_populations; break;
        case T_Mission_Automate_mission_automate_canaliser_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_canaliser_populations; break;
        case T_Mission_Automate_mission_automate_controler_populations_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_controler_populations_dans_zone; break;
        case T_Mission_Automate_mission_automate_securiser_zone_contre_populations : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_securiser_zone_contre_populations; break;
        case T_Mission_Automate_mission_automate_asy_commettre_exactions_sur_population_dans_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asy_commettre_exactions_sur_population_dans_zone; break;
        case T_Mission_Automate_mission_automate_generique : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_generique; break;
        case T_Mission_Automate_mission_automate_nbc_reconnaitre_un_axe : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_un_axe; break;
        case T_Mission_Automate_mission_automate_nbc_reconnaitre_une_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_une_zone; break;
        case T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_armer_un_site_de_decontamination; break;
        case T_Mission_Automate_mission_automate_nbc_decontaminer_une_zone : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_decontaminer_une_zone; break;
        case T_Mission_Automate_mission_automate_nbc_reconnaitre_des_sites_rota : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_des_sites_rota; break;
        case T_Mission_Automate_mission_automate_rens_rohum_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_rohum_renseigner_sur; break;
        case T_Mission_Automate_mission_automate_rens_rohum_orienter_guider : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_rohum_orienter_guider; break;
        case T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_rohum_sexfiltrer; break;
        case T_Mission_Automate_mission_automate_rens_roim_cl289_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_roim_cl289_renseigner_sur; break;
        case T_Mission_Automate_mission_automate_rens_roim_sdti_renseigner_sur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_roim_sdti_renseigner_sur; break;
        case T_Mission_Automate_mission_automate_rens_roem_appuyer : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_roem_appuyer; break;
        case T_Mission_Automate_mission_automate_rens_roem_renseignersur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_roem_renseignersur; break;
        case T_Mission_Automate_mission_automate_rens_radint_renseignersur : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_radint_renseignersur; break;
        case T_Mission_Automate_mission_automate_trans_commander : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_trans_commander; break;

        default:
            assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateInterface
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateInterface()
{
    switch( nMissionId_ )
    {
        case eMission_Automate_ABC_ArmerPointDeControle : CreateMission_ABC_ArmerPointDeControle(); break;
        case eMission_Automate_ABC_OrganiserAccueilColonneRefugies : CreateMission_ABC_OrganiserAccueilColonneRefugies(); break;
        case eMission_Automate_ABC_ProtegerPC : CreateMission_ABC_ProtegerPC(); break;
        case eMission_Automate_ABC_Appuyer : CreateMission_ABC_Appuyer(); break;
        case eMission_Automate_ABC_Attaquer : CreateMission_ABC_Attaquer(); break;
        case eMission_Automate_ABC_ContreAttaquer : CreateMission_ABC_ContreAttaquer(); break;
        case eMission_Automate_ABC_Controler : CreateMission_ABC_Controler(); break;
        case eMission_Automate_ABC_Couvrir : CreateMission_ABC_Couvrir(); break;
        case eMission_Automate_ABC_DonnerCoupArret : CreateMission_ABC_DonnerCoupArret(); break;
        case eMission_Automate_ABC_Eclairer : CreateMission_ABC_Eclairer(); break;
        case eMission_Automate_ABC_EffectuerRaid : CreateMission_ABC_EffectuerRaid(); break;
        case eMission_Automate_ABC_Interdire : CreateMission_ABC_Interdire(); break;
        case eMission_Automate_ABC_Jalonner : CreateMission_ABC_Jalonner(); break;
        case eMission_Automate_ABC_PrendrePreciserContact : CreateMission_ABC_PrendrePreciserContact(); break;
        case eMission_Automate_ABC_Reconnaitre : CreateMission_ABC_Reconnaitre(); break;
        case eMission_Automate_ABC_ReconnaitrePoint : CreateMission_ABC_ReconnaitrePoint(); break;
        case eMission_Automate_ABC_Reduire : CreateMission_ABC_Reduire(); break;
        case eMission_Automate_ABC_ReleverEnDepassant : CreateMission_ABC_ReleverEnDepassant(); break;
        case eMission_Automate_ABC_Semparer : CreateMission_ABC_Semparer(); break;
        case eMission_Automate_ABC_Tenir : CreateMission_ABC_Tenir(); break;
        case eMission_Automate_ABC_Soutenir : CreateMission_ABC_Soutenir(); break;
        case eMission_Automate_ABC_Surveiller : CreateMission_ABC_Surveiller(); break;
        case eMission_Automate_ABC_FlancGarder : CreateMission_ABC_FlancGarder(); break;
        case eMission_Automate_ABC_RenseignerSur : CreateMission_ABC_RenseignerSur(); break;
        case eMission_Automate_ABC_Fixer : CreateMission_ABC_Fixer(); break;
        case eMission_Automate_ABC_Freiner : CreateMission_ABC_Freiner(); break;
        case eMission_Automate_ABC_SurveillerItineraire : CreateMission_ABC_SurveillerItineraire(); break;
        case eMission_Automate_ABC_RenseignerSurUnAxe : CreateMission_ABC_RenseignerSurUnAxe(); break;
        case eMission_Automate_ABC_RecueillirUnite : CreateMission_ABC_RecueillirUnite(); break;
        case eMission_Automate_ABC_EscorterUnite : CreateMission_ABC_EscorterUnite(); break;
        case eMission_Automate_ABC_EscorterUnites : CreateMission_ABC_EscorterUnites(); break;
        case eMission_Automate_ABC_AppuyerSurPosition : CreateMission_ABC_AppuyerSurPosition(); break;
        case eMission_Automate_ALAT_Heliporter : CreateMission_ALAT_Heliporter(); break;
        case eMission_Automate_ALAT_Jalonner : CreateMission_ALAT_Jalonner(); break;
        case eMission_Automate_ALAT_Helitransporter : CreateMission_ALAT_Helitransporter(); break;
        case eMission_Automate_ALAT_ExtrairePersonnel : CreateMission_ALAT_ExtrairePersonnel(); break;
        case eMission_Automate_ALAT_Appuyer : CreateMission_ALAT_Appuyer(); break;
        case eMission_Automate_ALAT_Couvrir : CreateMission_ALAT_Couvrir(); break;
        case eMission_Automate_ALAT_Surveiller : CreateMission_ALAT_Surveiller(); break;
        case eMission_Automate_ALAT_Reconnaitre : CreateMission_ALAT_Reconnaitre(); break;
        case eMission_Automate_ALAT_Freiner : CreateMission_ALAT_Freiner(); break;
        case eMission_Automate_ALAT_Escorter : CreateMission_ALAT_Escorter(); break;
        case eMission_Automate_ALAT_Attaquer : CreateMission_ALAT_Attaquer(); break;
        case eMission_Automate_ALAT_RecueillirFormationAeromobile : CreateMission_ALAT_RecueillirFormationAeromobile(); break;
        case eMission_Automate_ALAT_AttaquerFormationAeromobile : CreateMission_ALAT_AttaquerFormationAeromobile(); break;
        case eMission_Automate_ALAT_DonnerCoupArret : CreateMission_ALAT_DonnerCoupArret(); break;
        case eMission_Automate_ALAT_DetruireNeutraliserDansProfondeur : CreateMission_ALAT_DetruireNeutraliserDansProfondeur(); break;
        case eMission_Automate_ALAT_DetruireNeutraliserDansZone : CreateMission_ALAT_DetruireNeutraliserDansZone(); break;
        case eMission_Automate_ALAT_EffectuerRechercheEtSauvetage : CreateMission_ALAT_EffectuerRechercheEtSauvetage(); break;
        case eMission_Automate_ASA_DefendreZone : CreateMission_ASA_DefendreZone(); break;
        case eMission_Automate_ASA_DefendreSite : CreateMission_ASA_DefendreSite(); break;
        case eMission_Automate_ASA_Surveiller : CreateMission_ASA_Surveiller(); break;
        case eMission_Automate_ASA_DefendreItineraire : CreateMission_ASA_DefendreItineraire(); break;
        case eMission_Automate_ASA_MISTRAL_DefendreZone : CreateMission_ASA_MISTRAL_DefendreZone(); break;
        case eMission_Automate_ASA_MISTRAL_DefendreSite : CreateMission_ASA_MISTRAL_DefendreSite(); break;
        case eMission_Automate_ASA_MISTRAL_Surveiller : CreateMission_ASA_MISTRAL_Surveiller(); break;
        case eMission_Automate_ASA_HAWK_DefendreZone : CreateMission_ASA_HAWK_DefendreZone(); break;
        case eMission_Automate_ASA_HAWK_DefendreSite : CreateMission_ASA_HAWK_DefendreSite(); break;
        case eMission_Automate_ASA_HAWK_Surveiller : CreateMission_ASA_HAWK_Surveiller(); break;
        case eMission_Automate_ASA_DefendreUnites : CreateMission_ASA_DefendreUnites(); break;
        case eMission_Automate_ASS_SeDeployer : CreateMission_ASS_SeDeployer(); break;
        case eMission_Automate_ASS_ReconnaitreZonesDeploiement : CreateMission_ASS_ReconnaitreZonesDeploiement(); break;
        case eMission_Automate_ASS_AcquerirObjectifs : CreateMission_ASS_AcquerirObjectifs(); break;
        case eMission_Automate_GEN_SeDeployer : CreateMission_GEN_SeDeployer(); break;
        case eMission_Automate_GEN_FaireFranchir : CreateMission_GEN_FaireFranchir(); break;
        case eMission_Automate_GEN_RealiserSystemeObstacles : CreateMission_GEN_RealiserSystemeObstacles(); break;
        case eMission_Automate_GEN_FranchirDeViveForce : CreateMission_GEN_FranchirDeViveForce(); break;
        case eMission_Automate_GEN_RetablirItineraires : CreateMission_GEN_RetablirItineraires(); break;
        case eMission_Automate_GEN_AmenagerAireStationnement : CreateMission_GEN_AmenagerAireStationnement(); break;
        case eMission_Automate_GEN_AmenagerTerrainPoserSommaire : CreateMission_GEN_AmenagerTerrainPoserSommaire(); break;
        case eMission_Automate_GEN_RealiserZonePoser : CreateMission_GEN_RealiserZonePoser(); break;
        case eMission_Automate_GEN_ReconnaitreZoneDeploiement : CreateMission_GEN_ReconnaitreZoneDeploiement(); break;
        case eMission_Automate_GEN_RealiserCampPrisonniers : CreateMission_GEN_RealiserCampPrisonniers(); break;
        case eMission_Automate_GEN_RealiserCampRefugies : CreateMission_GEN_RealiserCampRefugies(); break;
        case eMission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial : CreateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial(); break;
        case eMission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation : CreateMission_GEN_RealiserTravauxSommairesAuProfitPopulation(); break;
        case eMission_Automate_GEN_AppuyerDebarquementTroupes : CreateMission_GEN_AppuyerDebarquementTroupes(); break;
        case eMission_Automate_GEN_OuvrirAxesProgression : CreateMission_GEN_OuvrirAxesProgression(); break;
        case eMission_Automate_INF_AttaquerSouplesse : CreateMission_INF_AttaquerSouplesse(); break;
        case eMission_Automate_INF_Appuyer : CreateMission_INF_Appuyer(); break;
        case eMission_Automate_INF_Controler : CreateMission_INF_Controler(); break;
        case eMission_Automate_INF_Couvrir : CreateMission_INF_Couvrir(); break;
        case eMission_Automate_INF_DonnerCoupArret : CreateMission_INF_DonnerCoupArret(); break;
        case eMission_Automate_INF_Eclairer : CreateMission_INF_Eclairer(); break;
        case eMission_Automate_INF_FlancGarder : CreateMission_INF_FlancGarder(); break;
        case eMission_Automate_INF_Freiner : CreateMission_INF_Freiner(); break;
        case eMission_Automate_INF_Harceler : CreateMission_INF_Harceler(); break;
        case eMission_Automate_INF_Interdire : CreateMission_INF_Interdire(); break;
        case eMission_Automate_INF_MenerDefenseUsure : CreateMission_INF_MenerDefenseUsure(); break;
        case eMission_Automate_INF_PrendrePreciserMaintenirContact : CreateMission_INF_PrendrePreciserMaintenirContact(); break;
        case eMission_Automate_INF_ReconnaissanceOffensive : CreateMission_INF_ReconnaissanceOffensive(); break;
        case eMission_Automate_INF_RecueillirUnite : CreateMission_INF_RecueillirUnite(); break;
        case eMission_Automate_INF_Reduire : CreateMission_INF_Reduire(); break;
        case eMission_Automate_INF_Relever : CreateMission_INF_Relever(); break;
        case eMission_Automate_INF_RompreContact : CreateMission_INF_RompreContact(); break;
        case eMission_Automate_INF_Surveiller : CreateMission_INF_Surveiller(); break;
        case eMission_Automate_INF_Semparer : CreateMission_INF_Semparer(); break;
        case eMission_Automate_INF_Fixer : CreateMission_INF_Fixer(); break;
        case eMission_Automate_INF_AttaquerEnTerrainDifficile : CreateMission_INF_AttaquerEnTerrainDifficile(); break;
        case eMission_Automate_INF_BarrerDirection : CreateMission_INF_BarrerDirection(); break;
        case eMission_Automate_INF_DefendreFerme : CreateMission_INF_DefendreFerme(); break;
        case eMission_Automate_INF_Tenir : CreateMission_INF_Tenir(); break;
        case eMission_Automate_INF_AppuyerUnFreinage : CreateMission_INF_AppuyerUnFreinage(); break;
        case eMission_Automate_INF_AppuyerUneAttaque : CreateMission_INF_AppuyerUneAttaque(); break;
        case eMission_Automate_INF_Soutenir : CreateMission_INF_Soutenir(); break;
        case eMission_Automate_INF_EscorterUnite : CreateMission_INF_EscorterUnite(); break;
        case eMission_Automate_INF_EscorterUnites : CreateMission_INF_EscorterUnites(); break;
        case eMission_Automate_INF_AppuyerSurPosition : CreateMission_INF_AppuyerSurPosition(); break;
        case eMission_Automate_LOG_SeDeployer : CreateMission_LOG_SeDeployer(); break;
        case eMission_Automate_LOG_AppuyerMouvement : CreateMission_LOG_AppuyerMouvement(); break;
        case eMission_Automate_LOG_ReconnaitreItineraire : CreateMission_LOG_ReconnaitreItineraire(); break;
        case eMission_Automate_LOG_Surveiller : CreateMission_LOG_Surveiller(); break;
        case eMission_Automate_LOG_TransporterUnites : CreateMission_LOG_TransporterUnites(); break;
        case eMission_Automate_LOG_AppuyerMouvementDansZone : CreateMission_LOG_AppuyerMouvementDansZone(); break;
        case eMission_Automate_LOG_MettreEnOeuvreZoneStationnement : CreateMission_LOG_MettreEnOeuvreZoneStationnement(); break;
        case eMission_Automate_LOG_AppuyerFranchissementDansZone : CreateMission_LOG_AppuyerFranchissementDansZone(); break;
        case eMission_Automate_LOG_BloquerZone : CreateMission_LOG_BloquerZone(); break;
        case eMission_Automate_LOG_ReconnaitreZoneContaminee : CreateMission_LOG_ReconnaitreZoneContaminee(); break;
        case eMission_Automate_LOG_ReconnaitreZoneDeDeploiement : CreateMission_LOG_ReconnaitreZoneDeDeploiement(); break;
        case eMission_Automate_LOG_ArmerPIAs : CreateMission_LOG_ArmerPIAs(); break;
        case eMission_Automate_Test_MoveTo : CreateMission_Test_MoveTo(); break;
        case eMission_Automate_Decrocher : CreateMission_Decrocher(); break;
        case eMission_Automate_ReleverSurPlace : CreateMission_ReleverSurPlace(); break;
        case eMission_Automate_FaireMouvement : CreateMission_FaireMouvement(); break;
        case eMission_Automate_SeFaireDecontaminer : CreateMission_SeFaireDecontaminer(); break;
        case eMission_Automate_Franchir : CreateMission_Franchir(); break;
        case eMission_Automate_Stationner : CreateMission_Stationner(); break;
        case eMission_Automate_SeFaireTransporter : CreateMission_SeFaireTransporter(); break;
        case eMission_Automate_SeRecompleter : CreateMission_SeRecompleter(); break;
        case eMission_Automate_SeRendre : CreateMission_SeRendre(); break;
        case eMission_Automate_FaireMouvementVersCampRefugies : CreateMission_FaireMouvementVersCampRefugies(); break;
        case eMission_Automate_InterdireFranchissementPopulations : CreateMission_InterdireFranchissementPopulations(); break;
        case eMission_Automate_FiltrerPopulations : CreateMission_FiltrerPopulations(); break;
        case eMission_Automate_CanaliserPopulations : CreateMission_CanaliserPopulations(); break;
        case eMission_Automate_ControlerPopulationsDansZone : CreateMission_ControlerPopulationsDansZone(); break;
        case eMission_Automate_SecuriserZoneContrePopulations : CreateMission_SecuriserZoneContrePopulations(); break;
        case eMission_Automate_ASY_CommettreExactionsSurPopulationDansZone : CreateMission_ASY_CommettreExactionsSurPopulationDansZone(); break;
        case eMission_Automate_Generique : CreateMission_Generique(); break;
        case eMission_Automate_NBC_ReconnaitreUnAxe : CreateMission_NBC_ReconnaitreUnAxe(); break;
        case eMission_Automate_NBC_ReconnaitreUneZone : CreateMission_NBC_ReconnaitreUneZone(); break;
        case eMission_Automate_NBC_ArmerUnSiteDeDecontamination : CreateMission_NBC_ArmerUnSiteDeDecontamination(); break;
        case eMission_Automate_NBC_DecontaminerUneZone : CreateMission_NBC_DecontaminerUneZone(); break;
        case eMission_Automate_NBC_ReconnaitreDesSitesROTA : CreateMission_NBC_ReconnaitreDesSitesROTA(); break;
        case eMission_Automate_RENS_ROHUM_RenseignerSur : CreateMission_RENS_ROHUM_RenseignerSur(); break;
        case eMission_Automate_RENS_ROHUM_OrienterGuider : CreateMission_RENS_ROHUM_OrienterGuider(); break;
        case eMission_Automate_RENS_ROHUM_SExfiltrer : CreateMission_RENS_ROHUM_SExfiltrer(); break;
        case eMission_Automate_RENS_ROIM_CL289_RenseignerSur : CreateMission_RENS_ROIM_CL289_RenseignerSur(); break;
        case eMission_Automate_RENS_ROIM_SDTI_RenseignerSur : CreateMission_RENS_ROIM_SDTI_RenseignerSur(); break;
        case eMission_Automate_RENS_ROEM_Appuyer : CreateMission_RENS_ROEM_Appuyer(); break;
        case eMission_Automate_RENS_ROEM_RenseignerSur : CreateMission_RENS_ROEM_RenseignerSur(); break;
        case eMission_Automate_RENS_RADINT_RenseignerSur : CreateMission_RENS_RADINT_RenseignerSur(); break;
        case eMission_Automate_TRANS_Commander : CreateMission_TRANS_Commander(); break;

        default:
            assert( false );
    }

    this->CreateDefaultParameters();
    this->CreateOkCancelButtons();
}


// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_ArmerPointDeControle
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_ArmerPointDeControle()
{
    ASN1T_Mission_Automate_ABC_ArmerPointDeControle& asnMission = *new ASN1T_Mission_Automate_ABC_ArmerPointDeControle();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_armer_point_de_controle;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_armer_point_de_controle = &asnMission;
    CreateLocation( asnMission.zone_de_controle, "Zone de controle");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_OrganiserAccueilColonneRefugies
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_OrganiserAccueilColonneRefugies()
{
    ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies& asnMission = *new ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_organiser_accueil_colonne_refugies;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_organiser_accueil_colonne_refugies = &asnMission;
    CreateLocation( asnMission.zone_installation, "Zone installation");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_ProtegerPC
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_ProtegerPC()
{
    ASN1T_Mission_Automate_ABC_ProtegerPC& asnMission = *new ASN1T_Mission_Automate_ABC_ProtegerPC();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_proteger_pc;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_proteger_pc = &asnMission;
    CreateAutomate( asnMission.pc, "Pc");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Appuyer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Appuyer()
{
    ASN1T_Mission_Automate_ABC_Appuyer& asnMission = *new ASN1T_Mission_Automate_ABC_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_appuyer = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Attaquer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Attaquer()
{
    ASN1T_Mission_Automate_ABC_Attaquer& asnMission = *new ASN1T_Mission_Automate_ABC_Attaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_attaquer = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_ContreAttaquer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_ContreAttaquer()
{
    ASN1T_Mission_Automate_ABC_ContreAttaquer& asnMission = *new ASN1T_Mission_Automate_ABC_ContreAttaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_contre_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_contre_attaquer = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Controler
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Controler()
{
    ASN1T_Mission_Automate_ABC_Controler& asnMission = *new ASN1T_Mission_Automate_ABC_Controler();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_controler;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_controler = &asnMission;
    CreateLocation( asnMission.zone_a_controler, "Zone a controler");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Couvrir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Couvrir()
{
    ASN1T_Mission_Automate_ABC_Couvrir& asnMission = *new ASN1T_Mission_Automate_ABC_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_couvrir = &asnMission;
    CreateLocation( asnMission.position_installation, "Position installation");
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_DonnerCoupArret
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_DonnerCoupArret()
{
    ASN1T_Mission_Automate_ABC_DonnerCoupArret& asnMission = *new ASN1T_Mission_Automate_ABC_DonnerCoupArret();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_donner_coup_arret;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_donner_coup_arret = &asnMission;
    CreateLocation( asnMission.zone_regroupement, "Zone regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Eclairer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Eclairer()
{
    ASN1T_Mission_Automate_ABC_Eclairer& asnMission = *new ASN1T_Mission_Automate_ABC_Eclairer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_eclairer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_eclairer = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_EffectuerRaid
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_EffectuerRaid()
{
    ASN1T_Mission_Automate_ABC_EffectuerRaid& asnMission = *new ASN1T_Mission_Automate_ABC_EffectuerRaid();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_effectuer_raid;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_effectuer_raid = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Interdire
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Interdire()
{
    ASN1T_Mission_Automate_ABC_Interdire& asnMission = *new ASN1T_Mission_Automate_ABC_Interdire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_interdire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_interdire = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Jalonner
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Jalonner()
{
    ASN1T_Mission_Automate_ABC_Jalonner& asnMission = *new ASN1T_Mission_Automate_ABC_Jalonner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_jalonner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_jalonner = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_PrendrePreciserContact
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_PrendrePreciserContact()
{
    ASN1T_Mission_Automate_ABC_PrendrePreciserContact& asnMission = *new ASN1T_Mission_Automate_ABC_PrendrePreciserContact();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_prendre_preciser_contact;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_prendre_preciser_contact = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Reconnaitre
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Reconnaitre()
{
    ASN1T_Mission_Automate_ABC_Reconnaitre& asnMission = *new ASN1T_Mission_Automate_ABC_Reconnaitre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_reconnaitre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_reconnaitre = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_ReconnaitrePoint
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_ReconnaitrePoint()
{
    ASN1T_Mission_Automate_ABC_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Automate_ABC_ReconnaitrePoint();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_reconnaitre_point;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_reconnaitre_point = &asnMission;
    CreateLocation( asnMission.point_a_reconnaitre, "Point a reconnaitre");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Reduire
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Reduire()
{
    ASN1T_Mission_Automate_ABC_Reduire& asnMission = *new ASN1T_Mission_Automate_ABC_Reduire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_reduire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_reduire = &asnMission;
    CreateLocation( asnMission.zone_resistance, "Zone resistance");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_ReleverEnDepassant
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_ReleverEnDepassant()
{
    ASN1T_Mission_Automate_ABC_ReleverEnDepassant& asnMission = *new ASN1T_Mission_Automate_ABC_ReleverEnDepassant();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_relever_en_depassant;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_relever_en_depassant = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Semparer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Semparer()
{
    ASN1T_Mission_Automate_ABC_Semparer& asnMission = *new ASN1T_Mission_Automate_ABC_Semparer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_semparer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_semparer = &asnMission;
    CreateLocation( asnMission.objectif, "Objectif");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Tenir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Tenir()
{
    ASN1T_Mission_Automate_ABC_Tenir& asnMission = *new ASN1T_Mission_Automate_ABC_Tenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_tenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_tenir = &asnMission;
    CreateLocation( asnMission.position_installation, "Position installation");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Soutenir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Soutenir()
{
    ASN1T_Mission_Automate_ABC_Soutenir& asnMission = *new ASN1T_Mission_Automate_ABC_Soutenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_soutenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_soutenir = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Surveiller()
{
    ASN1T_Mission_Automate_ABC_Surveiller& asnMission = *new ASN1T_Mission_Automate_ABC_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_surveiller = &asnMission;
    CreateLocation( asnMission.zone_installation, "Zone installation");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_FlancGarder
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_FlancGarder()
{
    ASN1T_Mission_Automate_ABC_FlancGarder& asnMission = *new ASN1T_Mission_Automate_ABC_FlancGarder();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_flanc_garder;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_flanc_garder = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_RenseignerSur()
{
    ASN1T_Mission_Automate_ABC_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_ABC_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_renseigner_sur = &asnMission;
    CreateLocation( asnMission.zone_a_renseigner, "Zone a renseigner");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Fixer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Fixer()
{
    ASN1T_Mission_Automate_ABC_Fixer& asnMission = *new ASN1T_Mission_Automate_ABC_Fixer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_fixer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_fixer = &asnMission;
    CreateAgentKnowledgeList( asnMission.ennemis, "Ennemis");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_Freiner
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_Freiner()
{
    ASN1T_Mission_Automate_ABC_Freiner& asnMission = *new ASN1T_Mission_Automate_ABC_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_freiner = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_SurveillerItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_SurveillerItineraire()
{
    ASN1T_Mission_Automate_ABC_SurveillerItineraire& asnMission = *new ASN1T_Mission_Automate_ABC_SurveillerItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_surveiller_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_surveiller_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_RenseignerSurUnAxe
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_RenseignerSurUnAxe()
{
    ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe& asnMission = *new ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_renseigner_sur_un_axe;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_renseigner_sur_un_axe = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_RecueillirUnite
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_RecueillirUnite()
{
    ASN1T_Mission_Automate_ABC_RecueillirUnite& asnMission = *new ASN1T_Mission_Automate_ABC_RecueillirUnite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_recueillir_unite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_recueillir_unite = &asnMission;
    CreateLocation( asnMission.zone_regroupement, "Zone regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_EscorterUnite
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_EscorterUnite()
{
    ASN1T_Mission_Automate_ABC_EscorterUnite& asnMission = *new ASN1T_Mission_Automate_ABC_EscorterUnite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_escorter_unite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_escorter_unite = &asnMission;
    CreateAgent( asnMission.unite_a_escorter, "Unite a escorter");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_EscorterUnites
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_EscorterUnites()
{
    ASN1T_Mission_Automate_ABC_EscorterUnites& asnMission = *new ASN1T_Mission_Automate_ABC_EscorterUnites();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_escorter_unites;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_escorter_unites = &asnMission;
    CreateAutomate( asnMission.automate_a_escorter, "Automate a escorter");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ABC_AppuyerSurPosition
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ABC_AppuyerSurPosition()
{
    ASN1T_Mission_Automate_ABC_AppuyerSurPosition& asnMission = *new ASN1T_Mission_Automate_ABC_AppuyerSurPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_appuyer_sur_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_abc_appuyer_sur_position = &asnMission;
    CreateLocation( asnMission.position_installation, "Position installation");
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Heliporter
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Heliporter()
{
    ASN1T_Mission_Automate_ALAT_Heliporter& asnMission = *new ASN1T_Mission_Automate_ALAT_Heliporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_heliporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_heliporter = &asnMission;
    CreateAgentList( asnMission.unites_a_heliporter, "Unites a heliporter");
    CreatePoint( asnMission.point_debarquement, "Point debarquement");
    CreatePoint( asnMission.point_embarquement, "Point embarquement");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
    CreateBool( asnMission.attendre_unites, "Attendre unites");
    CreateBool( asnMission.avec_materiel, "Avec materiel");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Jalonner
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Jalonner()
{
    ASN1T_Mission_Automate_ALAT_Jalonner& asnMission = *new ASN1T_Mission_Automate_ALAT_Jalonner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_jalonner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_jalonner = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Helitransporter
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Helitransporter()
{
    ASN1T_Mission_Automate_ALAT_Helitransporter& asnMission = *new ASN1T_Mission_Automate_ALAT_Helitransporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_helitransporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_helitransporter = &asnMission;
    CreateAgentList( asnMission.unites_a_helitransporter, "Unites a helitransporter");
    CreatePoint( asnMission.point_debarquement, "Point debarquement");
    CreatePoint( asnMission.point_embarquement, "Point embarquement");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
    CreateBool( asnMission.avec_materiel, "Avec materiel");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_ExtrairePersonnel
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_ExtrairePersonnel()
{
    ASN1T_Mission_Automate_ALAT_ExtrairePersonnel& asnMission = *new ASN1T_Mission_Automate_ALAT_ExtrairePersonnel();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_extraire_personnel;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_extraire_personnel = &asnMission;
    CreateAgentList( asnMission.unites_a_helitransporter, "Unites a helitransporter");
    CreatePoint( asnMission.point_debarquement, "Point debarquement");
    CreatePoint( asnMission.point_embarquement, "Point embarquement");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
    CreateBool( asnMission.avec_materiel, "Avec materiel");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Appuyer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Appuyer()
{
    ASN1T_Mission_Automate_ALAT_Appuyer& asnMission = *new ASN1T_Mission_Automate_ALAT_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_appuyer = &asnMission;
    CreateAgentList( asnMission.unites_a_appuyer, "Unites a appuyer");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    CreatePoint( asnMission.point_dislocation, "Point dislocation");
    CreateLocation( asnMission.zone, "Zone");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Couvrir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Couvrir()
{
    ASN1T_Mission_Automate_ALAT_Couvrir& asnMission = *new ASN1T_Mission_Automate_ALAT_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_couvrir = &asnMission;
    CreateAgentList( asnMission.unites_a_appuyer, "Unites a appuyer");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Surveiller()
{
    ASN1T_Mission_Automate_ALAT_Surveiller& asnMission = *new ASN1T_Mission_Automate_ALAT_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
    CreatePoint( asnMission.point_logistique, "Point logistique");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Reconnaitre
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Reconnaitre()
{
    ASN1T_Mission_Automate_ALAT_Reconnaitre& asnMission = *new ASN1T_Mission_Automate_ALAT_Reconnaitre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_reconnaitre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_reconnaitre = &asnMission;
    CreatePoint( asnMission.pointRegroupement, "PointRegroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Freiner
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Freiner()
{
    ASN1T_Mission_Automate_ALAT_Freiner& asnMission = *new ASN1T_Mission_Automate_ALAT_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_freiner = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Escorter
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Escorter()
{
    ASN1T_Mission_Automate_ALAT_Escorter& asnMission = *new ASN1T_Mission_Automate_ALAT_Escorter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_escorter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_escorter = &asnMission;
    CreateAgentList( asnMission.unites_a_escorter, "Unites a escorter");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_Attaquer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_Attaquer()
{
    ASN1T_Mission_Automate_ALAT_Attaquer& asnMission = *new ASN1T_Mission_Automate_ALAT_Attaquer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_attaquer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_attaquer = &asnMission;
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_RecueillirFormationAeromobile
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_RecueillirFormationAeromobile()
{
    ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile& asnMission = *new ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_recueillir_formation_aeromobile;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_recueillir_formation_aeromobile = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_AttaquerFormationAeromobile
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_AttaquerFormationAeromobile()
{
    ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile& asnMission = *new ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_attaquer_formation_aeromobile;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_attaquer_formation_aeromobile = &asnMission;
    CreateAgentKnowledgeList( asnMission.unites_a_attaquer, "Unites a attaquer");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_DonnerCoupArret
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_DonnerCoupArret()
{
    ASN1T_Mission_Automate_ALAT_DonnerCoupArret& asnMission = *new ASN1T_Mission_Automate_ALAT_DonnerCoupArret();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_donner_coup_arret;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_donner_coup_arret = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_DetruireNeutraliserDansProfondeur
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_DetruireNeutraliserDansProfondeur()
{
    ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur& asnMission = *new ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_profondeur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_detruire_neutraliser_dans_profondeur = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreatePoint( asnMission.point_dislocation, "Point dislocation");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
    CreateBool( asnMission.neutraliser, "Neutraliser");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_DetruireNeutraliserDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_DetruireNeutraliserDansZone()
{
    ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone& asnMission = *new ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_detruire_neutraliser_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreateNatureAtlas( asnMission.cibles_prioritaires, "Cibles prioritaires");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreatePoint( asnMission.point_dislocation, "Point dislocation");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
    ParamComboBox< ASN1T_EnumMissionALAT_AmbianceMission >* pSelector_ambiance_mission = &CreateVarList( asnMission.ambiance_mission, "Ambiance mission");
    pSelector_ambiance_mission->AddItem( "Aa", EnumMissionALAT_AmbianceMission::aa );
    pSelector_ambiance_mission->AddItem( "As", EnumMissionALAT_AmbianceMission::as );
    pSelector_ambiance_mission->AddItem( "Polyvalent", EnumMissionALAT_AmbianceMission::polyvalent );
    CreateBool( asnMission.neutraliser, "Neutraliser");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ALAT_EffectuerRechercheEtSauvetage
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ALAT_EffectuerRechercheEtSauvetage()
{
    ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage& asnMission = *new ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_effectuer_recherche_et_sauvetage;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_alat_effectuer_recherche_et_sauvetage = &asnMission;
    CreateAgentList( asnMission.unites_a_secourir, "Unites a secourir");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
    CreateObjectKnowledgeList( asnMission.plots_ravitaillement, "Plots ravitaillement");
    CreateBool( asnMission.ravitaillement_debut_mission, "Ravitaillement debut mission");
    ParamComboBox< ASN1T_EnumMissionALAT_PorteeAction >* pSelector_portee_action = &CreateVarList( asnMission.portee_action, "Portee action");
    pSelector_portee_action->AddItem( "Courte portee", EnumMissionALAT_PorteeAction::courte_portee );
    pSelector_portee_action->AddItem( "Moyenne portee", EnumMissionALAT_PorteeAction::moyenne_portee );
    pSelector_portee_action->AddItem( "Longue portee", EnumMissionALAT_PorteeAction::longue_portee );
    pSelector_portee_action->AddItem( "Sans munitions", EnumMissionALAT_PorteeAction::sans_munitions );
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_DefendreZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_DefendreZone()
{
    ASN1T_Mission_Automate_ASA_DefendreZone& asnMission = *new ASN1T_Mission_Automate_ASA_DefendreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_defendre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_defendre_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePointList( asnMission.positions_sections, "Positions sections");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_DefendreSite
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_DefendreSite()
{
    ASN1T_Mission_Automate_ASA_DefendreSite& asnMission = *new ASN1T_Mission_Automate_ASA_DefendreSite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_defendre_site;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_defendre_site = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePointList( asnMission.positions_sections, "Positions sections");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_Surveiller()
{
    ASN1T_Mission_Automate_ASA_Surveiller& asnMission = *new ASN1T_Mission_Automate_ASA_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePointList( asnMission.positions_sections, "Positions sections");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_DefendreItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_DefendreItineraire()
{
    ASN1T_Mission_Automate_ASA_DefendreItineraire& asnMission = *new ASN1T_Mission_Automate_ASA_DefendreItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_defendre_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_defendre_itineraire = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_MISTRAL_DefendreZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_MISTRAL_DefendreZone()
{
    ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone& asnMission = *new ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_mistral_defendre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_mistral_defendre_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    ParamComboBox< ASN1T_EnumModeDeploiement >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement, "Mode deploiement");
    pSelector_mode_deploiement->AddItem( "En carre", EnumModeDeploiement::en_carre );
    pSelector_mode_deploiement->AddItem( "En triangle", EnumModeDeploiement::en_triangle );
    pSelector_mode_deploiement->AddItem( "Nasse trois sections", EnumModeDeploiement::nasse_trois_sections );
    pSelector_mode_deploiement->AddItem( "Nasse quatre sections", EnumModeDeploiement::nasse_quatre_sections );
    pSelector_mode_deploiement->AddItem( "Double rideau", EnumModeDeploiement::double_rideau );
    pSelector_mode_deploiement->AddItem( "Simple rideau", EnumModeDeploiement::simple_rideau );
    CreateNumeric( asnMission.angle, "Angle");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_MISTRAL_DefendreSite
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_MISTRAL_DefendreSite()
{
    ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite& asnMission = *new ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_mistral_defendre_site;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_mistral_defendre_site = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    ParamComboBox< ASN1T_EnumModeDeploiement >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement, "Mode deploiement");
    pSelector_mode_deploiement->AddItem( "En carre", EnumModeDeploiement::en_carre );
    pSelector_mode_deploiement->AddItem( "En triangle", EnumModeDeploiement::en_triangle );
    pSelector_mode_deploiement->AddItem( "Nasse trois sections", EnumModeDeploiement::nasse_trois_sections );
    pSelector_mode_deploiement->AddItem( "Nasse quatre sections", EnumModeDeploiement::nasse_quatre_sections );
    pSelector_mode_deploiement->AddItem( "Double rideau", EnumModeDeploiement::double_rideau );
    pSelector_mode_deploiement->AddItem( "Simple rideau", EnumModeDeploiement::simple_rideau );
    CreateNumeric( asnMission.angle, "Angle");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_MISTRAL_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_MISTRAL_Surveiller()
{
    ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller& asnMission = *new ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_mistral_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_mistral_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    ParamComboBox< ASN1T_EnumModeDeploiement >* pSelector_mode_deploiement = &CreateVarList( asnMission.mode_deploiement, "Mode deploiement");
    pSelector_mode_deploiement->AddItem( "En carre", EnumModeDeploiement::en_carre );
    pSelector_mode_deploiement->AddItem( "En triangle", EnumModeDeploiement::en_triangle );
    pSelector_mode_deploiement->AddItem( "Nasse trois sections", EnumModeDeploiement::nasse_trois_sections );
    pSelector_mode_deploiement->AddItem( "Nasse quatre sections", EnumModeDeploiement::nasse_quatre_sections );
    pSelector_mode_deploiement->AddItem( "Double rideau", EnumModeDeploiement::double_rideau );
    pSelector_mode_deploiement->AddItem( "Simple rideau", EnumModeDeploiement::simple_rideau );
    CreateNumeric( asnMission.angle, "Angle");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_HAWK_DefendreZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_HAWK_DefendreZone()
{
    ASN1T_Mission_Automate_ASA_HAWK_DefendreZone& asnMission = *new ASN1T_Mission_Automate_ASA_HAWK_DefendreZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_hawk_defendre_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_hawk_defendre_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePointList( asnMission.positions_sections, "Positions sections");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_HAWK_DefendreSite
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_HAWK_DefendreSite()
{
    ASN1T_Mission_Automate_ASA_HAWK_DefendreSite& asnMission = *new ASN1T_Mission_Automate_ASA_HAWK_DefendreSite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_hawk_defendre_site;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_hawk_defendre_site = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePointList( asnMission.positions_sections, "Positions sections");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_HAWK_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_HAWK_Surveiller()
{
    ASN1T_Mission_Automate_ASA_HAWK_Surveiller& asnMission = *new ASN1T_Mission_Automate_ASA_HAWK_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_hawk_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_hawk_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreatePointList( asnMission.positions_sections, "Positions sections");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASA_DefendreUnites
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASA_DefendreUnites()
{
    ASN1T_Mission_Automate_ASA_DefendreUnites& asnMission = *new ASN1T_Mission_Automate_ASA_DefendreUnites();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_defendre_unites;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asa_defendre_unites = &asnMission;
    CreateAgentList( asnMission.unites, "Unites");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASS_SeDeployer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASS_SeDeployer()
{
    ASN1T_Mission_Automate_ASS_SeDeployer& asnMission = *new ASN1T_Mission_Automate_ASS_SeDeployer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_ass_se_deployer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_ass_se_deployer = &asnMission;
    CreateLocation( asnMission.zone_deploiement, "Zone deploiement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASS_ReconnaitreZonesDeploiement
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASS_ReconnaitreZonesDeploiement()
{
    ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& asnMission = *new ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_ass_reconnaitre_zones_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_ass_reconnaitre_zones_deploiement = &asnMission;
    CreatePointList( asnMission.positions_a_reconnaitre, "Positions a reconnaitre");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASS_AcquerirObjectifs
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASS_AcquerirObjectifs()
{
    ASN1T_Mission_Automate_ASS_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Automate_ASS_AcquerirObjectifs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_ass_acquerir_objectifs;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_ass_acquerir_objectifs = &asnMission;
    CreatePolygonList( asnMission.zones_a_observer, "Zones a observer");
    CreatePointList( asnMission.positions_deploiement, "Positions deploiement");
    CreateNatureAtlas( asnMission.categories, "Categories");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_SeDeployer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_SeDeployer()
{
    ASN1T_Mission_Automate_GEN_SeDeployer& asnMission = *new ASN1T_Mission_Automate_GEN_SeDeployer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_se_deployer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_se_deployer = &asnMission;
    CreateLocation( asnMission.zone_deploiement, "Zone deploiement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_FaireFranchir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_FaireFranchir()
{
    ASN1T_Mission_Automate_GEN_FaireFranchir& asnMission = *new ASN1T_Mission_Automate_GEN_FaireFranchir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_faire_franchir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_faire_franchir = &asnMission;
    CreateObjectKnowledgeList( asnMission.sites_de_franchissement, "Sites de franchissement");
    CreateLocation( asnMission.zone_regroupement, "Zone regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_RealiserSystemeObstacles
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_RealiserSystemeObstacles()
{
    ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_systeme_obstacles;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_systeme_obstacles = &asnMission;
    CreateGenObjectList( asnMission.obstacles, "Obstacles");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_FranchirDeViveForce
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_FranchirDeViveForce()
{
    ASN1T_Mission_Automate_GEN_FranchirDeViveForce& asnMission = *new ASN1T_Mission_Automate_GEN_FranchirDeViveForce();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_franchir_de_vive_force;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_franchir_de_vive_force = &asnMission;
    CreateObjectKnowledgeList( asnMission.obstacles_a_franchir, "Obstacles a franchir");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_RetablirItineraires
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_RetablirItineraires()
{
    ASN1T_Mission_Automate_GEN_RetablirItineraires& asnMission = *new ASN1T_Mission_Automate_GEN_RetablirItineraires();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_retablir_itineraires;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_retablir_itineraires = &asnMission;
    CreatePathList( asnMission.itineraires, "Itineraires");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_AmenagerAireStationnement
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_AmenagerAireStationnement()
{
    ASN1T_Mission_Automate_GEN_AmenagerAireStationnement& asnMission = *new ASN1T_Mission_Automate_GEN_AmenagerAireStationnement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_amenager_aire_stationnement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_amenager_aire_stationnement = &asnMission;
    CreateGenObjectList( asnMission.travaux, "Travaux");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_AmenagerTerrainPoserSommaire
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_AmenagerTerrainPoserSommaire()
{
    ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& asnMission = *new ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_amenager_terrain_poser_sommaire = &asnMission;
    CreateGenObject( asnMission.terrain, "Terrain");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_RealiserZonePoser
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_RealiserZonePoser()
{
    ASN1T_Mission_Automate_GEN_RealiserZonePoser& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserZonePoser();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_zone_poser;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_zone_poser = &asnMission;
    CreateGenObject( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_ReconnaitreZoneDeploiement
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_ReconnaitreZoneDeploiement()
{
    ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement& asnMission = *new ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_reconnaitre_zone_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_reconnaitre_zone_deploiement = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_RealiserCampPrisonniers
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_RealiserCampPrisonniers()
{
    ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_camp_prisonniers;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_camp_prisonniers = &asnMission;
    CreateLocation( asnMission.camp, "Camp");
    CreateAutomate( asnMission.tc2, "Tc2");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_RealiserCampRefugies
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_RealiserCampRefugies()
{
    ASN1T_Mission_Automate_GEN_RealiserCampRefugies& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserCampRefugies();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_camp_refugies;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_camp_refugies = &asnMission;
    CreateLocation( asnMission.camp, "Camp");
    CreateAutomate( asnMission.tc2, "Tc2");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial()
{
    ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial = &asnMission;
    CreateGenObjectList( asnMission.plates_formes, "Plates formes");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_RealiserTravauxSommairesAuProfitPopulation
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_RealiserTravauxSommairesAuProfitPopulation()
{
    ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_travaux_sommaires_au_profit_population;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_realiser_travaux_sommaires_au_profit_population = &asnMission;
    CreateGenObjectList( asnMission.travaux, "Travaux");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_AppuyerDebarquementTroupes
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_AppuyerDebarquementTroupes()
{
    ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes& asnMission = *new ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_appuyer_debarquement_troupes;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_appuyer_debarquement_troupes = &asnMission;
    CreateGenObjectList( asnMission.zones_debarquement, "Zones debarquement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_GEN_OuvrirAxesProgression
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_GEN_OuvrirAxesProgression()
{
    ASN1T_Mission_Automate_GEN_OuvrirAxesProgression& asnMission = *new ASN1T_Mission_Automate_GEN_OuvrirAxesProgression();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_ouvrir_axes_progression;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_gen_ouvrir_axes_progression = &asnMission;
    CreatePathList( asnMission.axes, "Axes");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_AttaquerSouplesse
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_AttaquerSouplesse()
{
    ASN1T_Mission_Automate_INF_AttaquerSouplesse& asnMission = *new ASN1T_Mission_Automate_INF_AttaquerSouplesse();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_attaquer_souplesse;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_attaquer_souplesse = &asnMission;
    CreatePointList( asnMission.objectifs, "Objectifs");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Appuyer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Appuyer()
{
    ASN1T_Mission_Automate_INF_Appuyer& asnMission = *new ASN1T_Mission_Automate_INF_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_appuyer = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Controler
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Controler()
{
    ASN1T_Mission_Automate_INF_Controler& asnMission = *new ASN1T_Mission_Automate_INF_Controler();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_controler;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_controler = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Couvrir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Couvrir()
{
    ASN1T_Mission_Automate_INF_Couvrir& asnMission = *new ASN1T_Mission_Automate_INF_Couvrir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_couvrir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_couvrir = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
    CreatePointList( asnMission.objectifs, "Objectifs");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_DonnerCoupArret
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_DonnerCoupArret()
{
    ASN1T_Mission_Automate_INF_DonnerCoupArret& asnMission = *new ASN1T_Mission_Automate_INF_DonnerCoupArret();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_donner_coup_arret;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_donner_coup_arret = &asnMission;
    CreateLocation( asnMission.zone_regroupement, "Zone regroupement");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Eclairer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Eclairer()
{
    ASN1T_Mission_Automate_INF_Eclairer& asnMission = *new ASN1T_Mission_Automate_INF_Eclairer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_eclairer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_eclairer = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_FlancGarder
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_FlancGarder()
{
    ASN1T_Mission_Automate_INF_FlancGarder& asnMission = *new ASN1T_Mission_Automate_INF_FlancGarder();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_flanc_garder;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_flanc_garder = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Freiner
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Freiner()
{
    ASN1T_Mission_Automate_INF_Freiner& asnMission = *new ASN1T_Mission_Automate_INF_Freiner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_freiner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_freiner = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Harceler
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Harceler()
{
    ASN1T_Mission_Automate_INF_Harceler& asnMission = *new ASN1T_Mission_Automate_INF_Harceler();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_harceler;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_harceler = &asnMission;
    CreateLocation( asnMission.zone_surveillance, "Zone surveillance");
    CreateLocation( asnMission.point_regroupement, "Point regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Interdire
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Interdire()
{
    ASN1T_Mission_Automate_INF_Interdire& asnMission = *new ASN1T_Mission_Automate_INF_Interdire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_interdire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_interdire = &asnMission;
    CreatePointList( asnMission.points_a_interdire, "Points a interdire");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_MenerDefenseUsure
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_MenerDefenseUsure()
{
    ASN1T_Mission_Automate_INF_MenerDefenseUsure& asnMission = *new ASN1T_Mission_Automate_INF_MenerDefenseUsure();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_mener_defense_usure;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_mener_defense_usure = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_PrendrePreciserMaintenirContact
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_PrendrePreciserMaintenirContact()
{
    ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact& asnMission = *new ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_prendre_preciser_maintenir_contact;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_prendre_preciser_maintenir_contact = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_ReconnaissanceOffensive
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_ReconnaissanceOffensive()
{
    ASN1T_Mission_Automate_INF_ReconnaissanceOffensive& asnMission = *new ASN1T_Mission_Automate_INF_ReconnaissanceOffensive();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_reconnaissance_offensive;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_reconnaissance_offensive = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_RecueillirUnite
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_RecueillirUnite()
{
    ASN1T_Mission_Automate_INF_RecueillirUnite& asnMission = *new ASN1T_Mission_Automate_INF_RecueillirUnite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_recueillir_unite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_recueillir_unite = &asnMission;
    CreateLocation( asnMission.zone_deploiement, "Zone deploiement");
    CreateAutomate( asnMission.compagnie, "Compagnie");
    CreatePointList( asnMission.pias, "Pias");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Reduire
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Reduire()
{
    ASN1T_Mission_Automate_INF_Reduire& asnMission = *new ASN1T_Mission_Automate_INF_Reduire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_reduire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_reduire = &asnMission;
    CreatePointList( asnMission.points_resistance, "Points resistance");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Relever
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Relever()
{
    ASN1T_Mission_Automate_INF_Relever& asnMission = *new ASN1T_Mission_Automate_INF_Relever();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_relever;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_relever = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_RompreContact
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_RompreContact()
{
    ASN1T_Mission_Automate_INF_RompreContact& asnMission = *new ASN1T_Mission_Automate_INF_RompreContact();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_rompre_contact;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_rompre_contact = &asnMission;
    CreateLocation( asnMission.zone_regroupement, "Zone regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Surveiller()
{
    ASN1T_Mission_Automate_INF_Surveiller& asnMission = *new ASN1T_Mission_Automate_INF_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
    CreateLocation( asnMission.zone_regroupement, "Zone regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Semparer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Semparer()
{
    ASN1T_Mission_Automate_INF_Semparer& asnMission = *new ASN1T_Mission_Automate_INF_Semparer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_semparer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_semparer = &asnMission;
    CreateLocation( asnMission.objectif, "Objectif");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Fixer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Fixer()
{
    ASN1T_Mission_Automate_INF_Fixer& asnMission = *new ASN1T_Mission_Automate_INF_Fixer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_fixer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_fixer = &asnMission;
    CreateAgentKnowledgeList( asnMission.ennemis, "Ennemis");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_AttaquerEnTerrainDifficile
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_AttaquerEnTerrainDifficile()
{
    ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile& asnMission = *new ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_attaquer_en_terrain_difficile;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_attaquer_en_terrain_difficile = &asnMission;
    CreatePointList( asnMission.objectifs, "Objectifs");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_BarrerDirection
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_BarrerDirection()
{
    ASN1T_Mission_Automate_INF_BarrerDirection& asnMission = *new ASN1T_Mission_Automate_INF_BarrerDirection();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_barrer_direction;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_barrer_direction = &asnMission;
    CreateLocation( asnMission.position, "Position");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_DefendreFerme
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_DefendreFerme()
{
    ASN1T_Mission_Automate_INF_DefendreFerme& asnMission = *new ASN1T_Mission_Automate_INF_DefendreFerme();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_defendre_ferme;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_defendre_ferme = &asnMission;
    CreateLocation( asnMission.position, "Position");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Tenir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Tenir()
{
    ASN1T_Mission_Automate_INF_Tenir& asnMission = *new ASN1T_Mission_Automate_INF_Tenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_tenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_tenir = &asnMission;
    CreateLocation( asnMission.position, "Position");
    CreateBool( asnMission.preparer_terrain, "Preparer terrain");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_AppuyerUnFreinage
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_AppuyerUnFreinage()
{
    ASN1T_Mission_Automate_INF_AppuyerUnFreinage& asnMission = *new ASN1T_Mission_Automate_INF_AppuyerUnFreinage();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_appuyer_un_freinage;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_appuyer_un_freinage = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_AppuyerUneAttaque
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_AppuyerUneAttaque()
{
    ASN1T_Mission_Automate_INF_AppuyerUneAttaque& asnMission = *new ASN1T_Mission_Automate_INF_AppuyerUneAttaque();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_appuyer_une_attaque;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_appuyer_une_attaque = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_Soutenir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_Soutenir()
{
    ASN1T_Mission_Automate_INF_Soutenir& asnMission = *new ASN1T_Mission_Automate_INF_Soutenir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_soutenir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_soutenir = &asnMission;
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_EscorterUnite
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_EscorterUnite()
{
    ASN1T_Mission_Automate_INF_EscorterUnite& asnMission = *new ASN1T_Mission_Automate_INF_EscorterUnite();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_escorter_unite;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_escorter_unite = &asnMission;
    CreateAgent( asnMission.unite_a_escorter, "Unite a escorter");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_EscorterUnites
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_EscorterUnites()
{
    ASN1T_Mission_Automate_INF_EscorterUnites& asnMission = *new ASN1T_Mission_Automate_INF_EscorterUnites();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_escorter_unites;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_escorter_unites = &asnMission;
    CreateAutomate( asnMission.automate_a_escorter, "Automate a escorter");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_INF_AppuyerSurPosition
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_INF_AppuyerSurPosition()
{
    ASN1T_Mission_Automate_INF_AppuyerSurPosition& asnMission = *new ASN1T_Mission_Automate_INF_AppuyerSurPosition();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_appuyer_sur_position;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_inf_appuyer_sur_position = &asnMission;
    CreateLocation( asnMission.position_installation, "Position installation");
    CreateAutomate( asnMission.compagnie, "Compagnie");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_SeDeployer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_SeDeployer()
{
    ASN1T_Mission_Automate_LOG_SeDeployer& asnMission = *new ASN1T_Mission_Automate_LOG_SeDeployer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_se_deployer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_se_deployer = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_AppuyerMouvement
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_AppuyerMouvement()
{
    ASN1T_Mission_Automate_LOG_AppuyerMouvement& asnMission = *new ASN1T_Mission_Automate_LOG_AppuyerMouvement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_appuyer_mouvement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_appuyer_mouvement = &asnMission;
    CreateAgentList( asnMission.unites_a_appuyer, "Unites a appuyer");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_ReconnaitreItineraire
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_ReconnaitreItineraire()
{
    ASN1T_Mission_Automate_LOG_ReconnaitreItineraire& asnMission = *new ASN1T_Mission_Automate_LOG_ReconnaitreItineraire();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_reconnaitre_itineraire;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_reconnaitre_itineraire = &asnMission;
    CreatePath( asnMission.itineraire_log, "Itineraire log");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_Surveiller
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_Surveiller()
{
    ASN1T_Mission_Automate_LOG_Surveiller& asnMission = *new ASN1T_Mission_Automate_LOG_Surveiller();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_surveiller;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_surveiller = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_TransporterUnites
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_TransporterUnites()
{
    ASN1T_Mission_Automate_LOG_TransporterUnites& asnMission = *new ASN1T_Mission_Automate_LOG_TransporterUnites();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_transporter_unites;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_transporter_unites = &asnMission;
    CreateAgentList( asnMission.unites_a_transporter, "Unites a transporter");
    CreatePoint( asnMission.point_rendez_vous, "Point rendez vous");
    CreatePoint( asnMission.point_destination, "Point destination");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_AppuyerMouvementDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_AppuyerMouvementDansZone()
{
    ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone& asnMission = *new ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_appuyer_mouvement_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_appuyer_mouvement_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_MettreEnOeuvreZoneStationnement
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_MettreEnOeuvreZoneStationnement()
{
    ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement& asnMission = *new ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_mettre_en_oeuvre_zone_stationnement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_mettre_en_oeuvre_zone_stationnement = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_AppuyerFranchissementDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_AppuyerFranchissementDansZone()
{
    ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone& asnMission = *new ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_appuyer_franchissement_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_appuyer_franchissement_dans_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_BloquerZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_BloquerZone()
{
    ASN1T_Mission_Automate_LOG_BloquerZone& asnMission = *new ASN1T_Mission_Automate_LOG_BloquerZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_bloquer_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_bloquer_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_ReconnaitreZoneContaminee
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_ReconnaitreZoneContaminee()
{
    ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee& asnMission = *new ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_reconnaitre_zone_contaminee;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_reconnaitre_zone_contaminee = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_ReconnaitreZoneDeDeploiement
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_ReconnaitreZoneDeDeploiement()
{
    ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement& asnMission = *new ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_reconnaitre_zone_de_deploiement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_reconnaitre_zone_de_deploiement = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_LOG_ArmerPIAs
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_LOG_ArmerPIAs()
{
    ASN1T_Mission_Automate_LOG_ArmerPIAs& asnMission = *new ASN1T_Mission_Automate_LOG_ArmerPIAs();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_armer_pias;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_log_armer_pias = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_Test_MoveTo
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_Test_MoveTo()
{
    ASN1T_Mission_Automate_Test_MoveTo& asnMission = *new ASN1T_Mission_Automate_Test_MoveTo();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_test_move_to;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_test_move_to = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_Decrocher
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_Decrocher()
{
    ASN1T_Mission_Automate_Decrocher& asnMission = *new ASN1T_Mission_Automate_Decrocher();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_decrocher;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_decrocher = &asnMission;
    CreateLocation( asnMission.point_regroupement, "Point regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ReleverSurPlace
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ReleverSurPlace()
{
    ASN1T_Mission_Automate_ReleverSurPlace& asnMission = *new ASN1T_Mission_Automate_ReleverSurPlace();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_relever_sur_place;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_relever_sur_place = &asnMission;
    CreateAutomate( asnMission.compagnie_a_relever, "Compagnie a relever");
    CreateLocation( asnMission.zone_installation, "Zone installation");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_FaireMouvement
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_FaireMouvement()
{
    ASN1T_Mission_Automate_FaireMouvement& asnMission = *new ASN1T_Mission_Automate_FaireMouvement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_faire_mouvement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_faire_mouvement = &asnMission;
    CreatePath( asnMission.itineraire, "Itineraire");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_SeFaireDecontaminer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_SeFaireDecontaminer()
{
    ASN1T_Mission_Automate_SeFaireDecontaminer& asnMission = *new ASN1T_Mission_Automate_SeFaireDecontaminer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_se_faire_decontaminer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_se_faire_decontaminer = &asnMission;
    CreateLocation( asnMission.zone_arrivee, "Zone arrivee");
    CreateObjectKnowledge( asnMission.site_decontamination, "Site decontamination");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_Franchir
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_Franchir()
{
    ASN1T_Mission_Automate_Franchir& asnMission = *new ASN1T_Mission_Automate_Franchir();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_franchir;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_franchir = &asnMission;
    CreateObjectKnowledge( asnMission.pont_flottant, "Pont flottant");
    CreateLocation( asnMission.zone_arrivee, "Zone arrivee");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_Stationner
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_Stationner()
{
    ASN1T_Mission_Automate_Stationner& asnMission = *new ASN1T_Mission_Automate_Stationner();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_stationner;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_stationner = &asnMission;
    CreateLocation( asnMission.zone_attente, "Zone attente");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_SeFaireTransporter
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_SeFaireTransporter()
{
    ASN1T_Mission_Automate_SeFaireTransporter& asnMission = *new ASN1T_Mission_Automate_SeFaireTransporter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_se_faire_transporter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_se_faire_transporter = &asnMission;
    CreateLocation( asnMission.zone_embarquement, "Zone embarquement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_SeRecompleter
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_SeRecompleter()
{
    ASN1T_Mission_Automate_SeRecompleter& asnMission = *new ASN1T_Mission_Automate_SeRecompleter();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_se_recompleter;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_se_recompleter = &asnMission;
    CreateLocation( asnMission.zone_ravitaillement, "Zone ravitaillement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_SeRendre
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_SeRendre()
{
    ASN1T_Mission_Automate_SeRendre& asnMission = *new ASN1T_Mission_Automate_SeRendre();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_se_rendre;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_se_rendre = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_FaireMouvementVersCampRefugies
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_FaireMouvementVersCampRefugies()
{
    ASN1T_Mission_Automate_FaireMouvementVersCampRefugies& asnMission = *new ASN1T_Mission_Automate_FaireMouvementVersCampRefugies();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_faire_mouvement_vers_camp_refugies;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_faire_mouvement_vers_camp_refugies = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_InterdireFranchissementPopulations
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_InterdireFranchissementPopulations()
{
    ASN1T_Mission_Automate_InterdireFranchissementPopulations& asnMission = *new ASN1T_Mission_Automate_InterdireFranchissementPopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_interdire_franchissement_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_interdire_franchissement_populations = &asnMission;
    CreatePointList( asnMission.points, "Points");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_FiltrerPopulations
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_FiltrerPopulations()
{
    ASN1T_Mission_Automate_FiltrerPopulations& asnMission = *new ASN1T_Mission_Automate_FiltrerPopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_filtrer_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_filtrer_populations = &asnMission;
    CreatePointList( asnMission.points, "Points");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_CanaliserPopulations
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_CanaliserPopulations()
{
    ASN1T_Mission_Automate_CanaliserPopulations& asnMission = *new ASN1T_Mission_Automate_CanaliserPopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_canaliser_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_canaliser_populations = &asnMission;
    CreateLocationList( asnMission.zones, "Zones");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ControlerPopulationsDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ControlerPopulationsDansZone()
{
    ASN1T_Mission_Automate_ControlerPopulationsDansZone& asnMission = *new ASN1T_Mission_Automate_ControlerPopulationsDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_controler_populations_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_controler_populations_dans_zone = &asnMission;
    CreatePolygonList( asnMission.zones, "Zones");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_SecuriserZoneContrePopulations
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_SecuriserZoneContrePopulations()
{
    ASN1T_Mission_Automate_SecuriserZoneContrePopulations& asnMission = *new ASN1T_Mission_Automate_SecuriserZoneContrePopulations();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_securiser_zone_contre_populations;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_securiser_zone_contre_populations = &asnMission;
    CreatePointList( asnMission.points, "Points");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_ASY_CommettreExactionsSurPopulationDansZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_ASY_CommettreExactionsSurPopulationDansZone()
{
    ASN1T_Mission_Automate_ASY_CommettreExactionsSurPopulationDansZone& asnMission = *new ASN1T_Mission_Automate_ASY_CommettreExactionsSurPopulationDansZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asy_commettre_exactions_sur_population_dans_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_asy_commettre_exactions_sur_population_dans_zone = &asnMission;
    CreatePolygonList( asnMission.zones, "Zones");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_Generique
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_Generique()
{
    ASN1T_Mission_Automate_Generique& asnMission = *new ASN1T_Mission_Automate_Generique();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_generique;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_generique = &asnMission;
    CreatePolygonList( asnMission.zones, "Zones");
    CreatePointList( asnMission.points, "Points");
    CreatePathList( asnMission.itineraires, "Itineraires");
    CreateAgentList( asnMission.unites, "Unites");
    CreateAutomateList( asnMission.automates, "Automates");
    CreateAgentKnowledgeList( asnMission.connaissances_agents, "Connaissances agents");
    CreateObjectKnowledgeList( asnMission.connaissances_objets, "Connaissances objets");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_NBC_ReconnaitreUnAxe
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_NBC_ReconnaitreUnAxe()
{
    ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe& asnMission = *new ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_reconnaitre_un_axe;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_un_axe = &asnMission;
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_NBC_ReconnaitreUneZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_NBC_ReconnaitreUneZone()
{
    ASN1T_Mission_Automate_NBC_ReconnaitreUneZone& asnMission = *new ASN1T_Mission_Automate_NBC_ReconnaitreUneZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_reconnaitre_une_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_une_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_NBC_ArmerUnSiteDeDecontamination
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_NBC_ArmerUnSiteDeDecontamination()
{
    ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *new ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_armer_un_site_de_decontamination = &asnMission;
    CreateLocation( asnMission.site, "Site");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_NBC_DecontaminerUneZone
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_NBC_DecontaminerUneZone()
{
    ASN1T_Mission_Automate_NBC_DecontaminerUneZone& asnMission = *new ASN1T_Mission_Automate_NBC_DecontaminerUneZone();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_decontaminer_une_zone;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_decontaminer_une_zone = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_NBC_ReconnaitreDesSitesROTA
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_NBC_ReconnaitreDesSitesROTA()
{
    ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& asnMission = *new ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_reconnaitre_des_sites_rota;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_des_sites_rota = &asnMission;
    CreateObjectKnowledgeList( asnMission.sites_ROTA, "Sites ROTA");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_RENS_ROHUM_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_RENS_ROHUM_RenseignerSur()
{
    ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_rohum_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_rohum_renseigner_sur = &asnMission;
    CreateLocation( asnMission.zone_a_observer, "Zone a observer");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_RENS_ROHUM_OrienterGuider
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_RENS_ROHUM_OrienterGuider()
{
    ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider& asnMission = *new ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_rohum_orienter_guider;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_rohum_orienter_guider = &asnMission;
    CreateAutomate( asnMission.ami, "Ami");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_RENS_ROHUM_SExfiltrer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_RENS_ROHUM_SExfiltrer()
{
    ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *new ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_rohum_sexfiltrer = &asnMission;
    CreateLocation( asnMission.zone_de_regroupement, "Zone de regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_RENS_ROIM_CL289_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_RENS_ROIM_CL289_RenseignerSur()
{
    ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_roim_cl289_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_roim_cl289_renseigner_sur = &asnMission;
    CreatePath( asnMission.ficelle_de_vole, "Ficelle de vole");
    CreatePolygonList( asnMission.zones_de_recherche, "Zones de recherche");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_RENS_ROIM_SDTI_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_RENS_ROIM_SDTI_RenseignerSur()
{
    ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_roim_sdti_renseigner_sur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_roim_sdti_renseigner_sur = &asnMission;
    CreatePath( asnMission.ficelle_de_vole, "Ficelle de vole");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_RENS_ROEM_Appuyer
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_RENS_ROEM_Appuyer()
{
    ASN1T_Mission_Automate_RENS_ROEM_Appuyer& asnMission = *new ASN1T_Mission_Automate_RENS_ROEM_Appuyer();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_roem_appuyer;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_roem_appuyer = &asnMission;
    CreatePoint( asnMission.position_deploiement, "Position deploiement");
    CreatePoint( asnMission.point_regroupement, "Point regroupement");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_RENS_ROEM_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_RENS_ROEM_RenseignerSur()
{
    ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_roem_renseignersur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_roem_renseignersur = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_RENS_RADINT_RenseignerSur
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_RENS_RADINT_RenseignerSur()
{
    ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_radint_renseignersur;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_rens_radint_renseignersur = &asnMission;
    CreateLocation( asnMission.zone, "Zone");
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_TRANS_Commander
// Created: AGR
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateMission_TRANS_Commander()
{
    ASN1T_Mission_Automate_TRANS_Commander& asnMission = *new ASN1T_Mission_Automate_TRANS_Commander();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_trans_commander;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_trans_commander = &asnMission;
    CreateLocation( asnMission.zone_deploiement, "Zone deploiement");
}

