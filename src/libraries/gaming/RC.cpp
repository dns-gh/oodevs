// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "RC.h"
#include "clients_kernel/Agent_ABC.h"
#include "InternalLinks.h"

//-----------------------------------------------------------------------------
// Name: RC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
RC::RC( const Entity_ABC& agent, const Simulation& simulation, const ASN1T_MsgCR& asnMsg )
    : Report_ABC( agent, simulation )
{
    Initialize( asnMsg  );
}

//-----------------------------------------------------------------------------
// Name: RC destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
RC::~RC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: RC::Initialize
// Created: NLD 2002-10-07
//-----------------------------------------------------------------------------
void RC::Initialize( const ASN1T_MsgCR& asnMsg )
{
    std::stringstream strMsg;
    switch( asnMsg.cr.t )
    {
    
        case T_MsgCR_cr_cr_ras : strMsg << "ras"; break;
        case T_MsgCR_cr_cr_en_poste_face_a_obstacle : strMsg << "en poste face a obstacle" << " - ConnaissanceObjet : " << " " << ObjectKnowledgeLink( asnMsg.cr.u.cr_en_poste_face_a_obstacle ); break;
        case T_MsgCR_cr_cr_en_contournement_obstacle : strMsg << "en contournement obstacle"; break;
        case T_MsgCR_cr_cr_mission_impossible : strMsg << "mission impossible"; break;
        case T_MsgCR_cr_cr_progression_sur_axe : strMsg << "progression sur axe"; break;
        case T_MsgCR_cr_cr_progression_vers_position : strMsg << "progression vers position"; break;
        case T_MsgCR_cr_cr_arrive_sur_position : strMsg << "arrive sur position"; break;
        case T_MsgCR_cr_cr_reconnaissance_point : strMsg << "reconnaissance point"; break;
        case T_MsgCR_cr_cr_en_poste_face_a_direction_dangereuse : strMsg << "en poste face a direction dangereuse"; break;
        case T_MsgCR_cr_cr_en_jalonnement : strMsg << "en jalonnement"; break;
        case T_MsgCR_cr_cr_preparation_mission : strMsg << "preparation mission" << " - delai : " << asnMsg.cr.u.cr_preparation_mission->delai; break;
        case T_MsgCR_cr_cr_debut_mission : strMsg << "debut mission"; break;
        case T_MsgCR_cr_cr_fin_mission : strMsg << "fin mission"; break;
        case T_MsgCR_cr_cr_dispositif_installe : strMsg << "dispositif installe"; break;
        case T_MsgCR_cr_cr_en_position : strMsg << "en position"; break;
        case T_MsgCR_cr_cr_objectif_atteint : strMsg << "objectif atteint"; break;
        case T_MsgCR_cr_cr_debouche : strMsg << "debouche"; break;
        case T_MsgCR_cr_cr_rejoint_unite : strMsg << "rejoint unite"; break;
        case T_MsgCR_cr_cr_escorte_unite : strMsg << "escorte unite"; break;
        case T_MsgCR_cr_cr_en_freinage : strMsg << "en freinage"; break;
        case T_MsgCR_cr_cr_assaut : strMsg << "assaut"; break;
        case T_MsgCR_cr_cr_en_couverture : strMsg << "en couverture"; break;
        case T_MsgCR_cr_cr_en_eclairage : strMsg << "en eclairage"; break;
        case T_MsgCR_cr_cr_en_manoeuvre_ravitaillement : strMsg << "en manoeuvre ravitaillement"; break;
        case T_MsgCR_cr_cr_sur_pia : strMsg << "sur pia" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_sur_pia ); break;
        case T_MsgCR_cr_cr_passage_sur_pia : strMsg << "passage sur pia"; break;
        case T_MsgCR_cr_cr_unite_recueillie : strMsg << "unite recueillie" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_unite_recueillie ); break;
        case T_MsgCR_cr_cr_en_cours_de_franchissement : strMsg << "en cours de franchissement"; break;
        case T_MsgCR_cr_cr_attente_ordre_poursuivre : strMsg << "attente ordre poursuivre"; break;
        case T_MsgCR_cr_cr_attente_ordre_deboucher : strMsg << "attente ordre deboucher"; break;
        case T_MsgCR_cr_cr_fixe : strMsg << "fixe"; break;
        case T_MsgCR_cr_cr_en_esquive : strMsg << "en esquive"; break;
        case T_MsgCR_cr_cr_prise_contact : strMsg << "prise contact"; break;
        case T_MsgCR_cr_cr_rupture_contact : strMsg << "rupture contact"; break;
        case T_MsgCR_cr_cr_precision_contact : strMsg << "precision contact" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_precision_contact ); break;
        case T_MsgCR_cr_cr_ennemi_detruit : strMsg << "ennemi detruit" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_ennemi_detruit ); break;
        case T_MsgCR_cr_cr_pris_a_partie_par_tir_direct : strMsg << "pris a partie par tir direct"; break;
        case T_MsgCR_cr_cr_pris_sous_tir_artillerie : strMsg << "pris sous tir artillerie"; break;
        case T_MsgCR_cr_cr_point_tenu_par_eni : strMsg << "point tenu par eni" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_point_tenu_par_eni ); break;
        case T_MsgCR_cr_cr_progression_vers_eni : strMsg << "progression vers eni" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_progression_vers_eni ); break;
        case T_MsgCR_cr_cr_decroche : strMsg << "decroche"; break;
        case T_MsgCR_cr_cr_eni_sur_objectif : strMsg << "eni sur objectif" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_eni_sur_objectif ); break;
        case T_MsgCR_cr_cr_riposte : strMsg << "riposte"; break;
        case T_MsgCR_cr_cr_pret_pour_engagement_sur_ennemi : strMsg << "pret pour engagement sur ennemi"; break;
        case T_MsgCR_cr_cr_debut_controle_zone : strMsg << "debut controle zone"; break;
        case T_MsgCR_cr_cr_fin_controle_zone : strMsg << "fin controle zone"; break;
        case T_MsgCR_cr_cr_debut_surveillance : strMsg << "debut surveillance"; break;
        case T_MsgCR_cr_cr_fin_surveillance : strMsg << "fin surveillance"; break;
        case T_MsgCR_cr_cr_en_infiltration : strMsg << "en infiltration"; break;
        case T_MsgCR_cr_cr_en_exfiltration : strMsg << "en exfiltration"; break;
        case T_MsgCR_cr_cr_attente_condition_favorable_pour_infiltration : strMsg << "attente condition favorable pour infiltration"; break;
        case T_MsgCR_cr_cr_drone_lance : strMsg << "drone lance"; break;
        case T_MsgCR_cr_cr_drone_en_exploitation : strMsg << "drone en exploitation"; break;
        case T_MsgCR_cr_cr_drone_exploite : strMsg << "drone exploite"; break;
        case T_MsgCR_cr_cr_drone_disponible : strMsg << "drone disponible"; break;
        case T_MsgCR_cr_cr_debut_mise_en_oeuvre_drone : strMsg << "debut mise en oeuvre drone"; break;
        case T_MsgCR_cr_cr_fin_mise_en_oeuvre_drone : strMsg << "fin mise en oeuvre drone"; break;
        case T_MsgCR_cr_cr_activation_brouillage : strMsg << "activation brouillage"; break;
        case T_MsgCR_cr_cr_desactivation_brouillage : strMsg << "desactivation brouillage"; break;
        case T_MsgCR_cr_cr_activation_systeme_ecoute : strMsg << "activation systeme ecoute"; break;
        case T_MsgCR_cr_cr_desactivation_systeme_ecoute : strMsg << "desactivation systeme ecoute"; break;
        case T_MsgCR_cr_cr_activation_systeme_ecoute_radar : strMsg << "activation systeme ecoute radar"; break;
        case T_MsgCR_cr_cr_desactivation_systeme_ecoute_radar : strMsg << "desactivation systeme ecoute radar"; break;
        case T_MsgCR_cr_cr_debut_liaison : strMsg << "debut liaison"; break;
        case T_MsgCR_cr_cr_fin_liaison : strMsg << "fin liaison"; break;
        case T_MsgCR_cr_cr_etablissement_liaison_impossible : strMsg << "etablissement liaison impossible"; break;
        case T_MsgCR_cr_cr_unite_trop_distante : strMsg << "unite trop distante"; break;
        case T_MsgCR_cr_cr_enregistrement_donnes : strMsg << "enregistrement donnes"; break;
        case T_MsgCR_cr_cr_exploitation_donnees : strMsg << "exploitation donnees"; break;
        case T_MsgCR_cr_cr_extraction_donnes : strMsg << "extraction donnes"; break;
        case T_MsgCR_cr_cr_releve : strMsg << "releve" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_releve ); break;
        case T_MsgCR_cr_cr_suit_unite : strMsg << "suit unite"; break;
        case T_MsgCR_cr_cr_passe_en_appui : strMsg << "passe en appui"; break;
        case T_MsgCR_cr_cr_en_appui : strMsg << "en appui"; break;
        case T_MsgCR_cr_cr_non_operationnelle : strMsg << "non operationnel"; break;
        case T_MsgCR_cr_cr_operationnelle : strMsg << "operationnel"; break;
        case T_MsgCR_cr_cr_ambiance_vitesse : strMsg << "ambiance vitesse"; break;
        case T_MsgCR_cr_cr_ambiance_surete : strMsg << "ambiance surete"; break;
        case T_MsgCR_cr_cr_sur_lima : strMsg << "sur lima" << " - id : " << " " << asnMsg.cr.u.cr_sur_lima; break;
        case T_MsgCR_cr_cr_debut_travaux : strMsg << "debut travaux"; break;
        case T_MsgCR_cr_cr_fin_travaux : strMsg << "fin travaux"; break;
        case T_MsgCR_cr_cr_obstacle_degage : strMsg << "obstacle degage"; break;
        case T_MsgCR_cr_cr_organisation_chantier : strMsg << "organisation chantier"; break;
        case T_MsgCR_cr_cr_desorganisation_chantier : strMsg << "desorganisation chantier"; break;
        case T_MsgCR_cr_cr_reconnaissance_position_obstacle : strMsg << "reconnaissance position obstacle"; break;
        case T_MsgCR_cr_cr_contournement_realise : strMsg << "contournement realise"; break;
        case T_MsgCR_cr_cr_attente_renforcement : strMsg << "attente renforcement"; break;
        case T_MsgCR_cr_cr_debut_exploitation_site_franchissement : strMsg << "debut exploitation site franchissement"; break;
        case T_MsgCR_cr_cr_fin_exploitation_site_franchissement : strMsg << "fin exploitation site franchissement"; break;
        case T_MsgCR_cr_cr_reussite_renforcement : strMsg << "reussite renforcement" << " - pion_renforcant : " << AgentLink( asnMsg.cr.u.cr_reussite_renforcement->pion_renforcant ) << " - automate_renforce : " << AgentLink( asnMsg.cr.u.cr_reussite_renforcement->automate_renforce ); break;
        case T_MsgCR_cr_cr_echec_renforcement : strMsg << "echec renforcement" << " - pion_renforcant : " << AgentLink( asnMsg.cr.u.cr_echec_renforcement->pion_renforcant ) << " - automate_renforce : " << AgentLink( asnMsg.cr.u.cr_echec_renforcement->automate_renforce ); break;
        case T_MsgCR_cr_cr_obstacle_en_attente_activation : strMsg << "obstacle en attente activation" << " - ConnaissanceObjet : " << " " << ObjectKnowledgeLink( asnMsg.cr.u.cr_obstacle_en_attente_activation ); break;
        case T_MsgCR_cr_cr_debut_amenagement_berges : strMsg << "debut amenagement berges"; break;
        case T_MsgCR_cr_cr_fin_amenagement_berges : strMsg << "fin amenagement berges"; break;
        case T_MsgCR_cr_cr_regime_maintenance_delai_depasse : strMsg << "regime maintenance delai depasse"; break;
        case T_MsgCR_cr_cr_section_deployee : strMsg << "section deployee"; break;
        case T_MsgCR_cr_cr_a_nouveau_disponible_apres_reparation : strMsg << "a nouveau disponible apres reparation"; break;
        case T_MsgCR_cr_cr_materiel_retour_de_maintenance : strMsg << "materiel retour de maintenance" << " - TypeEquipement : " << " " << asnMsg.cr.u.cr_materiel_retour_de_maintenance; break;
        case T_MsgCR_cr_cr_materiel_repare_sur_place : strMsg << "materiel repare sur place" << " - TypeEquipement : " << " " << asnMsg.cr.u.cr_materiel_repare_sur_place; break;
        case T_MsgCR_cr_cr_demande_ravitaillement_dotations : strMsg << "demande ravitaillement dotations"; break;
        case T_MsgCR_cr_cr_demande_ravitaillement_stock : strMsg << "demande ravitaillement stock"; break;
        case T_MsgCR_cr_cr_demande_evacuation_sanitaire : strMsg << "demande evacuation sanitaire"; break;
        case T_MsgCR_cr_cr_demande_evacuation_materiel : strMsg << "demande evacuation materiel"; break;
        case T_MsgCR_cr_cr_allocation_consentie_bientot_epuisee : strMsg << "allocation consentie bientot epuisee" << " - TypeDotation : " << " " << asnMsg.cr.u.cr_allocation_consentie_bientot_epuisee; break;
        case T_MsgCR_cr_cr_depassement_capacite_stockage : strMsg << "depassement capacite stockage"; break;
        case T_MsgCR_cr_cr_seuil_logistique_dotation_depasse : strMsg << "seuil logistique dotation depasse" << " - TypeDotation : " << " " << asnMsg.cr.u.cr_seuil_logistique_dotation_depasse; break;
        case T_MsgCR_cr_cr_seuil_logistique_stock_depasse : strMsg << "seuil logistique stock depasse" << " - TypeDotation : " << " " << asnMsg.cr.u.cr_seuil_logistique_stock_depasse; break;
        case T_MsgCR_cr_cr_ravitaillement_dotations_effectue : strMsg << "ravitaillement dotations effectue"; break;
        case T_MsgCR_cr_cr_ravitaillement_stock_effectue : strMsg << "ravitaillement stock effectue"; break;
        case T_MsgCR_cr_cr_ravitaillement_dotations_annule : strMsg << "ravitaillement dotations annule"; break;
        case T_MsgCR_cr_cr_ravitaillement_stock_annule : strMsg << "ravitaillement stock annule"; break;
        case T_MsgCR_cr_cr_humain_retour_de_sante : strMsg << "humain retour de sante"; break;
        case T_MsgCR_cr_cr_alerte_disponibilite_moyens_releve : strMsg << "alerte disponibilite moyens releve"; break;
        case T_MsgCR_cr_cr_alerte_disponibilite_moyens_ramassage : strMsg << "alerte disponibilite moyens ramassage"; break;
        case T_MsgCR_cr_cr_alerte_disponibilite_medecins : strMsg << "alerte disponibilite medecins"; break;
        case T_MsgCR_cr_cr_alerte_disponibilite_remorqueurs : strMsg << "alerte disponibilite remorqueurs"; break;
        case T_MsgCR_cr_cr_alerte_disponibilite_reparateurs : strMsg << "alerte disponibilite reparateurs"; break;
        case T_MsgCR_cr_cr_alerte_disponibilite_vecteurs : strMsg << "alerte disponibilite vecteurs"; break;
        case T_MsgCR_cr_cr_pret_materiel_en_cours : strMsg << "pret materiel en cours"; break;
        case T_MsgCR_cr_cr_pret_materiel_annule : strMsg << "pret materiel annule"; break;
        case T_MsgCR_cr_cr_pret_materiel_effectue : strMsg << "pret materiel effectue"; break;
        case T_MsgCR_cr_cr_pret_materiel_impossible : strMsg << "pret materiel impossible"; break;
        case T_MsgCR_cr_cr_pret_materiel_partiellement_effectue : strMsg << "pret materiel partiellement effectue"; break;
        case T_MsgCR_cr_cr_recuperation_materiel_prete_impossible : strMsg << "recuperation materiel prete impossible"; break;
        case T_MsgCR_cr_cr_recuperation_materiel_prete_effectuee : strMsg << "recuperation materiel prete effectuee"; break;
        case T_MsgCR_cr_cr_recuperation_materiel_prete_partiellement_effectuee : strMsg << "recuperation materiel prete partiellement effectuee"; break;
        case T_MsgCR_cr_cr_materiel_prete : strMsg << "materiel prete"; break;
        case T_MsgCR_cr_cr_materiel_rendu : strMsg << "materiel rendu"; break;
        case T_MsgCR_cr_cr_convoi_deplacement_vers_point_chargement : strMsg << "convoi_ deplacement vers point chargement"; break;
        case T_MsgCR_cr_cr_convoi_chargement_en_cours : strMsg << "convoi_ chargement en cours"; break;
        case T_MsgCR_cr_cr_convoi_chargement_effectue : strMsg << "convoi_ chargement effectue"; break;
        case T_MsgCR_cr_cr_convoi_deplacement_vers_point_dechargement : strMsg << "convoi_ deplacement vers point dechargement"; break;
        case T_MsgCR_cr_cr_convoi_dechargement_en_cours : strMsg << "convoi_ dechargement en cours"; break;
        case T_MsgCR_cr_cr_convoi_dechargement_effectue : strMsg << "convoi_ dechargement effectue"; break;
        case T_MsgCR_cr_cr_convoi_retour : strMsg << "convoi_ retour"; break;
        case T_MsgCR_cr_cr_en_cours_ravitaillement : strMsg << "en cours ravitaillement"; break;
        case T_MsgCR_cr_cr_ravitaillement_termine : strMsg << "ravitaillement termine"; break;
        case T_MsgCR_cr_cr_helicoptere_en_observation : strMsg << "helicoptere en observation"; break;
        case T_MsgCR_cr_cr_perte_info_guidage : strMsg << "perte info guidage"; break;
        case T_MsgCR_cr_cr_transport_unite_pas_prete : strMsg << "transport unite pas prete" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_transport_unite_pas_prete ); break;
        case T_MsgCR_cr_cr_transport_unite_prete : strMsg << "transport unite prete" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_transport_unite_prete ); break;
        case T_MsgCR_cr_cr_transport_en_cours : strMsg << "transport en cours"; break;
        case T_MsgCR_cr_cr_transport_embarquement : strMsg << "transport embarquement"; break;
        case T_MsgCR_cr_cr_transport_debarquement : strMsg << "transport debarquement"; break;
        case T_MsgCR_cr_cr_vehicule_laisse_sur_position : strMsg << "vehicule laisse sur position"; break;
        case T_MsgCR_cr_cr_rejoint_position_embarquement : strMsg << "rejoint position embarquement"; break;
        case T_MsgCR_cr_cr_en_attente_transporteurs : strMsg << "en attente transporteurs"; break;
        case T_MsgCR_cr_cr_contamine_nbc : strMsg << "contamine nbc"; break;
        case T_MsgCR_cr_cr_decontamine_nbc : strMsg << "decontamine nbc"; break;
        case T_MsgCR_cr_cr_tenue_protection_nbc_mise : strMsg << "tenue protection nbc mise"; break;
        case T_MsgCR_cr_cr_tenue_protection_nbc_enlevee : strMsg << "tenue protection nbc enlevee"; break;
        case T_MsgCR_cr_cr_alerte_nbc_niveau_4 : strMsg << "alerte nbc niveau4"; break;
        case T_MsgCR_cr_cr_fin_alerte_nbc_niveau_4 : strMsg << "fin alerte nbc niveau4"; break;
        case T_MsgCR_cr_cr_site_decontamination_anime : strMsg << "site decontamination anime"; break;
        case T_MsgCR_cr_cr_site_decontamination_desactive : strMsg << "site decontamination desactive"; break;
        case T_MsgCR_cr_cr_demande_decontamination : strMsg << "demande decontamination"; break;
        case T_MsgCR_cr_cr_zone_reconnue : strMsg << "zone reconnue"; break;
        case T_MsgCR_cr_cr_zone_decontaminee : strMsg << "zone decontaminee"; break;
        case T_MsgCR_cr_cr_site_rota_reconnu : strMsg << "site rota reconnu"; break;
        case T_MsgCR_cr_cr_incident_nbc_termine : strMsg << "incident nbc termine"; break;
        case T_MsgCR_cr_cr_unite_decontaminee : strMsg << "unite decontaminee" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_unite_decontaminee ); break;
        case T_MsgCR_cr_cr_site_sature : strMsg << "site sature"; break;
        case T_MsgCR_cr_cr_debut_mesure : strMsg << "debut mesure"; break;
        case T_MsgCR_cr_cr_fin_mesure : strMsg << "fin mesure"; break;
        case T_MsgCR_cr_cr_reconnaissance_terminee : strMsg << "reconnaissance terminee"; break;
        case T_MsgCR_cr_cr_reconnaissance_initiale_terminee : strMsg << "reconnaissance initiale terminee"; break;
        case T_MsgCR_cr_cr_rejoint_plot_ravitaillement : strMsg << "rejoint plot ravitaillement"; break;
        case T_MsgCR_cr_cr_premier_coup_parti : strMsg << "premier coup parti"; break;
        case T_MsgCR_cr_cr_tir_execute : strMsg << "tir execute"; break;
        case T_MsgCR_cr_cr_tir_hors_de_portee : strMsg << "tir hors de portee"; break;
        case T_MsgCR_cr_cr_tir_impossible_plus_de_munitions : strMsg << "tir impossible plus de munitions"; break;
        case T_MsgCR_cr_cr_tir_annule : strMsg << "tir annule"; break;
        case T_MsgCR_cr_cr_changement_de_position : strMsg << "changement de position"; break;
        case T_MsgCR_cr_cr_debut_installation : strMsg << "debut installation"; break;
        case T_MsgCR_cr_cr_fin_installation : strMsg << "fin installation"; break;
        case T_MsgCR_cr_cr_pret_pour_conduite_tir : strMsg << "pret pour conduite tir"; break;
        case T_MsgCR_cr_cr_observation_tir_indirect : strMsg << "observation tir indirect" << " - TirPion : " << " " << asnMsg.cr.u.cr_observation_tir_indirect; break;
        case T_MsgCR_cr_cr_zone_implantation_occupee : strMsg << "zone implantation occupee"; break;
        case T_MsgCR_cr_cr_installation_sur_zone_implantation : strMsg << "installation sur zone implantation"; break;
        case T_MsgCR_cr_cr_reception_ordre_de_feu : strMsg << "reception ordre de feu"; break;
        case T_MsgCR_cr_cr_pret_au_tir : strMsg << "pret au tir"; break;
        case T_MsgCR_cr_cr_presence_population : strMsg << "presence population"; break;
        case T_MsgCR_cr_cr_orientation_population_vers_camp_refugies : strMsg << "orientation population vers camp refugies"; break;
        case T_MsgCR_cr_cr_pas_de_camp_refugies_disponible : strMsg << "pas de camp refugies disponible"; break;
        case T_MsgCR_cr_cr_presence_ennemi_rendu : strMsg << "presence ennemi rendu"; break;
        case T_MsgCR_cr_cr_orientation_ennemi_rendu_vers_camp_prisonniers : strMsg << "orientation ennemi rendu vers camp prisonniers"; break;
        case T_MsgCR_cr_cr_pas_de_camp_prisonniers_disponible : strMsg << "pas de camp prisonniers disponible"; break;
        case T_MsgCR_cr_cr_rendu : strMsg << "rendu"; break;
        case T_MsgCR_cr_cr_tir_dans_zone_interdite : strMsg << "tir dans zone interdite"; break;
        case T_MsgCR_cr_cr_tir_sur_camp_ami : strMsg << "tir sur camp ami" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tir_sur_camp_ami ); break;
        case T_MsgCR_cr_cr_tire_par_camp_ami : strMsg << "tire par camp ami" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tire_par_camp_ami ); break;
        case T_MsgCR_cr_cr_tir_sur_camp_neutre : strMsg << "tir sur camp neutre" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tir_sur_camp_neutre ); break;
        case T_MsgCR_cr_cr_tire_par_camp_neutre : strMsg << "tire par camp neutre" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tire_par_camp_neutre ); break;
        case T_MsgCR_cr_cr_tir_sur_civil : strMsg << "tir sur civil" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tir_sur_civil ); break;
        case T_MsgCR_cr_cr_tire_par_civil : strMsg << "tire par civil" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tire_par_civil ); break;
        case T_MsgCR_cr_cr_tir_indirect_fratricide : strMsg << "tir indirect fratricide"; break;
        case T_MsgCR_cr_cr_tir_indirect_sur_population : strMsg << "tir indirect sur population"; break;
        case T_MsgCR_cr_cr_destruction_pc : strMsg << "destruction pc"; break;
        case T_MsgCR_cr_cr_deces_blesse : strMsg << "deces blesse"; break;
        case T_MsgCR_cr_cr_deces_blesse_pendant_transport : strMsg << "deces blesse pendant transport"; break;
        case T_MsgCR_cr_cr_deces_blesse_pendant_hospitalisation : strMsg << "deces blesse pendant hospitalisation"; break;
        case T_MsgCR_cr_cr_mission_impossible_unites_subordonnees_non_operationnelles : strMsg << "mission impossible unites subordonnees non operationnelles"; break;
        case T_MsgCR_cr_cr_mission_impossible_lima_invalide : strMsg << "mission impossible lima invalide"; break;
        case T_MsgCR_cr_cr_mission_impossible_zone_hors_fuseau : strMsg << "mission impossible zone hors fuseau"; break;
        case T_MsgCR_cr_cr_rapport_de_force_defavorable : strMsg << "rapport de force defavorable"; break;
        case T_MsgCR_cr_cr_terrain_difficile : strMsg << "terrain difficile"; break;
        case T_MsgCR_cr_cr_plus_de_carburant : strMsg << "plus de carburant"; break;
        case T_MsgCR_cr_cr_dispositif_disperse : strMsg << "dispositif disperse"; break;
        case T_MsgCR_cr_cr_debut_neutralisation : strMsg << "debut neutralisation"; break;
        case T_MsgCR_cr_cr_fin_neutralisation : strMsg << "fin neutralisation"; break;
        case T_MsgCR_cr_cr_debut_silence_radio : strMsg << "debut silence radio"; break;
        case T_MsgCR_cr_cr_fin_silence_radio : strMsg << "fin silence radio"; break;
        case T_MsgCR_cr_cr_debut_silence_radar : strMsg << "debut silence radar"; break;
        case T_MsgCR_cr_cr_fin_silence_radar : strMsg << "fin silence radar"; break;
        case T_MsgCR_cr_cr_tir_restreint : strMsg << "tir restreint"; break;
        case T_MsgCR_cr_cr_tir_interdit : strMsg << "tir interdit"; break;
        case T_MsgCR_cr_cr_tir_libre : strMsg << "tir libre"; break;
        case T_MsgCR_cr_cr_unite_detectee : strMsg << "unite detectee" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_unite_detectee ); break;
        case T_MsgCR_cr_cr_unite_amie_reconnue : strMsg << "unite amie reconnue" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_unite_amie_reconnue ); break;
        case T_MsgCR_cr_cr_unite_ennemie_reconnue : strMsg << "unite ennemie reconnue" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_unite_ennemie_reconnue ); break;
        case T_MsgCR_cr_cr_unite_neutre_reconnue : strMsg << "unite neutre reconnue" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_unite_neutre_reconnue ); break;
        case T_MsgCR_cr_cr_unite_amie_identifiee : strMsg << "unite amie identifiee" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_unite_amie_identifiee ); break;
        case T_MsgCR_cr_cr_unite_ennemie_identifiee : strMsg << "unite ennemie identifiee" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_unite_ennemie_identifiee ); break;
        case T_MsgCR_cr_cr_unite_neutre_identifiee : strMsg << "unite neutre identifiee" << " - ConnaissanceAgent : " << " " << AgentKnowledgeLink( asnMsg.cr.u.cr_unite_neutre_identifiee ); break;
        case T_MsgCR_cr_cr_objet_detecte : strMsg << "objet detecte" << " - ConnaissanceObjet : " << " " << ObjectKnowledgeLink( asnMsg.cr.u.cr_objet_detecte ); break;
        case T_MsgCR_cr_cr_trace : strMsg << "trace" << " - message : " << " " << asnMsg.cr.u.cr_trace; break;
        case T_MsgCR_cr_cr_trace_id : strMsg << "trace id" << " - float : " << " " << asnMsg.cr.u.cr_trace_id; break;
        case T_MsgCR_cr_cr_emploi_force_interdit : strMsg << "emploi force interdit"; break;
        case T_MsgCR_cr_cr_maintien_a_distance_par_moyens_non_letaux : strMsg << "maintien a distance par moyens non letaux"; break;
        case T_MsgCR_cr_cr_dispersion_par_moyens_de_defense_actifs : strMsg << "dispersion par moyens de defense actifs"; break;
        case T_MsgCR_cr_cr_armes_letales_autorisees : strMsg << "armes letales autorisees"; break;
        case T_MsgCR_cr_cr_population_detectee : strMsg << "population detectee" << " - ConnaissancePopulation : " << " " << PopulationKnowledgeLink( asnMsg.cr.u.cr_population_detectee ); break;
        case T_MsgCR_cr_cr_prise_a_partie_par_population : strMsg << "prise a partie par population" << " - ConnaissancePopulation : " << " " << PopulationKnowledgeLink( asnMsg.cr.u.cr_prise_a_partie_par_population ); break;
        case T_MsgCR_cr_cr_riposte_contre_population : strMsg << "riposte contre population" << " - ConnaissancePopulation : " << " " << PopulationKnowledgeLink( asnMsg.cr.u.cr_riposte_contre_population ); break;
        case T_MsgCR_cr_cr_population_dangereuse_a_proximite : strMsg << "population dangereuse a proximite"; break;
        case T_MsgCR_cr_cr_esquive_face_a_population : strMsg << "esquive face a population"; break;
        case T_MsgCR_cr_cr_decrochage_face_a_population : strMsg << "decrochage face a population"; break;
        case T_MsgCR_cr_cr_mise_en_place_controle_de_foule : strMsg << "mise en place controle de foule"; break;
        case T_MsgCR_cr_cr_mise_en_place_filtrage : strMsg << "mise en place filtrage"; break;
        case T_MsgCR_cr_cr_animation_barrage : strMsg << "animation barrage"; break;
        case T_MsgCR_cr_cr_animation_canalisation : strMsg << "animation canalisation"; break;
        case T_MsgCR_cr_cr_collecte_renseignements_sur_populations : strMsg << "collecte renseignements sur populations"; break;
        case T_MsgCR_cr_cr_population_reconnue : strMsg << "population reconnue"; break;
        case T_MsgCR_cr_cr_situation_difficilement_tenable_face_a_population : strMsg << "situation difficilement tenable face a population"; break;
        case T_MsgCR_cr_cr_utilisation_armes_non_letales_face_a_population : strMsg << "utilisation armes non letales face a population"; break;
        case T_MsgCR_cr_cr_utilisation_moyens_defense_actifs_face_a_population : strMsg << "utilisation moyens defense actifs face a population"; break;
        case T_MsgCR_cr_cr_utilisation_armements_face_a_population : strMsg << "utilisation armements face a population"; break;
        case T_MsgCR_cr_cr_debut_intervention_face_a_population : strMsg << "debut intervention face a population" << " - id : " << " " << asnMsg.cr.u.cr_debut_intervention_face_a_population; break;
        case T_MsgCR_cr_cr_population_victime_affrontements : strMsg << "population victime affrontements" << " - victimes : " << asnMsg.cr.u.cr_population_victime_affrontements->victimes << " - dont_blesses : " << asnMsg.cr.u.cr_population_victime_affrontements->dont_blesses; break;
        case T_MsgCR_cr_cr_attitude_population : strMsg << "attitude population" << " - id : " << " " << asnMsg.cr.u.cr_attitude_population; break;
        case T_MsgCR_cr_cr_aggravation_attitude : strMsg << "aggravation attitude"; break;
        case T_MsgCR_cr_cr_affrontement_avec_forces_ennemies : strMsg << "affrontement avec forces ennemies"; break;
        case T_MsgCR_cr_cr_population_temporairement_controlee : strMsg << "population temporairement controlee"; break;
        case T_MsgCR_cr_cr_population_temporairement_repoussee : strMsg << "population temporairement repoussee"; break;
        case T_MsgCR_cr_cr_reprise_des_affrontements : strMsg << "reprise des affrontements"; break;
        case T_MsgCR_cr_cr_agression_de_population : strMsg << "agression de population" << " - ConnaissancePopulation : " << " " << PopulationKnowledgeLink( asnMsg.cr.u.cr_agression_de_population ); break;
        case T_MsgCR_cr_cr_agression_par_population : strMsg << "agression par population" << " - ConnaissancePopulation : " << " " << PopulationKnowledgeLink( asnMsg.cr.u.cr_agression_par_population ); break;
        case T_MsgCR_cr_cr_riposte_contre_agression_par_population : strMsg << "riposte contre agression par population" << " - ConnaissancePopulation : " << " " << PopulationKnowledgeLink( asnMsg.cr.u.cr_riposte_contre_agression_par_population ); break;
        case T_MsgCR_cr_cr_attaque_installation : strMsg << "attaque installation"; break;
        case T_MsgCR_cr_cr_en_stationnement : strMsg << "en stationnement"; break;
        case T_MsgCR_cr_cr_bloquee : strMsg << "bloquee"; break;
        case T_MsgCR_cr_cr_filtree : strMsg << "filtree"; break;
        case T_MsgCR_cr_cr_attentat_terroriste_dans_population : strMsg << "attentat terroriste dans population"; break;
        case T_MsgCR_cr_cr_population_victime_explosion_mines : strMsg << "population victime explosion mines" << " - victimes : " << asnMsg.cr.u.cr_population_victime_explosion_mines->victimes << " - dont_blesses : " << asnMsg.cr.u.cr_population_victime_explosion_mines->dont_blesses; break;
        case T_MsgCR_cr_cr_execution_attentat : strMsg << "execution attentat"; break;

