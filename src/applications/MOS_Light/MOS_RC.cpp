//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RC.cpp $
// $Author: Nld $
// $Modtime: 4/07/05 12:35 $
// $Revision: 59 $
// $Workfile: MOS_RC.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"

#include "MOS_RC.h"

#ifndef MOS_USE_INLINE
#   include "MOS_RC.inl"
#endif

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_MOSServer.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Agent.h"
#include "MOS_AttrEditor.h"
#include "MOS_RCListViewItem.h"
#include "MOS_RCSubItem.h"
#include "MOS_Net_Def.h"
#include "MOS_LimaFuncCheckListItem.h"

#include <QWidgetStack.h>


using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_RC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_RC::MOS_RC( MOS_Agent& agent )
:   agent_  ( agent )
,   vPos_   ( agent_.GetPos() )
,   nTime_  ( MOS_App::GetApp().GetTime() )
,   pItem_  ( 0 )
{
}
 
//-----------------------------------------------------------------------------
// Name: MOS_RC destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_RC::~MOS_RC()
{
    if( pItem_ != 0 ) // if != 0 then attrEditor != 0
        agent_.GetAttributeEditor()->DeleteRCItem( *pItem_ );
    textVector_.clear();
}

//-----------------------------------------------------------------------------
// Name: MOS_RC::Initialize
// Created: NLD 2002-10-07
//-----------------------------------------------------------------------------
void MOS_RC::Initialize( const ASN1T_MsgCR& asnMsg )
{
    switch( asnMsg.cr.t )
    {
        case T_MsgCR_cr_cr_ras                                          : strTitle_ = "RAS"; break;
        case T_MsgCR_cr_cr_en_poste_face_a_obstacle                     : strTitle_ = MT_FormatString( "En poste face � l'obstacle - %d", asnMsg.cr.u.cr_en_poste_face_a_obstacle ); break;
        case T_MsgCR_cr_cr_en_contournement_obstacle                    : strTitle_ = "En contournement de l'obstacle"; break;
//        case T_MsgCR_cr_cr_heure_debut_mission_depassee                 : strTitle_ = "Heure d�but mission d�pass�"; break;
//        case T_MsgCR_cr_cr_heure_fin_mission_depassee                   : strTitle_ = "Heure fin mission d�pass�e"; break;
        case T_MsgCR_cr_cr_mission_impossible                           : strTitle_ = "Mission impossible"; break;
        case T_MsgCR_cr_cr_pret_pour_nouvelle_mission                   : strTitle_ = "Pr�t pour nouvelle mission"; break;
        case T_MsgCR_cr_cr_rejoint_fuseau                               : strTitle_ = "Rejoint fuseau"; break;
        case T_MsgCR_cr_cr_progression_sur_axe                          : strTitle_ = "Progression sur axe"; break;
        case T_MsgCR_cr_cr_progression_vers_localisation                : strTitle_ = "Progression vers localisation"; break;
        case T_MsgCR_cr_cr_arrive_sur_localisation                      : strTitle_ = "Arrive sur localisation"; break;
        case T_MsgCR_cr_cr_reconnaissance_localisation                  : strTitle_ = "Reconnaissance localisation"; break;
//        case T_MsgCR_cr_cr_tombe_en_garde_face_a_direction_dangereuse   : strTitle_ = "Tombe en garde face � direction dangereuse"; break;
        case T_MsgCR_cr_cr_en_poste_face_a_direction_dangereuse         : strTitle_ = "En poste face � direction dangereuse"; break;
        case T_MsgCR_cr_cr_en_poste_sur_bond                            : strTitle_ = "En poste sur bond"; break;
        case T_MsgCR_cr_cr_en_jalonnement                               : strTitle_ = "En jalonnement"; break;
        case T_MsgCR_cr_cr_precision_contact                            : strTitle_ = MT_FormatString( "Pr�cision contact - eni %d", asnMsg.cr.u.cr_precision_contact ); break;
//        case T_MsgCR_cr_cr_reprise_contact                              : strTitle_ = MT_FormatString( "Reprise contact - eni %d", asnMsg.cr.u.cr_reprise_contact ); break;
        case T_MsgCR_cr_cr_pion_prise_contact                           : strTitle_ = MT_FormatString( "Pion prise de contact - eni %d", asnMsg.cr.u.cr_pion_prise_contact ); break;
        case T_MsgCR_cr_cr_pion_rupture_contact                         : strTitle_ = MT_FormatString( "Pion rupture contact - eni %d", asnMsg.cr.u.cr_pion_rupture_contact ); break;
        case T_MsgCR_cr_cr_ennemi_detruit                               : strTitle_ = MT_FormatString( "Ennemi d�truit - eni %d", asnMsg.cr.u.cr_ennemi_detruit ); break;
        case T_MsgCR_cr_cr_perte_de_contact_visuel                      : strTitle_ = MT_FormatString( "Perte de contact visuel - eni %d", asnMsg.cr.u.cr_perte_de_contact_visuel ); break;
        case T_MsgCR_cr_cr_poste_face_eni                               : strTitle_ = MT_FormatString( "Poste face � l'ennemi - eni %d", asnMsg.cr.u.cr_poste_face_eni ); break;
//        case T_MsgCR_cr_cr_eni_hors_fuseau                              : strTitle_ = MT_FormatString( "Ennemi hors du fuseau - eni %d", asnMsg.cr.u.cr_eni_hors_fuseau ); break;
        case T_MsgCR_cr_cr_prise_a_partie                               : strTitle_ = MT_FormatString( "Prise � partie - eni %d", asnMsg.cr.u.cr_prise_a_partie ); break;
        case T_MsgCR_cr_cr_point_tenu_par_eni                           : strTitle_ = MT_FormatString( "Point tenu par ennemi - eni %d", asnMsg.cr.u.cr_point_tenu_par_eni ); break;
//        case T_MsgCR_cr_cr_demande_appui_sur_eni                        : strTitle_ = MT_FormatString( "Demande d'appui sur ennemi - eni %d", asnMsg.cr.u.cr_demande_appui_sur_eni ); break;
        case T_MsgCR_cr_cr_progression_vers_eni                         : strTitle_ = MT_FormatString( "Progression vers ennemi - eni %d", asnMsg.cr.u.cr_progression_vers_eni ); break;
        case T_MsgCR_cr_cr_presence_eni_probable                        : strTitle_ = "Pr�sence ennemi probable"; break;
        case T_MsgCR_cr_cr_decroche                                     : strTitle_ = "D�croche"; break;
        case T_MsgCR_cr_cr_eni_sur_objectif                             : strTitle_ = MT_FormatString( "Ennemi sur objectif - eni %d", asnMsg.cr.u.cr_eni_sur_objectif ); break;
        case T_MsgCR_cr_cr_riposte                                      : strTitle_ = "Riposte"; break;
        case T_MsgCR_cr_cr_suit_unite                                   : strTitle_ = "Suit unit�"; break;
        case T_MsgCR_cr_cr_passe_en_appui                               : strTitle_ = "Passe en appui"; break;
        case T_MsgCR_cr_cr_en_appui                                     : strTitle_ = "En appui"; break;
        case T_MsgCR_cr_cr_situation_log_favorable                      : strTitle_ = "Situation logistique favorable"; break;
        case T_MsgCR_cr_cr_situation_log_defavorable                    : strTitle_ = "Situation logistique d�favorable"; break;
        case T_MsgCR_cr_cr_detruit_tactiquement                         : strTitle_ = "D�truit tactiquement"; break;
        case T_MsgCR_cr_cr_ambiance_vitesse                             : strTitle_ = "Ambiance vitesse"; break;
        case T_MsgCR_cr_cr_ambiance_surete                              : strTitle_ = "Ambiance suret�"; break;
        case T_MsgCR_cr_cr_sur_lima                                     : strTitle_ = MT_FormatString( "Sur lima - %s", MOS_LimaFuncCheckListItem::BuildNameFuncLima( (E_FuncLimaType)asnMsg.cr.u.cr_sur_lima ).c_str() ); break;
        case T_MsgCR_cr_cr_sur_type_terrain_particulier                 : strTitle_ = MT_FormatString( "Sur type terrain particulier - Terrain %d", asnMsg.cr.u.cr_sur_type_terrain_particulier ); break;
//        case T_MsgCR_cr_cr_debut_calcul_it                              : strTitle_ = "D�but calcul itin�raire"; break;
//        case T_MsgCR_cr_cr_fin_calcul_it                                : strTitle_ = "Fin calcul itin�raire"; break;
//        case T_MsgCR_cr_cr_calcul_it_impossible                         : strTitle_ = "Calcul itin�raire impossible"; break;
//        case T_MsgCR_cr_cr_debut_calcul_position                        : strTitle_ = "D�but calcul position"; break;
//        case T_MsgCR_cr_cr_fin_calcul_position                          : strTitle_ = "Fin calcul position"; break;
//        case T_MsgCR_cr_cr_calcul_position_impossible                   : strTitle_ = "Calcul position impossible"; break;
        case T_MsgCR_cr_cr_travaux_realises                             : strTitle_ = "Travaux r�alis�s"; break;
        case T_MsgCR_cr_cr_obstacle_degage                              : strTitle_ = "Obstacle d�gag�"; break;
        case T_MsgCR_cr_cr_section_deployee                             : strTitle_ = "Section d�ploy�e"; break;
        case T_MsgCR_cr_cr_materiel_retour_de_maintenance               : strTitle_ = "Materiel retour de maintenance"; break;
        case T_MsgCR_cr_cr_reconnaissance_terminee                      : strTitle_ = "Reconnaissance terminee"; break;
        case T_MsgCR_cr_cr_reconnaissance_initiale_terminee             : strTitle_ = "Reconnaissance initiale terminee"; break;
//        case T_MsgCR_cr_cr_declenchement_aussitot_pret                  : strTitle_ = "D�clenchement aussit�t pr�t"; break;
//        case T_MsgCR_cr_cr_declenchement_hso                            : strTitle_ = "D�clenchement HSO"; break;
//        case T_MsgCR_cr_cr_declenchement_au_commandement                : strTitle_ = "D�clenchement au commandement"; break;
        case T_MsgCR_cr_cr_premier_coup_parti                           : strTitle_ = "Premier coup parti"; break;
        case T_MsgCR_cr_cr_tir_execute                                  : strTitle_ = "Tir ex�cut�"; break;
//        case T_MsgCR_cr_cr_tir_annule_unite_de_tir_indisponible         : strTitle_ = "Tir annul� - unit� de tir indisponible"; break;
//        case T_MsgCR_cr_cr_erreur_pas_de_zone_trouvee                   : strTitle_ = "Erreur pas de zone trouv�e"; break;
//        case T_MsgCR_cr_cr_indisponible                                 : strTitle_ = "Indisponible"; break;
        case T_MsgCR_cr_cr_rejoint_plot_ravitaillement                  : strTitle_ = "Rejoint plot ravitaillement"; break;
//        case T_MsgCR_cr_cr_attente_reconnaissance_terminee              : strTitle_ = "Attente reconnaissance termin�e"; break;
//        case T_MsgCR_cr_cr_avertissement_zone_deploiement_inchangee     : strTitle_ = "Avertissement zone d�ploiement inchang�e"; break;
//        case T_MsgCR_cr_cr_erreur_zone_a_rejoindre_occupee              : strTitle_ = "Erreur zone � rejoindre occup�e"; break;
//        case T_MsgCR_cr_cr_disponible                                   : strTitle_ = "Disponible"; break;
        case T_MsgCR_cr_cr_changement_de_position                       : strTitle_ = "Changement de position"; break;
        case T_MsgCR_cr_cr_debut_surveillance                           : strTitle_ = "D�but surveillance"; break;
        case T_MsgCR_cr_cr_debut_installation                           : strTitle_ = "D�but installation"; break;
        case T_MsgCR_cr_cr_pret_pour_conduite_tir                       : strTitle_ = "Pr�t pour conduite de tir"; break;       
//        case T_MsgCR_cr_cr_demande_ravitaillement                       : strTitle_ = MT_FormatString( "Demande de ravitaillement - automate %d", asnMsg.cr.u.cr_demande_ravitaillement ); break;
//        case T_MsgCR_cr_cr_demande_ravitaillement_acceptee              : strTitle_ = MT_FormatString( "Demande de ravitaillement accept�e - automate %d", asnMsg.cr.u.cr_demande_ravitaillement_acceptee ); break;
//        case T_MsgCR_cr_cr_demande_ravitaillement_refusee               : strTitle_ = MT_FormatString( "Demande de ravitaillement refus�e - automate %d", asnMsg.cr.u.cr_demande_ravitaillement_refusee ); break;
//        case T_MsgCR_cr_cr_debut_ravitaillement_dans                    : strTitle_ = MT_FormatString( "Debut ravitaillement dans %d minutes - automate %d", asnMsg.cr.u.cr_debut_ravitaillement_dans->minutes, asnMsg.cr.u.cr_debut_ravitaillement_dans->agent ); break;
//        case T_MsgCR_cr_cr_ravitaillement_annule                        : strTitle_ = MT_FormatString( "Ravitaillement annul� - automate %d", asnMsg.cr.u.cr_ravitaillement_annule ); break;
//        case T_MsgCR_cr_cr_debut_ravitaillement                         : strTitle_ = MT_FormatString( "D�but du ravitaillement - automate %d", asnMsg.cr.u.cr_debut_ravitaillement ); break;
//        case T_MsgCR_cr_cr_fin_ravitaillement                           : strTitle_ = MT_FormatString( "Fin du ravitaillement - automate %d", asnMsg.cr.u.cr_fin_ravitaillement); break;
//        case T_MsgCR_cr_cr_reception_blesses                            : strTitle_ = "R�ception bless�s"; break;
//        case T_MsgCR_cr_cr_evacuation_blesses                           : strTitle_ = "Evacuation bless�s"; break;
//        case T_MsgCR_cr_cr_fin_traitement_blesses                       : strTitle_ = "Fin traitement bless�s"; break;
//        case T_MsgCR_cr_cr_reception_materiel                           : strTitle_ = "R�ception materiel"; break;
//        case T_MsgCR_cr_cr_evacuation_materiel                          : strTitle_ = "Evacuation materiel"; break;
//        case T_MsgCR_cr_cr_fin_traitement_materiel                      : strTitle_ = "Fin traitement materiel"; break;
        case T_MsgCR_cr_cr_organisation_chantier                        : strTitle_ = "Organisation chantier"; break;
        case T_MsgCR_cr_cr_desorganisation_chantier                     : strTitle_ = "D�sorganisation chantier"; break;
        case T_MsgCR_cr_cr_reconnaissance_position_obstacle             : strTitle_ = "Reconnaissance position obstacle"; break;  
        case T_MsgCR_cr_cr_contournement_realise                        : strTitle_ = "Contournement r�alis�"; break;
        case T_MsgCR_cr_cr_attente_renforcement                         : strTitle_ = "Attente renforcement"; break;
        case T_MsgCR_cr_cr_debut_travaux                                : strTitle_ = "D�but travaux"; break;
        case T_MsgCR_cr_cr_debut_franchissement                         : strTitle_ = "D�but franchissement"; break;
        case T_MsgCR_cr_cr_fin_franchissement                           : strTitle_ = "Fin franchissement"; break;
//        case T_MsgCR_cr_cr_fait_observe                                 : strTitle_ = "Fait observ�"; break;
        case T_MsgCR_cr_cr_observation_tir_indirect                     : strTitle_ = MT_FormatString( "Observation tir indirect ID %d", asnMsg.cr.u.cr_observation_tir_indirect ); break;       
        case T_MsgCR_cr_cr_contamine_nbc                                : strTitle_ = "Contamin� NBC"; break;
        case T_MsgCR_cr_cr_decontamine_nbc                              : strTitle_ = "D�contamin� NBC"; break;
        case T_MsgCR_cr_cr_tenue_protection_nbc_mise                    : strTitle_ = "Protection NBC mise"; break;
        case T_MsgCR_cr_cr_tenue_protection_nbc_enlevee                 : strTitle_ = "Protection NBC enlev�e"; break;
        case T_MsgCR_cr_cr_reussite_renforcement                        : strTitle_ = MT_FormatString( "Reussite renforcement pion %d -> automate %d", asnMsg.cr.u.cr_reussite_renforcement->pion_renforcant, asnMsg.cr.u.cr_reussite_renforcement->automate_renforce ); break;
        case T_MsgCR_cr_cr_debut_mission                                : strTitle_ = "D�but mission"; break;
        case T_MsgCR_cr_cr_fin_mission                                  : strTitle_ = "Fin mission"; break;
        case T_MsgCR_cr_cr_automate_prise_contact                       : strTitle_ = "Automate prise de contact"; break;
        case T_MsgCR_cr_cr_automate_rupture_contact                     : strTitle_ = "Automate rupture de contact"; break;
//        case T_MsgCR_cr_cr_pret_a_jalonner                              : strTitle_ = "Pr�t � jalonner"; break;
        case T_MsgCR_cr_cr_dispositif_installe                          : strTitle_ = "Dispositif install�"; break;
        case T_MsgCR_cr_cr_en_position                                  : strTitle_ = "En position"; break;
        case T_MsgCR_cr_cr_objectif_atteint                             : strTitle_ = "Objectif atteint"; break;
//        case T_MsgCR_cr_cr_pion_bloque                                  : strTitle_ = MT_FormatString( "Pion %d bloqu�", asnMsg.cr.u.cr_pion_bloque ); break;
        case T_MsgCR_cr_cr_fuseau_rejoint                               : strTitle_ = "Fuseau rejoint"; break;
        case T_MsgCR_cr_cr_debouche                                     : strTitle_ = "D�bouche"; break;
        case T_MsgCR_cr_cr_alerte_nbc                                   : strTitle_ = "Alerte NBC"; break;
        case T_MsgCR_cr_cr_site_decontamination_anime                   : strTitle_ = "Site de d�contamination anim�"; break;
        case T_MsgCR_cr_cr_site_decontamination_desactive               : strTitle_ = "Site de d�contamination d�sactiv�"; break;
        case T_MsgCR_cr_cr_demande_decontamination                      : strTitle_ = "Demande de d�contamination"; break;
//        case T_MsgCR_cr_cr_demande_de_releve                            : strTitle_ = MT_FormatString( "Demande de rel�ve agent %d", asnMsg.cr.u.cr_demande_de_releve ); break;
        case T_MsgCR_cr_cr_releve                                       : strTitle_ = MT_FormatString( "Rel�ve agent %d", asnMsg.cr.u.cr_releve ); break;
        case T_MsgCR_cr_cr_sur_pia                                      : strTitle_ = MT_FormatString( "Sur PIA %d", asnMsg.cr.u.cr_sur_pia ); break;
        case T_MsgCR_cr_cr_passage_sur_pia                              : strTitle_ = "Passage sur PIA"; break;
        case T_MsgCR_cr_cr_unite_recueillie                             : strTitle_ = MT_FormatString( "Unit� %d recueillie", asnMsg.cr.u.cr_unite_recueillie ); break;
        case T_MsgCR_cr_cr_en_controle_zone                             : strTitle_ = "En contr�le de zone"; break;
        case T_MsgCR_cr_cr_en_surveillance                              : strTitle_ = "En surveillance"; break;
        case T_MsgCR_cr_cr_en_infiltration                              : strTitle_ = "En infiltration"; break;
        case T_MsgCR_cr_cr_en_exfiltration                              : strTitle_ = "En exfiltration"; break;
        case T_MsgCR_cr_cr_attente_condition_favorable_pour_infiltration: strTitle_ = "Attente condition favorable pour infiltration"; break;
        case T_MsgCR_cr_cr_en_cours_de_franchissement                   : strTitle_ = "En cours de franchissement"; break;
        case T_MsgCR_cr_cr_reception_ordre_de_feu                       : strTitle_ = "R�ception ordre de feu"; break;
        case T_MsgCR_cr_cr_pret_au_tir                                  : strTitle_ = "Pr�t au tir"; break;
        case T_MsgCR_cr_cr_itineraire_retabli                           : strTitle_ = "Itineraire r�tabli"; break;
        case T_MsgCR_cr_cr_renforcement_bientot_fini                    : strTitle_ = MT_FormatString( "Renforcement bient�t fini - unit� %d", asnMsg.cr.u.cr_renforcement_bientot_fini ); break;      
        case T_MsgCR_cr_cr_echec_renforcement                           : strTitle_ = MT_FormatString( "echec renforcement pion %d -> automate %d", asnMsg.cr.u.cr_echec_renforcement->pion_renforcant, asnMsg.cr.u.cr_echec_renforcement->automate_renforce ); break;
        case T_MsgCR_cr_cr_attente_ordre_poursuivre                     : strTitle_ = "Attente ordre poursuivre"; break;
        case T_MsgCR_cr_cr_attente_ordre_deboucher                      : strTitle_ = "Attente ordre d�boucher"; break;
        case T_MsgCR_cr_cr_drone_lance                                  : strTitle_ = "Drone lanc�"; break;
        case T_MsgCR_cr_cr_drone_en_exploitation                        : strTitle_ = "Drone en exploitation"; break;
        case T_MsgCR_cr_cr_drone_exploite                               : strTitle_ = "Drone exploite"; break;
        case T_MsgCR_cr_cr_brouille                                     : strTitle_ = "Brouill�"; break;
        case T_MsgCR_cr_cr_en_ecoute                                    : strTitle_ = "En �coute"; break;
        case T_MsgCR_cr_cr_demande_ravitaillement                       : strTitle_ = "Demande ravitaillement"; break;
        case T_MsgCR_cr_cr_demande_evacuation_sanitaire                 : strTitle_ = "Demande �vacuation sanitaire"; break;
        case T_MsgCR_cr_cr_demande_evacuation_materiel                  : strTitle_ = "Demande �vacuation mat�rielle"; break;
        case T_MsgCR_cr_cr_en_cours_ravitaillement                      : strTitle_ = "En cours ravitaillement"; break;
        case T_MsgCR_cr_cr_ravitaillement_termine                       : strTitle_ = "Ravitaillemene termin�"; break;
        case T_MsgCR_cr_cr_helicoptere_en_observation                   : strTitle_ = "H�licopt�re en observation"; break;
        case T_MsgCR_cr_cr_perte_info_guidage                           : strTitle_ = "Perte info guidage"; break;
        case T_MsgCR_cr_cr_zone_reconnue                                : strTitle_ = "Zone reconnue"; break;
        case T_MsgCR_cr_cr_zone_decontaminee                            : strTitle_ = "Zone d�contamin�e"; break;
        case T_MsgCR_cr_cr_site_rota_reconnu                            : strTitle_ = "Site rota reconnu"; break;
        case T_MsgCR_cr_cr_incident_nbc_termine                         : strTitle_ = "Incident NBC termin�"; break;
        case T_MsgCR_cr_cr_obstacle_en_attente_activation               : strTitle_ = MT_FormatString( "Obstacle %d en attente activation", asnMsg.cr.u.cr_obstacle_en_attente_activation ); break;
        case T_MsgCR_cr_cr_allocation_consentie_bientot_epuisee         : strTitle_ = MT_FormatString( "Allocation consentie bientot �puis�e pour dotation type %s", MOS_App::GetApp().GetRessourceName( asnMsg.cr.u.cr_allocation_consentie_bientot_epuisee ).c_str() ); break;
        case T_MsgCR_cr_cr_pion_decontamine                             : strTitle_ = MT_FormatString( "Pion %d d�contamin�", asnMsg.cr.u.cr_pion_decontamine ); break;
        case T_MsgCR_cr_cr_tir_dans_zone_interdite                      : strTitle_ = "Tir dans zone interdite"; break;
        case T_MsgCR_cr_cr_tir_sur_camp_ami                             : strTitle_ = "Tir sur camp ami"; break;
        case T_MsgCR_cr_cr_tire_par_camp_ami                            : strTitle_ = "Tir� par camp ami"; break;
        case T_MsgCR_cr_cr_tir_sur_camp_neutre                          : strTitle_ = "Tir sur camp neutre"; break;
        case T_MsgCR_cr_cr_tire_par_camp_neutre                         : strTitle_ = "Tir� par camp neutre"; break;
        case T_MsgCR_cr_cr_tir_sur_civil                                : strTitle_ = "Tir sur civil"; break;
        case T_MsgCR_cr_cr_tire_par_civil                               : strTitle_ = "Tir� par civil"; break;
        case T_MsgCR_cr_cr_destruction_pc                               : strTitle_ = "Destruction PC"; break;
        case T_MsgCR_cr_cr_deces_blesse                                 : strTitle_ = "D�c�s bless�"; break;
        case T_MsgCR_cr_cr_deces_blesse_pendant_transport               : strTitle_ = "D�c�s bless� pendant transport"; break;
        case T_MsgCR_cr_cr_deces_blesse_pendant_hospitalisation         : strTitle_ = "D�c�s bless� pendant hospitalisation"; break;
        case T_MsgCR_cr_cr_transport_unite_pas_prete                    : strTitle_ = MT_FormatString( "Transport : pion %d pas pr�t", asnMsg.cr.u.cr_transport_unite_pas_prete ); break;
        case T_MsgCR_cr_cr_trace                                        : strTitle_ = asnMsg.cr.u.cr_trace; break;
        case T_MsgCR_cr_cr_trace_id                                     : strTitle_ = MT_FormatString( "Trace adaptation : %d", asnMsg.cr.u.cr_trace_id ); break;
        case T_MsgCR_cr_cr_presence_population                            : strTitle_ = "Presence population"; break;
        case T_MsgCR_cr_cr_orientation_population_vers_camp_refugies      : strTitle_ = "Orientation population vers camp de r�fugi�s"; break;
        case T_MsgCR_cr_cr_pas_de_camp_refugies_disponible                : strTitle_ = "Pas de camp de r�fugi�s disponible"; break;
        case T_MsgCR_cr_cr_presence_ennemi_rendu                          : strTitle_ = "Pr�sence ennemi rendu"; break;
        case T_MsgCR_cr_cr_orientation_ennemi_rendu_vers_camp_prisonniers : strTitle_ = "Orientation ennemi rendu vers camp des prisonniers"; break;
        case T_MsgCR_cr_cr_pas_de_camp_prisonniers_disponible             : strTitle_ = "Pas de camp de prisonniers disponible"; break;
        case T_MsgCR_cr_cr_transport_en_cours                           : strTitle_ = "Transport en cours"; break;
        case T_MsgCR_cr_cr_transport_embarquement                       : strTitle_ = "Transport embarquement"; break;
        case T_MsgCR_cr_cr_transport_debarquement                       : strTitle_ = "Transport d�barquement"; break;
        case T_MsgCR_cr_cr_zone_implantation_occupee                    : strTitle_ = "Zone implantation occup�e"; break;
        case T_MsgCR_cr_cr_installation_sur_zone_implantation           : strTitle_ = "Installation sur zone implantation"; break;
        case T_MsgCR_cr_cr_tir_hors_de_portee                           : strTitle_ = "Tir hors de port�e"; break;
        case T_MsgCR_cr_cr_tir_annule                                   : strTitle_ = "Tir annul�"; break;

        case T_MsgCR_cr_cr_en_ecoute_radar                              : strTitle_ = "En �coute radar"; break;
        case T_MsgCR_cr_cr_debut_mise_en_oeuvre_drone                   : strTitle_ = "Debut mise en oeuvre drone"; break;
        case T_MsgCR_cr_cr_fin_mise_en_oeuvre_drone                     : strTitle_ = "Fin mise en oeuvre drone"; break;                 
        case T_MsgCR_cr_cr_vehicule_laisse_sur_position                 : strTitle_ = "V�hicule laiss� sur position"; break;
        case T_MsgCR_cr_cr_rejoint_position_embarquement                : strTitle_ = "Rejoint position embarquement"; break;
        case T_MsgCR_cr_cr_en_attente_transporteurs                     : strTitle_ = "En attente transporteurs"; break;
        case T_MsgCR_cr_cr_mission_impossible_pions_subordonnes_non_operationnels  : strTitle_ = "Mission impossible :  pions subordonn�s non operationnels"; break;
        case T_MsgCR_cr_cr_mission_impossible_lima_invalide                        : strTitle_ = "Mission impossible : lima invalide"; break;
        case T_MsgCR_cr_cr_mission_impossible_zone_hors_fuseau                     : strTitle_ = "Mission impossible : zone hors fuseau"; break;
        case T_MsgCR_cr_cr_rapport_de_force_defavorable                            : strTitle_ = "Rapport de force d�favorable"; break;
        case T_MsgCR_cr_cr_terrain_difficile                                       : strTitle_ = "Terrain difficile"; break;
        case T_MsgCR_cr_cr_plus_de_carburant                                       : strTitle_ = "Plus de carburant"; break;
        case T_MsgCR_cr_cr_dispositif_disperse                                     : strTitle_ = "Dispositif dispers�"; break;
        case T_MsgCR_cr_cr_pion_neutralise                                         : strTitle_ = "Pion neutralis�"; break;
        case T_MsgCR_cr_cr_silence_radio                                           : strTitle_ = "Silence radio"; break;
        case T_MsgCR_cr_cr_tir_restreint                                           : strTitle_ = "Tir restreint"; break;
        case T_MsgCR_cr_cr_tir_interdit                                            : strTitle_ = "Tir interdit"; break;
        case T_MsgCR_cr_cr_tir_libre                                               : strTitle_ = "Tir libre"; break;                                                                       


        case T_MsgCR_cr_cr_preparation_mission                          : 
            {
                if( asnMsg.cr.u.cr_preparation_mission->m.delaiPresent )
                    strTitle_ = MT_FormatString( "Preparation mission - D�lai %d minutes", asnMsg.cr.u.cr_preparation_mission->delai );
                else
                    strTitle_ = MT_FormatString( "Preparation mission - Pas de d�lai" );
                break;
            }          
//        case T_MsgCR_cr_cr_demande_de_tir                             : 
         /*   {
                strTitle_ = MT_FormatString( "Demande de tir EO - pos %s", std::string( (char*)asnMsg.cr.u.cr_demande_de_tir->position_cible.data, 15 ).c_str() ); 
                if( asnMsg.cr.u.cr_demande_de_tir->m.ciblePresent )
                    strTitle_ += MT_FormatString( " - Cible %d", asnMsg.cr.u.cr_demande_de_tir->cible );
                break;
            }*/
/*        case T_MsgCR_cr_cr_demande_appui : 
            {
                if( asnMsg.cr.u.cr_demande_appui == EnumTypeDemandeAppui::genie )
                    strTitle_ = "Demande d'appui GENIE";
                else if( asnMsg.cr.u.cr_demande_appui == EnumTypeDemandeAppui::artillerie )
                    strTitle_ = "Demande d'appui ARTILLERIE";
                else
                    assert( false );
                break;
            }*/




        default:
            assert( false );
    }

    if( agent_.GetAttributeEditor() != 0 )
        agent_.GetAttributeEditor()->CreateRCItem( *this );
}


