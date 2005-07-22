/**
 * This file was generated by the Objective Systems ASN1C Compiler
 * (http://www.obj-sys.com).  Version: 5.31, Date: 20-Jul-2005.
 */
#ifndef SCIPIOAUTOMATEORDER_H
#define SCIPIOAUTOMATEORDER_H

#include <stdio.h>
#include <stdlib.h>
#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "ScipioBase.h"
#include "ScipioOrder.h"
#include "ScipioPionOrder.h"
#include "ScipioAutomateMission_ALAT.h"
#include "ScipioAutomateMission_ASA.h"
#include "ScipioAutomateMission_ASS.h"
#include "ScipioAutomateMission_ABC.h"
#include "ScipioAutomateMission_GEN.h"
#include "ScipioAutomateMission_INF.h"
#include "ScipioAutomateMission_LOG.h"
#include "ScipioAutomateMission_Misc.h"
#include "ScipioAutomateMission_NBC.h"
#include "ScipioAutomateMission_RENS.h"
#include "ScipioAutomateMission_TRANS.h"

/**************************************************************/
/*                                                            */
/*  Mission_Automate                                          */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_Mission_Automate_mission_automate_alat_effectuer_recherche_et_sauvetage 1
#define T_Mission_Automate_mission_automate_alat_extraire_personnel 2
#define T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_profondeur 3
#define T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_zone 4
#define T_Mission_Automate_mission_automate_alat_donner_coup_arret 5
#define T_Mission_Automate_mission_automate_alat_attaquer_formation_aeromobile 6
#define T_Mission_Automate_mission_automate_alat_recueillir_formation_aeromobile 7
#define T_Mission_Automate_mission_automate_alat_heliporter 8
#define T_Mission_Automate_mission_automate_alat_jalonner 9
#define T_Mission_Automate_mission_automate_alat_helitransporter 10
#define T_Mission_Automate_mission_automate_alat_appuyer 11
#define T_Mission_Automate_mission_automate_alat_couvrir 12
#define T_Mission_Automate_mission_automate_alat_surveiller 13
#define T_Mission_Automate_mission_automate_alat_reconnaitre 14
#define T_Mission_Automate_mission_automate_alat_freiner 15
#define T_Mission_Automate_mission_automate_alat_escorter 16
#define T_Mission_Automate_mission_automate_alat_attaquer 17
#define T_Mission_Automate_mission_automate_asa_defendre_site 18
#define T_Mission_Automate_mission_automate_asa_defendre_zone 19
#define T_Mission_Automate_mission_automate_asa_defendre_unites 20
#define T_Mission_Automate_mission_automate_asa_surveiller 21
#define T_Mission_Automate_mission_automate_asa_defendre_itineraire 22
#define T_Mission_Automate_mission_automate_asa_mistral_defendre_zone 23
#define T_Mission_Automate_mission_automate_asa_mistral_defendre_site 24
#define T_Mission_Automate_mission_automate_asa_mistral_surveiller 25
#define T_Mission_Automate_mission_automate_asa_hawk_defendre_zone 26
#define T_Mission_Automate_mission_automate_asa_hawk_defendre_site 27
#define T_Mission_Automate_mission_automate_asa_hawk_surveiller 28
#define T_Mission_Automate_mission_automate_ass_se_deployer 29
#define T_Mission_Automate_mission_automate_ass_reconnaitre_zones_deploiement 30
#define T_Mission_Automate_mission_automate_ass_acquerir_objectifs 31
#define T_Mission_Automate_mission_automate_abc_appuyer 32
#define T_Mission_Automate_mission_automate_abc_attaquer 33
#define T_Mission_Automate_mission_automate_abc_contre_attaquer 34
#define T_Mission_Automate_mission_automate_abc_controler 35
#define T_Mission_Automate_mission_automate_abc_couvrir 36
#define T_Mission_Automate_mission_automate_abc_donner_coup_arret 37
#define T_Mission_Automate_mission_automate_abc_eclairer 38
#define T_Mission_Automate_mission_automate_abc_effectuer_raid 39
#define T_Mission_Automate_mission_automate_abc_interdire 40
#define T_Mission_Automate_mission_automate_abc_jalonner 41
#define T_Mission_Automate_mission_automate_abc_prendre_preciser_contact 42
#define T_Mission_Automate_mission_automate_abc_reconnaitre 43
#define T_Mission_Automate_mission_automate_abc_reconnaitre_point 44
#define T_Mission_Automate_mission_automate_abc_reduire 45
#define T_Mission_Automate_mission_automate_abc_relever_en_depassant 46
#define T_Mission_Automate_mission_automate_abc_semparer 47
#define T_Mission_Automate_mission_automate_abc_tenir 48
#define T_Mission_Automate_mission_automate_abc_soutenir 49
#define T_Mission_Automate_mission_automate_abc_surveiller 50
#define T_Mission_Automate_mission_automate_abc_flanc_garder 51
#define T_Mission_Automate_mission_automate_abc_renseigner_sur 52
#define T_Mission_Automate_mission_automate_abc_fixer 53
#define T_Mission_Automate_mission_automate_abc_freiner 54
#define T_Mission_Automate_mission_automate_abc_surveiller_itineraire 55
#define T_Mission_Automate_mission_automate_abc_renseigner_sur_un_axe 56
#define T_Mission_Automate_mission_automate_abc_recueillir_unite 57
#define T_Mission_Automate_mission_automate_abc_escorter_un_convoi 58
#define T_Mission_Automate_mission_automate_abc_armer_point_de_controle 59
#define T_Mission_Automate_mission_automate_abc_organiser_accueil_colonne_refugies 60
#define T_Mission_Automate_mission_automate_abc_proteger_pc 61
#define T_Mission_Automate_mission_automate_gen_se_deployer 62
#define T_Mission_Automate_mission_automate_gen_faire_franchir 63
#define T_Mission_Automate_mission_automate_gen_realiser_systeme_obstacles 64
#define T_Mission_Automate_mission_automate_gen_franchir_de_vive_force 65
#define T_Mission_Automate_mission_automate_gen_retablir_itineraires 66
#define T_Mission_Automate_mission_automate_gen_amenager_aire_stationnement 67
#define T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire 68
#define T_Mission_Automate_mission_automate_gen_realiser_zone_poser 69
#define T_Mission_Automate_mission_automate_gen_reconnaitre_zone_deploiement 70
#define T_Mission_Automate_mission_automate_gen_realiser_camp_prisonniers 71
#define T_Mission_Automate_mission_automate_gen_realiser_camp_refugies 72
#define T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial 73
#define T_Mission_Automate_mission_automate_gen_realiser_travaux_sommaires_au_profit_population 74
#define T_Mission_Automate_mission_automate_gen_appuyer_debarquement_troupes 75
#define T_Mission_Automate_mission_automate_gen_ouvrir_axes_progression 76
#define T_Mission_Automate_mission_automate_inf_attaquer_souplesse 77
#define T_Mission_Automate_mission_automate_inf_appuyer 78
#define T_Mission_Automate_mission_automate_inf_controler 79
#define T_Mission_Automate_mission_automate_inf_couvrir 80
#define T_Mission_Automate_mission_automate_inf_donner_coup_arret 81
#define T_Mission_Automate_mission_automate_inf_eclairer 82
#define T_Mission_Automate_mission_automate_inf_flanc_garder 83
#define T_Mission_Automate_mission_automate_inf_freiner 84
#define T_Mission_Automate_mission_automate_inf_harceler 85
#define T_Mission_Automate_mission_automate_inf_interdire 86
#define T_Mission_Automate_mission_automate_inf_mener_defense_usure 87
#define T_Mission_Automate_mission_automate_inf_prendre_preciser_maintenir_contact 88
#define T_Mission_Automate_mission_automate_inf_reconnaissance_offensive 89
#define T_Mission_Automate_mission_automate_inf_recueillir_unite 90
#define T_Mission_Automate_mission_automate_inf_reduire 91
#define T_Mission_Automate_mission_automate_inf_relever 92
#define T_Mission_Automate_mission_automate_inf_rompre_contact 93
#define T_Mission_Automate_mission_automate_inf_surveiller 94
#define T_Mission_Automate_mission_automate_inf_semparer 95
#define T_Mission_Automate_mission_automate_inf_fixer 96
#define T_Mission_Automate_mission_automate_inf_attaquer_en_terrain_difficile 97
#define T_Mission_Automate_mission_automate_inf_barrer_direction 98
#define T_Mission_Automate_mission_automate_inf_defendre_ferme 99
#define T_Mission_Automate_mission_automate_inf_tenir 100
#define T_Mission_Automate_mission_automate_inf_appuyer_un_freinage 101
#define T_Mission_Automate_mission_automate_inf_appuyer_une_attaque 102
#define T_Mission_Automate_mission_automate_inf_soutenir 103
#define T_Mission_Automate_mission_automate_log_se_deployer 104
#define T_Mission_Automate_mission_automate_log_appuyer_mouvement 105
#define T_Mission_Automate_mission_automate_log_reconnaitre_itineraire 106
#define T_Mission_Automate_mission_automate_log_transporter_formation_blindee 107
#define T_Mission_Automate_mission_automate_log_appuyer_mouvement_dans_zone 108
#define T_Mission_Automate_mission_automate_log_mettre_en_oeuvre_zone_stationnement 109
#define T_Mission_Automate_mission_automate_log_appuyer_franchissement_dans_zone 110
#define T_Mission_Automate_mission_automate_log_bloquer_zone 111
#define T_Mission_Automate_mission_automate_log_reconnaitre_zone_contaminee 112
#define T_Mission_Automate_mission_automate_log_reconnaitre_zone_de_deploiement 113
#define T_Mission_Automate_mission_automate_log_surveiller 114
#define T_Mission_Automate_mission_automate_log_armer_pias 115
#define T_Mission_Automate_mission_automate_test_move_to 116
#define T_Mission_Automate_mission_automate_decrocher 117
#define T_Mission_Automate_mission_automate_relever_sur_place 118
#define T_Mission_Automate_mission_automate_faire_mouvement 119
#define T_Mission_Automate_mission_automate_se_faire_decontaminer 120
#define T_Mission_Automate_mission_automate_franchir 121
#define T_Mission_Automate_mission_automate_stationner 122
#define T_Mission_Automate_mission_automate_se_faire_transporter 123
#define T_Mission_Automate_mission_automate_se_recompleter 124
#define T_Mission_Automate_mission_automate_se_rendre 125
#define T_Mission_Automate_mission_automate_faire_mouvement_vers_camp_refugies 126
#define T_Mission_Automate_mission_automate_generique 127
#define T_Mission_Automate_mission_automate_nbc_reconnaitre_un_axe 128
#define T_Mission_Automate_mission_automate_nbc_reconnaitre_une_zone 129
#define T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination 130
#define T_Mission_Automate_mission_automate_nbc_decontaminer_une_zone 131
#define T_Mission_Automate_mission_automate_nbc_reconnaitre_des_sites_rota 132
#define T_Mission_Automate_mission_automate_rens_rohum_renseigner_sur 133
#define T_Mission_Automate_mission_automate_rens_rohum_orienter_guider 134
#define T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer 135
#define T_Mission_Automate_mission_automate_rens_roim_cl289_renseigner_sur 136
#define T_Mission_Automate_mission_automate_rens_roim_sdti_renseigner_sur 137
#define T_Mission_Automate_mission_automate_rens_roem_appuyer 138
#define T_Mission_Automate_mission_automate_rens_roem_renseignersur 139
#define T_Mission_Automate_mission_automate_rens_radint_renseignersur 140
#define T_Mission_Automate_mission_automate_trans_commander 141

