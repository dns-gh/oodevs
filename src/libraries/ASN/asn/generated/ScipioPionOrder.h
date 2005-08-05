/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 05-Aug-2005.
 */
#ifndef SCIPIOPIONORDER_H
#define SCIPIOPIONORDER_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioOrder.h"
#include "ScipioPionMission_ALAT.h"
#include "ScipioPionMission_ASA.h"
#include "ScipioPionMission_ASS.h"
#include "ScipioPionMission_ABC.h"
#include "ScipioPionMission_GEN.h"
#include "ScipioPionMission_INF.h"
#include "ScipioPionMission_LOG.h"
#include "ScipioPionMission_Misc.h"
#include "ScipioPionMission_NBC.h"
#include "ScipioPionMission_RENS.h"
#include "ScipioPionMission_TRANS.h"

/**************************************************************/
/*                                                            */
/*  Mission_Pion                                              */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_Mission_Pion_mission_pion_alat_evacuation_sanitaire 1
#define T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur 2
#define T_Mission_Pion_mission_pion_alat_reconnaitre_contour_ennemi 3
#define T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone 4
#define T_Mission_Pion_mission_pion_alat_detruire_aeromobile 5
#define T_Mission_Pion_mission_pion_alat_freiner 6
#define T_Mission_Pion_mission_pion_alat_jalonner 7
#define T_Mission_Pion_mission_pion_alat_escorter 8
#define T_Mission_Pion_mission_pion_alat_heliporter 9
#define T_Mission_Pion_mission_pion_alat_heliporter_helitransporter_plot_ravitaillement 10
#define T_Mission_Pion_mission_pion_alat_helitransporter 11
#define T_Mission_Pion_mission_pion_alat_effectuer_recherche_et_sauvetage 12
#define T_Mission_Pion_mission_pion_alat_imex 13
#define T_Mission_Pion_mission_pion_alat_eclairer 14
#define T_Mission_Pion_mission_pion_alat_surveiller 15
#define T_Mission_Pion_mission_pion_alat_appuyer_direct_au_contact 16
#define T_Mission_Pion_mission_pion_alat_couvrir 17
#define T_Mission_Pion_mission_pion_alat_attaquer 18
#define T_Mission_Pion_mission_pion_asa_defendre_site 19
#define T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position 20
#define T_Mission_Pion_mission_pion_asa_defendre_zone 21
#define T_Mission_Pion_mission_pion_asa_defendre_zone_a_partir_position 22
#define T_Mission_Pion_mission_pion_asa_surveiller 23
#define T_Mission_Pion_mission_pion_asa_defendre_unites 24
#define T_Mission_Pion_mission_pion_asa_mistral_defendre_site 25
#define T_Mission_Pion_mission_pion_asa_mistral_defendre_site_a_partir_position 26
#define T_Mission_Pion_mission_pion_asa_mistral_defendre_zone 27
#define T_Mission_Pion_mission_pion_asa_mistral_defendre_zone_a_partir_position 28
#define T_Mission_Pion_mission_pion_asa_mistral_surveiller 29
#define T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre 30
#define T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre_sur_position 31
#define T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement 32
#define T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni 33
#define T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs 34
#define T_Mission_Pion_mission_pion_ass_eop_acquerir_objectifs 35
#define T_Mission_Pion_mission_pion_ass_ratac_acquerir_objectifs 36
#define T_Mission_Pion_mission_pion_abc_reconnaitre_axe 37
#define T_Mission_Pion_mission_pion_abc_soutenir 38
#define T_Mission_Pion_mission_pion_abc_couvrir 39
#define T_Mission_Pion_mission_pion_abc_flanc_garder 40
#define T_Mission_Pion_mission_pion_abc_reduire 41
#define T_Mission_Pion_mission_pion_abc_appuyer 42
#define T_Mission_Pion_mission_pion_abc_fixer 43
#define T_Mission_Pion_mission_pion_abc_renseigner_sur 44
#define T_Mission_Pion_mission_pion_abc_controler_zone 45
#define T_Mission_Pion_mission_pion_abc_surveiller 46
#define T_Mission_Pion_mission_pion_abc_barrer 47
#define T_Mission_Pion_mission_pion_abc_attaquer 48
#define T_Mission_Pion_mission_pion_abc_reconnaitre_point 49
#define T_Mission_Pion_mission_pion_abc_eclairer 50
#define T_Mission_Pion_mission_pion_abc_jalonner 51
#define T_Mission_Pion_mission_pion_abc_contre_attaquer 52
#define T_Mission_Pion_mission_pion_abc_semparer_zone 53
#define T_Mission_Pion_mission_pion_abc_relever_unite 54
#define T_Mission_Pion_mission_pion_abc_armer_pia 55
#define T_Mission_Pion_mission_pion_abc_ouvrir_itineraire 56
#define T_Mission_Pion_mission_pion_abc_freiner 57
#define T_Mission_Pion_mission_pion_abc_escorter_un_convoi 58
#define T_Mission_Pion_mission_pion_abc_armer_point_de_controle 59
#define T_Mission_Pion_mission_pion_abc_organiser_accueil_colonne_refugies 60
#define T_Mission_Pion_mission_pion_gen_realiser_fosse_ac 61
#define T_Mission_Pion_mission_pion_gen_realiser_abattis_mine 62
#define T_Mission_Pion_mission_pion_gen_realiser_point_mine 63
#define T_Mission_Pion_mission_pion_gen_realiser_barricade 64
#define T_Mission_Pion_mission_pion_gen_realiser_eboulement 65
#define T_Mission_Pion_mission_pion_gen_realiser_bouchon_mine 66
#define T_Mission_Pion_mission_pion_gen_detruire_route 67
#define T_Mission_Pion_mission_pion_gen_detruire_pont 68
#define T_Mission_Pion_mission_pion_gen_securiser_itineraire 69
#define T_Mission_Pion_mission_pion_gen_degager_abattis_mine 70
#define T_Mission_Pion_mission_pion_gen_degager_eboulement 71
#define T_Mission_Pion_mission_pion_gen_degager_rues 72
#define T_Mission_Pion_mission_pion_gen_combler_entonnoir 73
#define T_Mission_Pion_mission_pion_gen_combler_fosse_ac 74
#define T_Mission_Pion_mission_pion_gen_creer_contournement 75
#define T_Mission_Pion_mission_pion_gen_realiser_zone_minee_lineaire 76
#define T_Mission_Pion_mission_pion_gen_ouvrir_trouee 77
#define T_Mission_Pion_mission_pion_gen_renforcer 78
#define T_Mission_Pion_mission_pion_gen_franchir_entonnoir 79
#define T_Mission_Pion_mission_pion_gen_franchir_fosse_ac 80
#define T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion 81
#define T_Mission_Pion_mission_pion_gen_realiser_postes_tir 82
#define T_Mission_Pion_mission_pion_gen_realiser_travaux_protection 83
#define T_Mission_Pion_mission_pion_gen_retablir_itineraire 84
#define T_Mission_Pion_mission_pion_gen_equiper_exploiter 85
#define T_Mission_Pion_mission_pion_gen_executer_variantement 86
#define T_Mission_Pion_mission_pion_gen_reconnaitre_site_franchissement 87
#define T_Mission_Pion_mission_pion_gen_reconnaitre_itineraire 88
#define T_Mission_Pion_mission_pion_gen_deminer_site_franchissement 89
#define T_Mission_Pion_mission_pion_gen_reconnaitre_zone 90
#define T_Mission_Pion_mission_pion_gen_creer_plate_forme 91
#define T_Mission_Pion_mission_pion_gen_creer_piste 92
#define T_Mission_Pion_mission_pion_gen_creer_aire_poser 93
#define T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs 94
#define T_Mission_Pion_mission_pion_gen_activer_obstacle 95
#define T_Mission_Pion_mission_pion_gen_ameliorer_mobilite 96
#define T_Mission_Pion_mission_pion_gen_realiser_aire_logistique 97
#define T_Mission_Pion_mission_pion_gen_depolluer_zone 98
#define T_Mission_Pion_mission_pion_gen_amenager_terrain_poser 99
#define T_Mission_Pion_mission_pion_gen_realiser_plate_forme_deploiement 100
#define T_Mission_Pion_mission_pion_gen_reprendre_mission_construction 101
#define T_Mission_Pion_mission_pion_gen_realiser_zone_poser_helicoptere 102
#define T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu 103
#define T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers 104
#define T_Mission_Pion_mission_pion_gen_realiser_camp_refugies 105
#define T_Mission_Pion_mission_pion_inf_reconnaitre_axe 106
#define T_Mission_Pion_mission_pion_inf_couvrir 107
#define T_Mission_Pion_mission_pion_inf_reduire 108
#define T_Mission_Pion_mission_pion_inf_appuyer 109
#define T_Mission_Pion_mission_pion_inf_fixer 110
#define T_Mission_Pion_mission_pion_inf_controler_secteur 111
#define T_Mission_Pion_mission_pion_inf_surveiller_secteur 112
#define T_Mission_Pion_mission_pion_inf_barrer 113
#define T_Mission_Pion_mission_pion_inf_reconnaitre_point 114
#define T_Mission_Pion_mission_pion_inf_eclairer 115
#define T_Mission_Pion_mission_pion_inf_interdire 116
#define T_Mission_Pion_mission_pion_inf_semparer 117
#define T_Mission_Pion_mission_pion_inf_prendre_preciser_contact 118
#define T_Mission_Pion_mission_pion_inf_defendre 119
#define T_Mission_Pion_mission_pion_inf_detruire_embuscade 120
#define T_Mission_Pion_mission_pion_inf_contre_attaquer 121
#define T_Mission_Pion_mission_pion_inf_freiner 122
#define T_Mission_Pion_mission_pion_inf_flanc_garder 123
#define T_Mission_Pion_mission_pion_inf_organiser_un_check_point 124
#define T_Mission_Pion_mission_pion_inf_harceler 125
#define T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main 126
#define T_Mission_Pion_mission_pion_inf_escorter_un_convoi 127
#define T_Mission_Pion_mission_pion_inf_renseigner_sur 128
#define T_Mission_Pion_mission_pion_inf_soutenir 129
#define T_Mission_Pion_mission_pion_inf_securiser 130
#define T_Mission_Pion_mission_pion_inf_recueillir 131
#define T_Mission_Pion_mission_pion_inf_repondre_a_aggression 132
#define T_Mission_Pion_mission_pion_log_se_deployer 133
#define T_Mission_Pion_mission_pion_log_distribuer_moyens 134
#define T_Mission_Pion_mission_pion_log_trier_blesses 135
#define T_Mission_Pion_mission_pion_log_traiter_blesses 136
#define T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses 137
#define T_Mission_Pion_mission_pion_log_soigner_blesses 138
#define T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales 139
#define T_Mission_Pion_mission_pion_log_decontaminer_blesses 140
#define T_Mission_Pion_mission_pion_log_reparer 141
#define T_Mission_Pion_mission_pion_log_evacuer 142
#define T_Mission_Pion_mission_pion_log_convoyer 143
#define T_Mission_Pion_mission_pion_log_distribuer_ressources 144
#define T_Mission_Pion_mission_pion_log_constituer_convoi 145
#define T_Mission_Pion_mission_pion_log_livrer 146
#define T_Mission_Pion_mission_pion_log_preparer_bascule 147
#define T_Mission_Pion_mission_pion_log_appuyer_mouvement 148
#define T_Mission_Pion_mission_pion_log_reconnaitre_itineraire 149
#define T_Mission_Pion_mission_pion_log_transporter_unite_chars 150
#define T_Mission_Pion_mission_pion_log_reconnaitre_zone_contaminee 151
#define T_Mission_Pion_mission_pion_log_appuyer_mouvement_dans_zone 152
#define T_Mission_Pion_mission_pion_log_mettre_en_oeuvre_zone_stationnement 153
#define T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point 154
#define T_Mission_Pion_mission_pion_log_bloquer_zone 155
#define T_Mission_Pion_mission_pion_log_reconnaitre_zone_de_deploiement 156
#define T_Mission_Pion_mission_pion_log_armer_pia 157
#define T_Mission_Pion_mission_pion_log_surveiller 158
#define T_Mission_Pion_mission_pion_test_heliporter 159
#define T_Mission_Pion_mission_pion_test_move_to 160
#define T_Mission_Pion_mission_pion_test_create_object 161
#define T_Mission_Pion_mission_pion_test_destroy_object 162
#define T_Mission_Pion_mission_pion_test_create_bypass 163
#define T_Mission_Pion_mission_pion_test_fire 164
#define T_Mission_Pion_mission_pion_test_change_posture 165
#define T_Mission_Pion_mission_pion_test_path_find 166
#define T_Mission_Pion_mission_pion_test_find_position 167
#define T_Mission_Pion_mission_pion_test_reinforce 168
#define T_Mission_Pion_mission_pion_plastron_ennemi 169
#define T_Mission_Pion_mission_pion_faire_mouvement 170
#define T_Mission_Pion_mission_pion_suivre 171
#define T_Mission_Pion_mission_pion_se_faire_decontaminer 172
#define T_Mission_Pion_mission_pion_franchir 173
#define T_Mission_Pion_mission_pion_decrocher 174
#define T_Mission_Pion_mission_pion_stationner 175
#define T_Mission_Pion_mission_pion_se_faire_transporter 176
#define T_Mission_Pion_mission_pion_generique 177
#define T_Mission_Pion_mission_pion_milice_provoquer 178
#define T_Mission_Pion_mission_pion_milice_prendre_et_tenir 179
#define T_Mission_Pion_mission_pion_milice_detruire_embuscade 180
#define T_Mission_Pion_mission_pion_joint_marine_transporter 181
#define T_Mission_Pion_mission_pion_joint_air_effectuer_raid 182
#define T_Mission_Pion_mission_pion_nbc_animer_un_plot_de_decontamination 183
#define T_Mission_Pion_mission_pion_nbc_reconnaitre_itineraire 184
#define T_Mission_Pion_mission_pion_nbc_reconnaitre_zone 185
#define T_Mission_Pion_mission_pion_nbc_decontaminer_zone 186
#define T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota 187
#define T_Mission_Pion_mission_pion_rens_rohum_renseigner_sur 188
#define T_Mission_Pion_mission_pion_rens_rohum_sexfiltrer 189
#define T_Mission_Pion_mission_pion_rens_rohum_orienter_guider 190
#define T_Mission_Pion_mission_pion_rens_roem_appuyer 191
#define T_Mission_Pion_mission_pion_rens_roem_renseigner_sur 192
#define T_Mission_Pion_mission_pion_rens_roem_mettre_en_oeuvre 193
#define T_Mission_Pion_mission_pion_rens_radint_renseigner_sur 194
#define T_Mission_Pion_mission_pion_rens_radint_mettre_en_oeuvre 195
#define T_Mission_Pion_mission_pion_rens_roim_mettre_en_oeuvre 196
#define T_Mission_Pion_mission_pion_rens_roim_sdti_renseigner_sur 197
#define T_Mission_Pion_mission_pion_rens_roim_cl289_renseigner_sur 198
#define T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison 199