//-----------------------------------------------------------------------------
// Name: MOS_RC::Initialize
// Created: NLD 2003-04-04
//-----------------------------------------------------------------------------
void MOS_RC::Initialize( const ASN1T_MsgAttenteOrdreConduite& asnMsg )
{
    //$$$$ TEMPORAIRE
    strTitle_ = "Attente d'ordre";

    for( uint i = 0; i < asnMsg.ordres_conduite.n; ++i )
    {
        E_OrderConduiteID nOrderConduiteID;
        switch( asnMsg.ordres_conduite.elem[i].t  )
        {
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_attendre_se_poster                              : nOrderConduiteID = eOrdreConduite_AttendreSePoster                            ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_poursuivre                                      : nOrderConduiteID = eOrdreConduite_Poursuivre                                  ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_contourner                                 : nOrderConduiteID = eOrdreConduite_Pion_Contourner                             ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_decrocher                                       : nOrderConduiteID = eOrdreConduite_Decrocher                              ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_deboucher                                       : nOrderConduiteID = eOrdreConduite_Deboucher                              ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_activer_obstacle                           : nOrderConduiteID = eOrdreConduite_Pion_ActiverObstacle                        ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_executer_tir                               : nOrderConduiteID = eOrdreConduite_Pion_ExecuterTir                            ; break;
            default:
                assert( false );
        }
        conduiteOrderVector_.push_back( nOrderConduiteID );
//        if( pAgentAttrEditor )
//            pItem_->AddOrderConduite( nOrderConduiteID );
    }

    
    MOS_AttrEditor* pAgentAttrEditor = agent_.GetAttributeEditor();
    if( pAgentAttrEditor )
        pAgentAttrEditor->CreateRCItem( *this );
}

