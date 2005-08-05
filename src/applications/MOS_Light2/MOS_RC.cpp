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

#include "MOS_Light2_Pch.h"
#include "MOS_RC.h"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"

#ifndef MOS_USE_INLINE
#   include "MOS_RC.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_RC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_RC::MOS_RC( MOS_Agent& agent )
    : MOS_Report_ABC( agent.GetPos() )
    , agent_        ( agent )
{
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
    // $$$$ AGE 2005-03-14: 
    case T_MsgCR_cr_cr_zone_reconnue                                 : strMsg << "Zone reconnue"; break;
    case T_MsgCR_cr_cr_zone_decontaminee                             : strMsg << "Zone décontaminée"; break;
    case T_MsgCR_cr_cr_site_rota_reconnu                             : strMsg << "Site ROTA reconnu"; break;
    case T_MsgCR_cr_cr_incident_nbc_termine                          : strMsg << "Incident NBC terminé"; break;
    case T_MsgCR_cr_cr_pion_decontamine                              : strMsg << "Pion décontaminé"; break;
    case T_MsgCR_cr_cr_tir_dans_zone_interdite                       : strMsg << "Tir dans zone interdite"; break;
    case T_MsgCR_cr_cr_tir_sur_camp_ami                              : strMsg << "Tir sur camp ami"; break;
    case T_MsgCR_cr_cr_tire_par_camp_ami                             : strMsg << "Tiré par camp ami"; break;
    case T_MsgCR_cr_cr_tir_sur_camp_neutre                           : strMsg << "Tir sur camp neutre"; break;
    case T_MsgCR_cr_cr_tire_par_camp_neutre                          : strMsg << "Tiré par camp neutre"; break;
    case T_MsgCR_cr_cr_tir_sur_civil                                 : strMsg << "Tir sur civil"; break;
    case T_MsgCR_cr_cr_tire_par_civil                                : strMsg << "Tiré par civil"; break;
    case T_MsgCR_cr_cr_destruction_pc                                : strMsg << "Destruction PC"; break;
    case T_MsgCR_cr_cr_deces_blesse                                  : strMsg << "Décès blessé"; break;
    case T_MsgCR_cr_cr_deces_blesse_pendant_transport                : strMsg << "Décès blessé pendant transport"; break;
    case T_MsgCR_cr_cr_deces_blesse_pendant_hospitalisation          : strMsg << "Décès blessé pendadnt hospitalisation"; break;
    case T_MsgCR_cr_cr_trace                                         : strMsg << "Trace"; break;
    case T_MsgCR_cr_cr_trace_id                                      : strMsg << "Trace ID"; break;
    case T_MsgCR_cr_cr_obstacle_en_attente_activation                : strMsg << "Obstacle en attente d'activation"; break;
    case T_MsgCR_cr_cr_materiel_retour_de_maintenance                : strMsg << "Materiel retour de maintenance"; break;
    case T_MsgCR_cr_cr_allocation_consentie_bientot_epuisee          : strMsg << "allocation consentie bientot épuisée"; break;
    case T_MsgCR_cr_cr_en_cours_ravitaillement                       : strMsg << "En cours de ravitaillement"; break;
    case T_MsgCR_cr_cr_ravitaillement_termine                        : strMsg << "Ravitaillement terminé"; break;
    case T_MsgCR_cr_cr_helicoptere_en_observation                    : strMsg << "Hélicoptère en observation"; break;
    case T_MsgCR_cr_cr_perte_info_guidage                            : strMsg << "Perte d'infos de guidage"; break;
    case T_MsgCR_cr_cr_attente_ordre_poursuivre                      : strMsg << "En attente d'ordre prousuivre"; break;
    case T_MsgCR_cr_cr_attente_ordre_deboucher                       : strMsg << "En attente d'ordre déboucher"; break;
    case T_MsgCR_cr_cr_drone_lance                                   : strMsg << "Drone lancé"; break;
    case T_MsgCR_cr_cr_drone_en_exploitation                         : strMsg << "Drone en exploitation"; break;
    case T_MsgCR_cr_cr_drone_exploite                                : strMsg << "Drone exploité"; break;
    case T_MsgCR_cr_cr_brouille                                      : strMsg << "Brouille"; break;
    case T_MsgCR_cr_cr_en_ecoute                                     : strMsg << "EnEcoute"; break;
    case T_MsgCR_cr_cr_transport_unite_pas_prete                     : strMsg << "Unité transportée pas prête"; break;
    case T_MsgCR_cr_cr_presence_population                           : strMsg << "Présence de population"; break;
    case T_MsgCR_cr_cr_orientation_population_vers_camp_refugies     : strMsg << "Orientation de la population vers un camp de réfugiés"; break;
    case T_MsgCR_cr_cr_pas_de_camp_refugies_disponible               : strMsg << "Pas de camp de réfugiés disponible"; break;
    case T_MsgCR_cr_cr_presence_ennemi_rendu                         : strMsg << "Présence d'un ennemi rendu"; break;
    case T_MsgCR_cr_cr_orientation_ennemi_rendu_vers_camp_prisonniers: strMsg << "Orientation d'un ennemi rendu vers un camp de prisonniers"; break;
    case T_MsgCR_cr_cr_pas_de_camp_prisonniers_disponible            : strMsg << "Pas de camp de prisonniers disponible"; break;
    // $$$$ AGE 2005-03-14: 
    case T_MsgCR_cr_cr_ras                                           : strMsg << "RAS"; break;
    case T_MsgCR_cr_cr_en_poste_face_a_obstacle                      : strMsg << "En poste face à l'obstacle - " << ObjectKnowledgeLink( asnMsg.cr.u.cr_en_poste_face_a_obstacle ); break;
    case T_MsgCR_cr_cr_en_contournement_obstacle                     : strMsg << "En contournement de l'obstacle"; break;
    case T_MsgCR_cr_cr_mission_impossible                            : strMsg << "Mission impossible"; break;
    case T_MsgCR_cr_cr_pret_pour_nouvelle_mission                    : strMsg << "Prêt pour nouvelle mission"; break;
    case T_MsgCR_cr_cr_rejoint_fuseau                                : strMsg << "Rejoint fuseau"; break;
    case T_MsgCR_cr_cr_progression_sur_axe                           : strMsg << "Progression sur axe"; break;
    case T_MsgCR_cr_cr_progression_vers_localisation                 : strMsg << "Progression vers localisation"; break;
    case T_MsgCR_cr_cr_arrive_sur_localisation                       : strMsg << "Arrive sur localisation"; break;
    case T_MsgCR_cr_cr_reconnaissance_localisation                   : strMsg << "Reconnaissance localisation"; break;
    case T_MsgCR_cr_cr_en_poste_face_a_direction_dangereuse          : strMsg << "En poste face à direction dangereuse"; break;
    case T_MsgCR_cr_cr_en_poste_sur_bond                             : strMsg << "En poste sur bond"; break;
    case T_MsgCR_cr_cr_en_jalonnement                                : strMsg << "En jalonnement"; break;
    case T_MsgCR_cr_cr_precision_contact                             : strMsg << "Précision contact - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_precision_contact ); break;
    case T_MsgCR_cr_cr_pion_prise_contact                            : strMsg << "Pion prise de contact - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_pion_prise_contact ); break;
    case T_MsgCR_cr_cr_pion_rupture_contact                          : strMsg << "Pion rupture contact - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_pion_rupture_contact ); break;
    case T_MsgCR_cr_cr_ennemi_detruit                                : strMsg << "Ennemi détruit - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_ennemi_detruit ); break;
    case T_MsgCR_cr_cr_perte_de_contact_visuel                       : strMsg << "Perte de contact visuel - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_perte_de_contact_visuel ); break;
    case T_MsgCR_cr_cr_poste_face_eni                                : strMsg << "Poste face à l'ennemi - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_poste_face_eni ); break;
    case T_MsgCR_cr_cr_prise_a_partie                                : strMsg << "Prise à partie - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_prise_a_partie ); break;
    case T_MsgCR_cr_cr_point_tenu_par_eni                            : strMsg << "Point tenu par ennemi - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_point_tenu_par_eni ); break;
    case T_MsgCR_cr_cr_progression_vers_eni                          : strMsg << "Progression vers ennemi - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_progression_vers_eni ); break;
    case T_MsgCR_cr_cr_presence_eni_probable                         : strMsg << "Présence ennemi probable"; break;
    case T_MsgCR_cr_cr_decroche                                      : strMsg << "Décroche"; break;
    case T_MsgCR_cr_cr_eni_sur_objectif                              : strMsg << "Ennemi sur objectif - eni " << AgentKnowledgeLink( asnMsg.cr.u.cr_eni_sur_objectif ); break;
    case T_MsgCR_cr_cr_riposte                                       : strMsg << "Riposte"; break;
    case T_MsgCR_cr_cr_suit_unite                                    : strMsg << "Suit unité"; break;
    case T_MsgCR_cr_cr_passe_en_appui                                : strMsg << "Passe en appui"; break;
    case T_MsgCR_cr_cr_en_appui                                      : strMsg << "En appui"; break;
    case T_MsgCR_cr_cr_situation_log_favorable                       : strMsg << "Situation logistique favorable"; break;
    case T_MsgCR_cr_cr_situation_log_defavorable                     : strMsg << "Situation logistique défavorable"; break;
    case T_MsgCR_cr_cr_detruit_tactiquement                          : strMsg << "Détruit tactiquement"; break;
    case T_MsgCR_cr_cr_ambiance_vitesse                              : strMsg << "Ambiance vitesse"; break;
    case T_MsgCR_cr_cr_ambiance_surete                               : strMsg << "Ambiance sureté"; break;
    case T_MsgCR_cr_cr_sur_lima                                      : strMsg << "Sur lima"; break;
    case T_MsgCR_cr_cr_sur_type_terrain_particulier                  : strMsg << "Sur type terrain particulier - Terrain " << asnMsg.cr.u.cr_sur_type_terrain_particulier; break;
    case T_MsgCR_cr_cr_travaux_realises                              : strMsg << "Travaux réalisés"; break;
    case T_MsgCR_cr_cr_obstacle_degage                               : strMsg << "Obstacle dégagé"; break;
    case T_MsgCR_cr_cr_section_deployee                              : strMsg << "Section déployée"; break;
    case T_MsgCR_cr_cr_reconnaissance_terminee                       : strMsg << "Reconnaissance terminee"; break;
    case T_MsgCR_cr_cr_reconnaissance_initiale_terminee              : strMsg << "Reconnaissance initiale terminee"; break;
    case T_MsgCR_cr_cr_premier_coup_parti                            : strMsg << "Premier coup parti"; break;
    case T_MsgCR_cr_cr_tir_execute                                   : strMsg << "Tir exécuté"; break;
    case T_MsgCR_cr_cr_rejoint_plot_ravitaillement                   : strMsg << "Rejoint plot ravitaillement"; break;
    case T_MsgCR_cr_cr_changement_de_position                        : strMsg << "Changement de position"; break;
    case T_MsgCR_cr_cr_debut_surveillance                            : strMsg << "Début surveillance"; break;
    case T_MsgCR_cr_cr_debut_installation                            : strMsg << "Début installation"; break;
    case T_MsgCR_cr_cr_pret_pour_conduite_tir                        : strMsg << "Prêt pour conduite de tir"; break;       
    case T_MsgCR_cr_cr_demande_ravitaillement                        : strMsg << "Demande de ravitaillement"; break;
    case T_MsgCR_cr_cr_organisation_chantier                         : strMsg << "Organisation chantier"; break;
    case T_MsgCR_cr_cr_desorganisation_chantier                      : strMsg << "Désorganisation chantier"; break;
    case T_MsgCR_cr_cr_reconnaissance_position_obstacle              : strMsg << "Reconnaissance position obstacle"; break;  
    case T_MsgCR_cr_cr_contournement_realise                         : strMsg << "Contournement réalisé"; break;
    case T_MsgCR_cr_cr_attente_renforcement                          : strMsg << "Attente renforcement"; break;
    case T_MsgCR_cr_cr_debut_travaux                                 : strMsg << "Début travaux"; break;
    case T_MsgCR_cr_cr_debut_franchissement                          : strMsg << "Début franchissement"; break;
    case T_MsgCR_cr_cr_fin_franchissement                            : strMsg << "Fin franchissement"; break;
    case T_MsgCR_cr_cr_observation_tir_indirect                      : strMsg << "Observation tir indirect ID #" << asnMsg.cr.u.cr_observation_tir_indirect; break;       
    case T_MsgCR_cr_cr_contamine_nbc                                 : strMsg << "Contaminé NBC"; break;
    case T_MsgCR_cr_cr_decontamine_nbc                               : strMsg << "Décontaminé NBC"; break;
    case T_MsgCR_cr_cr_tenue_protection_nbc_mise                     : strMsg << "Protection NBC mise"; break;
    case T_MsgCR_cr_cr_tenue_protection_nbc_enlevee                  : strMsg << "Protection NBC enlevée"; break;
    case T_MsgCR_cr_cr_reussite_renforcement                         : strMsg << "Reussite renforcement pion " << AgentLink( asnMsg.cr.u.cr_reussite_renforcement->pion_renforcant ) << " -> automate " << AgentLink( asnMsg.cr.u.cr_reussite_renforcement->automate_renforce ); break;
    case T_MsgCR_cr_cr_debut_mission                                 : strMsg << "Début mission"; break;
    case T_MsgCR_cr_cr_fin_mission                                   : strMsg << "Fin mission"; break;
    case T_MsgCR_cr_cr_automate_prise_contact                        : strMsg << "Automate prise de contact"; break;
    case T_MsgCR_cr_cr_automate_rupture_contact                      : strMsg << "Automate rupture de contact"; break;
    case T_MsgCR_cr_cr_dispositif_installe                           : strMsg << "Dispositif installé"; break;
    case T_MsgCR_cr_cr_en_position                                   : strMsg << "En position"; break;
    case T_MsgCR_cr_cr_objectif_atteint                              : strMsg << "Objectif atteint"; break;
    case T_MsgCR_cr_cr_fuseau_rejoint                                : strMsg << "Fuseau rejoint"; break;
    case T_MsgCR_cr_cr_debouche                                      : strMsg << "Débouche"; break;
    case T_MsgCR_cr_cr_alerte_nbc                                    : strMsg << "Alerte NBC"; break;
    case T_MsgCR_cr_cr_site_decontamination_anime                    : strMsg << "Plot de décontamination animé"; break;
    case T_MsgCR_cr_cr_site_decontamination_desactive                : strMsg << "Plot de décontamination désactivé"; break;
    case T_MsgCR_cr_cr_demande_decontamination                       : strMsg << "Demande de décontamination"; break;
    case T_MsgCR_cr_cr_releve                                        : strMsg << "Relève agent " << AgentLink( asnMsg.cr.u.cr_releve ); break;
    case T_MsgCR_cr_cr_sur_pia                                       : strMsg << "Sur PIA " << AgentLink( asnMsg.cr.u.cr_sur_pia ); break;
    case T_MsgCR_cr_cr_passage_sur_pia                               : strMsg << "Passage sur PIA"; break;
    case T_MsgCR_cr_cr_unite_recueillie                              : strMsg << "Unité " << AgentLink( asnMsg.cr.u.cr_unite_recueillie ) <<  " recueillie"; break;
    case T_MsgCR_cr_cr_en_controle_zone                              : strMsg << "En contrôle de zone"; break;
    case T_MsgCR_cr_cr_en_surveillance                               : strMsg << "En surveillance"; break;
    case T_MsgCR_cr_cr_en_infiltration                               : strMsg << "En infiltration"; break;
    case T_MsgCR_cr_cr_en_exfiltration                               : strMsg << "En exfiltration"; break;
    case T_MsgCR_cr_cr_attente_condition_favorable_pour_infiltration : strMsg << "Attente condition favorable pour infiltration"; break;
    case T_MsgCR_cr_cr_en_cours_de_franchissement                    : strMsg << "En cours de franchissement"; break;
    case T_MsgCR_cr_cr_reception_ordre_de_feu                        : strMsg << "Réception ordre de feu"; break;
    case T_MsgCR_cr_cr_pret_au_tir                                   : strMsg << "Prêt au tir"; break;
    case T_MsgCR_cr_cr_demande_evacuation_sanitaire                  : strMsg << "Demande évacuation sanitaire"; break;
    case T_MsgCR_cr_cr_demande_evacuation_materiel                   : strMsg << "Demande évacuation de matériel"; break;
    case T_MsgCR_cr_cr_itineraire_retabli                            : strMsg << "Itineraire rétabli"; break;
    case T_MsgCR_cr_cr_renforcement_bientot_fini                     : strMsg << "Renforcement bientôt fini - unité " << AgentLink( asnMsg.cr.u.cr_renforcement_bientot_fini ); break;      
    case T_MsgCR_cr_cr_echec_renforcement                            : strMsg << "echec renforcement pion " << AgentLink( asnMsg.cr.u.cr_echec_renforcement->pion_renforcant ) << " -> automate " << AgentLink( asnMsg.cr.u.cr_echec_renforcement->automate_renforce ); break;
    case T_MsgCR_cr_cr_transport_en_cours                            : strMsg << "Transport en cours"; break;
    case T_MsgCR_cr_cr_transport_embarquement                        : strMsg << "Transport embarquement"; break;
    case T_MsgCR_cr_cr_transport_debarquement                        : strMsg << "Transport débarquement"; break;
    case T_MsgCR_cr_cr_zone_implantation_occupee                     : strMsg << "Zone implantation occupée"; break;
    case T_MsgCR_cr_cr_installation_sur_zone_implantation            : strMsg << "Installation sur zone implantation"; break;
    case T_MsgCR_cr_cr_tir_hors_de_portee                            : strMsg << "Tir hors de portée"; break;
    case T_MsgCR_cr_cr_tir_annule                                    : strMsg << "Tir annulé"; break;
    case T_MsgCR_cr_cr_en_ecoute_radar                               : strMsg << "En écoute radar"; break;
    case T_MsgCR_cr_cr_debut_mise_en_oeuvre_drone                    : strMsg << "Debut mise en oeuvre drone"; break;
    case T_MsgCR_cr_cr_fin_mise_en_oeuvre_drone                      : strMsg << "Fin mise en oeuvre drone"; break;                 
    case T_MsgCR_cr_cr_vehicule_laisse_sur_position                  : strMsg << "Véhicule laissé sur position"; break;
    case T_MsgCR_cr_cr_rejoint_position_embarquement                 : strMsg << "Rejoint position embarquement"; break;
    case T_MsgCR_cr_cr_en_attente_transporteurs                      : strMsg << "En attente transporteurs"; break;
    case T_MsgCR_cr_cr_mission_impossible_pions_subordonnes_non_operationnels  : strMsg << "Mission impossible :  pions subordonnés non operationnels"; break;
    case T_MsgCR_cr_cr_mission_impossible_lima_invalide                        : strMsg << "Mission impossible : lima invalide"; break;
    case T_MsgCR_cr_cr_mission_impossible_zone_hors_fuseau                     : strMsg << "Mission impossible : zone hors fuseau"; break;
    case T_MsgCR_cr_cr_rapport_de_force_defavorable                            : strMsg << "Rapport de force défavorable"; break;
    case T_MsgCR_cr_cr_terrain_difficile                                       : strMsg << "Terrain difficile"; break;
    case T_MsgCR_cr_cr_plus_de_carburant                                       : strMsg << "Plus de carburant"; break;
    case T_MsgCR_cr_cr_dispositif_disperse                                     : strMsg << "Dispositif dispersé"; break;
    case T_MsgCR_cr_cr_pion_neutralise                                         : strMsg << "Pion neutralisé"; break;
    case T_MsgCR_cr_cr_silence_radio                                           : strMsg << "Silence radio"; break;
    case T_MsgCR_cr_cr_tir_restreint                                           : strMsg << "Tir restreint"; break;
    case T_MsgCR_cr_cr_tir_interdit                                            : strMsg << "Tir interdit"; break;
    case T_MsgCR_cr_cr_tir_libre                                               : strMsg << "Tir libre"; break;                                                                       
    case T_MsgCR_cr_cr_humain_retour_de_sante                                  : strMsg << "Humain retour de santé"; break;
    case T_MsgCR_cr_cr_ravitaillement_dotations_effectue                       : strMsg << "Ravitaillement des dotations effectué"; break;
    case T_MsgCR_cr_cr_ravitaillement_stock_effectue                           : strMsg << "Ravitaillement du stock effectué"; break;

    case T_MsgCR_cr_cr_preparation_mission                           : 
        {
            if( asnMsg.cr.u.cr_preparation_mission->m.delaiPresent )
                strMsg << "Preparation mission - Délai " << asnMsg.cr.u.cr_preparation_mission->delai << " minutes";
            else
                strMsg << "Preparation mission - Pas de délai";
            break;
        }  
    default:
        assert( false );
    }

    strTitle_ = strMsg.str();
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
        return std::string( " !Agent non trouvé! " );

    std::stringstream str;
    str << "<a href=\"Agent:" << nId << "\">" << pAgent->GetName() << "</a>";
    return str.str();

}