/*    case T_MsgCR_cr_cr_preparation_mission                           : 
        {
            if( asnMsg.cr.u.cr_preparation_mission->m.delaiPresent )
                strMsg << "Preparation mission - Délai " << asnMsg.cr.u.cr_preparation_mission->delai << " minutes";
            else
                strMsg << "Preparation mission - Pas de délai";
            break;
        }  */
    default:
        strMsg << "Unknown RC " << asnMsg.cr.t;
    }

    strTitle_ = strMsg.str();
    
    // conversion from ASN 'type_cr' to Report_ABC::E_Type
    switch( asnMsg.type_cr )
    {
    case EnumTypeCR::message:
        eType_ = Report_ABC::eMessage;
        break;
    case EnumTypeCR::operationel:
        eType_ = Report_ABC::eRC;
        break;
    case EnumTypeCR::evenement_exceptionnel:
        eType_ = Report_ABC::eEvent;
        break;
    case EnumTypeCR::warning:
        eType_ = Report_ABC::eWarning;
        break;
    default:
        assert( false );
    }
}

// $$$$ AGE 2006-05-12: resoudre tous ces trucs pour afficher un nom

// -----------------------------------------------------------------------------
// Name: RC::ObjectKnowledgeLink
// Created: APE 2004-09-09
// -----------------------------------------------------------------------------
std::string RC::ObjectKnowledgeLink( ASN1T_OID nId )
{
    return InternalLinks::CreateLink( InternalLinks::objectKnowledge_, nId, QString::number( nId ) ).ascii();
}

// -----------------------------------------------------------------------------
// Name: RC::AgentKnowledgeLink
// Created: APE 2004-09-09
// -----------------------------------------------------------------------------
std::string RC::AgentKnowledgeLink( ASN1T_OID nId )
{
    return InternalLinks::CreateLink( InternalLinks::agentKnowledge_, nId, QString::number( nId ) ).ascii();
}

// -----------------------------------------------------------------------------
// Name: RC::PopulationKnowledgeLink
// Created: APE 2004-09-09
// -----------------------------------------------------------------------------
std::string RC::PopulationKnowledgeLink( ASN1T_OID nId )
{
    return InternalLinks::CreateLink( InternalLinks::populationKnowledge_, nId, QString::number( nId ) ).ascii();
}

// -----------------------------------------------------------------------------
// Name: RC::AgentLink
// Created: APE 2004-09-09
// -----------------------------------------------------------------------------
std::string RC::AgentLink( ASN1T_OID nId )
{
    return InternalLinks::CreateLink( InternalLinks::agent_, nId, QString::number( nId ) ).ascii();
}