//$$$ A VIRER
/*typedef enum E_ModeDeDeclenchementTir
{
    eModeDeclenchement_AussitotPret,
    eModeDeclenchement_HSO,
    eModeDeclenchement_AuCommandement,
};

//-----------------------------------------------------------------------------
// Name: MOS_RC::CreateParamsRCDemandeDeTirEO
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_RC::CreateParamsRCDemandeDeTirEO( DIN_Input& input )
{
    uint8       bEmblee;
    uint8       nFamillePion;
    uint8       bPoster;
    MT_Vector2D vCoordCentreCible;
    uint8       nModeTir;

    input >> bEmblee;
    input >> nFamillePion;
    input >> bPoster;
    input >> vCoordCentreCible;
    input >> nModeTir;

    textVector_.push_back( MT_FormatString( "%s", bEmblee ? "Tir d'embl�e" : "Tir de mise en place" ) );
    textVector_.push_back( MT_FormatString( "Famille pion : %d", nFamillePion ) );
    textVector_.push_back( MT_FormatString( "Attitude : %s", bPoster ? "En poste" : "En mouvement" ) );
    textVector_.push_back( MT_FormatString( "Coord cible : %.2f/%.2f", vCoordCentreCible.rX_, vCoordCentreCible.rY_ ) );
    switch( nModeTir) 
    {
        case eModeDeclenchement_AussitotPret:   textVector_.push_back( "Mode : aussitot pret" );
        case eModeDeclenchement_HSO:            textVector_.push_back( "Mode : HSA" ); 
        case eModeDeclenchement_AuCommandement: textVector_.push_back( "Mode : Au commandement" );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_RC::CreateParamsRCArriveeSurPosition
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_RC::CreateParamsRCArriveeSurPosition( DIN_Input& input )
{
    std::string strPos;
    MT_Vector2D vPos;
    
    input >> strPos;
    input >> vPos;
    
    textVector_.push_back( MT_FormatString( "%s : %.2f / %.2f", strPos.c_str(), vPos.rX_, vPos.rY_ ) );
}

           */