typedef struct EXTERN ASN1T_Mission_Automate {
   int t;
   union {
      /* t = 1 */
      ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage *mission_automate_alat_effectuer_recherche_et_sauvetage;
      /* t = 2 */
      ASN1T_Mission_Automate_ALAT_ExtrairePersonnel *mission_automate_alat_extraire_personnel;
      /* t = 3 */
      ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur *mission_automate_alat_detruire_neutraliser_dans_profondeur;
      /* t = 4 */
      ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansZone *mission_automate_alat_detruire_neutraliser_dans_zone;
      /* t = 5 */
      ASN1T_Mission_Automate_ALAT_DonnerCoupArret *mission_automate_alat_donner_coup_arret;
      /* t = 6 */
      ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile *mission_automate_alat_attaquer_formation_aeromobile;
      /* t = 7 */
      ASN1T_Mission_Automate_ALAT_RecueillirFormationAeromobile *mission_automate_alat_recueillir_formation_aeromobile;
      /* t = 8 */
      ASN1T_Mission_Automate_ALAT_Heliporter *mission_automate_alat_heliporter;
      /* t = 9 */
      ASN1T_Mission_Automate_ALAT_Jalonner *mission_automate_alat_jalonner;
      /* t = 10 */
      ASN1T_Mission_Automate_ALAT_Helitransporter *mission_automate_alat_helitransporter;
      /* t = 11 */
      ASN1T_Mission_Automate_ALAT_Appuyer *mission_automate_alat_appuyer;
      /* t = 12 */
      ASN1T_Mission_Automate_ALAT_Couvrir *mission_automate_alat_couvrir;
      /* t = 13 */
      ASN1T_Mission_Automate_ALAT_Surveiller *mission_automate_alat_surveiller;
      /* t = 14 */
      ASN1T_Mission_Automate_ALAT_Reconnaitre *mission_automate_alat_reconnaitre;
      /* t = 15 */
      ASN1T_Mission_Automate_ALAT_Freiner *mission_automate_alat_freiner;
      /* t = 16 */
      ASN1T_Mission_Automate_ALAT_Escorter *mission_automate_alat_escorter;
      /* t = 17 */
      ASN1T_Mission_Automate_ALAT_Attaquer *mission_automate_alat_attaquer;
      /* t = 18 */
      ASN1T_Mission_Automate_ASA_DefendreSite *mission_automate_asa_defendre_site;
      /* t = 19 */
      ASN1T_Mission_Automate_ASA_DefendreZone *mission_automate_asa_defendre_zone;
      /* t = 20 */
      ASN1T_Mission_Automate_ASA_DefendreUnites *mission_automate_asa_defendre_unites;
      /* t = 21 */
      ASN1T_Mission_Automate_ASA_Surveiller *mission_automate_asa_surveiller;
      /* t = 22 */
      ASN1T_Mission_Automate_ASA_DefendreItineraire *mission_automate_asa_defendre_itineraire;
      /* t = 23 */
      ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone *mission_automate_asa_mistral_defendre_zone;
      /* t = 24 */
      ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite *mission_automate_asa_mistral_defendre_site;
      /* t = 25 */
      ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller *mission_automate_asa_mistral_surveiller;
      /* t = 26 */
      ASN1T_Mission_Automate_ASA_HAWK_DefendreZone *mission_automate_asa_hawk_defendre_zone;
      /* t = 27 */
      ASN1T_Mission_Automate_ASA_HAWK_DefendreSite *mission_automate_asa_hawk_defendre_site;
      /* t = 28 */
      ASN1T_Mission_Automate_ASA_HAWK_Surveiller *mission_automate_asa_hawk_surveiller;
      /* t = 29 */
      ASN1T_Mission_Automate_ASS_SeDeployer *mission_automate_ass_se_deployer;
      /* t = 30 */
      ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement *mission_automate_ass_reconnaitre_zones_deploiement;
      /* t = 31 */
      ASN1T_Mission_Automate_ASS_AcquerirObjectifs *mission_automate_ass_acquerir_objectifs;
      /* t = 32 */
      ASN1T_Mission_Automate_ABC_Appuyer *mission_automate_abc_appuyer;
      /* t = 33 */
      ASN1T_Mission_Automate_ABC_Attaquer *mission_automate_abc_attaquer;
      /* t = 34 */
      ASN1T_Mission_Automate_ABC_ContreAttaquer *mission_automate_abc_contre_attaquer;
      /* t = 35 */
      ASN1T_Mission_Automate_ABC_Controler *mission_automate_abc_controler;
      /* t = 36 */
      ASN1T_Mission_Automate_ABC_Couvrir *mission_automate_abc_couvrir;
      /* t = 37 */
      ASN1T_Mission_Automate_ABC_DonnerCoupArret *mission_automate_abc_donner_coup_arret;
      /* t = 38 */
      ASN1T_Mission_Automate_ABC_Eclairer *mission_automate_abc_eclairer;
      /* t = 39 */
      ASN1T_Mission_Automate_ABC_EffectuerRaid *mission_automate_abc_effectuer_raid;
      /* t = 40 */
      ASN1T_Mission_Automate_ABC_Interdire *mission_automate_abc_interdire;
      /* t = 41 */
      ASN1T_Mission_Automate_ABC_Jalonner *mission_automate_abc_jalonner;
      /* t = 42 */
      ASN1T_Mission_Automate_ABC_PrendrePreciserContact *mission_automate_abc_prendre_preciser_contact;
      /* t = 43 */
      ASN1T_Mission_Automate_ABC_Reconnaitre *mission_automate_abc_reconnaitre;
      /* t = 44 */
      ASN1T_Mission_Automate_ABC_ReconnaitrePoint *mission_automate_abc_reconnaitre_point;
      /* t = 45 */
      ASN1T_Mission_Automate_ABC_Reduire *mission_automate_abc_reduire;
      /* t = 46 */
      ASN1T_Mission_Automate_ABC_ReleverEnDepassant *mission_automate_abc_relever_en_depassant;
      /* t = 47 */
      ASN1T_Mission_Automate_ABC_Semparer *mission_automate_abc_semparer;
      /* t = 48 */
      ASN1T_Mission_Automate_ABC_Tenir *mission_automate_abc_tenir;
      /* t = 49 */
      ASN1T_Mission_Automate_ABC_Soutenir *mission_automate_abc_soutenir;
      /* t = 50 */
      ASN1T_Mission_Automate_ABC_Surveiller *mission_automate_abc_surveiller;
      /* t = 51 */
      ASN1T_Mission_Automate_ABC_FlancGarder *mission_automate_abc_flanc_garder;
      /* t = 52 */
      ASN1T_Mission_Automate_ABC_RenseignerSur *mission_automate_abc_renseigner_sur;
      /* t = 53 */
      ASN1T_Mission_Automate_ABC_Fixer *mission_automate_abc_fixer;
      /* t = 54 */
      ASN1T_Mission_Automate_ABC_Freiner *mission_automate_abc_freiner;
      /* t = 55 */
      ASN1T_Mission_Automate_ABC_SurveillerItineraire *mission_automate_abc_surveiller_itineraire;
      /* t = 56 */
      ASN1T_Mission_Automate_ABC_RenseignerSurUnAxe *mission_automate_abc_renseigner_sur_un_axe;
      /* t = 57 */
      ASN1T_Mission_Automate_ABC_RecueillirUnite *mission_automate_abc_recueillir_unite;
      /* t = 58 */
      ASN1T_Mission_Automate_ABC_EscorterUnConvoi *mission_automate_abc_escorter_un_convoi;
      /* t = 59 */
      ASN1T_Mission_Automate_ABC_ArmerPointDeControle *mission_automate_abc_armer_point_de_controle;
      /* t = 60 */
      ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies *mission_automate_abc_organiser_accueil_colonne_refugies;
      /* t = 61 */
      ASN1T_Mission_Automate_ABC_ProtegerPC *mission_automate_abc_proteger_pc;
      /* t = 62 */
      ASN1T_Mission_Automate_GEN_SeDeployer *mission_automate_gen_se_deployer;
      /* t = 63 */
      ASN1T_Mission_Automate_GEN_FaireFranchir *mission_automate_gen_faire_franchir;
      /* t = 64 */
      ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles *mission_automate_gen_realiser_systeme_obstacles;
      /* t = 65 */
      ASN1T_Mission_Automate_GEN_FranchirDeViveForce *mission_automate_gen_franchir_de_vive_force;
      /* t = 66 */
      ASN1T_Mission_Automate_GEN_RetablirItineraires *mission_automate_gen_retablir_itineraires;
      /* t = 67 */
      ASN1T_Mission_Automate_GEN_AmenagerAireStationnement *mission_automate_gen_amenager_aire_stationnement;
      /* t = 68 */
      ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire *mission_automate_gen_amenager_terrain_poser_sommaire;
      /* t = 69 */
      ASN1T_Mission_Automate_GEN_RealiserZonePoser *mission_automate_gen_realiser_zone_poser;
      /* t = 70 */
      ASN1T_Mission_Automate_GEN_ReconnaitreZoneDeploiement *mission_automate_gen_reconnaitre_zone_deploiement;
      /* t = 71 */
      ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers *mission_automate_gen_realiser_camp_prisonniers;
      /* t = 72 */
      ASN1T_Mission_Automate_GEN_RealiserCampRefugies *mission_automate_gen_realiser_camp_refugies;
      /* t = 73 */
      ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial *mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial;
      /* t = 74 */
      ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation *mission_automate_gen_realiser_travaux_sommaires_au_profit_population;
      /* t = 75 */
      ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes *mission_automate_gen_appuyer_debarquement_troupes;
      /* t = 76 */
      ASN1T_Mission_Automate_GEN_OuvrirAxesProgression *mission_automate_gen_ouvrir_axes_progression;
      /* t = 77 */
      ASN1T_Mission_Automate_INF_AttaquerSouplesse *mission_automate_inf_attaquer_souplesse;
      /* t = 78 */
      ASN1T_Mission_Automate_INF_Appuyer *mission_automate_inf_appuyer;
      /* t = 79 */
      ASN1T_Mission_Automate_INF_Controler *mission_automate_inf_controler;
      /* t = 80 */
      ASN1T_Mission_Automate_INF_Couvrir *mission_automate_inf_couvrir;
      /* t = 81 */
      ASN1T_Mission_Automate_INF_DonnerCoupArret *mission_automate_inf_donner_coup_arret;
      /* t = 82 */
      ASN1T_Mission_Automate_INF_Eclairer *mission_automate_inf_eclairer;
      /* t = 83 */
      ASN1T_Mission_Automate_INF_FlancGarder *mission_automate_inf_flanc_garder;
      /* t = 84 */
      ASN1T_Mission_Automate_INF_Freiner *mission_automate_inf_freiner;
      /* t = 85 */
      ASN1T_Mission_Automate_INF_Harceler *mission_automate_inf_harceler;
      /* t = 86 */
      ASN1T_Mission_Automate_INF_Interdire *mission_automate_inf_interdire;
      /* t = 87 */
      ASN1T_Mission_Automate_INF_MenerDefenseUsure *mission_automate_inf_mener_defense_usure;
      /* t = 88 */
      ASN1T_Mission_Automate_INF_PrendrePreciserMaintenirContact *mission_automate_inf_prendre_preciser_maintenir_contact;
      /* t = 89 */
      ASN1T_Mission_Automate_INF_ReconnaissanceOffensive *mission_automate_inf_reconnaissance_offensive;
      /* t = 90 */
      ASN1T_Mission_Automate_INF_RecueillirUnite *mission_automate_inf_recueillir_unite;
      /* t = 91 */
      ASN1T_Mission_Automate_INF_Reduire *mission_automate_inf_reduire;
      /* t = 92 */
      ASN1T_Mission_Automate_INF_Relever *mission_automate_inf_relever;
      /* t = 93 */
      ASN1T_Mission_Automate_INF_RompreContact *mission_automate_inf_rompre_contact;
      /* t = 94 */
      ASN1T_Mission_Automate_INF_Surveiller *mission_automate_inf_surveiller;
      /* t = 95 */
      ASN1T_Mission_Automate_INF_Semparer *mission_automate_inf_semparer;
      /* t = 96 */
      ASN1T_Mission_Automate_INF_Fixer *mission_automate_inf_fixer;
      /* t = 97 */
      ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile *mission_automate_inf_attaquer_en_terrain_difficile;
      /* t = 98 */
      ASN1T_Mission_Automate_INF_BarrerDirection *mission_automate_inf_barrer_direction;
      /* t = 99 */
      ASN1T_Mission_Automate_INF_DefendreFerme *mission_automate_inf_defendre_ferme;
      /* t = 100 */
      ASN1T_Mission_Automate_INF_Tenir *mission_automate_inf_tenir;
      /* t = 101 */
      ASN1T_Mission_Automate_INF_AppuyerUnFreinage *mission_automate_inf_appuyer_un_freinage;
      /* t = 102 */
      ASN1T_Mission_Automate_INF_AppuyerUneAttaque *mission_automate_inf_appuyer_une_attaque;
      /* t = 103 */
      ASN1T_Mission_Automate_INF_Soutenir *mission_automate_inf_soutenir;
      /* t = 104 */
      ASN1T_Mission_Automate_LOG_SeDeployer *mission_automate_log_se_deployer;
      /* t = 105 */
      ASN1T_Mission_Automate_LOG_AppuyerMouvement *mission_automate_log_appuyer_mouvement;
      /* t = 106 */
      ASN1T_Mission_Automate_LOG_ReconnaitreItineraire *mission_automate_log_reconnaitre_itineraire;
      /* t = 107 */
      ASN1T_Mission_Automate_LOG_TransporterFormationBlindee *mission_automate_log_transporter_formation_blindee;
      /* t = 108 */
      ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone *mission_automate_log_appuyer_mouvement_dans_zone;
      /* t = 109 */
      ASN1T_Mission_Automate_LOG_MettreEnOeuvreZoneStationnement *mission_automate_log_mettre_en_oeuvre_zone_stationnement;
      /* t = 110 */
      ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone *mission_automate_log_appuyer_franchissement_dans_zone;
      /* t = 111 */
      ASN1T_Mission_Automate_LOG_BloquerZone *mission_automate_log_bloquer_zone;
      /* t = 112 */
      ASN1T_Mission_Automate_LOG_ReconnaitreZoneContaminee *mission_automate_log_reconnaitre_zone_contaminee;
      /* t = 113 */
      ASN1T_Mission_Automate_LOG_ReconnaitreZoneDeDeploiement *mission_automate_log_reconnaitre_zone_de_deploiement;
      /* t = 114 */
      ASN1T_Mission_Automate_LOG_Surveiller *mission_automate_log_surveiller;
      /* t = 115 */
      ASN1T_Mission_Automate_LOG_ArmerPIAs *mission_automate_log_armer_pias;
      /* t = 116 */
      ASN1T_Mission_Automate_Test_MoveTo *mission_automate_test_move_to;
      /* t = 117 */
      ASN1T_Mission_Automate_Decrocher *mission_automate_decrocher;
      /* t = 118 */
      ASN1T_Mission_Automate_ReleverSurPlace *mission_automate_relever_sur_place;
      /* t = 119 */
      ASN1T_Mission_Automate_FaireMouvement *mission_automate_faire_mouvement;
      /* t = 120 */
      ASN1T_Mission_Automate_SeFaireDecontaminer *mission_automate_se_faire_decontaminer;
      /* t = 121 */
      ASN1T_Mission_Automate_Franchir *mission_automate_franchir;
      /* t = 122 */
      ASN1T_Mission_Automate_Stationner *mission_automate_stationner;
      /* t = 123 */
      ASN1T_Mission_Automate_SeFaireTransporter *mission_automate_se_faire_transporter;
      /* t = 124 */
      ASN1T_Mission_Automate_SeRecompleter *mission_automate_se_recompleter;
      /* t = 125 */
      ASN1T_Mission_Automate_SeRendre *mission_automate_se_rendre;
      /* t = 126 */
      ASN1T_Mission_Automate_FaireMouvementVersCampRefugies *mission_automate_faire_mouvement_vers_camp_refugies;
      /* t = 127 */
      ASN1T_Mission_Automate_Generique *mission_automate_generique;
      /* t = 128 */
      ASN1T_Mission_Automate_NBC_ReconnaitreUnAxe *mission_automate_nbc_reconnaitre_un_axe;
      /* t = 129 */
      ASN1T_Mission_Automate_NBC_ReconnaitreUneZone *mission_automate_nbc_reconnaitre_une_zone;
      /* t = 130 */
      ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination *mission_automate_nbc_armer_un_site_de_decontamination;
      /* t = 131 */
      ASN1T_Mission_Automate_NBC_DecontaminerUneZone *mission_automate_nbc_decontaminer_une_zone;
      /* t = 132 */
      ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA *mission_automate_nbc_reconnaitre_des_sites_rota;
      /* t = 133 */
      ASN1T_Mission_Automate_RENS_ROHUM_RenseignerSur *mission_automate_rens_rohum_renseigner_sur;
      /* t = 134 */
      ASN1T_Mission_Automate_RENS_ROHUM_OrienterGuider *mission_automate_rens_rohum_orienter_guider;
      /* t = 135 */
      ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer *mission_automate_rens_rohum_sexfiltrer;
      /* t = 136 */
      ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur *mission_automate_rens_roim_cl289_renseigner_sur;
      /* t = 137 */
      ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur *mission_automate_rens_roim_sdti_renseigner_sur;
      /* t = 138 */
      ASN1T_Mission_Automate_RENS_ROEM_Appuyer *mission_automate_rens_roem_appuyer;
      /* t = 139 */
      ASN1T_Mission_Automate_RENS_ROEM_RenseignerSur *mission_automate_rens_roem_renseignersur;
      /* t = 140 */
      ASN1T_Mission_Automate_RENS_RADINT_RenseignerSur *mission_automate_rens_radint_renseignersur;
      /* t = 141 */
      ASN1T_Mission_Automate_TRANS_Commander *mission_automate_trans_commander;
   } u;
} ASN1T_Mission_Automate;