typedef struct EXTERN ASN1T_Mission_Pion {
   int t;
   union {
      /* t = 1 */
      ASN1T_Mission_Pion_ALAT_EvacuationSanitaire *mission_pion_alat_evacuation_sanitaire;
      /* t = 2 */
      ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur *mission_pion_alat_reconnaitre_dans_la_profondeur;
      /* t = 3 */
      ASN1T_Mission_Pion_ALAT_ReconnaitreContourEnnemi *mission_pion_alat_reconnaitre_contour_ennemi;
      /* t = 4 */
      ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone *mission_pion_alat_detruire_neutraliser_dans_zone;
      /* t = 5 */
      ASN1T_Mission_Pion_ALAT_DetruireAeromobile *mission_pion_alat_detruire_aeromobile;
      /* t = 6 */
      ASN1T_Mission_Pion_ALAT_Freiner *mission_pion_alat_freiner;
      /* t = 7 */
      ASN1T_Mission_Pion_ALAT_Jalonner *mission_pion_alat_jalonner;
      /* t = 8 */
      ASN1T_Mission_Pion_ALAT_Escorter *mission_pion_alat_escorter;
      /* t = 9 */
      ASN1T_Mission_Pion_ALAT_Heliporter *mission_pion_alat_heliporter;
      /* t = 10 */
      ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement *mission_pion_alat_heliporter_helitransporter_plot_ravitaillement;
      /* t = 11 */
      ASN1T_Mission_Pion_ALAT_Helitransporter *mission_pion_alat_helitransporter;
      /* t = 12 */
      ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage *mission_pion_alat_effectuer_recherche_et_sauvetage;
      /* t = 13 */
      ASN1T_Mission_Pion_ALAT_IMEX *mission_pion_alat_imex;
      /* t = 14 */
      ASN1T_Mission_Pion_ALAT_Eclairer *mission_pion_alat_eclairer;
      /* t = 15 */
      ASN1T_Mission_Pion_ALAT_Surveiller *mission_pion_alat_surveiller;
      /* t = 16 */
      ASN1T_Mission_Pion_ALAT_AppuyerDirectAuContact *mission_pion_alat_appuyer_direct_au_contact;
      /* t = 17 */
      ASN1T_Mission_Pion_ALAT_Couvrir *mission_pion_alat_couvrir;
      /* t = 18 */
      ASN1T_Mission_Pion_ALAT_Attaquer *mission_pion_alat_attaquer;
      /* t = 19 */
      ASN1T_Mission_Pion_ASA_DefendreSite *mission_pion_asa_defendre_site;
      /* t = 20 */
      ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition *mission_pion_asa_defendre_site_a_partir_position;
      /* t = 21 */
      ASN1T_Mission_Pion_ASA_DefendreZone *mission_pion_asa_defendre_zone;
      /* t = 22 */
      ASN1T_Mission_Pion_ASA_DefendreZoneAPartirPosition *mission_pion_asa_defendre_zone_a_partir_position;
      /* t = 23 */
      ASN1T_Mission_Pion_ASA_Surveiller *mission_pion_asa_surveiller;
      /* t = 24 */
      ASN1T_Mission_Pion_ASA_DefendreUnites *mission_pion_asa_defendre_unites;
      /* t = 25 */
      ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite *mission_pion_asa_mistral_defendre_site;
      /* t = 26 */
      ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition *mission_pion_asa_mistral_defendre_site_a_partir_position;
      /* t = 27 */
      ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone *mission_pion_asa_mistral_defendre_zone;
      /* t = 28 */
      ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition *mission_pion_asa_mistral_defendre_zone_a_partir_position;
      /* t = 29 */
      ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller *mission_pion_asa_mistral_surveiller;
      /* t = 30 */
      ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre *mission_pion_ass_assurer_mise_en_oeuvre;
      /* t = 31 */
      ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition *mission_pion_ass_assurer_mise_en_oeuvre_sur_position;
      /* t = 32 */
      ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement *mission_pion_ass_reconnaitre_zone_deploiement;
      /* t = 33 */
      ASN1T_Mission_Pion_ASS_AcquerirLanceursEni *mission_pion_ass_acquerir_lanceurs_eni;
      /* t = 34 */
      ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs *mission_pion_ass_eo_acquerir_objectifs;
      /* t = 35 */
      ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs *mission_pion_ass_eop_acquerir_objectifs;
      /* t = 36 */
      ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs *mission_pion_ass_ratac_acquerir_objectifs;
      /* t = 37 */
      ASN1T_Mission_Pion_ABC_ReconnaitreAxe *mission_pion_abc_reconnaitre_axe;
      /* t = 38 */
      ASN1T_Mission_Pion_ABC_Soutenir *mission_pion_abc_soutenir;
      /* t = 39 */
      ASN1T_Mission_Pion_ABC_Couvrir *mission_pion_abc_couvrir;
      /* t = 40 */
      ASN1T_Mission_Pion_ABC_FlancGarder *mission_pion_abc_flanc_garder;
      /* t = 41 */
      ASN1T_Mission_Pion_ABC_Reduire *mission_pion_abc_reduire;
      /* t = 42 */
      ASN1T_Mission_Pion_ABC_Appuyer *mission_pion_abc_appuyer;
      /* t = 43 */
      ASN1T_Mission_Pion_ABC_Fixer *mission_pion_abc_fixer;
      /* t = 44 */
      ASN1T_Mission_Pion_ABC_RenseignerSur *mission_pion_abc_renseigner_sur;
      /* t = 45 */
      ASN1T_Mission_Pion_ABC_ControlerZone *mission_pion_abc_controler_zone;
      /* t = 46 */
      ASN1T_Mission_Pion_ABC_Surveiller *mission_pion_abc_surveiller;
      /* t = 47 */
      ASN1T_Mission_Pion_ABC_Barrer *mission_pion_abc_barrer;
      /* t = 48 */
      ASN1T_Mission_Pion_ABC_Attaquer *mission_pion_abc_attaquer;
      /* t = 49 */
      ASN1T_Mission_Pion_ABC_ReconnaitrePoint *mission_pion_abc_reconnaitre_point;
      /* t = 50 */
      ASN1T_Mission_Pion_ABC_Eclairer *mission_pion_abc_eclairer;
      /* t = 51 */
      ASN1T_Mission_Pion_ABC_Jalonner *mission_pion_abc_jalonner;
      /* t = 52 */
      ASN1T_Mission_Pion_ABC_ContreAttaquer *mission_pion_abc_contre_attaquer;
      /* t = 53 */
      ASN1T_Mission_Pion_ABC_SemparerZone *mission_pion_abc_semparer_zone;
      /* t = 54 */
      ASN1T_Mission_Pion_ABC_ReleverUnite *mission_pion_abc_relever_unite;
      /* t = 55 */
      ASN1T_Mission_Pion_ABC_ArmerPIA *mission_pion_abc_armer_pia;
      /* t = 56 */
      ASN1T_Mission_Pion_ABC_OuvrirItineraire *mission_pion_abc_ouvrir_itineraire;
      /* t = 57 */
      ASN1T_Mission_Pion_ABC_Freiner *mission_pion_abc_freiner;
      /* t = 58 */
      ASN1T_Mission_Pion_ABC_EscorterUnConvoi *mission_pion_abc_escorter_un_convoi;
      /* t = 59 */
      ASN1T_Mission_Pion_ABC_ArmerPointDeControle *mission_pion_abc_armer_point_de_controle;
      /* t = 60 */
      ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies *mission_pion_abc_organiser_accueil_colonne_refugies;
      /* t = 61 */
      ASN1T_Mission_Pion_GEN_RealiserFosseAC *mission_pion_gen_realiser_fosse_ac;
      /* t = 62 */
      ASN1T_Mission_Pion_GEN_RealiserAbattisMine *mission_pion_gen_realiser_abattis_mine;
      /* t = 63 */
      ASN1T_Mission_Pion_GEN_RealiserPointMine *mission_pion_gen_realiser_point_mine;
      /* t = 64 */
      ASN1T_Mission_Pion_GEN_RealiserBarricade *mission_pion_gen_realiser_barricade;
      /* t = 65 */
      ASN1T_Mission_Pion_GEN_RealiserEboulement *mission_pion_gen_realiser_eboulement;
      /* t = 66 */
      ASN1T_Mission_Pion_GEN_RealiserBouchonMine *mission_pion_gen_realiser_bouchon_mine;
      /* t = 67 */
      ASN1T_Mission_Pion_GEN_DetruireRoute *mission_pion_gen_detruire_route;
      /* t = 68 */
      ASN1T_Mission_Pion_GEN_DetruirePont *mission_pion_gen_detruire_pont;
      /* t = 69 */
      ASN1T_Mission_Pion_GEN_SecuriserItineraire *mission_pion_gen_securiser_itineraire;
      /* t = 70 */
      ASN1T_Mission_Pion_GEN_DegagerAbattisMine *mission_pion_gen_degager_abattis_mine;
      /* t = 71 */
      ASN1T_Mission_Pion_GEN_DegagerEboulement *mission_pion_gen_degager_eboulement;
      /* t = 72 */
      ASN1T_Mission_Pion_GEN_DegagerRues *mission_pion_gen_degager_rues;
      /* t = 73 */
      ASN1T_Mission_Pion_GEN_ComblerEntonnoir *mission_pion_gen_combler_entonnoir;
      /* t = 74 */
      ASN1T_Mission_Pion_GEN_ComblerFosseAC *mission_pion_gen_combler_fosse_ac;
      /* t = 75 */
      ASN1T_Mission_Pion_GEN_CreerContournement *mission_pion_gen_creer_contournement;
      /* t = 76 */
      ASN1T_Mission_Pion_GEN_RealiserZoneMineeLineaire *mission_pion_gen_realiser_zone_minee_lineaire;
      /* t = 77 */
      ASN1T_Mission_Pion_GEN_OuvrirTrouee *mission_pion_gen_ouvrir_trouee;
      /* t = 78 */
      ASN1T_Mission_Pion_GEN_Renforcer *mission_pion_gen_renforcer;
      /* t = 79 */
      ASN1T_Mission_Pion_GEN_FranchirEntonnoir *mission_pion_gen_franchir_entonnoir;
      /* t = 80 */
      ASN1T_Mission_Pion_GEN_FranchirFosseAC *mission_pion_gen_franchir_fosse_ac;
      /* t = 81 */
      ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion *mission_pion_gen_realiser_zone_minee_par_dispersion;
      /* t = 82 */
      ASN1T_Mission_Pion_GEN_RealiserPostesTir *mission_pion_gen_realiser_postes_tir;
      /* t = 83 */
      ASN1T_Mission_Pion_GEN_RealiserTravauxProtection *mission_pion_gen_realiser_travaux_protection;
      /* t = 84 */
      ASN1T_Mission_Pion_GEN_RetablirItineraire *mission_pion_gen_retablir_itineraire;
      /* t = 85 */
      ASN1T_Mission_Pion_GEN_EquiperExploiter *mission_pion_gen_equiper_exploiter;
      /* t = 86 */
      ASN1T_Mission_Pion_GEN_ExecuterVariantement *mission_pion_gen_executer_variantement;
      /* t = 87 */
      ASN1T_Mission_Pion_GEN_ReconnaitreSiteFranchissement *mission_pion_gen_reconnaitre_site_franchissement;
      /* t = 88 */
      ASN1T_Mission_Pion_GEN_ReconnaitreItineraire *mission_pion_gen_reconnaitre_itineraire;
      /* t = 89 */
      ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement *mission_pion_gen_deminer_site_franchissement;
      /* t = 90 */
      ASN1T_Mission_Pion_GEN_ReconnaitreZone *mission_pion_gen_reconnaitre_zone;
      /* t = 91 */
      ASN1T_Mission_Pion_GEN_CreerPlateForme *mission_pion_gen_creer_plate_forme;
      /* t = 92 */
      ASN1T_Mission_Pion_GEN_CreerPiste *mission_pion_gen_creer_piste;
      /* t = 93 */
      ASN1T_Mission_Pion_GEN_CreerAirePoser *mission_pion_gen_creer_aire_poser;
      /* t = 94 */
      ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs *mission_pion_gen_neutraliser_engins_explosifs;
      /* t = 95 */
      ASN1T_Mission_Pion_GEN_ActiverObstacle *mission_pion_gen_activer_obstacle;
      /* t = 96 */
      ASN1T_Mission_Pion_GEN_AmeliorerMobilite *mission_pion_gen_ameliorer_mobilite;
      /* t = 97 */
      ASN1T_Mission_Pion_GEN_RealiserAireLogistique *mission_pion_gen_realiser_aire_logistique;
      /* t = 98 */
      ASN1T_Mission_Pion_GEN_DepolluerZone *mission_pion_gen_depolluer_zone;
      /* t = 99 */
      ASN1T_Mission_Pion_GEN_AmenagerTerrainPoser *mission_pion_gen_amenager_terrain_poser;
      /* t = 100 */
      ASN1T_Mission_Pion_GEN_RealiserPlateFormeDeploiement *mission_pion_gen_realiser_plate_forme_deploiement;
      /* t = 101 */
      ASN1T_Mission_Pion_GEN_ReprendreMissionConstruction *mission_pion_gen_reprendre_mission_construction;
      /* t = 102 */
      ASN1T_Mission_Pion_GEN_RealiserZonePoserHelicoptere *mission_pion_gen_realiser_zone_poser_helicoptere;
      /* t = 103 */
      ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU *mission_pion_gen_appuyer_debarquement_troupes_gu;
      /* t = 104 */
      ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers *mission_pion_gen_realiser_camp_prisonniers;
      /* t = 105 */
      ASN1T_Mission_Pion_GEN_RealiserCampRefugies *mission_pion_gen_realiser_camp_refugies;
      /* t = 106 */
      ASN1T_Mission_Pion_INF_ReconnaitreAxe *mission_pion_inf_reconnaitre_axe;
      /* t = 107 */
      ASN1T_Mission_Pion_INF_Couvrir *mission_pion_inf_couvrir;
      /* t = 108 */
      ASN1T_Mission_Pion_INF_Reduire *mission_pion_inf_reduire;
      /* t = 109 */
      ASN1T_Mission_Pion_INF_Appuyer *mission_pion_inf_appuyer;
      /* t = 110 */
      ASN1T_Mission_Pion_INF_Fixer *mission_pion_inf_fixer;
      /* t = 111 */
      ASN1T_Mission_Pion_INF_ControlerSecteur *mission_pion_inf_controler_secteur;
      /* t = 112 */
      ASN1T_Mission_Pion_INF_SurveillerSecteur *mission_pion_inf_surveiller_secteur;
      /* t = 113 */
      ASN1T_Mission_Pion_INF_Barrer *mission_pion_inf_barrer;
      /* t = 114 */
      ASN1T_Mission_Pion_INF_ReconnaitrePoint *mission_pion_inf_reconnaitre_point;
      /* t = 115 */
      ASN1T_Mission_Pion_INF_Eclairer *mission_pion_inf_eclairer;
      /* t = 116 */
      ASN1T_Mission_Pion_INF_Interdire *mission_pion_inf_interdire;
      /* t = 117 */
      ASN1T_Mission_Pion_INF_Semparer *mission_pion_inf_semparer;
      /* t = 118 */
      ASN1T_Mission_Pion_INF_PrendrePreciserContact *mission_pion_inf_prendre_preciser_contact;
      /* t = 119 */
      ASN1T_Mission_Pion_INF_Defendre *mission_pion_inf_defendre;
      /* t = 120 */
      ASN1T_Mission_Pion_INF_DetruireEmbuscade *mission_pion_inf_detruire_embuscade;
      /* t = 121 */
      ASN1T_Mission_Pion_INF_ContreAttaquer *mission_pion_inf_contre_attaquer;
      /* t = 122 */
      ASN1T_Mission_Pion_INF_Freiner *mission_pion_inf_freiner;
      /* t = 123 */
      ASN1T_Mission_Pion_INF_FlancGarder *mission_pion_inf_flanc_garder;
      /* t = 124 */
      ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint *mission_pion_inf_organiser_un_check_point;
      /* t = 125 */
      ASN1T_Mission_Pion_INF_Harceler *mission_pion_inf_harceler;
      /* t = 126 */
      ASN1T_Mission_Pion_INF_DetruireParCoupDeMain *mission_pion_inf_detruire_par_coup_de_main;
      /* t = 127 */
      ASN1T_Mission_Pion_INF_EscorterUnConvoi *mission_pion_inf_escorter_un_convoi;
      /* t = 128 */
      ASN1T_Mission_Pion_INF_RenseignerSur *mission_pion_inf_renseigner_sur;
      /* t = 129 */
      ASN1T_Mission_Pion_INF_Soutenir *mission_pion_inf_soutenir;
      /* t = 130 */
      ASN1T_Mission_Pion_INF_Securiser *mission_pion_inf_securiser;
      /* t = 131 */
      ASN1T_Mission_Pion_INF_Recueillir *mission_pion_inf_recueillir;
      /* t = 132 */
      ASN1T_Mission_Pion_INF_RepondreAAggression *mission_pion_inf_repondre_a_aggression;
      /* t = 133 */
      ASN1T_Mission_Pion_LOG_SeDeployer *mission_pion_log_se_deployer;
      /* t = 134 */
      ASN1T_Mission_Pion_LOG_DistribuerMoyens *mission_pion_log_distribuer_moyens;
      /* t = 135 */
      ASN1T_Mission_Pion_LOG_TrierBlesses *mission_pion_log_trier_blesses;
      /* t = 136 */
      ASN1T_Mission_Pion_LOG_TraiterBlesses *mission_pion_log_traiter_blesses;
      /* t = 137 */
      ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses *mission_pion_log_trier_et_traiter_blesses;
      /* t = 138 */
      ASN1T_Mission_Pion_LOG_SoignerBlesses *mission_pion_log_soigner_blesses;
      /* t = 139 */
      ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales *mission_pion_log_prendre_en_compte_reactions_mentales;
      /* t = 140 */
      ASN1T_Mission_Pion_LOG_DecontaminerBlesses *mission_pion_log_decontaminer_blesses;
      /* t = 141 */
      ASN1T_Mission_Pion_LOG_Reparer *mission_pion_log_reparer;
      /* t = 142 */
      ASN1T_Mission_Pion_LOG_Evacuer *mission_pion_log_evacuer;
      /* t = 143 */
      ASN1T_Mission_Pion_LOG_Convoyer *mission_pion_log_convoyer;
      /* t = 144 */
      ASN1T_Mission_Pion_LOG_DistribuerRessources *mission_pion_log_distribuer_ressources;
      /* t = 145 */
      ASN1T_Mission_Pion_LOG_ConstituerConvoi *mission_pion_log_constituer_convoi;
      /* t = 146 */
      ASN1T_Mission_Pion_LOG_Livrer *mission_pion_log_livrer;
      /* t = 147 */
      ASN1T_Mission_Pion_LOG_PreparerBascule *mission_pion_log_preparer_bascule;
      /* t = 148 */
      ASN1T_Mission_Pion_LOG_AppuyerMouvement *mission_pion_log_appuyer_mouvement;
      /* t = 149 */
      ASN1T_Mission_Pion_LOG_ReconnaitreItineraire *mission_pion_log_reconnaitre_itineraire;
      /* t = 150 */
      ASN1T_Mission_Pion_LOG_TransporterUniteChars *mission_pion_log_transporter_unite_chars;
      /* t = 151 */
      ASN1T_Mission_Pion_LOG_ReconnaitreZoneContaminee *mission_pion_log_reconnaitre_zone_contaminee;
      /* t = 152 */
      ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone *mission_pion_log_appuyer_mouvement_dans_zone;
      /* t = 153 */
      ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement *mission_pion_log_mettre_en_oeuvre_zone_stationnement;
      /* t = 154 */
      ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint *mission_pion_log_appuyer_franchissement_sur_un_point;
      /* t = 155 */
      ASN1T_Mission_Pion_LOG_BloquerZone *mission_pion_log_bloquer_zone;
      /* t = 156 */
      ASN1T_Mission_Pion_LOG_ReconnaitreZoneDeDeploiement *mission_pion_log_reconnaitre_zone_de_deploiement;
      /* t = 157 */
      ASN1T_Mission_Pion_LOG_ArmerPIA *mission_pion_log_armer_pia;
      /* t = 158 */
      ASN1T_Mission_Pion_LOG_Surveiller *mission_pion_log_surveiller;
      /* t = 159 */
      ASN1T_Mission_Pion_Test_Heliporter *mission_pion_test_heliporter;
      /* t = 160 */
      ASN1T_Mission_Pion_Test_MoveTo *mission_pion_test_move_to;
      /* t = 161 */
      ASN1T_Mission_Pion_Test_CreateObject *mission_pion_test_create_object;
      /* t = 162 */
      ASN1T_Mission_Pion_Test_DestroyObject *mission_pion_test_destroy_object;
      /* t = 163 */
      ASN1T_Mission_Pion_Test_CreateBypass *mission_pion_test_create_bypass;
      /* t = 164 */
      ASN1T_Mission_Pion_Test_Fire *mission_pion_test_fire;
      /* t = 165 */
      ASN1T_Mission_Pion_Test_ChangePosture *mission_pion_test_change_posture;
      /* t = 166 */
      ASN1T_Mission_Pion_Test_PathFind *mission_pion_test_path_find;
      /* t = 167 */
      ASN1T_Mission_Pion_Test_FindPosition *mission_pion_test_find_position;
      /* t = 168 */
      ASN1T_Mission_Pion_Test_Reinforce *mission_pion_test_reinforce;
      /* t = 169 */
      ASN1T_Mission_Pion_PlastronEnnemi *mission_pion_plastron_ennemi;
      /* t = 170 */
      ASN1T_Mission_Pion_FaireMouvement *mission_pion_faire_mouvement;
      /* t = 171 */
      ASN1T_Mission_Pion_Suivre *mission_pion_suivre;
      /* t = 172 */
      ASN1T_Mission_Pion_SeFaireDecontaminer *mission_pion_se_faire_decontaminer;
      /* t = 173 */
      ASN1T_Mission_Pion_Franchir *mission_pion_franchir;
      /* t = 174 */
      ASN1T_Mission_Pion_Decrocher *mission_pion_decrocher;
      /* t = 175 */
      ASN1T_Mission_Pion_Stationner *mission_pion_stationner;
      /* t = 176 */
      ASN1T_Mission_Pion_SeFaireTransporter *mission_pion_se_faire_transporter;
      /* t = 177 */
      ASN1T_Mission_Pion_Generique *mission_pion_generique;
      /* t = 178 */
      ASN1T_Mission_Pion_MILICE_Provoquer *mission_pion_milice_provoquer;
      /* t = 179 */
      ASN1T_Mission_Pion_MILICE_PrendreEtTenir *mission_pion_milice_prendre_et_tenir;
      /* t = 180 */
      ASN1T_Mission_Pion_MILICE_DetruireEmbuscade *mission_pion_milice_detruire_embuscade;
      /* t = 181 */
      ASN1T_Mission_Pion_JOINT_MARINE_Transporter *mission_pion_joint_marine_transporter;
      /* t = 182 */
      ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid *mission_pion_joint_air_effectuer_raid;
      /* t = 183 */
      ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination *mission_pion_nbc_animer_un_plot_de_decontamination;
      /* t = 184 */
      ASN1T_Mission_Pion_NBC_ReconnaitreItineraire *mission_pion_nbc_reconnaitre_itineraire;
      /* t = 185 */
      ASN1T_Mission_Pion_NBC_ReconnaitreZone *mission_pion_nbc_reconnaitre_zone;
      /* t = 186 */
      ASN1T_Mission_Pion_NBC_DecontaminerZone *mission_pion_nbc_decontaminer_zone;
      /* t = 187 */
      ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA *mission_pion_nbc_reconnaitre_un_site_rota;
      /* t = 188 */
      ASN1T_Mission_Pion_RENS_ROHUM_RenseignerSur *mission_pion_rens_rohum_renseigner_sur;
      /* t = 189 */
      ASN1T_Mission_Pion_RENS_ROHUM_SExfiltrer *mission_pion_rens_rohum_sexfiltrer;
      /* t = 190 */
      ASN1T_Mission_Pion_RENS_ROHUM_OrienterGuider *mission_pion_rens_rohum_orienter_guider;
      /* t = 191 */
      ASN1T_Mission_Pion_RENS_ROEM_Appuyer *mission_pion_rens_roem_appuyer;
      /* t = 192 */
      ASN1T_Mission_Pion_RENS_ROEM_RenseignerSur *mission_pion_rens_roem_renseigner_sur;
      /* t = 193 */
      ASN1T_Mission_Pion_RENS_ROEM_MettreEnOeuvre *mission_pion_rens_roem_mettre_en_oeuvre;
      /* t = 194 */
      ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur *mission_pion_rens_radint_renseigner_sur;
      /* t = 195 */
      ASN1T_Mission_Pion_RENS_RADINT_MettreEnOeuvre *mission_pion_rens_radint_mettre_en_oeuvre;
      /* t = 196 */
      ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre *mission_pion_rens_roim_mettre_en_oeuvre;
      /* t = 197 */
      ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur *mission_pion_rens_roim_sdti_renseigner_sur;
      /* t = 198 */
      ASN1T_Mission_Pion_RENS_ROIM_CL289_RenseignerSur *mission_pion_rens_roim_cl289_renseigner_sur;
      /* t = 199 */
      ASN1T_Mission_Pion_TRANS_EtablirGererLiaison *mission_pion_trans_etablir_gerer_liaison;
   } u;
} ASN1T_Mission_Pion;

