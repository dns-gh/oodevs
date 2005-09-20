//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_RC.cpp $
// $Author: Nld $
// $Modtime: 4/07/05 12:33 $
// $Revision: 16 $
// $Workfile: MOS_RC.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_RC.h"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"

//-----------------------------------------------------------------------------
// Name: MOS_RC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_RC::MOS_RC( MOS_Agent& agent )
    : MOS_Report_ABC( agent.GetPos() )
    , agent_        ( agent )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MOS_RC destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_RC::~MOS_RC()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_RC::Initialize
// Created: NLD 2002-10-07
//-----------------------------------------------------------------------------
void MOS_RC::Initialize( const ASN1T_MsgCR& asnMsg )
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
        //case T_MsgCR_cr_cr_preparation_mission : strMsg << "preparation mission" << " - delai : " << asnMsg.cr.u.cr_preparation_mission->delai; break;
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
        case T_MsgCR_cr_cr_section_deployee : strMsg << "section deployee"; break;
        case T_MsgCR_cr_cr_a_nouveau_disponible_apres_reparation : strMsg << "a nouveau disponible apres reparation"; break;
        case T_MsgCR_cr_cr_materiel_retour_de_maintenance : strMsg << "materiel retour de maintenance"; break;
        case T_MsgCR_cr_cr_materiel_repare_sur_place : strMsg << "materiel repare sur place"; break;
        case T_MsgCR_cr_cr_demande_ravitaillement : strMsg << "demande ravitaillement"; break;
        case T_MsgCR_cr_cr_demande_evacuation_sanitaire : strMsg << "demande evacuation sanitaire"; break;
        case T_MsgCR_cr_cr_demande_evacuation_materiel : strMsg << "demande evacuation materiel"; break;
        case T_MsgCR_cr_cr_allocation_consentie_bientot_epuisee : strMsg << "allocation consentie bientot epuisee" << " - TypeDotation : " << " " << asnMsg.cr.u.cr_allocation_consentie_bientot_epuisee; break;
        case T_MsgCR_cr_cr_ravitaillement_dotations_effectue : strMsg << "ravitaillement dotations effectue"; break;
        case T_MsgCR_cr_cr_ravitaillement_stock_effectue : strMsg << "ravitaillement stock effectue"; break;
        case T_MsgCR_cr_cr_humain_retour_de_sante : strMsg << "humain retour de sante"; break;
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
        case T_MsgCR_cr_cr_tir_annule : strMsg << "tir annule"; break;
        case T_MsgCR_cr_cr_changement_de_position : strMsg << "changement de position"; break;
        case T_MsgCR_cr_cr_debut_installation : strMsg << "debut installation"; break;
        case T_MsgCR_cr_cr_fin_installation : strMsg << "fin installation"; break;
        case T_MsgCR_cr_cr_pret_pour_conduite_tir : strMsg << "pret pour conduite tir"; break;
        case T_MsgCR_cr_cr_observation_tir_indirect : strMsg << "observation tir indirect" << " - TirIndirect : " << " " << asnMsg.cr.u.cr_observation_tir_indirect; break;
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
        case T_MsgCR_cr_cr_tir_dans_zone_interdite : strMsg << "tir dans zone interdite"; break;
        case T_MsgCR_cr_cr_tir_sur_camp_ami : strMsg << "tir sur camp ami" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tir_sur_camp_ami ); break;
        case T_MsgCR_cr_cr_tire_par_camp_ami : strMsg << "tire par camp ami" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tire_par_camp_ami ); break;
        case T_MsgCR_cr_cr_tir_sur_camp_neutre : strMsg << "tir sur camp neutre" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tir_sur_camp_neutre ); break;
        case T_MsgCR_cr_cr_tire_par_camp_neutre : strMsg << "tire par camp neutre" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tire_par_camp_neutre ); break;
        case T_MsgCR_cr_cr_tir_sur_civil : strMsg << "tir sur civil" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tir_sur_civil ); break;
        case T_MsgCR_cr_cr_tire_par_civil : strMsg << "tire par civil" << " - Pion : " << " " << AgentLink( asnMsg.cr.u.cr_tire_par_civil ); break;
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
        case T_MsgCR_cr_cr_objet_detecte : strMsg << "objet detecte" << " - ConnaissanceObjet : " << " " << ObjectKnowledgeLink( asnMsg.cr.u.cr_objet_detecte ); break;
        case T_MsgCR_cr_cr_trace : strMsg << "trace" << " - message : " << " " << asnMsg.cr.u.cr_trace; break;
        case T_MsgCR_cr_cr_trace_id : strMsg << "trace id" << " - float : " << " " << asnMsg.cr.u.cr_trace_id; break;

        case T_MsgCR_cr_cr_preparation_mission : 
        {
            if( asnMsg.cr.u.cr_preparation_mission->m.delaiPresent )
                strMsg << "preparation mission - D�lai " << asnMsg.cr.u.cr_preparation_mission->delai << " minutes";
            else
                strMsg << "preparation mission";
            break;
        }  
    default:
        assert( false );
    }

    strTitle_ = strMsg.str();
    switch( asnMsg.type_cr )
    {
    case EnumTypeCR::message:
        eType_ = MOS_Report_ABC::eMessage;
        break;
    case EnumTypeCR::operationel:
        eType_ = MOS_Report_ABC::eRC;
        break;
    case EnumTypeCR::evenement_exceptionnel:
        eType_ = MOS_Report_ABC::eEvent;
        break;
    case EnumTypeCR::warning:
        eType_ = MOS_Report_ABC::eWarning;
        break;
    default:
        assert( false );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_RC::Initialize
// Created: NLD 2003-04-04
//-----------------------------------------------------------------------------
void MOS_RC::Initialize( const ASN1T_MsgAttenteOrdreConduite& asnMsg )
{
    strTitle_ = "Attente d'ordre";

    for( uint i = 0; i < asnMsg.ordres_conduite.n; ++i )
    {
        uint nOrderConduiteID;
        switch( asnMsg.ordres_conduite.elem[i].t  )
        {
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_attendre_se_poster     : nOrderConduiteID = eOrdreConduite_AttendreSePoster                            ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_poursuivre             : nOrderConduiteID = eOrdreConduite_Poursuivre                                  ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_decrocher              : nOrderConduiteID = eOrdreConduite_Decrocher                              ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_deboucher              : nOrderConduiteID = eOrdreConduite_Deboucher                              ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_executer_tir      : nOrderConduiteID = eOrdreConduite_Pion_ExecuterTir                            ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_contourner        : nOrderConduiteID = eOrdreConduite_Pion_Contourner                             ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_activer_obstacle  : nOrderConduiteID = eOrdreConduite_Pion_ActiverObstacle                        ; break;
            default:
                assert( false );
        }
        fragOrderVector_.push_back( nOrderConduiteID );
    }
    eType_ = MOS_Report_ABC::eRC;
}




// -----------------------------------------------------------------------------
// Name: MOS_RC::ObjectKnowledgeLink
/** @param  nId 
    @return 
*/
// Created: APE 2004-09-09
// -----------------------------------------------------------------------------
std::string MOS_RC::ObjectKnowledgeLink( ASN1T_OID nId )
{
    std::stringstream str;
    str << "<a href=\"ObjectKnowledge:" << nId << "\">" << nId << "</a>";
    return str.str();
}


// -----------------------------------------------------------------------------
// Name: MOS_RC::AgentKnowledgeLink
/** @param  nId 
    @return 
*/
// Created: APE 2004-09-09
// -----------------------------------------------------------------------------
std::string MOS_RC::AgentKnowledgeLink( ASN1T_OID nId )
{
    std::stringstream str;
    str << "<a href=\"AgentKnowledge:" << nId << "\">" << nId << "</a>";
    return str.str();
}


// -----------------------------------------------------------------------------
// Name: MOS_RC::AgentLink
/** @param  nId 
    @return 
    */
// Created: APE 2004-09-09
// -----------------------------------------------------------------------------
std::string MOS_RC::AgentLink( ASN1T_OID nId )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nId );
    if( pAgent == 0 )
        return std::string( " !Agent non trouv�! " );

    std::stringstream str;
    str << "<a href=\"Agent:" << nId << "\">" << pAgent->GetName() << "</a>";
    return str.str();

}