class EXTERN ASN1C_Mission_Automate : public ASN1CType {
public:
   ASN1T_Mission_Automate& msgData;
   ASN1C_Mission_Automate (
      ASN1MessageBuffer& msgBuf, ASN1T_Mission_Automate& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_Mission_Automate (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate* pvalue);
EXTERN int asn1PD_Mission_Automate (ASN1CTXT* ctxt_p, ASN1T_Mission_Automate* pvalue);

EXTERN void asn1Print_Mission_Automate (ASN1ConstCharPtr name, ASN1T_Mission_Automate* pvalue);

/**************************************************************/
/*                                                            */
/*  EnumAutomateOrderFormation                                */
/*                                                            */
/**************************************************************/

struct EXTERN EnumAutomateOrderFormation {
   enum Root {
      un_echelon = 0,
      deux_echelons = 1
   } ;
} ;

typedef EnumAutomateOrderFormation::Root ASN1T_EnumAutomateOrderFormation;

class EXTERN ASN1C_EnumAutomateOrderFormation : public ASN1CType {
public:
   ASN1T_EnumAutomateOrderFormation& msgData;
   ASN1C_EnumAutomateOrderFormation (
      ASN1MessageBuffer& msgBuf, ASN1T_EnumAutomateOrderFormation& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_EnumAutomateOrderFormation (ASN1CTXT* ctxt_p, ASN1T_EnumAutomateOrderFormation value);
EXTERN int asn1PD_EnumAutomateOrderFormation (ASN1CTXT* ctxt_p, ASN1T_EnumAutomateOrderFormation* pvalue);

EXTERN void asn1Print_EnumAutomateOrderFormation (ASN1ConstCharPtr name, ASN1T_EnumAutomateOrderFormation* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgAutomateOrder                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgAutomateOrder {
   ASN1T_Agent  oid_unite_executante;
   ASN1T_OID  order_id;
   ASN1T_OID  oid_limite_gauche;
   ASN1T_OID  oid_limite_droite;
   ASN1T_ListOID  oid_limas;
   ASN1T_Direction  direction_dangereuse;
   ASN1T_EnumAutomateOrderFormation  formation;
   ASN1T_Mission_Automate  mission;

   ASN1T_MsgAutomateOrder () {
      formation = EnumAutomateOrderFormation::deux_echelons;
   }
} ASN1T_MsgAutomateOrder;

class EXTERN ASN1C_MsgAutomateOrder : public ASN1CType {
public:
   ASN1T_MsgAutomateOrder& msgData;
   ASN1C_MsgAutomateOrder (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgAutomateOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgAutomateOrder (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrder* pvalue);
EXTERN int asn1PD_MsgAutomateOrder (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrder* pvalue);

EXTERN void asn1Print_MsgAutomateOrder (ASN1ConstCharPtr name, ASN1T_MsgAutomateOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgAutomateOrderAck                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgAutomateOrderAck {
   ASN1T_Agent  oid_unite_executante;
   ASN1T_OID  order_id;
   ASN1T_EnumOrderErrorCode  error_code;
} ASN1T_MsgAutomateOrderAck;

class EXTERN ASN1C_MsgAutomateOrderAck : public ASN1CType {
public:
   ASN1T_MsgAutomateOrderAck& msgData;
   ASN1C_MsgAutomateOrderAck (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgAutomateOrderAck& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgAutomateOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrderAck* pvalue);
EXTERN int asn1PD_MsgAutomateOrderAck (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateOrderAck* pvalue);

EXTERN void asn1Print_MsgAutomateOrderAck (ASN1ConstCharPtr name, ASN1T_MsgAutomateOrderAck* pvalue);

/**************************************************************/
/*                                                            */
/*  _SeqOfMsgPionOrder                                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T__SeqOfMsgPionOrder {
   ASN1UINT n;
   ASN1T_MsgPionOrder *elem;
} ASN1T__SeqOfMsgPionOrder;

class EXTERN ASN1C__SeqOfMsgPionOrder : public ASN1CType {
public:
   ASN1T__SeqOfMsgPionOrder& msgData;
   ASN1C__SeqOfMsgPionOrder (
      ASN1MessageBuffer& msgBuf, ASN1T__SeqOfMsgPionOrder& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE__SeqOfMsgPionOrder (ASN1CTXT* ctxt_p, ASN1T__SeqOfMsgPionOrder* pvalue);
EXTERN int asn1PD__SeqOfMsgPionOrder (ASN1CTXT* ctxt_p, ASN1T__SeqOfMsgPionOrder* pvalue);

EXTERN void asn1Print__SeqOfMsgPionOrder (ASN1ConstCharPtr name, ASN1T__SeqOfMsgPionOrder* pvalue);

/**************************************************************/
/*                                                            */
/*  MsgAutomateMRT                                            */
/*                                                            */
/**************************************************************/

typedef struct EXTERN ASN1T_MsgAutomateMRT {
   ASN1T_Agent  automate_id;
   ASN1T__SeqOfMsgPionOrder  missions;
} ASN1T_MsgAutomateMRT;

class EXTERN ASN1C_MsgAutomateMRT : public ASN1CType {
public:
   ASN1T_MsgAutomateMRT& msgData;
   ASN1C_MsgAutomateMRT (
      ASN1MessageBuffer& msgBuf, ASN1T_MsgAutomateMRT& data);
   int Encode ();
   int Decode ();
   void Print (ASN1ConstCharPtr name);
} ;

EXTERN int asn1PE_MsgAutomateMRT (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateMRT* pvalue);
EXTERN int asn1PD_MsgAutomateMRT (ASN1CTXT* ctxt_p, ASN1T_MsgAutomateMRT* pvalue);

EXTERN void asn1Print_MsgAutomateMRT (ASN1ConstCharPtr name, ASN1T_MsgAutomateMRT* pvalue);

#endif