class EXTERN ASN1C_Mission_Pion : public ASN1CType {
public:
   ASN1T_Mission_Pion& msgData;
   ASN1C_Mission_Pion (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Pion& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Pion (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion* pvalue);
EXTERN int asn1PD_Mission_Pion (ASN1CTXT* ctxt_p, ASN1T_Mission_Pion* pvalue);

EXTERN void asn1Print_Mission_Pion (ASN1ConstCharPtr name, ASN1T_Mission_Pion* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPionOrder                                              */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPionOrder {
   struct {
      unsigned oid_limite_gauchePresent : 1;
      unsigned oid_limite_droitePresent : 1;
   } m;
   ASN1T_Agent  oid_unite_executante;
   ASN1T_OID  order_id;
   ASN1T_OID  oid_limite_gauche;
   ASN1T_OID  oid_limite_droite;
   ASN1T_ListOID  oid_limas;
   ASN1T_Direction  direction_dangereuse;
   ASN1T_Mission_Pion  mission;

   ASN1T_MsgPionOrder () {
      m.oid_limite_gauchePresent = 0;
      m.oid_limite_droitePresent = 0;
   }
} ASN1T_MsgPionOrder;

class EXTERN ASN1C_MsgPionOrder : public ASN1CType {
public:
   ASN1T_MsgPionOrder& msgData;
   ASN1C_MsgPionOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPionOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPionOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrder* pvalue);
EXTERN int asn1PD_MsgPionOrder (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrder* pvalue);

EXTERN void asn1Print_MsgPionOrder (ASN1ConstCharPtr name, ASN1T_MsgPionOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgPionOrderAck                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgPionOrderAck {
   ASN1T_Agent  oid_unite_executante;
   ASN1T_OID  order_id;
   ASN1T_EnumOrderErrorCode  error_code;
} ASN1T_MsgPionOrderAck;

class EXTERN ASN1C_MsgPionOrderAck : public ASN1CType {
public:
   ASN1T_MsgPionOrderAck& msgData;
   ASN1C_MsgPionOrderAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgPionOrderAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgPionOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrderAck* pvalue);
EXTERN int asn1PD_MsgPionOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgPionOrderAck* pvalue);

EXTERN void asn1Print_MsgPionOrderAck (ASN1ConstCharPtr name, ASN1T_MsgPionOrderAck* pvalue);

#endif
