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
	messages_[ "RAS" ] = new MessageReport( "RAS", eRC_RAS, T_MsgCR_cr_cr_ras, CommunicationType::eReportEnemy );
	messages_[ "EnPosteFaceAObstacle" ] = new MessageReport( "EnPosteFaceAObstacle", eRC_EnPosteFaceAObstacle, T_MsgCR_cr_cr_en_poste_face_a_obstacle, CommunicationType::eReportObstacle );
	messages_[ "EnContournementObstacle" ] = new MessageReport( "EnContournementObstacle", eRC_EnContournementObstacle, T_MsgCR_cr_cr_en_contournement_obstacle, CommunicationType::eReportObstacle );
	messages_[ "MissionImpossible" ] = new MessageReport( "MissionImpossible", eRC_MissionImpossible, T_MsgCR_cr_cr_mission_impossible, CommunicationType::eReportMission );
	messages_[ "ProgressionSurAxe" ] = new MessageReport( "ProgressionSurAxe", eRC_ProgressionSurAxe, T_MsgCR_cr_cr_progression_sur_axe, CommunicationType::eReportMission );
	messages_[ "ProgressionVersPosition" ] = new MessageReport( "ProgressionVersPosition", eRC_ProgressionVersPosition, T_MsgCR_cr_cr_progression_vers_position, CommunicationType::eReportMission );
	messages_[ "ArriveSurPosition" ] = new MessageReport( "ArriveSurPosition", eRC_ArriveSurPosition, T_MsgCR_cr_cr_arrive_sur_position, CommunicationType::eReportMission );
	messages_[ "ReconnaissancePoint" ] = new MessageReport( "ReconnaissancePoint", eRC_ReconnaissancePoint, T_MsgCR_cr_cr_reconnaissance_point, CommunicationType::eReportMission );
	messages_[ "EnPosteFaceADirectionDangereuse" ] = new MessageReport( "EnPosteFaceADirectionDangereuse", eRC_EnPosteFaceADirectionDangereuse, T_MsgCR_cr_cr_en_poste_face_a_direction_dangereuse, CommunicationType::eReportMission );
	messages_[ "EnJalonnement" ] = new MessageReport( "EnJalonnement", eRC_EnJalonnement, T_MsgCR_cr_cr_en_jalonnement, CommunicationType::eReportMission );
	messages_[ "PreparationMission" ] = new MessageReport( "PreparationMission", eRC_PreparationMission, T_MsgCR_cr_cr_preparation_mission, CommunicationType::eReportMission );
	messages_[ "DebutMission" ] = new MessageReport( "DebutMission", eRC_DebutMission, T_MsgCR_cr_cr_debut_mission, CommunicationType::eReportMission );
	messages_[ "FinMission" ] = new MessageReport( "FinMission", eRC_FinMission, T_MsgCR_cr_cr_fin_mission, CommunicationType::eReportMission );
	messages_[ "DispositifInstalle" ] = new MessageReport( "DispositifInstalle", eRC_DispositifInstalle, T_MsgCR_cr_cr_dispositif_installe, CommunicationType::eReportMission );
	messages_[ "EnPosition" ] = new MessageReport( "EnPosition", eRC_EnPosition, T_MsgCR_cr_cr_en_position, CommunicationType::eReportMission );
	messages_[ "ObjectifAtteint" ] = new MessageReport( "ObjectifAtteint", eRC_ObjectifAtteint, T_MsgCR_cr_cr_objectif_atteint, CommunicationType::eReportMission );
	messages_[ "Debouche" ] = new MessageReport( "Debouche", eRC_Debouche, T_MsgCR_cr_cr_debouche, CommunicationType::eReportMission );
	messages_[ "RejointUnite" ] = new MessageReport( "RejointUnite", eRC_RejointUnite, T_MsgCR_cr_cr_rejoint_unite, CommunicationType::eReportMission );
	messages_[ "EscorteUnite" ] = new MessageReport( "EscorteUnite", eRC_EscorteUnite, T_MsgCR_cr_cr_escorte_unite, CommunicationType::eReportMission );
	messages_[ "EnFreinage" ] = new MessageReport( "EnFreinage", eRC_EnFreinage, T_MsgCR_cr_cr_en_freinage, CommunicationType::eReportMission );
	messages_[ "Assaut" ] = new MessageReport( "Assaut", eRC_Assaut, T_MsgCR_cr_cr_assaut, CommunicationType::eReportMission );
	messages_[ "EnCouverture" ] = new MessageReport( "EnCouverture", eRC_EnCouverture, T_MsgCR_cr_cr_en_couverture, CommunicationType::eReportMission );
	messages_[ "EnEclairage" ] = new MessageReport( "EnEclairage", eRC_EnEclairage, T_MsgCR_cr_cr_en_eclairage, CommunicationType::eReportMission );
	messages_[ "EnManoeuvreRavitaillement" ] = new MessageReport( "EnManoeuvreRavitaillement", eRC_EnManoeuvreRavitaillement, T_MsgCR_cr_cr_en_manoeuvre_ravitaillement, CommunicationType::eReportMission );
	messages_[ "SurPIA" ] = new MessageReport( "SurPIA", eRC_SurPIA, T_MsgCR_cr_cr_sur_pia, CommunicationType::eReportMission );
	messages_[ "PassageSurPIA" ] = new MessageReport( "PassageSurPIA", eRC_PassageSurPIA, T_MsgCR_cr_cr_passage_sur_pia, CommunicationType::eReportMission );
	messages_[ "UniteRecueillie" ] = new MessageReport( "UniteRecueillie", eRC_UniteRecueillie, T_MsgCR_cr_cr_unite_recueillie, CommunicationType::eReportMission );
	messages_[ "EnCoursDeFranchissement" ] = new MessageReport( "EnCoursDeFranchissement", eRC_EnCoursDeFranchissement, T_MsgCR_cr_cr_en_cours_de_franchissement, CommunicationType::eReportMission );
	messages_[ "AttenteOrdrePoursuivre" ] = new MessageReport( "AttenteOrdrePoursuivre", eRC_AttenteOrdrePoursuivre, T_MsgCR_cr_cr_attente_ordre_poursuivre, CommunicationType::eReportMission );
	messages_[ "AttenteOrdreDeboucher" ] = new MessageReport( "AttenteOrdreDeboucher", eRC_AttenteOrdreDeboucher, T_MsgCR_cr_cr_attente_ordre_deboucher, CommunicationType::eReportMission );
	messages_[ "Fixe" ] = new MessageReport( "Fixe", eRC_Fixe, T_MsgCR_cr_cr_fixe, CommunicationType::eReportEnemy );
	messages_[ "EnEsquive" ] = new MessageReport( "EnEsquive", eRC_EnEsquive, T_MsgCR_cr_cr_en_esquive, CommunicationType::eReportEnemy );
	messages_[ "PriseContact" ] = new MessageReport( "PriseContact", eRC_PriseContact, T_MsgCR_cr_cr_prise_contact, CommunicationType::eReportEnemy );
	messages_[ "RuptureContact" ] = new MessageReport( "RuptureContact", eRC_RuptureContact, T_MsgCR_cr_cr_rupture_contact, CommunicationType::eReportEnemy );
	messages_[ "PrecisionContact" ] = new MessageReport( "PrecisionContact", eRC_PrecisionContact, T_MsgCR_cr_cr_precision_contact, CommunicationType::eReportEnemy );
	messages_[ "EnnemiDetruit" ] = new MessageReport( "EnnemiDetruit", eRC_EnnemiDetruit, T_MsgCR_cr_cr_ennemi_detruit, CommunicationType::eReportEnemy );
	messages_[ "PrisAPartieParTirDirect" ] = new MessageReport( "PrisAPartieParTirDirect", eRC_PrisAPartieParTirDirect, T_MsgCR_cr_cr_pris_a_partie_par_tir_direct, CommunicationType::eReportEnemy );
	messages_[ "PrisSousTirArtillerie" ] = new MessageReport( "PrisSousTirArtillerie", eRC_PrisSousTirArtillerie, T_MsgCR_cr_cr_pris_sous_tir_artillerie, CommunicationType::eReportEnemy );
	messages_[ "PointTenuParEni" ] = new MessageReport( "PointTenuParEni", eRC_PointTenuParEni, T_MsgCR_cr_cr_point_tenu_par_eni, CommunicationType::eReportEnemy );
	messages_[ "ProgressionVersEni" ] = new MessageReport( "ProgressionVersEni", eRC_ProgressionVersEni, T_MsgCR_cr_cr_progression_vers_eni, CommunicationType::eReportEnemy );
	messages_[ "Decroche" ] = new MessageReport( "Decroche", eRC_Decroche, T_MsgCR_cr_cr_decroche, CommunicationType::eReportEnemy );
	messages_[ "EniSurObjectif" ] = new MessageReport( "EniSurObjectif", eRC_EniSurObjectif, T_MsgCR_cr_cr_eni_sur_objectif, CommunicationType::eReportEnemy );
	messages_[ "Riposte" ] = new MessageReport( "Riposte", eRC_Riposte, T_MsgCR_cr_cr_riposte, CommunicationType::eReportEnemy );
	messages_[ "DebutControleZone" ] = new MessageReport( "DebutControleZone", eRC_DebutControleZone, T_MsgCR_cr_cr_debut_controle_zone, CommunicationType::eReportMission );
	messages_[ "FinControleZone" ] = new MessageReport( "FinControleZone", eRC_FinControleZone, T_MsgCR_cr_cr_fin_controle_zone, CommunicationType::eReportMission );
	messages_[ "DebutSurveillance" ] = new MessageReport( "DebutSurveillance", eRC_DebutSurveillance, T_MsgCR_cr_cr_debut_surveillance, CommunicationType::eReportMission );
	messages_[ "FinSurveillance" ] = new MessageReport( "FinSurveillance", eRC_FinSurveillance, T_MsgCR_cr_cr_fin_surveillance, CommunicationType::eReportMission );
	messages_[ "EnInfiltration" ] = new MessageReport( "EnInfiltration", eRC_EnInfiltration, T_MsgCR_cr_cr_en_infiltration, CommunicationType::eReportMission );
	messages_[ "EnExfiltration" ] = new MessageReport( "EnExfiltration", eRC_EnExfiltration, T_MsgCR_cr_cr_en_exfiltration, CommunicationType::eReportMission );
	messages_[ "AttenteConditionFavorablePourInfiltration" ] = new MessageReport( "AttenteConditionFavorablePourInfiltration", eRC_AttenteConditionFavorablePourInfiltration, T_MsgCR_cr_cr_attente_condition_favorable_pour_infiltration, CommunicationType::eReportMission );
	messages_[ "DroneLance" ] = new MessageReport( "DroneLance", eRC_DroneLance, T_MsgCR_cr_cr_drone_lance, CommunicationType::eReportMission );
	messages_[ "DroneEnExploitation" ] = new MessageReport( "DroneEnExploitation", eRC_DroneEnExploitation, T_MsgCR_cr_cr_drone_en_exploitation, CommunicationType::eReportMission );
	messages_[ "DroneExploite" ] = new MessageReport( "DroneExploite", eRC_DroneExploite, T_MsgCR_cr_cr_drone_exploite, CommunicationType::eReportMission );
	messages_[ "DroneDisponible" ] = new MessageReport( "DroneDisponible", eRC_DroneDisponible, T_MsgCR_cr_cr_drone_disponible, CommunicationType::eReportMission );
	messages_[ "DebutMiseEnOeuvreDrone" ] = new MessageReport( "DebutMiseEnOeuvreDrone", eRC_DebutMiseEnOeuvreDrone, T_MsgCR_cr_cr_debut_mise_en_oeuvre_drone, CommunicationType::eReportMission );
	messages_[ "FinMiseEnOeuvreDrone" ] = new MessageReport( "FinMiseEnOeuvreDrone", eRC_FinMiseEnOeuvreDrone, T_MsgCR_cr_cr_fin_mise_en_oeuvre_drone, CommunicationType::eReportMission );
	messages_[ "ActivationBrouillage" ] = new MessageReport( "ActivationBrouillage", eRC_ActivationBrouillage, T_MsgCR_cr_cr_activation_brouillage, CommunicationType::eReportMission );
	messages_[ "DesactivationBrouillage" ] = new MessageReport( "DesactivationBrouillage", eRC_DesactivationBrouillage, T_MsgCR_cr_cr_desactivation_brouillage, CommunicationType::eReportMission );
	messages_[ "ActivationSystemeEcoute" ] = new MessageReport( "ActivationSystemeEcoute", eRC_ActivationSystemeEcoute, T_MsgCR_cr_cr_activation_systeme_ecoute, CommunicationType::eReportMission );
	messages_[ "DesactivationSystemeEcoute" ] = new MessageReport( "DesactivationSystemeEcoute", eRC_DesactivationSystemeEcoute, T_MsgCR_cr_cr_desactivation_systeme_ecoute, CommunicationType::eReportMission );
	messages_[ "ActivationSystemeEcouteRadar" ] = new MessageReport( "ActivationSystemeEcouteRadar", eRC_ActivationSystemeEcouteRadar, T_MsgCR_cr_cr_activation_systeme_ecoute_radar, CommunicationType::eReportMission );
	messages_[ "DesactivationSystemeEcouteRadar" ] = new MessageReport( "DesactivationSystemeEcouteRadar", eRC_DesactivationSystemeEcouteRadar, T_MsgCR_cr_cr_desactivation_systeme_ecoute_radar, CommunicationType::eReportMission );
	messages_[ "DebutLiaison" ] = new MessageReport( "DebutLiaison", eRC_DebutLiaison, T_MsgCR_cr_cr_debut_liaison, CommunicationType::eReportMission );
	messages_[ "FinLiaison" ] = new MessageReport( "FinLiaison", eRC_FinLiaison, T_MsgCR_cr_cr_fin_liaison, CommunicationType::eReportMission );
	messages_[ "EtablissementLiaisonImpossible" ] = new MessageReport( "EtablissementLiaisonImpossible", eRC_EtablissementLiaisonImpossible, T_MsgCR_cr_cr_etablissement_liaison_impossible, CommunicationType::eReportMission );
	messages_[ "UniteTropDistante" ] = new MessageReport( "UniteTropDistante", eRC_UniteTropDistante, T_MsgCR_cr_cr_unite_trop_distante, CommunicationType::eReportMission );
	messages_[ "EnregistrementDonnes" ] = new MessageReport( "EnregistrementDonnes", eRC_EnregistrementDonnes, T_MsgCR_cr_cr_enregistrement_donnes, CommunicationType::eReportMission );
	messages_[ "ExploitationDonnees" ] = new MessageReport( "ExploitationDonnees", eRC_ExploitationDonnees, T_MsgCR_cr_cr_exploitation_donnees, CommunicationType::eReportMission );
	messages_[ "ExtractionDonnes" ] = new MessageReport( "ExtractionDonnes", eRC_ExtractionDonnes, T_MsgCR_cr_cr_extraction_donnes, CommunicationType::eReportMission );
	messages_[ "Releve" ] = new MessageReport( "Releve", eRC_Releve, T_MsgCR_cr_cr_releve, CommunicationType::eReportMission );
	messages_[ "SuitUnite" ] = new MessageReport( "SuitUnite", eRC_SuitUnite, T_MsgCR_cr_cr_suit_unite, CommunicationType::eReportMission );
	messages_[ "PasseEnAppui" ] = new MessageReport( "PasseEnAppui", eRC_PasseEnAppui, T_MsgCR_cr_cr_passe_en_appui, CommunicationType::eReportMission );
	messages_[ "EnAppui" ] = new MessageReport( "EnAppui", eRC_EnAppui, T_MsgCR_cr_cr_en_appui, CommunicationType::eReportMission );
	messages_[ "NonOperationnel" ] = new MessageReport( "NonOperationnel", eRC_NonOperationnel, T_MsgCR_cr_cr_non_operationnelle, CommunicationType::eReportLogisticSituation );
	messages_[ "Operationnel" ] = new MessageReport( "Operationnel", eRC_Operationnel, T_MsgCR_cr_cr_operationnelle, CommunicationType::eReportLogisticSituation );
	messages_[ "AmbianceVitesse" ] = new MessageReport( "AmbianceVitesse", eRC_AmbianceVitesse, T_MsgCR_cr_cr_ambiance_vitesse, CommunicationType::eReportMission );
	messages_[ "AmbianceSurete" ] = new MessageReport( "AmbianceSurete", eRC_AmbianceSurete, T_MsgCR_cr_cr_ambiance_surete, CommunicationType::eReportMission );
	messages_[ "SurLima" ] = new MessageReport( "SurLima", eRC_SurLima, T_MsgCR_cr_cr_sur_lima, CommunicationType::eReportMission );
	messages_[ "DebutTravaux" ] = new MessageReport( "DebutTravaux", eRC_DebutTravaux, T_MsgCR_cr_cr_debut_travaux, CommunicationType::eReportMission );
	messages_[ "FinTravaux" ] = new MessageReport( "FinTravaux", eRC_FinTravaux, T_MsgCR_cr_cr_fin_travaux, CommunicationType::eReportObstacle );
	messages_[ "ObstacleDegage" ] = new MessageReport( "ObstacleDegage", eRC_ObstacleDegage, T_MsgCR_cr_cr_obstacle_degage, CommunicationType::eReportObstacle );
	messages_[ "OrganisationChantier" ] = new MessageReport( "OrganisationChantier", eRC_OrganisationChantier, T_MsgCR_cr_cr_organisation_chantier, CommunicationType::eReportMission );
	messages_[ "DesorganisationChantier" ] = new MessageReport( "DesorganisationChantier", eRC_DesorganisationChantier, T_MsgCR_cr_cr_desorganisation_chantier, CommunicationType::eReportMission );
	messages_[ "ReconnaissancePositionObstacle" ] = new MessageReport( "ReconnaissancePositionObstacle", eRC_ReconnaissancePositionObstacle, T_MsgCR_cr_cr_reconnaissance_position_obstacle, CommunicationType::eReportMission );
	messages_[ "ContournementRealise" ] = new MessageReport( "ContournementRealise", eRC_ContournementRealise, T_MsgCR_cr_cr_contournement_realise, CommunicationType::eReportObstacle );
	messages_[ "AttenteRenforcement" ] = new MessageReport( "AttenteRenforcement", eRC_AttenteRenforcement, T_MsgCR_cr_cr_attente_renforcement, CommunicationType::eReportMission );
	messages_[ "DebutExploitationSiteFranchissement" ] = new MessageReport( "DebutExploitationSiteFranchissement", eRC_DebutExploitationSiteFranchissement, T_MsgCR_cr_cr_debut_exploitation_site_franchissement, CommunicationType::eReportMission );
	messages_[ "FinExploitationSiteFranchissement" ] = new MessageReport( "FinExploitationSiteFranchissement", eRC_FinExploitationSiteFranchissement, T_MsgCR_cr_cr_fin_exploitation_site_franchissement, CommunicationType::eReportMission );
	messages_[ "ReussiteRenforcement" ] = new MessageReport( "ReussiteRenforcement", eRC_ReussiteRenforcement, T_MsgCR_cr_cr_reussite_renforcement, CommunicationType::eReportMission );
	messages_[ "EchecRenforcement" ] = new MessageReport( "EchecRenforcement", eRC_EchecRenforcement, T_MsgCR_cr_cr_echec_renforcement, CommunicationType::eReportMission );
	messages_[ "ObstacleEnAttenteActivation" ] = new MessageReport( "ObstacleEnAttenteActivation", eRC_ObstacleEnAttenteActivation, T_MsgCR_cr_cr_obstacle_en_attente_activation, CommunicationType::eReportObstacle );
	messages_[ "DebutAmenagementBerges" ] = new MessageReport( "DebutAmenagementBerges", eRC_DebutAmenagementBerges, T_MsgCR_cr_cr_debut_amenagement_berges, CommunicationType::eReportMission );
	messages_[ "FinAmenagementBerges" ] = new MessageReport( "FinAmenagementBerges", eRC_FinAmenagementBerges, T_MsgCR_cr_cr_fin_amenagement_berges, CommunicationType::eReportMission );
	messages_[ "RegimeMaintenanceDelaiDepasse" ] = new MessageReport( "RegimeMaintenanceDelaiDepasse", eRC_RegimeMaintenanceDelaiDepasse, T_MsgCR_cr_cr_regime_maintenance_delai_depasse, CommunicationType::eReportLogisticSituation );
	messages_[ "SectionDeployee" ] = new MessageReport( "SectionDeployee", eRC_SectionDeployee, T_MsgCR_cr_cr_section_deployee, CommunicationType::eReportMission );
	messages_[ "ANouveauDisponibleApresReparation" ] = new MessageReport( "ANouveauDisponibleApresReparation", eRC_ANouveauDisponibleApresReparation, T_MsgCR_cr_cr_a_nouveau_disponible_apres_reparation, CommunicationType::eReportLogisticSituation );
	messages_[ "MaterielRetourDeMaintenance" ] = new MessageReport( "MaterielRetourDeMaintenance", eRC_MaterielRetourDeMaintenance, T_MsgCR_cr_cr_materiel_retour_de_maintenance, CommunicationType::eReportLogisticSituation );
	messages_[ "MaterielRepareSurPlace" ] = new MessageReport( "MaterielRepareSurPlace", eRC_MaterielRepareSurPlace, T_MsgCR_cr_cr_materiel_repare_sur_place, CommunicationType::eReportLogisticSituation );
	messages_[ "DemandeRavitaillementDotations" ] = new MessageReport( "DemandeRavitaillementDotations", eRC_DemandeRavitaillementDotations, T_MsgCR_cr_cr_demande_ravitaillement_dotations, CommunicationType::eReportLogisticSituation );
	messages_[ "DemandeRavitaillementStock" ] = new MessageReport( "DemandeRavitaillementStock", eRC_DemandeRavitaillementStock, T_MsgCR_cr_cr_demande_ravitaillement_stock, CommunicationType::eReportLogisticSituation );
	messages_[ "DemandeEvacuationSanitaire" ] = new MessageReport( "DemandeEvacuationSanitaire", eRC_DemandeEvacuationSanitaire, T_MsgCR_cr_cr_demande_evacuation_sanitaire, CommunicationType::eReportLogisticSituation );
	messages_[ "DemandeEvacuationMateriel" ] = new MessageReport( "DemandeEvacuationMateriel", eRC_DemandeEvacuationMateriel, T_MsgCR_cr_cr_demande_evacuation_materiel, CommunicationType::eReportLogisticSituation );
	messages_[ "AllocationConsentieBientotEpuisee" ] = new MessageReport( "AllocationConsentieBientotEpuisee", eRC_AllocationConsentieBientotEpuisee, T_MsgCR_cr_cr_allocation_consentie_bientot_epuisee, CommunicationType::eReportLogisticSituation );
	messages_[ "DepassementCapaciteStockage" ] = new MessageReport( "DepassementCapaciteStockage", eRC_DepassementCapaciteStockage, T_MsgCR_cr_cr_depassement_capacite_stockage, CommunicationType::eReportLogisticSituation );
	messages_[ "SeuilLogistiqueDotationDepasse" ] = new MessageReport( "SeuilLogistiqueDotationDepasse", eRC_SeuilLogistiqueDotationDepasse, T_MsgCR_cr_cr_seuil_logistique_dotation_depasse, CommunicationType::eReportLogisticSituation );
	messages_[ "SeuilLogistiqueStockDepasse" ] = new MessageReport( "SeuilLogistiqueStockDepasse", eRC_SeuilLogistiqueStockDepasse, T_MsgCR_cr_cr_seuil_logistique_stock_depasse, CommunicationType::eReportLogisticSituation );
	messages_[ "RavitaillementDotationsEffectue" ] = new MessageReport( "RavitaillementDotationsEffectue", eRC_RavitaillementDotationsEffectue, T_MsgCR_cr_cr_ravitaillement_dotations_effectue, CommunicationType::eReportLogisticSituation );
	messages_[ "RavitaillementStockEffectue" ] = new MessageReport( "RavitaillementStockEffectue", eRC_RavitaillementStockEffectue, T_MsgCR_cr_cr_ravitaillement_stock_effectue, CommunicationType::eReportLogisticSituation );
	messages_[ "RavitaillementDotationsAnnule" ] = new MessageReport( "RavitaillementDotationsAnnule", eRC_RavitaillementDotationsAnnule, T_MsgCR_cr_cr_ravitaillement_dotations_annule, CommunicationType::eReportLogisticSituation );
	messages_[ "RavitaillementStockAnnule" ] = new MessageReport( "RavitaillementStockAnnule", eRC_RavitaillementStockAnnule, T_MsgCR_cr_cr_ravitaillement_stock_annule, CommunicationType::eReportLogisticSituation );
	messages_[ "HumainRetourDeSante" ] = new MessageReport( "HumainRetourDeSante", eRC_HumainRetourDeSante, T_MsgCR_cr_cr_humain_retour_de_sante, CommunicationType::eReportLogisticSituation );
	messages_[ "PretMaterielEnCours" ] = new MessageReport( "PretMaterielEnCours", eRC_PretMaterielEnCours, T_MsgCR_cr_cr_pret_materiel_en_cours, CommunicationType::eReportLogisticSituation );
	messages_[ "PretMaterielAnnule" ] = new MessageReport( "PretMaterielAnnule", eRC_PretMaterielAnnule, T_MsgCR_cr_cr_pret_materiel_annule, CommunicationType::eReportLogisticSituation );
	messages_[ "PretMaterielEffectue" ] = new MessageReport( "PretMaterielEffectue", eRC_PretMaterielEffectue, T_MsgCR_cr_cr_pret_materiel_effectue, CommunicationType::eReportLogisticSituation );
	messages_[ "PretMaterielImpossible" ] = new MessageReport( "PretMaterielImpossible", eRC_PretMaterielImpossible, T_MsgCR_cr_cr_pret_materiel_impossible, CommunicationType::eReportLogisticSituation );
	messages_[ "PretMaterielPartiellementEffectue" ] = new MessageReport( "PretMaterielPartiellementEffectue", eRC_PretMaterielPartiellementEffectue, T_MsgCR_cr_cr_pret_materiel_partiellement_effectue, CommunicationType::eReportLogisticSituation );
	messages_[ "RecuperationMaterielPreteImpossible" ] = new MessageReport( "RecuperationMaterielPreteImpossible", eRC_RecuperationMaterielPreteImpossible, T_MsgCR_cr_cr_recuperation_materiel_prete_impossible, CommunicationType::eReportLogisticSituation );
	messages_[ "RecuperationMaterielPreteEffectuee" ] = new MessageReport( "RecuperationMaterielPreteEffectuee", eRC_RecuperationMaterielPreteEffectuee, T_MsgCR_cr_cr_recuperation_materiel_prete_effectuee, CommunicationType::eReportLogisticSituation );
	messages_[ "RecuperationMaterielPretePartiellementEffectuee" ] = new MessageReport( "RecuperationMaterielPretePartiellementEffectuee", eRC_RecuperationMaterielPretePartiellementEffectuee, T_MsgCR_cr_cr_recuperation_materiel_prete_partiellement_effectuee, CommunicationType::eReportLogisticSituation );
	messages_[ "MaterielPrete" ] = new MessageReport( "MaterielPrete", eRC_MaterielPrete, T_MsgCR_cr_cr_materiel_prete, CommunicationType::eReportLogisticSituation );
	messages_[ "MaterielRendu" ] = new MessageReport( "MaterielRendu", eRC_MaterielRendu, T_MsgCR_cr_cr_materiel_rendu, CommunicationType::eReportLogisticSituation );
	messages_[ "Convoi_DeplacementVersPointChargement" ] = new MessageReport( "Convoi_DeplacementVersPointChargement", eRC_Convoi_DeplacementVersPointChargement, T_MsgCR_cr_cr_convoi_deplacement_vers_point_chargement, CommunicationType::eReportLogisticSituation );
	messages_[ "Convoi_ChargementEnCours" ] = new MessageReport( "Convoi_ChargementEnCours", eRC_Convoi_ChargementEnCours, T_MsgCR_cr_cr_convoi_chargement_en_cours, CommunicationType::eReportLogisticSituation );
	messages_[ "Convoi_ChargementEffectue" ] = new MessageReport( "Convoi_ChargementEffectue", eRC_Convoi_ChargementEffectue, T_MsgCR_cr_cr_convoi_chargement_effectue, CommunicationType::eReportLogisticSituation );
	messages_[ "Convoi_DeplacementVersPointDechargement" ] = new MessageReport( "Convoi_DeplacementVersPointDechargement", eRC_Convoi_DeplacementVersPointDechargement, T_MsgCR_cr_cr_convoi_deplacement_vers_point_dechargement, CommunicationType::eReportLogisticSituation );
	messages_[ "Convoi_DechargementEnCours" ] = new MessageReport( "Convoi_DechargementEnCours", eRC_Convoi_DechargementEnCours, T_MsgCR_cr_cr_convoi_dechargement_en_cours, CommunicationType::eReportLogisticSituation );
	messages_[ "Convoi_DechargementEffectue" ] = new MessageReport( "Convoi_DechargementEffectue", eRC_Convoi_DechargementEffectue, T_MsgCR_cr_cr_convoi_dechargement_effectue, CommunicationType::eReportLogisticSituation );
	messages_[ "Convoi_Retour" ] = new MessageReport( "Convoi_Retour", eRC_Convoi_Retour, T_MsgCR_cr_cr_convoi_retour, CommunicationType::eReportLogisticSituation );
	messages_[ "EnCoursRavitaillement" ] = new MessageReport( "EnCoursRavitaillement", eRC_EnCoursRavitaillement, T_MsgCR_cr_cr_en_cours_ravitaillement, CommunicationType::eReportLogisticSituation );
	messages_[ "RavitaillementTermine" ] = new MessageReport( "RavitaillementTermine", eRC_RavitaillementTermine, T_MsgCR_cr_cr_ravitaillement_termine, CommunicationType::eReportLogisticSituation );
	messages_[ "HelicoptereEnObservation" ] = new MessageReport( "HelicoptereEnObservation", eRC_HelicoptereEnObservation, T_MsgCR_cr_cr_helicoptere_en_observation, CommunicationType::eReportMission );
	messages_[ "PerteInfoGuidage" ] = new MessageReport( "PerteInfoGuidage", eRC_PerteInfoGuidage, T_MsgCR_cr_cr_perte_info_guidage, CommunicationType::eReportMission );
	messages_[ "TransportUnitePasPrete" ] = new MessageReport( "TransportUnitePasPrete", eRC_TransportUnitePasPrete, T_MsgCR_cr_cr_transport_unite_pas_prete, CommunicationType::eReportMission );
	messages_[ "TransportUnitePrete" ] = new MessageReport( "TransportUnitePrete", eRC_TransportUnitePrete, T_MsgCR_cr_cr_transport_unite_prete, CommunicationType::eReportMission );
	messages_[ "TransportEnCours" ] = new MessageReport( "TransportEnCours", eRC_TransportEnCours, T_MsgCR_cr_cr_transport_en_cours, CommunicationType::eReportMission );
	messages_[ "TransportEmbarquement" ] = new MessageReport( "TransportEmbarquement", eRC_TransportEmbarquement, T_MsgCR_cr_cr_transport_embarquement, CommunicationType::eReportMission );
	messages_[ "TransportDebarquement" ] = new MessageReport( "TransportDebarquement", eRC_TransportDebarquement, T_MsgCR_cr_cr_transport_debarquement, CommunicationType::eReportMission );
	messages_[ "VehiculeLaisseSurPosition" ] = new MessageReport( "VehiculeLaisseSurPosition", eRC_VehiculeLaisseSurPosition, T_MsgCR_cr_cr_vehicule_laisse_sur_position, CommunicationType::eReportMission );
	messages_[ "RejointPositionEmbarquement" ] = new MessageReport( "RejointPositionEmbarquement", eRC_RejointPositionEmbarquement, T_MsgCR_cr_cr_rejoint_position_embarquement, CommunicationType::eReportMission );
	messages_[ "EnAttenteTransporteurs" ] = new MessageReport( "EnAttenteTransporteurs", eRC_EnAttenteTransporteurs, T_MsgCR_cr_cr_en_attente_transporteurs, CommunicationType::eReportMission );
	messages_[ "ContamineNBC" ] = new MessageReport( "ContamineNBC", eRC_ContamineNBC, T_MsgCR_cr_cr_contamine_nbc, CommunicationType::eReportNbc );
	messages_[ "DecontamineNBC" ] = new MessageReport( "DecontamineNBC", eRC_DecontamineNBC, T_MsgCR_cr_cr_decontamine_nbc, CommunicationType::eReportNbc );
	messages_[ "TenueProtectionNBCMise" ] = new MessageReport( "TenueProtectionNBCMise", eRC_TenueProtectionNBCMise, T_MsgCR_cr_cr_tenue_protection_nbc_mise, CommunicationType::eReportNbc );
	messages_[ "TenueProtectionNBCEnlevee" ] = new MessageReport( "TenueProtectionNBCEnlevee", eRC_TenueProtectionNBCEnlevee, T_MsgCR_cr_cr_tenue_protection_nbc_enlevee, CommunicationType::eReportNbc );
	messages_[ "AlerteNBCNiveau4" ] = new MessageReport( "AlerteNBCNiveau4", eRC_AlerteNBCNiveau4, T_MsgCR_cr_cr_alerte_nbc_niveau_4, CommunicationType::eReportNbc );
	messages_[ "FinAlerteNBCNiveau4" ] = new MessageReport( "FinAlerteNBCNiveau4", eRC_FinAlerteNBCNiveau4, T_MsgCR_cr_cr_fin_alerte_nbc_niveau_4, CommunicationType::eReportNbc );
	messages_[ "SiteDecontaminationAnime" ] = new MessageReport( "SiteDecontaminationAnime", eRC_SiteDecontaminationAnime, T_MsgCR_cr_cr_site_decontamination_anime, CommunicationType::eReportMission );
	messages_[ "SiteDecontaminationDesactive" ] = new MessageReport( "SiteDecontaminationDesactive", eRC_SiteDecontaminationDesactive, T_MsgCR_cr_cr_site_decontamination_desactive, CommunicationType::eReportMission );
	messages_[ "DemandeDecontamination" ] = new MessageReport( "DemandeDecontamination", eRC_DemandeDecontamination, T_MsgCR_cr_cr_demande_decontamination, CommunicationType::eReportNbc );
	messages_[ "ZoneReconnue" ] = new MessageReport( "ZoneReconnue", eRC_ZoneReconnue, T_MsgCR_cr_cr_zone_reconnue, CommunicationType::eReportMission );
	messages_[ "ZoneDecontaminee" ] = new MessageReport( "ZoneDecontaminee", eRC_ZoneDecontaminee, T_MsgCR_cr_cr_zone_decontaminee, CommunicationType::eReportMission );
	messages_[ "SiteROTAReconnu" ] = new MessageReport( "SiteROTAReconnu", eRC_SiteROTAReconnu, T_MsgCR_cr_cr_site_rota_reconnu, CommunicationType::eReportMission );
	messages_[ "IncidentNBCTermine" ] = new MessageReport( "IncidentNBCTermine", eRC_IncidentNBCTermine, T_MsgCR_cr_cr_incident_nbc_termine, CommunicationType::eReportNbc );
	messages_[ "UniteDecontaminee" ] = new MessageReport( "UniteDecontaminee", eRC_UniteDecontaminee, T_MsgCR_cr_cr_unite_decontaminee, CommunicationType::eReportMission );
	messages_[ "SiteSature" ] = new MessageReport( "SiteSature", eRC_SiteSature, T_MsgCR_cr_cr_site_sature, CommunicationType::eReportMission );
	messages_[ "DebutMesure" ] = new MessageReport( "DebutMesure", eRC_DebutMesure, T_MsgCR_cr_cr_debut_mesure, CommunicationType::eReportMission );
	messages_[ "FinMesure" ] = new MessageReport( "FinMesure", eRC_FinMesure, T_MsgCR_cr_cr_fin_mesure, CommunicationType::eReportMission );
	messages_[ "ReconnaissanceTerminee" ] = new MessageReport( "ReconnaissanceTerminee", eRC_ReconnaissanceTerminee, T_MsgCR_cr_cr_reconnaissance_terminee, CommunicationType::eReportMission );
	messages_[ "ReconnaissanceInitialeTerminee" ] = new MessageReport( "ReconnaissanceInitialeTerminee", eRC_ReconnaissanceInitialeTerminee, T_MsgCR_cr_cr_reconnaissance_initiale_terminee, CommunicationType::eReportMission );
	messages_[ "RejointPlotRavitaillement" ] = new MessageReport( "RejointPlotRavitaillement", eRC_RejointPlotRavitaillement, T_MsgCR_cr_cr_rejoint_plot_ravitaillement, CommunicationType::eReportMission );
	messages_[ "PremierCoupParti" ] = new MessageReport( "PremierCoupParti", eRC_PremierCoupParti, T_MsgCR_cr_cr_premier_coup_parti, CommunicationType::eReportArtillery );
	messages_[ "TirExecute" ] = new MessageReport( "TirExecute", eRC_TirExecute, T_MsgCR_cr_cr_tir_execute, CommunicationType::eReportArtillery );
	messages_[ "TirHorsDePortee" ] = new MessageReport( "TirHorsDePortee", eRC_TirHorsDePortee, T_MsgCR_cr_cr_tir_hors_de_portee, CommunicationType::eReportArtillery );
	messages_[ "TirImpossiblePlusDeMunitions" ] = new MessageReport( "TirImpossiblePlusDeMunitions", eRC_TirImpossiblePlusDeMunitions, T_MsgCR_cr_cr_tir_impossible_plus_de_munitions, CommunicationType::eReportArtillery );
	messages_[ "TirAnnule" ] = new MessageReport( "TirAnnule", eRC_TirAnnule, T_MsgCR_cr_cr_tir_annule, CommunicationType::eReportArtillery );
	messages_[ "ChangementDePosition" ] = new MessageReport( "ChangementDePosition", eRC_ChangementDePosition, T_MsgCR_cr_cr_changement_de_position, CommunicationType::eReportMission );
	messages_[ "DebutInstallation" ] = new MessageReport( "DebutInstallation", eRC_DebutInstallation, T_MsgCR_cr_cr_debut_installation, CommunicationType::eReportMission );
	messages_[ "FinInstallation" ] = new MessageReport( "FinInstallation", eRC_FinInstallation, T_MsgCR_cr_cr_fin_installation, CommunicationType::eReportMission );
	messages_[ "PretPourConduiteTir" ] = new MessageReport( "PretPourConduiteTir", eRC_PretPourConduiteTir, T_MsgCR_cr_cr_pret_pour_conduite_tir, CommunicationType::eReportArtillery );
	messages_[ "ObservationTirIndirect" ] = new MessageReport( "ObservationTirIndirect", eRC_ObservationTirIndirect, T_MsgCR_cr_cr_observation_tir_indirect, CommunicationType::eReportArtillery );
	messages_[ "ZoneImplantationOccupee" ] = new MessageReport( "ZoneImplantationOccupee", eRC_ZoneImplantationOccupee, T_MsgCR_cr_cr_zone_implantation_occupee, CommunicationType::eReportMission );
	messages_[ "InstallationSurZoneImplantation" ] = new MessageReport( "InstallationSurZoneImplantation", eRC_InstallationSurZoneImplantation, T_MsgCR_cr_cr_installation_sur_zone_implantation, CommunicationType::eReportMission );
	messages_[ "ReceptionOrdreDeFeu" ] = new MessageReport( "ReceptionOrdreDeFeu", eRC_ReceptionOrdreDeFeu, T_MsgCR_cr_cr_reception_ordre_de_feu, CommunicationType::eReportArtillery );
	messages_[ "PretAuTir" ] = new MessageReport( "PretAuTir", eRC_PretAuTir, T_MsgCR_cr_cr_pret_au_tir, CommunicationType::eReportArtillery );
	messages_[ "PresencePopulation" ] = new MessageReport( "PresencePopulation", eRC_PresencePopulation, T_MsgCR_cr_cr_presence_population, CommunicationType::eReportEnemy );
	messages_[ "OrientationPopulationVersCampRefugies" ] = new MessageReport( "OrientationPopulationVersCampRefugies", eRC_OrientationPopulationVersCampRefugies, T_MsgCR_cr_cr_orientation_population_vers_camp_refugies, CommunicationType::eReportEnemy );
	messages_[ "PasDeCampRefugiesDisponible" ] = new MessageReport( "PasDeCampRefugiesDisponible", eRC_PasDeCampRefugiesDisponible, T_MsgCR_cr_cr_pas_de_camp_refugies_disponible, CommunicationType::eReportEnemy );
	messages_[ "PresenceEnnemiRendu" ] = new MessageReport( "PresenceEnnemiRendu", eRC_PresenceEnnemiRendu, T_MsgCR_cr_cr_presence_ennemi_rendu, CommunicationType::eReportEnemy );
	messages_[ "OrientationEnnemiRenduVersCampPrisonniers" ] = new MessageReport( "OrientationEnnemiRenduVersCampPrisonniers", eRC_OrientationEnnemiRenduVersCampPrisonniers, T_MsgCR_cr_cr_orientation_ennemi_rendu_vers_camp_prisonniers, CommunicationType::eReportEnemy );
	messages_[ "PasDeCampPrisonniersDisponible" ] = new MessageReport( "PasDeCampPrisonniersDisponible", eRC_PasDeCampPrisonniersDisponible, T_MsgCR_cr_cr_pas_de_camp_prisonniers_disponible, CommunicationType::eReportEnemy );
	messages_[ "Rendu" ] = new MessageReport( "Rendu", eRC_Rendu, T_MsgCR_cr_cr_rendu, CommunicationType::eReportMission );
	messages_[ "TirDansZoneInterdite" ] = new MessageReport( "TirDansZoneInterdite", eRC_TirDansZoneInterdite, T_MsgCR_cr_cr_tir_dans_zone_interdite, CommunicationType::eReportEnemy );
	messages_[ "TirSurCampAmi" ] = new MessageReport( "TirSurCampAmi", eRC_TirSurCampAmi, T_MsgCR_cr_cr_tir_sur_camp_ami, CommunicationType::eReportEnemy );
	messages_[ "TireParCampAmi" ] = new MessageReport( "TireParCampAmi", eRC_TireParCampAmi, T_MsgCR_cr_cr_tire_par_camp_ami, CommunicationType::eReportEnemy );
	messages_[ "TirSurCampNeutre" ] = new MessageReport( "TirSurCampNeutre", eRC_TirSurCampNeutre, T_MsgCR_cr_cr_tir_sur_camp_neutre, CommunicationType::eReportEnemy );
	messages_[ "TireParCampNeutre" ] = new MessageReport( "TireParCampNeutre", eRC_TireParCampNeutre, T_MsgCR_cr_cr_tire_par_camp_neutre, CommunicationType::eReportEnemy );
	messages_[ "TirSurCivil" ] = new MessageReport( "TirSurCivil", eRC_TirSurCivil, T_MsgCR_cr_cr_tir_sur_civil, CommunicationType::eReportEnemy );
	messages_[ "TireParCivil" ] = new MessageReport( "TireParCivil", eRC_TireParCivil, T_MsgCR_cr_cr_tire_par_civil, CommunicationType::eReportEnemy );
	messages_[ "DestructionPC" ] = new MessageReport( "DestructionPC", eRC_DestructionPC, T_MsgCR_cr_cr_destruction_pc, CommunicationType::eReportEnemy );
	messages_[ "DecesBlesse" ] = new MessageReport( "DecesBlesse", eRC_DecesBlesse, T_MsgCR_cr_cr_deces_blesse, CommunicationType::eReportLogisticSituation );
	messages_[ "DecesBlessePendantTransport" ] = new MessageReport( "DecesBlessePendantTransport", eRC_DecesBlessePendantTransport, T_MsgCR_cr_cr_deces_blesse_pendant_transport, CommunicationType::eReportLogisticSituation );
	messages_[ "DecesBlessePendantHospitalisation" ] = new MessageReport( "DecesBlessePendantHospitalisation", eRC_DecesBlessePendantHospitalisation, T_MsgCR_cr_cr_deces_blesse_pendant_hospitalisation, CommunicationType::eReportLogisticSituation );
	messages_[ "MissionImpossibleUnitesSubordonneesNonOperationnelles" ] = new MessageReport( "MissionImpossibleUnitesSubordonneesNonOperationnelles", eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles, T_MsgCR_cr_cr_mission_impossible_unites_subordonnees_non_operationnelles, CommunicationType::eReportScipio );
	messages_[ "MissionImpossibleLimaInvalide" ] = new MessageReport( "MissionImpossibleLimaInvalide", eRC_MissionImpossibleLimaInvalide, T_MsgCR_cr_cr_mission_impossible_lima_invalide, CommunicationType::eReportScipio );
	messages_[ "MissionImpossibleZoneHorsFuseau" ] = new MessageReport( "MissionImpossibleZoneHorsFuseau", eRC_MissionImpossibleZoneHorsFuseau, T_MsgCR_cr_cr_mission_impossible_zone_hors_fuseau, CommunicationType::eReportScipio );
	messages_[ "RapportDeForceDefavorable" ] = new MessageReport( "RapportDeForceDefavorable", eRC_RapportDeForceDefavorable, T_MsgCR_cr_cr_rapport_de_force_defavorable, CommunicationType::eReportEnemy );
	messages_[ "TerrainDifficile" ] = new MessageReport( "TerrainDifficile", eRC_TerrainDifficile, T_MsgCR_cr_cr_terrain_difficile, CommunicationType::eReportScipio );
	messages_[ "PlusDeCarburant" ] = new MessageReport( "PlusDeCarburant", eRC_PlusDeCarburant, T_MsgCR_cr_cr_plus_de_carburant, CommunicationType::eReportLogisticSituation );
	messages_[ "DispositifDisperse" ] = new MessageReport( "DispositifDisperse", eRC_DispositifDisperse, T_MsgCR_cr_cr_dispositif_disperse, CommunicationType::eReportScipio );
	messages_[ "DebutNeutralisation" ] = new MessageReport( "DebutNeutralisation", eRC_DebutNeutralisation, T_MsgCR_cr_cr_debut_neutralisation, CommunicationType::eReportEnemy );
	messages_[ "FinNeutralisation" ] = new MessageReport( "FinNeutralisation", eRC_FinNeutralisation, T_MsgCR_cr_cr_fin_neutralisation, CommunicationType::eReportEnemy );
	messages_[ "DebutSilenceRadio" ] = new MessageReport( "DebutSilenceRadio", eRC_DebutSilenceRadio, T_MsgCR_cr_cr_debut_silence_radio, CommunicationType::eReportEnemy );
	messages_[ "FinSilenceRadio" ] = new MessageReport( "FinSilenceRadio", eRC_FinSilenceRadio, T_MsgCR_cr_cr_fin_silence_radio, CommunicationType::eReportEnemy );
	messages_[ "DebutSilenceRadar" ] = new MessageReport( "DebutSilenceRadar", eRC_DebutSilenceRadar, T_MsgCR_cr_cr_debut_silence_radar, CommunicationType::eReportEnemy );
	messages_[ "FinSilenceRadar" ] = new MessageReport( "FinSilenceRadar", eRC_FinSilenceRadar, T_MsgCR_cr_cr_fin_silence_radar, CommunicationType::eReportEnemy );
	messages_[ "TirRestreint" ] = new MessageReport( "TirRestreint", eRC_TirRestreint, T_MsgCR_cr_cr_tir_restreint, CommunicationType::eReportEnemy );
	messages_[ "TirInterdit" ] = new MessageReport( "TirInterdit", eRC_TirInterdit, T_MsgCR_cr_cr_tir_interdit, CommunicationType::eReportEnemy );
	messages_[ "TirLibre" ] = new MessageReport( "TirLibre", eRC_TirLibre, T_MsgCR_cr_cr_tir_libre, CommunicationType::eReportEnemy );
	messages_[ "UniteDetectee" ] = new MessageReport( "UniteDetectee", eRC_UniteDetectee, T_MsgCR_cr_cr_unite_detectee, CommunicationType::eReportEnemy );
	messages_[ "UniteAmieReconnue" ] = new MessageReport( "UniteAmieReconnue", eRC_UniteAmieReconnue, T_MsgCR_cr_cr_unite_amie_reconnue, CommunicationType::eReportEnemy );
	messages_[ "UniteEnnemieReconnue" ] = new MessageReport( "UniteEnnemieReconnue", eRC_UniteEnnemieReconnue, T_MsgCR_cr_cr_unite_ennemie_reconnue, CommunicationType::eReportEnemy );
	messages_[ "UniteNeutreReconnue" ] = new MessageReport( "UniteNeutreReconnue", eRC_UniteNeutreReconnue, T_MsgCR_cr_cr_unite_neutre_reconnue, CommunicationType::eReportEnemy );
	messages_[ "UniteAmieIdentifiee" ] = new MessageReport( "UniteAmieIdentifiee", eRC_UniteAmieIdentifiee, T_MsgCR_cr_cr_unite_amie_identifiee, CommunicationType::eReportEnemy );
	messages_[ "UniteEnnemieIdentifiee" ] = new MessageReport( "UniteEnnemieIdentifiee", eRC_UniteEnnemieIdentifiee, T_MsgCR_cr_cr_unite_ennemie_identifiee, CommunicationType::eReportEnemy );
	messages_[ "UniteNeutreIdentifiee" ] = new MessageReport( "UniteNeutreIdentifiee", eRC_UniteNeutreIdentifiee, T_MsgCR_cr_cr_unite_neutre_identifiee, CommunicationType::eReportEnemy );
	messages_[ "ObjetDetecte" ] = new MessageReport( "ObjetDetecte", eRC_ObjetDetecte, T_MsgCR_cr_cr_objet_detecte, CommunicationType::eReportObstacle );
	messages_[ "Trace" ] = new MessageReport( "Trace", eRC_Trace, T_MsgCR_cr_cr_trace, CommunicationType::eReportScipio );
	messages_[ "TraceID" ] = new MessageReport( "TraceID", eRC_TraceID, T_MsgCR_cr_cr_trace_id, CommunicationType::eReportScipio );
	messages_[ "EmploiForceInterdit" ] = new MessageReport( "EmploiForceInterdit", eRC_EmploiForceInterdit, T_MsgCR_cr_cr_emploi_force_interdit, CommunicationType::eReportMission );
	messages_[ "MaintienADistanceParMoyensNonLetaux" ] = new MessageReport( "MaintienADistanceParMoyensNonLetaux", eRC_MaintienADistanceParMoyensNonLetaux, T_MsgCR_cr_cr_maintien_a_distance_par_moyens_non_letaux, CommunicationType::eReportMission );
	messages_[ "DispersionParMoyensDeDefenseActifs" ] = new MessageReport( "DispersionParMoyensDeDefenseActifs", eRC_DispersionParMoyensDeDefenseActifs, T_MsgCR_cr_cr_dispersion_par_moyens_de_defense_actifs, CommunicationType::eReportMission );
	messages_[ "ArmesLetalesAutorisees" ] = new MessageReport( "ArmesLetalesAutorisees", eRC_ArmesLetalesAutorisees, T_MsgCR_cr_cr_armes_letales_autorisees, CommunicationType::eReportMission );
	messages_[ "PopulationDetectee" ] = new MessageReport( "PopulationDetectee", eRC_PopulationDetectee, T_MsgCR_cr_cr_population_detectee, CommunicationType::eReportMission );
	messages_[ "PriseAPartieParPopulation" ] = new MessageReport( "PriseAPartieParPopulation", eRC_PriseAPartieParPopulation, T_MsgCR_cr_cr_prise_a_partie_par_population, CommunicationType::eReportMission );
	messages_[ "RiposteContrePopulation" ] = new MessageReport( "RiposteContrePopulation", eRC_RiposteContrePopulation, T_MsgCR_cr_cr_riposte_contre_population, CommunicationType::eReportMission );
	messages_[ "PopulationDangereuseAProximite" ] = new MessageReport( "PopulationDangereuseAProximite", eRC_PopulationDangereuseAProximite, T_MsgCR_cr_cr_population_dangereuse_a_proximite, CommunicationType::eReportMission );
	messages_[ "EsquiveFaceAPopulation" ] = new MessageReport( "EsquiveFaceAPopulation", eRC_EsquiveFaceAPopulation, T_MsgCR_cr_cr_esquive_face_a_population, CommunicationType::eReportMission );
	messages_[ "DecrochageFaceAPopulation" ] = new MessageReport( "DecrochageFaceAPopulation", eRC_DecrochageFaceAPopulation, T_MsgCR_cr_cr_decrochage_face_a_population, CommunicationType::eReportMission );
	messages_[ "MiseEnPlaceControleDeFoule" ] = new MessageReport( "MiseEnPlaceControleDeFoule", eRC_MiseEnPlaceControleDeFoule, T_MsgCR_cr_cr_mise_en_place_controle_de_foule, CommunicationType::eReportMission );
	messages_[ "MiseEnPlaceFiltrage" ] = new MessageReport( "MiseEnPlaceFiltrage", eRC_MiseEnPlaceFiltrage, T_MsgCR_cr_cr_mise_en_place_filtrage, CommunicationType::eReportMission );
	messages_[ "AnimationBarrage" ] = new MessageReport( "AnimationBarrage", eRC_AnimationBarrage, T_MsgCR_cr_cr_animation_barrage, CommunicationType::eReportMission );
	messages_[ "AnimationCanalisation" ] = new MessageReport( "AnimationCanalisation", eRC_AnimationCanalisation, T_MsgCR_cr_cr_animation_canalisation, CommunicationType::eReportMission );
	messages_[ "CollecteRenseignementsSurPopulations" ] = new MessageReport( "CollecteRenseignementsSurPopulations", eRC_CollecteRenseignementsSurPopulations, T_MsgCR_cr_cr_collecte_renseignements_sur_populations, CommunicationType::eReportMission );
	messages_[ "PopulationReconnue" ] = new MessageReport( "PopulationReconnue", eRC_PopulationReconnue, T_MsgCR_cr_cr_population_reconnue, CommunicationType::eReportMission );
	messages_[ "AttitudeCalme" ] = new MessageReport( "AttitudeCalme", eRC_AttitudeCalme, T_MsgCR_cr_cr_attitude_calme, CommunicationType::eReportMission );
	messages_[ "AttitudeAgitee" ] = new MessageReport( "AttitudeAgitee", eRC_AttitudeAgitee, T_MsgCR_cr_cr_attitude_agitee, CommunicationType::eReportMission );
	messages_[ "AttitudeExcitee" ] = new MessageReport( "AttitudeExcitee", eRC_AttitudeExcitee, T_MsgCR_cr_cr_attitude_excitee, CommunicationType::eReportMission );
	messages_[ "AttitudeAgressive" ] = new MessageReport( "AttitudeAgressive", eRC_AttitudeAgressive, T_MsgCR_cr_cr_attitude_agressive, CommunicationType::eReportMission );
	messages_[ "AggravationAttitude" ] = new MessageReport( "AggravationAttitude", eRC_AggravationAttitude, T_MsgCR_cr_cr_aggravation_attitude, CommunicationType::eReportMission );
	messages_[ "AffrontementAvecForcesEnnemies" ] = new MessageReport( "AffrontementAvecForcesEnnemies", eRC_AffrontementAvecForcesEnnemies, T_MsgCR_cr_cr_affrontement_avec_forces_ennemies, CommunicationType::eReportMission );
	messages_[ "PopulationTemporairementControlee" ] = new MessageReport( "PopulationTemporairementControlee", eRC_PopulationTemporairementControlee, T_MsgCR_cr_cr_population_temporairement_controlee, CommunicationType::eReportMission );
	messages_[ "PopulationTemporairementRepoussee" ] = new MessageReport( "PopulationTemporairementRepoussee", eRC_PopulationTemporairementRepoussee, T_MsgCR_cr_cr_population_temporairement_repoussee, CommunicationType::eReportMission );
	messages_[ "RepriseDesAffrontements" ] = new MessageReport( "RepriseDesAffrontements", eRC_RepriseDesAffrontements, T_MsgCR_cr_cr_reprise_des_affrontements, CommunicationType::eReportMission );
	messages_[ "AgressionDePopulation" ] = new MessageReport( "AgressionDePopulation", eRC_AgressionDePopulation, T_MsgCR_cr_cr_agression_de_population, CommunicationType::eReportMission );
	messages_[ "AgressionParPopulation" ] = new MessageReport( "AgressionParPopulation", eRC_AgressionParPopulation, T_MsgCR_cr_cr_agression_par_population, CommunicationType::eReportMission );
	messages_[ "RiposteContreAgressionParPopulation" ] = new MessageReport( "RiposteContreAgressionParPopulation", eRC_RiposteContreAgressionParPopulation, T_MsgCR_cr_cr_riposte_contre_agression_par_population, CommunicationType::eReportMission );
	messages_[ "AttaqueInstallation" ] = new MessageReport( "AttaqueInstallation", eRC_AttaqueInstallation, T_MsgCR_cr_cr_attaque_installation, CommunicationType::eReportMission );
	messages_[ "EnStationnement" ] = new MessageReport( "EnStationnement", eRC_EnStationnement, T_MsgCR_cr_cr_en_stationnement, CommunicationType::eReportMission );
	messages_[ "ExecutionAttentat" ] = new MessageReport( "ExecutionAttentat", eRC_ExecutionAttentat, T_MsgCR_cr_cr_execution_attentat, CommunicationType::eReportMission );
	
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