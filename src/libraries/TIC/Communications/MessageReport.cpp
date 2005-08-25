// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/MessageReport.cpp $
// $Author: Sbo $
// $Modtime: 20/07/05 18:06 $
// $Revision: 1 $
// $Workfile: MessageReport.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "MessageReport.h"

using namespace TIC;

MessageReport::T_MessageReportMap MessageReport::messages_;
const MessageReport               MessageReport::notDefined( "None", eNone, 0, CommunicationType::eNoSubType );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MessageReport::Initialize
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void MessageReport::Initialize()
{
    messages_[ "ras" ]                          = new MessageReport( "ras", eCr_ras, T_MsgCR_cr_cr_ras, CommunicationType::eReportEnemy );
    messages_[ "enPosteFaceAObstacle" ]         = new MessageReport( "enPosteFaceAObstacle", eCr_en_poste_face_a_obstacle, T_MsgCR_cr_cr_en_poste_face_a_obstacle, CommunicationType::eReportObstacle );
    messages_[ "enContournementObstacle" ]      = new MessageReport( "enContournementObstacle", eCr_en_contournement_obstacle, T_MsgCR_cr_cr_en_contournement_obstacle, CommunicationType::eReportObstacle );
    messages_[ "missionImpossible" ]            = new MessageReport( "missionImpossible", eCr_mission_impossible, T_MsgCR_cr_cr_mission_impossible, CommunicationType::eReportMission );
    messages_[ "pretPourNouvelleMission" ]      = new MessageReport( "pretPourNouvelleMission", eCr_pret_pour_nouvelle_mission, T_MsgCR_cr_cr_pret_pour_nouvelle_mission, CommunicationType::eReportMission );
    messages_[ "rejointFuseau" ]                = new MessageReport( "rejointFuseau", eCr_rejoint_fuseau, T_MsgCR_cr_cr_rejoint_fuseau, CommunicationType::eReportMission );
    messages_[ "fuseauRejoint" ]                = new MessageReport( "fuseauRejoint", eCr_fuseau_rejoint, T_MsgCR_cr_cr_fuseau_rejoint , CommunicationType::eReportMission );
    messages_[ "progressionSurAxe" ]            = new MessageReport( "progressionSurAxe", eCr_progression_sur_axe, T_MsgCR_cr_cr_progression_sur_axe, CommunicationType::eReportMission );
    messages_[ "progressionVersLocalisation" ]  = new MessageReport( "progressionVersLocalisation", eCr_progression_vers_localisation, T_MsgCR_cr_cr_progression_vers_localisation, CommunicationType::eReportMission );
    messages_[ "arriveSurLocalisation" ]        = new MessageReport( "arriveSurLocalisation", eCr_arrive_sur_localisation, T_MsgCR_cr_cr_arrive_sur_localisation, CommunicationType::eReportMission );
    messages_[ "reconnaissanceLocalisation" ]   = new MessageReport( "reconnaissanceLocalisation", eCr_reconnaissance_localisation, T_MsgCR_cr_cr_reconnaissance_localisation, CommunicationType::eReportMission );
    messages_[ "enPosteFaceADirectionDangereuse" ] = new MessageReport( "enPosteFaceADirectionDangereuse", eCr_en_poste_face_a_direction_dangereuse, T_MsgCR_cr_cr_en_poste_face_a_direction_dangereuse, CommunicationType::eReportMission );
    messages_[ "enPosteSurBond" ]               = new MessageReport( "enPosteSurBond", eCr_en_poste_sur_bond, T_MsgCR_cr_cr_en_poste_sur_bond, CommunicationType::eReportMission );
    messages_[ "enJalonnement" ]                = new MessageReport( "enJalonnement", eCr_en_jalonnement, T_MsgCR_cr_cr_en_jalonnement, CommunicationType::eReportMission );
    messages_[ "preparationMission" ]           = new MessageReport( "preparationMission", eCr_preparation_mission, T_MsgCR_cr_cr_preparation_mission, CommunicationType::eReportMission );
    messages_[ "debutMission" ]                 = new MessageReport( "debutMission", eCr_debut_mission, T_MsgCR_cr_cr_debut_mission, CommunicationType::eReportMission );
    messages_[ "finMission" ]                   = new MessageReport( "finMission", eCr_fin_mission, T_MsgCR_cr_cr_fin_mission, CommunicationType::eReportMission );
    messages_[ "dispositifInstalle" ]           = new MessageReport( "dispositifInstalle", eCr_dispositif_installe, T_MsgCR_cr_cr_dispositif_installe, CommunicationType::eReportMission );
    messages_[ "enPosition" ]                   = new MessageReport( "enPosition", eCr_en_position, T_MsgCR_cr_cr_en_position, CommunicationType::eReportMission );
    messages_[ "objectifAtteint" ]              = new MessageReport( "objectifAtteint", eCr_objectif_atteint, T_MsgCR_cr_cr_objectif_atteint, CommunicationType::eReportMission );
    messages_[ "debouche" ]                     = new MessageReport( "debouche", eCr_debouche, T_MsgCR_cr_cr_debouche, CommunicationType::eReportMission );
    messages_[ "surPia" ]                       = new MessageReport( "surPia", eCr_sur_pia, T_MsgCR_cr_cr_sur_pia, CommunicationType::eReportMission );
    messages_[ "passageSurPia" ]                = new MessageReport( "passageSurPia", eCr_passage_sur_pia, T_MsgCR_cr_cr_passage_sur_pia, CommunicationType::eReportMission );
    messages_[ "uniteRecueillie" ]              = new MessageReport( "uniteRecueillie", eCr_unite_recueillie, T_MsgCR_cr_cr_unite_recueillie, CommunicationType::eReportMission );
    messages_[ "enCoursDeFranchissement" ]      = new MessageReport( "enCoursDeFranchissement", eCr_en_cours_de_franchissement, T_MsgCR_cr_cr_en_cours_de_franchissement, CommunicationType::eReportMission );
    messages_[ "attenteOrdrePoursuivre" ]       = new MessageReport( "attenteOrdrePoursuivre", eCr_attente_ordre_poursuivre, T_MsgCR_cr_cr_attente_ordre_poursuivre, CommunicationType::eReportMission );
    messages_[ "attenteOrdreDeboucher" ]        = new MessageReport( "attenteOrdreDeboucher", eCr_attente_ordre_deboucher, T_MsgCR_cr_cr_attente_ordre_deboucher, CommunicationType::eReportMission );
    messages_[ "enControleZone" ]               = new MessageReport( "enControleZone", eCr_en_controle_zone, T_MsgCR_cr_cr_en_controle_zone, CommunicationType::eReportMission );
    messages_[ "enSurveillance" ]               = new MessageReport( "enSurveillance", eCr_en_surveillance, T_MsgCR_cr_cr_en_surveillance, CommunicationType::eReportMission );
    messages_[ "enInfiltration" ]               = new MessageReport( "enInfiltration", eCr_en_infiltration, T_MsgCR_cr_cr_en_infiltration, CommunicationType::eReportMission );
    messages_[ "enExfiltration" ]               = new MessageReport( "enExfiltration", eCr_en_exfiltration, T_MsgCR_cr_cr_en_exfiltration, CommunicationType::eReportMission );
    messages_[ "attenteConditionFavorablePourInfiltration" ] = new MessageReport( "attenteConditionFavorablePourInfiltration", eCr_attente_condition_favorable_pour_infiltration, T_MsgCR_cr_cr_attente_condition_favorable_pour_infiltration, CommunicationType::eReportMission );
    messages_[ "droneLance" ]                   = new MessageReport( "droneLance", eCr_drone_lance, T_MsgCR_cr_cr_drone_lance, CommunicationType::eReportMission );
    messages_[ "droneEnExploitation" ]          = new MessageReport( "droneEnExploitation", eCr_drone_en_exploitation, T_MsgCR_cr_cr_drone_en_exploitation, CommunicationType::eReportMission );
    messages_[ "droneExploite" ]                = new MessageReport( "droneExploite", eCr_drone_exploite, T_MsgCR_cr_cr_drone_exploite, CommunicationType::eReportMission );
    messages_[ "debutMiseEnOeuvreDrone" ]       = new MessageReport( "debutMiseEnOeuvreDrone", eCr_debut_mise_en_oeuvre_drone, T_MsgCR_cr_cr_debut_mise_en_oeuvre_drone, CommunicationType::eReportMission );
    messages_[ "finMiseEnOeuvreDrone" ]         = new MessageReport( "finMiseEnOeuvreDrone", eCr_fin_mise_en_oeuvre_drone, T_MsgCR_cr_cr_fin_mise_en_oeuvre_drone, CommunicationType::eReportMission );
    messages_[ "brouille" ]                     = new MessageReport( "brouille", eCr_brouille, T_MsgCR_cr_cr_brouille, CommunicationType::eReportMission );
    messages_[ "enEcoute" ]                     = new MessageReport( "enEcoute", eCr_en_ecoute, T_MsgCR_cr_cr_en_ecoute, CommunicationType::eReportMission );
    messages_[ "enEcouteRadar" ]                = new MessageReport( "enEcouteRadar", eCr_en_ecoute_radar, T_MsgCR_cr_cr_en_ecoute_radar, CommunicationType::eReportMission );
    messages_[ "automatePriseContact" ]         = new MessageReport( "automatePriseContact", eCr_automate_prise_contact, T_MsgCR_cr_cr_automate_prise_contact, CommunicationType::eReportEnemy );
    messages_[ "automateRuptureContact" ]       = new MessageReport( "automateRuptureContact", eCr_automate_rupture_contact, T_MsgCR_cr_cr_automate_rupture_contact, CommunicationType::eReportEnemy );
    messages_[ "pionPriseContact" ]             = new MessageReport( "pionPriseContact", eCr_pion_prise_contact, T_MsgCR_cr_cr_pion_prise_contact, CommunicationType::eReportEnemy );
    messages_[ "pionRuptureContact" ]           = new MessageReport( "pionRuptureContact", eCr_pion_rupture_contact, T_MsgCR_cr_cr_pion_rupture_contact, CommunicationType::eReportEnemy );
    messages_[ "precisionContact" ]             = new MessageReport( "precisionContact", eCr_precision_contact, T_MsgCR_cr_cr_precision_contact, CommunicationType::eReportEnemy );
    messages_[ "ennemiDetruit" ]                = new MessageReport( "ennemiDetruit", eCr_ennemi_detruit, T_MsgCR_cr_cr_ennemi_detruit, CommunicationType::eReportEnemy );
    messages_[ "perteDeContactVisuel" ]         = new MessageReport( "perteDeContactVisuel", eCr_perte_de_contact_visuel, T_MsgCR_cr_cr_perte_de_contact_visuel, CommunicationType::eReportEnemy );
    messages_[ "posteFaceEni" ]                 = new MessageReport( "posteFaceEni", eCr_poste_face_eni, T_MsgCR_cr_cr_poste_face_eni, CommunicationType::eReportEnemy );
    messages_[ "priseAPartie" ]                 = new MessageReport( "priseAPartie", eCr_prise_a_partie, T_MsgCR_cr_cr_prise_a_partie, CommunicationType::eReportEnemy );
    messages_[ "pointTenuParEni" ]              = new MessageReport( "pointTenuParEni", eCr_point_tenu_par_eni, T_MsgCR_cr_cr_point_tenu_par_eni, CommunicationType::eReportEnemy );
    messages_[ "progressionVersEni" ]           = new MessageReport( "progressionVersEni", eCr_progression_vers_eni, T_MsgCR_cr_cr_progression_vers_eni, CommunicationType::eReportEnemy );
    messages_[ "presenceEniProbable" ]          = new MessageReport( "presenceEniProbable", eCr_presence_eni_probable, T_MsgCR_cr_cr_presence_eni_probable, CommunicationType::eReportEnemy );
    messages_[ "decroche" ]                     = new MessageReport( "decroche", eCr_decroche, T_MsgCR_cr_cr_decroche, CommunicationType::eReportEnemy );
    messages_[ "eniSurObjectif" ]               = new MessageReport( "eniSurObjectif", eCr_eni_sur_objectif, T_MsgCR_cr_cr_eni_sur_objectif, CommunicationType::eReportEnemy );
    messages_[ "riposte" ]                      = new MessageReport( "riposte", eCr_riposte, T_MsgCR_cr_cr_riposte, CommunicationType::eReportEnemy );
    messages_[ "releve" ]                       = new MessageReport( "releve", eCr_releve, T_MsgCR_cr_cr_releve, CommunicationType::eReportMission );
    messages_[ "suitUnite" ]                    = new MessageReport( "suitUnite", eCr_suit_unite, T_MsgCR_cr_cr_suit_unite, CommunicationType::eReportMission );
    messages_[ "passeEnAppui" ]                 = new MessageReport( "passeEnAppui", eCr_passe_en_appui, T_MsgCR_cr_cr_passe_en_appui, CommunicationType::eReportMission );
    messages_[ "enAppui" ]                      = new MessageReport( "enAppui", eCr_en_appui, T_MsgCR_cr_cr_en_appui, CommunicationType::eReportMission );
    messages_[ "detruitTactiquement" ]          = new MessageReport( "detruitTactiquement", eCr_detruit_tactiquement, T_MsgCR_cr_cr_detruit_tactiquement, CommunicationType::eReportLogisticSituation );
    messages_[ "ambianceVitesse" ]              = new MessageReport( "ambianceVitesse", eCr_ambiance_vitesse, T_MsgCR_cr_cr_ambiance_vitesse, CommunicationType::eReportMission );
    messages_[ "ambianceSurete" ]               = new MessageReport( "ambianceSurete", eCr_ambiance_surete, T_MsgCR_cr_cr_ambiance_surete, CommunicationType::eReportMission );
    messages_[ "surLima" ]                      = new MessageReport( "surLima", eCr_sur_lima, T_MsgCR_cr_cr_sur_lima, CommunicationType::eReportMission );
    messages_[ "surTypeTerrainParticulier" ]    = new MessageReport( "surTypeTerrainParticulier", eCr_sur_type_terrain_particulier, T_MsgCR_cr_cr_sur_type_terrain_particulier, CommunicationType::eReportMission );
    messages_[ "obstacleEnAttenteActivation" ]  = new MessageReport( "obstacleEnAttenteActivation", eCr_obstacle_en_attente_activation, T_MsgCR_cr_cr_obstacle_en_attente_activation, CommunicationType::eReportObstacle );
    messages_[ "travauxRealises" ]              = new MessageReport( "travauxRealises", eCr_travaux_realises, T_MsgCR_cr_cr_travaux_realises, CommunicationType::eReportObstacle );
    messages_[ "obstacleDegage" ]               = new MessageReport( "obstacleDegage", eCr_obstacle_degage, T_MsgCR_cr_cr_obstacle_degage, CommunicationType::eReportObstacle );
    messages_[ "organisationChantier" ]         = new MessageReport( "organisationChantier", eCr_organisation_chantier, T_MsgCR_cr_cr_organisation_chantier, CommunicationType::eReportMission );
    messages_[ "desorganisationChantier" ]      = new MessageReport( "desorganisationChantier", eCr_desorganisation_chantier, T_MsgCR_cr_cr_desorganisation_chantier, CommunicationType::eReportMission );
    messages_[ "reconnaissancePositionObstacle" ] = new MessageReport( "reconnaissancePositionObstacle", eCr_reconnaissance_position_obstacle, T_MsgCR_cr_cr_reconnaissance_position_obstacle, CommunicationType::eReportMission );
    messages_[ "contournementRealise" ]         = new MessageReport( "contournementRealise", eCr_contournement_realise, T_MsgCR_cr_cr_contournement_realise, CommunicationType::eReportObstacle );
    messages_[ "attenteRenforcement" ]          = new MessageReport( "attenteRenforcement", eCr_attente_renforcement, T_MsgCR_cr_cr_attente_renforcement, CommunicationType::eReportMission );
    messages_[ "debutTravaux" ]                 = new MessageReport( "debutTravaux", eCr_debut_travaux, T_MsgCR_cr_cr_debut_travaux, CommunicationType::eReportMission );
    messages_[ "debutFranchissement" ]          = new MessageReport( "debutFranchissement", eCr_debut_franchissement, T_MsgCR_cr_cr_debut_franchissement, CommunicationType::eReportMission );
    messages_[ "finFranchissement" ]            = new MessageReport( "finFranchissement", eCr_fin_franchissement, T_MsgCR_cr_cr_fin_franchissement, CommunicationType::eReportMission );
    messages_[ "reussiteRenforcement" ]         = new MessageReport( "reussiteRenforcement", eCr_reussite_renforcement, T_MsgCR_cr_cr_reussite_renforcement, CommunicationType::eReportMission );
    messages_[ "echecRenforcement" ]            = new MessageReport( "echecRenforcement", eCr_echec_renforcement, T_MsgCR_cr_cr_echec_renforcement, CommunicationType::eReportMission );
    messages_[ "itineraireRetabli" ]            = new MessageReport( "itineraireRetabli", eCr_itineraire_retabli, T_MsgCR_cr_cr_itineraire_retabli, CommunicationType::eReportObstacle );
    messages_[ "renforcementBientotFini" ]      = new MessageReport( "renforcementBientotFini", eCr_renforcement_bientot_fini, T_MsgCR_cr_cr_renforcement_bientot_fini, CommunicationType::eReportMission );
    messages_[ "situationLogFavorable" ]        = new MessageReport( "situationLogFavorable", eCr_situation_log_favorable, T_MsgCR_cr_cr_situation_log_favorable, CommunicationType::eReportLogisticSituation );
    messages_[ "situationLogDefavorable" ]      = new MessageReport( "situationLogDefavorable", eCr_situation_log_defavorable, T_MsgCR_cr_cr_situation_log_defavorable, CommunicationType::eReportLogisticSituation );
    messages_[ "sectionDeployee" ]              = new MessageReport( "sectionDeployee", eCr_section_deployee, T_MsgCR_cr_cr_section_deployee, CommunicationType::eReportMission );
    messages_[ "materielRetourDeMaintenance" ]  = new MessageReport( "materielRetourDeMaintenance", eCr_materiel_retour_de_maintenance, T_MsgCR_cr_cr_materiel_retour_de_maintenance, CommunicationType::eReportLogisticSituation );
    messages_[ "demandeRavitaillement" ]        = new MessageReport( "demandeRavitaillement", eCr_demande_ravitaillement, T_MsgCR_cr_cr_demande_ravitaillement, CommunicationType::eReportLogisticSituation );
    messages_[ "demandeEvacuationSanitaire" ]   = new MessageReport( "demandeEvacuationSanitaire", eCr_demande_evacuation_sanitaire, T_MsgCR_cr_cr_demande_evacuation_sanitaire, CommunicationType::eReportLogisticSituation );
    messages_[ "demandeEvacuationMateriel" ]    = new MessageReport( "demandeEvacuationMateriel", eCr_demande_evacuation_materiel, T_MsgCR_cr_cr_demande_evacuation_materiel, CommunicationType::eReportLogisticSituation );
    messages_[ "allocationConsentieBientotEpuisee" ] = new MessageReport( "allocationConsentieBientotEpuisee", eCr_allocation_consentie_bientot_epuisee, T_MsgCR_cr_cr_allocation_consentie_bientot_epuisee, CommunicationType::eReportLogisticSituation );
    messages_[ "enCoursRavitaillement" ]        = new MessageReport( "enCoursRavitaillement", eCr_en_cours_ravitaillement, T_MsgCR_cr_cr_en_cours_ravitaillement, CommunicationType::eReportLogisticSituation );
    messages_[ "ravitaillementTermine" ]        = new MessageReport( "ravitaillementTermine", eCr_ravitaillement_termine, T_MsgCR_cr_cr_ravitaillement_termine, CommunicationType::eReportLogisticSituation );
    messages_[ "helicoptereEnObservation" ]     = new MessageReport( "helicoptereEnObservation", eCr_helicoptere_en_observation, T_MsgCR_cr_cr_helicoptere_en_observation, CommunicationType::eReportMission );
    messages_[ "perteInfoGuidage" ]             = new MessageReport( "perteInfoGuidage", eCr_perte_info_guidage, T_MsgCR_cr_cr_perte_info_guidage, CommunicationType::eReportMission );
    messages_[ "transportUnitePasPrete" ]       = new MessageReport( "transportUnitePasPrete", eCr_transport_unite_pas_prete, T_MsgCR_cr_cr_transport_unite_pas_prete, CommunicationType::eReportMission );
    messages_[ "transportEnCours" ]             = new MessageReport( "transportEnCours", eCr_transport_en_cours, T_MsgCR_cr_cr_transport_en_cours, CommunicationType::eReportMission );
    messages_[ "transportEmbarquement" ]        = new MessageReport( "transportEmbarquement", eCr_transport_embarquement, T_MsgCR_cr_cr_transport_embarquement, CommunicationType::eReportMission );
    messages_[ "transportDebarquement" ]        = new MessageReport( "transportDebarquement", eCr_transport_debarquement, T_MsgCR_cr_cr_transport_debarquement, CommunicationType::eReportMission );
    messages_[ "vehiculeLaisseSurPosition" ]    = new MessageReport( "vehiculeLaisseSurPosition", eCr_vehicule_laisse_sur_position, T_MsgCR_cr_cr_vehicule_laisse_sur_position, CommunicationType::eReportMission );
    messages_[ "rejointPositionEmbarquement" ]  = new MessageReport( "rejointPositionEmbarquement", eCr_rejoint_position_embarquement, T_MsgCR_cr_cr_rejoint_position_embarquement, CommunicationType::eReportMission );
    messages_[ "enAttenteTransporteurs" ]       = new MessageReport( "enAttenteTransporteurs", eCr_en_attente_transporteurs, T_MsgCR_cr_cr_en_attente_transporteurs, CommunicationType::eReportMission );
    messages_[ "contamineNbc" ]                 = new MessageReport( "contamineNbc", eCr_contamine_nbc, T_MsgCR_cr_cr_contamine_nbc, CommunicationType::eReportNbc );
    messages_[ "decontamineNbc" ]               = new MessageReport( "decontamineNbc", eCr_decontamine_nbc, T_MsgCR_cr_cr_decontamine_nbc, CommunicationType::eReportNbc );
    messages_[ "tenueProtectionNbcMise" ]       = new MessageReport( "tenueProtectionNbcMise", eCr_tenue_protection_nbc_mise, T_MsgCR_cr_cr_tenue_protection_nbc_mise, CommunicationType::eReportNbc );
    messages_[ "tenueProtectionNbcEnlevee" ]    = new MessageReport( "tenueProtectionNbcEnlevee", eCr_tenue_protection_nbc_enlevee, T_MsgCR_cr_cr_tenue_protection_nbc_enlevee, CommunicationType::eReportNbc );
    messages_[ "alerteNbc" ]                    = new MessageReport( "alerteNbc", eCr_alerte_nbc, T_MsgCR_cr_cr_alerte_nbc, CommunicationType::eReportNbc );
    messages_[ "siteDecontaminationAnime" ]     = new MessageReport( "siteDecontaminationAnime", eCr_site_decontamination_anime, T_MsgCR_cr_cr_site_decontamination_anime, CommunicationType::eReportMission );
    messages_[ "siteDecontaminationDesactive" ] = new MessageReport( "siteDecontaminationDesactive", eCr_site_decontamination_desactive, T_MsgCR_cr_cr_site_decontamination_desactive, CommunicationType::eReportMission );
    messages_[ "demandeDecontamination" ]       = new MessageReport( "demandeDecontamination", eCr_demande_decontamination, T_MsgCR_cr_cr_demande_decontamination, CommunicationType::eReportNbc );
    messages_[ "zoneReconnue" ]                 = new MessageReport( "zoneReconnue", eCr_zone_reconnue, T_MsgCR_cr_cr_zone_reconnue, CommunicationType::eReportMission );
    messages_[ "zoneDecontaminee" ]             = new MessageReport( "zoneDecontaminee", eCr_zone_decontaminee, T_MsgCR_cr_cr_zone_decontaminee, CommunicationType::eReportMission );
    messages_[ "siteRotaReconnu" ]              = new MessageReport( "siteRotaReconnu", eCr_site_rota_reconnu, T_MsgCR_cr_cr_site_rota_reconnu, CommunicationType::eReportMission );
    messages_[ "incidentNbcTermine" ]           = new MessageReport( "incidentNbcTermine", eCr_incident_nbc_termine, T_MsgCR_cr_cr_incident_nbc_termine, CommunicationType::eReportNbc );
    messages_[ "pionDecontamine" ]              = new MessageReport( "pionDecontamine", eCr_pion_decontamine, T_MsgCR_cr_cr_pion_decontamine, CommunicationType::eReportNbc );
    messages_[ "reconnaissanceTerminee" ]       = new MessageReport( "reconnaissanceTerminee", eCr_reconnaissance_terminee, T_MsgCR_cr_cr_reconnaissance_terminee, CommunicationType::eReportMission );
    messages_[ "reconnaissanceInitialeTerminee" ] = new MessageReport( "reconnaissanceInitialeTerminee", eCr_reconnaissance_initiale_terminee, T_MsgCR_cr_cr_reconnaissance_initiale_terminee, CommunicationType::eReportMission );
    messages_[ "rejointPlotRavitaillement" ]    = new MessageReport( "rejointPlotRavitaillement", eCr_rejoint_plot_ravitaillement, T_MsgCR_cr_cr_rejoint_plot_ravitaillement, CommunicationType::eReportMission );
    messages_[ "premierCoupParti" ]             = new MessageReport( "premierCoupParti", eCr_premier_coup_parti, T_MsgCR_cr_cr_premier_coup_parti, CommunicationType::eReportArtillery );
    messages_[ "tirExecute" ]                   = new MessageReport( "tirExecute", eCr_tir_execute, T_MsgCR_cr_cr_tir_execute, CommunicationType::eReportArtillery );
    messages_[ "tirHorsDePortee" ]              = new MessageReport( "tirHorsDePortee", eCr_tir_hors_de_portee, T_MsgCR_cr_cr_tir_hors_de_portee, CommunicationType::eReportArtillery );
    messages_[ "tirAnnule" ]                    = new MessageReport( "tirAnnule", eCr_tir_annule, T_MsgCR_cr_cr_tir_annule, CommunicationType::eReportArtillery );
    messages_[ "changementDePosition" ]         = new MessageReport( "changementDePosition", eCr_changement_de_position, T_MsgCR_cr_cr_changement_de_position, CommunicationType::eReportMission );
    messages_[ "debutSurveillance" ]            = new MessageReport( "debutSurveillance", eCr_debut_surveillance, T_MsgCR_cr_cr_debut_surveillance, CommunicationType::eReportMission );
    messages_[ "debutInstallation" ]            = new MessageReport( "debutInstallation", eCr_debut_installation, T_MsgCR_cr_cr_debut_installation, CommunicationType::eReportMission );
    messages_[ "pretPourConduiteTir" ]          = new MessageReport( "pretPourConduiteTir", eCr_pret_pour_conduite_tir, T_MsgCR_cr_cr_pret_pour_conduite_tir, CommunicationType::eReportArtillery );
    messages_[ "observationTirIndirect" ]       = new MessageReport( "observationTirIndirect", eCr_observation_tir_indirect, T_MsgCR_cr_cr_observation_tir_indirect, CommunicationType::eReportArtillery );
    messages_[ "zoneImplantationOccupee" ]      = new MessageReport( "zoneImplantationOccupee", eCr_zone_implantation_occupee, T_MsgCR_cr_cr_zone_implantation_occupee, CommunicationType::eReportMission );
    messages_[ "installationSurZoneImplantation" ] = new MessageReport( "installationSurZoneImplantation", eCr_installation_sur_zone_implantation, T_MsgCR_cr_cr_installation_sur_zone_implantation, CommunicationType::eReportMission );
    messages_[ "receptionOrdreDeFeu" ]          = new MessageReport( "receptionOrdreDeFeu", eCr_reception_ordre_de_feu, T_MsgCR_cr_cr_reception_ordre_de_feu, CommunicationType::eReportArtillery );
    messages_[ "pretAuTir" ]                    = new MessageReport( "pretAuTir", eCr_pret_au_tir, T_MsgCR_cr_cr_pret_au_tir, CommunicationType::eReportArtillery );
    messages_[ "presencePopulation" ]           = new MessageReport( "presencePopulation", eCr_presence_population, T_MsgCR_cr_cr_presence_population, CommunicationType::eReportEnemy );
    messages_[ "orientationPopulationVersCampRefugies" ] = new MessageReport( "orientationPopulationVersCampRefugies", eCr_orientation_population_vers_camp_refugies, T_MsgCR_cr_cr_orientation_population_vers_camp_refugies, CommunicationType::eReportEnemy );
    messages_[ "pasDeCampRefugiesDisponible" ]  = new MessageReport( "pasDeCampRefugiesDisponible", eCr_pas_de_camp_refugies_disponible, T_MsgCR_cr_cr_pas_de_camp_refugies_disponible, CommunicationType::eReportEnemy );
    messages_[ "presenceEnnemiRendu" ]          = new MessageReport( "presenceEnnemiRendu", eCr_presence_ennemi_rendu, T_MsgCR_cr_cr_presence_ennemi_rendu, CommunicationType::eReportEnemy );
    messages_[ "orientationEnnemiRenduVersCampPrisonniers" ] = new MessageReport( "orientationEnnemiRenduVersCampPrisonniers", eCr_orientation_ennemi_rendu_vers_camp_prisonniers, T_MsgCR_cr_cr_orientation_ennemi_rendu_vers_camp_prisonniers, CommunicationType::eReportEnemy );
    messages_[ "pasDeCampPrisonniersDisponible" ] = new MessageReport( "pasDeCampPrisonniersDisponible", eCr_pas_de_camp_prisonniers_disponible, T_MsgCR_cr_cr_pas_de_camp_prisonniers_disponible, CommunicationType::eReportEnemy );
    messages_[ "tirDansZoneInterdite" ]         = new MessageReport( "tirDansZoneInterdite", eCr_tir_dans_zone_interdite, T_MsgCR_cr_cr_tir_dans_zone_interdite, CommunicationType::eReportEnemy );
    messages_[ "tirSurCampAmi" ]                = new MessageReport( "tirSurCampAmi", eCr_tir_sur_camp_ami, T_MsgCR_cr_cr_tir_sur_camp_ami, CommunicationType::eReportEnemy );
    messages_[ "tireParCampAmi" ]               = new MessageReport( "tireParCampAmi", eCr_tire_par_camp_ami, T_MsgCR_cr_cr_tire_par_camp_ami, CommunicationType::eReportEnemy );
    messages_[ "tirSurCampNeutre" ]             = new MessageReport( "tirSurCampNeutre", eCr_tir_sur_camp_neutre, T_MsgCR_cr_cr_tir_sur_camp_neutre, CommunicationType::eReportEnemy );
    messages_[ "tireParCampNeutre" ]            = new MessageReport( "tireParCampNeutre", eCr_tire_par_camp_neutre, T_MsgCR_cr_cr_tire_par_camp_neutre, CommunicationType::eReportEnemy );
    messages_[ "tirSurCivil" ]                  = new MessageReport( "tirSurCivil", eCr_tir_sur_civil, T_MsgCR_cr_cr_tir_sur_civil, CommunicationType::eReportEnemy );
    messages_[ "tireParCivil" ]                 = new MessageReport( "tireParCivil", eCr_tire_par_civil, T_MsgCR_cr_cr_tire_par_civil, CommunicationType::eReportEnemy );
    messages_[ "destructionPc" ]                = new MessageReport( "destructionPc", eCr_destruction_pc, T_MsgCR_cr_cr_destruction_pc, CommunicationType::eReportEnemy );
    messages_[ "decesBlesse" ]                  = new MessageReport( "decesBlesse", eCr_deces_blesse, T_MsgCR_cr_cr_deces_blesse, CommunicationType::eReportLogisticSituation );
    messages_[ "decesBlessePendantTransport" ]  = new MessageReport( "decesBlessePendantTransport", eCr_deces_blesse_pendant_transport, T_MsgCR_cr_cr_deces_blesse_pendant_transport, CommunicationType::eReportLogisticSituation );
    messages_[ "decesBlessePendantHospitalisation" ] = new MessageReport( "decesBlessePendantHospitalisation", eCr_deces_blesse_pendant_hospitalisation, T_MsgCR_cr_cr_deces_blesse_pendant_hospitalisation, CommunicationType::eReportLogisticSituation );
    messages_[ "missionImpossiblePionsSubordonnesNonOperationnels" ] = new MessageReport( "missionImpossiblePionsSubordonnesNonOperationnels", eCr_mission_impossible_pions_subordonnes_non_operationnels, T_MsgCR_cr_cr_mission_impossible_pions_subordonnes_non_operationnels, CommunicationType::eReportScipio );
    messages_[ "missionImpossibleLimaInvalide" ] = new MessageReport( "missionImpossibleLimaInvalide", eCr_mission_impossible_lima_invalide, T_MsgCR_cr_cr_mission_impossible_lima_invalide, CommunicationType::eReportScipio );
    messages_[ "missionImpossibleZoneHorsFuseau" ] = new MessageReport( "missionImpossibleZoneHorsFuseau", eCr_mission_impossible_zone_hors_fuseau, T_MsgCR_cr_cr_mission_impossible_zone_hors_fuseau, CommunicationType::eReportScipio );
    messages_[ "rapportDeForceDefavorable" ]    = new MessageReport( "rapportDeForceDefavorable", eCr_rapport_de_force_defavorable, T_MsgCR_cr_cr_rapport_de_force_defavorable, CommunicationType::eReportEnemy );
    messages_[ "terrainDifficile" ]             = new MessageReport( "terrainDifficile", eCr_terrain_difficile, T_MsgCR_cr_cr_terrain_difficile, CommunicationType::eReportScipio );
    messages_[ "plusDeCarburant" ]              = new MessageReport( "plusDeCarburant", eCr_plus_de_carburant, T_MsgCR_cr_cr_plus_de_carburant, CommunicationType::eReportLogisticSituation );
    messages_[ "dispositifDisperse" ]           = new MessageReport( "dispositifDisperse", eCr_dispositif_disperse, T_MsgCR_cr_cr_dispositif_disperse, CommunicationType::eReportScipio );
    messages_[ "pionNeutralise" ]               = new MessageReport( "pionNeutralise", eCr_pion_neutralise, T_MsgCR_cr_cr_pion_neutralise, CommunicationType::eReportEnemy );
    messages_[ "silenceRadio" ]                 = new MessageReport( "silenceRadio", eCr_silence_radio, T_MsgCR_cr_cr_silence_radio, CommunicationType::eReportEnemy );
    messages_[ "tirRestreint" ]                 = new MessageReport( "tirRestreint", eCr_tir_restreint, T_MsgCR_cr_cr_tir_restreint, CommunicationType::eReportEnemy );
    messages_[ "tirInterdit" ]                  = new MessageReport( "tirInterdit", eCr_tir_interdit, T_MsgCR_cr_cr_tir_interdit, CommunicationType::eReportEnemy );
    messages_[ "tirLibre" ]                     = new MessageReport( "tirLibre", eCr_tir_libre, T_MsgCR_cr_cr_tir_libre, CommunicationType::eReportEnemy );
    messages_[ "trace" ]                        = new MessageReport( "trace", eCr_trace, T_MsgCR_cr_cr_trace, CommunicationType::eReportScipio );
    messages_[ "traceId" ]                      = new MessageReport( "traceId", eCr_trace_id, T_MsgCR_cr_cr_trace_id, CommunicationType::eReportScipio );
}

// -----------------------------------------------------------------------------
// Name: MessageReport::Terminate
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void MessageReport::Terminate()
{
    for( CIT_MessageReportMap it = messages_.begin(); it != messages_.end(); ++it )
        delete it->second;
    messages_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MessageReport constructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
MessageReport::MessageReport( const std::string& strName, E_Type nType, uint nAsnID, CommunicationType::ESubType eSubType )
    : strName_  ( strName )
    , nType_    ( nType   )
    , nAsnID_   ( nAsnID  )
    , eSubType_ ( eSubType )
{
}

// -----------------------------------------------------------------------------
// Name: MessageReport destructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
MessageReport::~MessageReport()
{
}