// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Rc/MIL_RC.cpp $
// $Author: Nld $
// $Modtime: 4/07/05 16:03 $
// $Revision: 18 $
// $Workfile: MIL_RC.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC.h"

#include "MIL_RC_EnPosteFaceAObstacle.h"
#include "MIL_RC_PreparationMission.h"
#include "MIL_RC_SurPIA.h"
#include "MIL_RC_UniteRecueillie.h"
#include "MIL_RC_PrecisionContact.h"
#include "MIL_RC_EnnemiDetruit.h"
#include "MIL_RC_PointTenuParEni.h"
#include "MIL_RC_ProgressionVersEni.h"
#include "MIL_RC_EniSurObjectif.h"
#include "MIL_RC_Releve.h"
#include "MIL_RC_SurLima.h"
#include "MIL_RC_ReussiteRenforcement.h"
#include "MIL_RC_EchecRenforcement.h"
#include "MIL_RC_ObstacleEnAttenteActivation.h"
#include "MIL_RC_AllocationConsentieBientotEpuisee.h"
#include "MIL_RC_TransportUnitePasPrete.h"
#include "MIL_RC_TransportUnitePrete.h"
#include "MIL_RC_UniteDecontaminee.h"
#include "MIL_RC_ObservationTirIndirect.h"
#include "MIL_RC_TirSurCampAmi.h"
#include "MIL_RC_TireParCampAmi.h"
#include "MIL_RC_TirSurCampNeutre.h"
#include "MIL_RC_TireParCampNeutre.h"
#include "MIL_RC_TirSurCivil.h"
#include "MIL_RC_TireParCivil.h"
#include "MIL_RC_UniteDetectee.h"
#include "MIL_RC_UniteAmieReconnue.h"
#include "MIL_RC_UniteEnnemieReconnue.h"
#include "MIL_RC_UniteNeutreReconnue.h"
#include "MIL_RC_UniteAmieIdentifiee.h"
#include "MIL_RC_UniteEnnemieIdentifiee.h"
#include "MIL_RC_UniteNeutreIdentifiee.h"
#include "MIL_RC_ObjetDetecte.h"
#include "MIL_RC_Trace.h"
#include "MIL_RC_TraceID.h"
#include "MIL_RC_PopulationDetectee.h"
#include "MIL_RC_PriseAPartieParPopulation.h"
#include "MIL_RC_RiposteSurPopulation.h"
#include "MIL_RC_RecuperationProgressive.h"
#include "MIL_RC_AgressionDePopulation.h"
#include "MIL_RC_AgressionParPopulation.h"
#include "MIL_RC_RiposteContreAgressionParPopulation.h"
#include "MIL_RC_AttaqueInstallation.h"


#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"

MIL_RC::T_RCMap MIL_RC::rcs_;

// Statics
const MIL_RC*                                   MIL_RC::pRcMissionImpossible_                  = 0;
const MIL_RC*                                   MIL_RC::pRcEnCoursDeFranchissement_            = 0;
const MIL_RC*                                   MIL_RC::pRcANouveauDisponibleApresReparation_  = 0;
const MIL_RC*                                   MIL_RC::pRcMaterielRepareSurPlace_             = 0;
const MIL_RC*                                   MIL_RC::pRcMaterielRetourDeMaintenance_        = 0;
const MIL_RC_ObservationTirIndirect*            MIL_RC::pRcObservationTirIndirect_             = 0;
const MIL_RC*                                   MIL_RC::pRcDemandeRavitaillement_              = 0;
const MIL_RC*                                   MIL_RC::pRcDemandeEvacuationSanitaire_         = 0;
const MIL_RC*                                   MIL_RC::pRcDemandeEvacuationMateriel_          = 0;
const MIL_RC*                                   MIL_RC::pRcDecesBlesse_                        = 0;
const MIL_RC*                                   MIL_RC::pRcDecesBlessePendantTransport_        = 0;
const MIL_RC*                                   MIL_RC::pRcDecesBlessePendantHospitalisation_  = 0;
const MIL_RC*                                   MIL_RC::pRcTirDansZoneInterdite_               = 0;
const MIL_RC_TirSurCampAmi*                     MIL_RC::pRcTirSurCampAmi_                      = 0;
const MIL_RC_TireParCampAmi*                    MIL_RC::pRcTireParCampAmi_                     = 0;
const MIL_RC_TirSurCampNeutre*                  MIL_RC::pRcTirSurCampNeutre_                   = 0;
const MIL_RC_TireParCampNeutre*                 MIL_RC::pRcTireParCampNeutre_                  = 0;
const MIL_RC_TirSurCivil*                       MIL_RC::pRcTirSurCivil_                        = 0;
const MIL_RC_TireParCivil*                      MIL_RC::pRcTireParCivil_                       = 0;
const MIL_RC*                                   MIL_RC::pRcDestructionPC_                      = 0;
const MIL_RC_UniteDecontaminee*                 MIL_RC::pRcUniteDecontaminee_                  = 0;
const MIL_RC_AllocationConsentieBientotEpuisee* MIL_RC::pRcAllocationConsentieBientotEpuisee_  = 0;
const MIL_RC*                                   MIL_RC::pRcPlusDeCarburant_                    = 0;
const MIL_RC*                                   MIL_RC::pRcTerrainDifficile_                   = 0;
const MIL_RC*                                   MIL_RC::pRcRavitaillementDotationsEffectue_    = 0;
const MIL_RC*                                   MIL_RC::pRcRavitaillementStockEffectue_        = 0;
const MIL_RC*                                   MIL_RC::pRcHumainRetourDeSante_                = 0;
const MIL_RC_UniteDetectee*                     MIL_RC::pRcUniteDetectee_                      = 0;
const MIL_RC_UniteAmieReconnue*                 MIL_RC::pRcUniteAmieReconnue_                  = 0;
const MIL_RC_UniteEnnemieReconnue*              MIL_RC::pRcUniteEnnemieReconnue_               = 0;
const MIL_RC_UniteNeutreReconnue*               MIL_RC::pRcUniteNeutreReconnue_                = 0;
const MIL_RC_UniteAmieIdentifiee*               MIL_RC::pRcUniteAmieIdentifiee_                = 0;
const MIL_RC_UniteEnnemieIdentifiee*            MIL_RC::pRcUniteEnnemieIdentifiee_             = 0;
const MIL_RC_UniteNeutreIdentifiee*             MIL_RC::pRcUniteNeutreIdentifiee_              = 0;
const MIL_RC_ObjetDetecte*                      MIL_RC::pRcObjetDetecte_                       = 0;
const MIL_RC*                                   MIL_RC::pRcRendu_                              = 0;

// -----------------------------------------------------------------------------
// Name: MIL_RC::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing RCs" );

    rcs_[ eRC_RAS ] = new MIL_RC( eRC_RAS, T_MsgCR_cr_cr_ras );
	rcs_[ eRC_EnPosteFaceAObstacle ] = new MIL_RC_EnPosteFaceAObstacle( eRC_EnPosteFaceAObstacle, T_MsgCR_cr_cr_en_poste_face_a_obstacle );
	rcs_[ eRC_EnContournementObstacle ] = new MIL_RC( eRC_EnContournementObstacle, T_MsgCR_cr_cr_en_contournement_obstacle );
	rcs_[ eRC_MissionImpossible ] = new MIL_RC( eRC_MissionImpossible, T_MsgCR_cr_cr_mission_impossible );
	rcs_[ eRC_ProgressionSurAxe ] = new MIL_RC( eRC_ProgressionSurAxe, T_MsgCR_cr_cr_progression_sur_axe );
	rcs_[ eRC_ProgressionVersPosition ] = new MIL_RC( eRC_ProgressionVersPosition, T_MsgCR_cr_cr_progression_vers_position );
	rcs_[ eRC_ArriveSurPosition ] = new MIL_RC( eRC_ArriveSurPosition, T_MsgCR_cr_cr_arrive_sur_position );
	rcs_[ eRC_ReconnaissancePoint ] = new MIL_RC( eRC_ReconnaissancePoint, T_MsgCR_cr_cr_reconnaissance_point );
	rcs_[ eRC_EnPosteFaceADirectionDangereuse ] = new MIL_RC( eRC_EnPosteFaceADirectionDangereuse, T_MsgCR_cr_cr_en_poste_face_a_direction_dangereuse );
	rcs_[ eRC_EnJalonnement ] = new MIL_RC( eRC_EnJalonnement, T_MsgCR_cr_cr_en_jalonnement );
	rcs_[ eRC_PreparationMission ] = new MIL_RC_PreparationMission( eRC_PreparationMission, T_MsgCR_cr_cr_preparation_mission );
	rcs_[ eRC_DebutMission ] = new MIL_RC( eRC_DebutMission, T_MsgCR_cr_cr_debut_mission );
	rcs_[ eRC_FinMission ] = new MIL_RC( eRC_FinMission, T_MsgCR_cr_cr_fin_mission );
	rcs_[ eRC_DispositifInstalle ] = new MIL_RC( eRC_DispositifInstalle, T_MsgCR_cr_cr_dispositif_installe );
	rcs_[ eRC_EnPosition ] = new MIL_RC( eRC_EnPosition, T_MsgCR_cr_cr_en_position );
	rcs_[ eRC_ObjectifAtteint ] = new MIL_RC( eRC_ObjectifAtteint, T_MsgCR_cr_cr_objectif_atteint );
	rcs_[ eRC_Debouche ] = new MIL_RC( eRC_Debouche, T_MsgCR_cr_cr_debouche );
	rcs_[ eRC_RejointUnite ] = new MIL_RC( eRC_RejointUnite, T_MsgCR_cr_cr_rejoint_unite );
	rcs_[ eRC_EscorteUnite ] = new MIL_RC( eRC_EscorteUnite, T_MsgCR_cr_cr_escorte_unite );
	rcs_[ eRC_EnFreinage ] = new MIL_RC( eRC_EnFreinage, T_MsgCR_cr_cr_en_freinage );
	rcs_[ eRC_Assaut ] = new MIL_RC( eRC_Assaut, T_MsgCR_cr_cr_assaut );
	rcs_[ eRC_EnCouverture ] = new MIL_RC( eRC_EnCouverture, T_MsgCR_cr_cr_en_couverture );
	rcs_[ eRC_EnEclairage ] = new MIL_RC( eRC_EnEclairage, T_MsgCR_cr_cr_en_eclairage );
	rcs_[ eRC_EnManoeuvreRavitaillement ] = new MIL_RC( eRC_EnManoeuvreRavitaillement, T_MsgCR_cr_cr_en_manoeuvre_ravitaillement );
	rcs_[ eRC_SurPIA ] = new MIL_RC_SurPIA( eRC_SurPIA, T_MsgCR_cr_cr_sur_pia );
	rcs_[ eRC_PassageSurPIA ] = new MIL_RC( eRC_PassageSurPIA, T_MsgCR_cr_cr_passage_sur_pia );
	rcs_[ eRC_UniteRecueillie ] = new MIL_RC_UniteRecueillie( eRC_UniteRecueillie, T_MsgCR_cr_cr_unite_recueillie );
	rcs_[ eRC_EnCoursDeFranchissement ] = new MIL_RC( eRC_EnCoursDeFranchissement, T_MsgCR_cr_cr_en_cours_de_franchissement );
	rcs_[ eRC_AttenteOrdrePoursuivre ] = new MIL_RC( eRC_AttenteOrdrePoursuivre, T_MsgCR_cr_cr_attente_ordre_poursuivre );
	rcs_[ eRC_AttenteOrdreDeboucher ] = new MIL_RC( eRC_AttenteOrdreDeboucher, T_MsgCR_cr_cr_attente_ordre_deboucher );
	rcs_[ eRC_Fixe ] = new MIL_RC( eRC_Fixe, T_MsgCR_cr_cr_fixe );
	rcs_[ eRC_EnEsquive ] = new MIL_RC( eRC_EnEsquive, T_MsgCR_cr_cr_en_esquive );
	rcs_[ eRC_PriseContact ] = new MIL_RC( eRC_PriseContact, T_MsgCR_cr_cr_prise_contact );
	rcs_[ eRC_RuptureContact ] = new MIL_RC( eRC_RuptureContact, T_MsgCR_cr_cr_rupture_contact );
	rcs_[ eRC_PrecisionContact ] = new MIL_RC_PrecisionContact( eRC_PrecisionContact, T_MsgCR_cr_cr_precision_contact );
	rcs_[ eRC_EnnemiDetruit ] = new MIL_RC_EnnemiDetruit( eRC_EnnemiDetruit, T_MsgCR_cr_cr_ennemi_detruit );
	rcs_[ eRC_PrisAPartieParTirDirect ] = new MIL_RC( eRC_PrisAPartieParTirDirect, T_MsgCR_cr_cr_pris_a_partie_par_tir_direct );
	rcs_[ eRC_PrisSousTirArtillerie ] = new MIL_RC( eRC_PrisSousTirArtillerie, T_MsgCR_cr_cr_pris_sous_tir_artillerie );
	rcs_[ eRC_PointTenuParEni ] = new MIL_RC_PointTenuParEni( eRC_PointTenuParEni, T_MsgCR_cr_cr_point_tenu_par_eni );
	rcs_[ eRC_ProgressionVersEni ] = new MIL_RC_ProgressionVersEni( eRC_ProgressionVersEni, T_MsgCR_cr_cr_progression_vers_eni );
	rcs_[ eRC_Decroche ] = new MIL_RC( eRC_Decroche, T_MsgCR_cr_cr_decroche );
	rcs_[ eRC_EniSurObjectif ] = new MIL_RC_EniSurObjectif( eRC_EniSurObjectif, T_MsgCR_cr_cr_eni_sur_objectif );
	rcs_[ eRC_Riposte ] = new MIL_RC( eRC_Riposte, T_MsgCR_cr_cr_riposte );
	rcs_[ eRC_DebutControleZone ] = new MIL_RC( eRC_DebutControleZone, T_MsgCR_cr_cr_debut_controle_zone );
	rcs_[ eRC_FinControleZone ] = new MIL_RC( eRC_FinControleZone, T_MsgCR_cr_cr_fin_controle_zone );
	rcs_[ eRC_DebutSurveillance ] = new MIL_RC( eRC_DebutSurveillance, T_MsgCR_cr_cr_debut_surveillance );
	rcs_[ eRC_FinSurveillance ] = new MIL_RC( eRC_FinSurveillance, T_MsgCR_cr_cr_fin_surveillance );
	rcs_[ eRC_EnInfiltration ] = new MIL_RC( eRC_EnInfiltration, T_MsgCR_cr_cr_en_infiltration );
	rcs_[ eRC_EnExfiltration ] = new MIL_RC( eRC_EnExfiltration, T_MsgCR_cr_cr_en_exfiltration );
	rcs_[ eRC_AttenteConditionFavorablePourInfiltration ] = new MIL_RC( eRC_AttenteConditionFavorablePourInfiltration, T_MsgCR_cr_cr_attente_condition_favorable_pour_infiltration );
	rcs_[ eRC_DroneLance ] = new MIL_RC( eRC_DroneLance, T_MsgCR_cr_cr_drone_lance );
	rcs_[ eRC_DroneEnExploitation ] = new MIL_RC( eRC_DroneEnExploitation, T_MsgCR_cr_cr_drone_en_exploitation );
	rcs_[ eRC_DroneExploite ] = new MIL_RC( eRC_DroneExploite, T_MsgCR_cr_cr_drone_exploite );
	rcs_[ eRC_DroneDisponible ] = new MIL_RC( eRC_DroneDisponible, T_MsgCR_cr_cr_drone_disponible );
	rcs_[ eRC_DebutMiseEnOeuvreDrone ] = new MIL_RC( eRC_DebutMiseEnOeuvreDrone, T_MsgCR_cr_cr_debut_mise_en_oeuvre_drone );
	rcs_[ eRC_FinMiseEnOeuvreDrone ] = new MIL_RC( eRC_FinMiseEnOeuvreDrone, T_MsgCR_cr_cr_fin_mise_en_oeuvre_drone );
	rcs_[ eRC_ActivationBrouillage ] = new MIL_RC( eRC_ActivationBrouillage, T_MsgCR_cr_cr_activation_brouillage );
	rcs_[ eRC_DesactivationBrouillage ] = new MIL_RC( eRC_DesactivationBrouillage, T_MsgCR_cr_cr_desactivation_brouillage );
	rcs_[ eRC_ActivationSystemeEcoute ] = new MIL_RC( eRC_ActivationSystemeEcoute, T_MsgCR_cr_cr_activation_systeme_ecoute );
	rcs_[ eRC_DesactivationSystemeEcoute ] = new MIL_RC( eRC_DesactivationSystemeEcoute, T_MsgCR_cr_cr_desactivation_systeme_ecoute );
	rcs_[ eRC_ActivationSystemeEcouteRadar ] = new MIL_RC( eRC_ActivationSystemeEcouteRadar, T_MsgCR_cr_cr_activation_systeme_ecoute_radar );
	rcs_[ eRC_DesactivationSystemeEcouteRadar ] = new MIL_RC( eRC_DesactivationSystemeEcouteRadar, T_MsgCR_cr_cr_desactivation_systeme_ecoute_radar );
	rcs_[ eRC_DebutLiaison ] = new MIL_RC( eRC_DebutLiaison, T_MsgCR_cr_cr_debut_liaison );
	rcs_[ eRC_FinLiaison ] = new MIL_RC( eRC_FinLiaison, T_MsgCR_cr_cr_fin_liaison );
	rcs_[ eRC_EtablissementLiaisonImpossible ] = new MIL_RC( eRC_EtablissementLiaisonImpossible, T_MsgCR_cr_cr_etablissement_liaison_impossible );
	rcs_[ eRC_UniteTropDistante ] = new MIL_RC( eRC_UniteTropDistante, T_MsgCR_cr_cr_unite_trop_distante );
	rcs_[ eRC_EnregistrementDonnes ] = new MIL_RC( eRC_EnregistrementDonnes, T_MsgCR_cr_cr_enregistrement_donnes );
	rcs_[ eRC_ExploitationDonnees ] = new MIL_RC( eRC_ExploitationDonnees, T_MsgCR_cr_cr_exploitation_donnees );
	rcs_[ eRC_ExtractionDonnes ] = new MIL_RC( eRC_ExtractionDonnes, T_MsgCR_cr_cr_extraction_donnes );
	rcs_[ eRC_Releve ] = new MIL_RC_Releve( eRC_Releve, T_MsgCR_cr_cr_releve );
	rcs_[ eRC_SuitUnite ] = new MIL_RC( eRC_SuitUnite, T_MsgCR_cr_cr_suit_unite );
	rcs_[ eRC_PasseEnAppui ] = new MIL_RC( eRC_PasseEnAppui, T_MsgCR_cr_cr_passe_en_appui );
	rcs_[ eRC_EnAppui ] = new MIL_RC( eRC_EnAppui, T_MsgCR_cr_cr_en_appui );
	rcs_[ eRC_NonOperationnel ] = new MIL_RC( eRC_NonOperationnel, T_MsgCR_cr_cr_non_operationnelle );
	rcs_[ eRC_Operationnel ] = new MIL_RC( eRC_Operationnel, T_MsgCR_cr_cr_operationnelle );
	rcs_[ eRC_AmbianceVitesse ] = new MIL_RC( eRC_AmbianceVitesse, T_MsgCR_cr_cr_ambiance_vitesse );
	rcs_[ eRC_AmbianceSurete ] = new MIL_RC( eRC_AmbianceSurete, T_MsgCR_cr_cr_ambiance_surete );
	rcs_[ eRC_SurLima ] = new MIL_RC_SurLima( eRC_SurLima, T_MsgCR_cr_cr_sur_lima );
	rcs_[ eRC_DebutTravaux ] = new MIL_RC( eRC_DebutTravaux, T_MsgCR_cr_cr_debut_travaux );
	rcs_[ eRC_FinTravaux ] = new MIL_RC( eRC_FinTravaux, T_MsgCR_cr_cr_fin_travaux );
	rcs_[ eRC_ObstacleDegage ] = new MIL_RC( eRC_ObstacleDegage, T_MsgCR_cr_cr_obstacle_degage );
	rcs_[ eRC_OrganisationChantier ] = new MIL_RC( eRC_OrganisationChantier, T_MsgCR_cr_cr_organisation_chantier );
	rcs_[ eRC_DesorganisationChantier ] = new MIL_RC( eRC_DesorganisationChantier, T_MsgCR_cr_cr_desorganisation_chantier );
	rcs_[ eRC_ReconnaissancePositionObstacle ] = new MIL_RC( eRC_ReconnaissancePositionObstacle, T_MsgCR_cr_cr_reconnaissance_position_obstacle );
	rcs_[ eRC_ContournementRealise ] = new MIL_RC( eRC_ContournementRealise, T_MsgCR_cr_cr_contournement_realise );
	rcs_[ eRC_AttenteRenforcement ] = new MIL_RC( eRC_AttenteRenforcement, T_MsgCR_cr_cr_attente_renforcement );
	rcs_[ eRC_DebutExploitationSiteFranchissement ] = new MIL_RC( eRC_DebutExploitationSiteFranchissement, T_MsgCR_cr_cr_debut_exploitation_site_franchissement );
	rcs_[ eRC_FinExploitationSiteFranchissement ] = new MIL_RC( eRC_FinExploitationSiteFranchissement, T_MsgCR_cr_cr_fin_exploitation_site_franchissement );
	rcs_[ eRC_ReussiteRenforcement ] = new MIL_RC_ReussiteRenforcement( eRC_ReussiteRenforcement, T_MsgCR_cr_cr_reussite_renforcement );
	rcs_[ eRC_EchecRenforcement ] = new MIL_RC_EchecRenforcement( eRC_EchecRenforcement, T_MsgCR_cr_cr_echec_renforcement );
	rcs_[ eRC_ObstacleEnAttenteActivation ] = new MIL_RC_ObstacleEnAttenteActivation( eRC_ObstacleEnAttenteActivation, T_MsgCR_cr_cr_obstacle_en_attente_activation );
	rcs_[ eRC_DebutAmenagementBerges ] = new MIL_RC( eRC_DebutAmenagementBerges, T_MsgCR_cr_cr_debut_amenagement_berges );
	rcs_[ eRC_FinAmenagementBerges ] = new MIL_RC( eRC_FinAmenagementBerges, T_MsgCR_cr_cr_fin_amenagement_berges );
	rcs_[ eRC_SectionDeployee ] = new MIL_RC( eRC_SectionDeployee, T_MsgCR_cr_cr_section_deployee );
	rcs_[ eRC_ANouveauDisponibleApresReparation ] = new MIL_RC( eRC_ANouveauDisponibleApresReparation, T_MsgCR_cr_cr_a_nouveau_disponible_apres_reparation );
	rcs_[ eRC_MaterielRetourDeMaintenance ] = new MIL_RC( eRC_MaterielRetourDeMaintenance, T_MsgCR_cr_cr_materiel_retour_de_maintenance );
	rcs_[ eRC_MaterielRepareSurPlace ] = new MIL_RC( eRC_MaterielRepareSurPlace, T_MsgCR_cr_cr_materiel_repare_sur_place );
	rcs_[ eRC_DemandeRavitaillement ] = new MIL_RC( eRC_DemandeRavitaillement, T_MsgCR_cr_cr_demande_ravitaillement );
	rcs_[ eRC_DemandeEvacuationSanitaire ] = new MIL_RC( eRC_DemandeEvacuationSanitaire, T_MsgCR_cr_cr_demande_evacuation_sanitaire );
	rcs_[ eRC_DemandeEvacuationMateriel ] = new MIL_RC( eRC_DemandeEvacuationMateriel, T_MsgCR_cr_cr_demande_evacuation_materiel );
	rcs_[ eRC_AllocationConsentieBientotEpuisee ] = new MIL_RC_AllocationConsentieBientotEpuisee( eRC_AllocationConsentieBientotEpuisee, T_MsgCR_cr_cr_allocation_consentie_bientot_epuisee );
	rcs_[ eRC_RavitaillementDotationsEffectue ] = new MIL_RC( eRC_RavitaillementDotationsEffectue, T_MsgCR_cr_cr_ravitaillement_dotations_effectue );
	rcs_[ eRC_RavitaillementStockEffectue ] = new MIL_RC( eRC_RavitaillementStockEffectue, T_MsgCR_cr_cr_ravitaillement_stock_effectue );
	rcs_[ eRC_HumainRetourDeSante ] = new MIL_RC( eRC_HumainRetourDeSante, T_MsgCR_cr_cr_humain_retour_de_sante );
	rcs_[ eRC_VSRAM_PretEnCours ] = new MIL_RC( eRC_VSRAM_PretEnCours, T_MsgCR_cr_cr_vsram_pret_en_cours );
	rcs_[ eRC_VSRAM_PretAnnule ] = new MIL_RC( eRC_VSRAM_PretAnnule, T_MsgCR_cr_cr_vsram_pret_annule );
	rcs_[ eRC_VSRAM_PretEffectue ] = new MIL_RC( eRC_VSRAM_PretEffectue, T_MsgCR_cr_cr_vsram_pret_effectue );
	rcs_[ eRC_VSRAM_RecuperationEffectuee ] = new MIL_RC( eRC_VSRAM_RecuperationEffectuee, T_MsgCR_cr_cr_vsram_recuperation_effectuee );
	rcs_[ eRC_Convoi_DeplacementVersPointChargement ] = new MIL_RC( eRC_Convoi_DeplacementVersPointChargement, T_MsgCR_cr_cr_convoi_deplacement_vers_point_chargement );
	rcs_[ eRC_Convoi_ChargementEnCours ] = new MIL_RC( eRC_Convoi_ChargementEnCours, T_MsgCR_cr_cr_convoi_chargement_en_cours );
	rcs_[ eRC_Convoi_ChargementEffectue ] = new MIL_RC( eRC_Convoi_ChargementEffectue, T_MsgCR_cr_cr_convoi_chargement_effectue );
	rcs_[ eRC_Convoi_DeplacementVersPointDechargement ] = new MIL_RC( eRC_Convoi_DeplacementVersPointDechargement, T_MsgCR_cr_cr_convoi_deplacement_vers_point_dechargement );
	rcs_[ eRC_Convoi_DechargementEnCours ] = new MIL_RC( eRC_Convoi_DechargementEnCours, T_MsgCR_cr_cr_convoi_dechargement_en_cours );
	rcs_[ eRC_Convoi_DechargementEffectue ] = new MIL_RC( eRC_Convoi_DechargementEffectue, T_MsgCR_cr_cr_convoi_dechargement_effectue );
	rcs_[ eRC_Convoi_Retour ] = new MIL_RC( eRC_Convoi_Retour, T_MsgCR_cr_cr_convoi_retour );
	rcs_[ eRC_EnCoursRavitaillement ] = new MIL_RC( eRC_EnCoursRavitaillement, T_MsgCR_cr_cr_en_cours_ravitaillement );
	rcs_[ eRC_RavitaillementTermine ] = new MIL_RC( eRC_RavitaillementTermine, T_MsgCR_cr_cr_ravitaillement_termine );
	rcs_[ eRC_HelicoptereEnObservation ] = new MIL_RC( eRC_HelicoptereEnObservation, T_MsgCR_cr_cr_helicoptere_en_observation );
	rcs_[ eRC_PerteInfoGuidage ] = new MIL_RC( eRC_PerteInfoGuidage, T_MsgCR_cr_cr_perte_info_guidage );
	rcs_[ eRC_TransportUnitePasPrete ] = new MIL_RC_TransportUnitePasPrete( eRC_TransportUnitePasPrete, T_MsgCR_cr_cr_transport_unite_pas_prete );
	rcs_[ eRC_TransportUnitePrete ] = new MIL_RC_TransportUnitePrete( eRC_TransportUnitePrete, T_MsgCR_cr_cr_transport_unite_prete );
	rcs_[ eRC_TransportEnCours ] = new MIL_RC( eRC_TransportEnCours, T_MsgCR_cr_cr_transport_en_cours );
	rcs_[ eRC_TransportEmbarquement ] = new MIL_RC( eRC_TransportEmbarquement, T_MsgCR_cr_cr_transport_embarquement );
	rcs_[ eRC_TransportDebarquement ] = new MIL_RC( eRC_TransportDebarquement, T_MsgCR_cr_cr_transport_debarquement );
	rcs_[ eRC_VehiculeLaisseSurPosition ] = new MIL_RC( eRC_VehiculeLaisseSurPosition, T_MsgCR_cr_cr_vehicule_laisse_sur_position );
	rcs_[ eRC_RejointPositionEmbarquement ] = new MIL_RC( eRC_RejointPositionEmbarquement, T_MsgCR_cr_cr_rejoint_position_embarquement );
	rcs_[ eRC_EnAttenteTransporteurs ] = new MIL_RC( eRC_EnAttenteTransporteurs, T_MsgCR_cr_cr_en_attente_transporteurs );
	rcs_[ eRC_ContamineNBC ] = new MIL_RC( eRC_ContamineNBC, T_MsgCR_cr_cr_contamine_nbc );
	rcs_[ eRC_DecontamineNBC ] = new MIL_RC( eRC_DecontamineNBC, T_MsgCR_cr_cr_decontamine_nbc );
	rcs_[ eRC_TenueProtectionNBCMise ] = new MIL_RC( eRC_TenueProtectionNBCMise, T_MsgCR_cr_cr_tenue_protection_nbc_mise );
	rcs_[ eRC_TenueProtectionNBCEnlevee ] = new MIL_RC( eRC_TenueProtectionNBCEnlevee, T_MsgCR_cr_cr_tenue_protection_nbc_enlevee );
	rcs_[ eRC_AlerteNBCNiveau4 ] = new MIL_RC( eRC_AlerteNBCNiveau4, T_MsgCR_cr_cr_alerte_nbc_niveau_4 );
	rcs_[ eRC_FinAlerteNBCNiveau4 ] = new MIL_RC( eRC_FinAlerteNBCNiveau4, T_MsgCR_cr_cr_fin_alerte_nbc_niveau_4 );
	rcs_[ eRC_SiteDecontaminationAnime ] = new MIL_RC( eRC_SiteDecontaminationAnime, T_MsgCR_cr_cr_site_decontamination_anime );
	rcs_[ eRC_SiteDecontaminationDesactive ] = new MIL_RC( eRC_SiteDecontaminationDesactive, T_MsgCR_cr_cr_site_decontamination_desactive );
	rcs_[ eRC_DemandeDecontamination ] = new MIL_RC( eRC_DemandeDecontamination, T_MsgCR_cr_cr_demande_decontamination );
	rcs_[ eRC_ZoneReconnue ] = new MIL_RC( eRC_ZoneReconnue, T_MsgCR_cr_cr_zone_reconnue );
	rcs_[ eRC_ZoneDecontaminee ] = new MIL_RC( eRC_ZoneDecontaminee, T_MsgCR_cr_cr_zone_decontaminee );
	rcs_[ eRC_SiteROTAReconnu ] = new MIL_RC( eRC_SiteROTAReconnu, T_MsgCR_cr_cr_site_rota_reconnu );
	rcs_[ eRC_IncidentNBCTermine ] = new MIL_RC( eRC_IncidentNBCTermine, T_MsgCR_cr_cr_incident_nbc_termine );
	rcs_[ eRC_UniteDecontaminee ] = new MIL_RC_UniteDecontaminee( eRC_UniteDecontaminee, T_MsgCR_cr_cr_unite_decontaminee );
	rcs_[ eRC_SiteSature ] = new MIL_RC( eRC_SiteSature, T_MsgCR_cr_cr_site_sature );
	rcs_[ eRC_DebutMesure ] = new MIL_RC( eRC_DebutMesure, T_MsgCR_cr_cr_debut_mesure );
	rcs_[ eRC_FinMesure ] = new MIL_RC( eRC_FinMesure, T_MsgCR_cr_cr_fin_mesure );
	rcs_[ eRC_ReconnaissanceTerminee ] = new MIL_RC( eRC_ReconnaissanceTerminee, T_MsgCR_cr_cr_reconnaissance_terminee );
	rcs_[ eRC_ReconnaissanceInitialeTerminee ] = new MIL_RC( eRC_ReconnaissanceInitialeTerminee, T_MsgCR_cr_cr_reconnaissance_initiale_terminee );
	rcs_[ eRC_RejointPlotRavitaillement ] = new MIL_RC( eRC_RejointPlotRavitaillement, T_MsgCR_cr_cr_rejoint_plot_ravitaillement );
	rcs_[ eRC_PremierCoupParti ] = new MIL_RC( eRC_PremierCoupParti, T_MsgCR_cr_cr_premier_coup_parti );
	rcs_[ eRC_TirExecute ] = new MIL_RC( eRC_TirExecute, T_MsgCR_cr_cr_tir_execute );
	rcs_[ eRC_TirHorsDePortee ] = new MIL_RC( eRC_TirHorsDePortee, T_MsgCR_cr_cr_tir_hors_de_portee );
	rcs_[ eRC_TirImpossiblePlusDeMunitions ] = new MIL_RC( eRC_TirImpossiblePlusDeMunitions, T_MsgCR_cr_cr_tir_impossible_plus_de_munitions );
	rcs_[ eRC_TirAnnule ] = new MIL_RC( eRC_TirAnnule, T_MsgCR_cr_cr_tir_annule );
	rcs_[ eRC_ChangementDePosition ] = new MIL_RC( eRC_ChangementDePosition, T_MsgCR_cr_cr_changement_de_position );
	rcs_[ eRC_DebutInstallation ] = new MIL_RC( eRC_DebutInstallation, T_MsgCR_cr_cr_debut_installation );
	rcs_[ eRC_FinInstallation ] = new MIL_RC( eRC_FinInstallation, T_MsgCR_cr_cr_fin_installation );
	rcs_[ eRC_PretPourConduiteTir ] = new MIL_RC( eRC_PretPourConduiteTir, T_MsgCR_cr_cr_pret_pour_conduite_tir );
	rcs_[ eRC_ObservationTirIndirect ] = new MIL_RC_ObservationTirIndirect( eRC_ObservationTirIndirect, T_MsgCR_cr_cr_observation_tir_indirect );
	rcs_[ eRC_ZoneImplantationOccupee ] = new MIL_RC( eRC_ZoneImplantationOccupee, T_MsgCR_cr_cr_zone_implantation_occupee );
	rcs_[ eRC_InstallationSurZoneImplantation ] = new MIL_RC( eRC_InstallationSurZoneImplantation, T_MsgCR_cr_cr_installation_sur_zone_implantation );
	rcs_[ eRC_ReceptionOrdreDeFeu ] = new MIL_RC( eRC_ReceptionOrdreDeFeu, T_MsgCR_cr_cr_reception_ordre_de_feu );
	rcs_[ eRC_PretAuTir ] = new MIL_RC( eRC_PretAuTir, T_MsgCR_cr_cr_pret_au_tir );
	rcs_[ eRC_PresencePopulation ] = new MIL_RC( eRC_PresencePopulation, T_MsgCR_cr_cr_presence_population );
	rcs_[ eRC_OrientationPopulationVersCampRefugies ] = new MIL_RC( eRC_OrientationPopulationVersCampRefugies, T_MsgCR_cr_cr_orientation_population_vers_camp_refugies );
	rcs_[ eRC_PasDeCampRefugiesDisponible ] = new MIL_RC( eRC_PasDeCampRefugiesDisponible, T_MsgCR_cr_cr_pas_de_camp_refugies_disponible );
	rcs_[ eRC_PresenceEnnemiRendu ] = new MIL_RC( eRC_PresenceEnnemiRendu, T_MsgCR_cr_cr_presence_ennemi_rendu );
	rcs_[ eRC_OrientationEnnemiRenduVersCampPrisonniers ] = new MIL_RC( eRC_OrientationEnnemiRenduVersCampPrisonniers, T_MsgCR_cr_cr_orientation_ennemi_rendu_vers_camp_prisonniers );
	rcs_[ eRC_PasDeCampPrisonniersDisponible ] = new MIL_RC( eRC_PasDeCampPrisonniersDisponible, T_MsgCR_cr_cr_pas_de_camp_prisonniers_disponible );
	rcs_[ eRC_Rendu ] = new MIL_RC( eRC_Rendu, T_MsgCR_cr_cr_rendu );
	rcs_[ eRC_TirDansZoneInterdite ] = new MIL_RC( eRC_TirDansZoneInterdite, T_MsgCR_cr_cr_tir_dans_zone_interdite );
	rcs_[ eRC_TirSurCampAmi ] = new MIL_RC_TirSurCampAmi( eRC_TirSurCampAmi, T_MsgCR_cr_cr_tir_sur_camp_ami );
	rcs_[ eRC_TireParCampAmi ] = new MIL_RC_TireParCampAmi( eRC_TireParCampAmi, T_MsgCR_cr_cr_tire_par_camp_ami );
	rcs_[ eRC_TirSurCampNeutre ] = new MIL_RC_TirSurCampNeutre( eRC_TirSurCampNeutre, T_MsgCR_cr_cr_tir_sur_camp_neutre );
	rcs_[ eRC_TireParCampNeutre ] = new MIL_RC_TireParCampNeutre( eRC_TireParCampNeutre, T_MsgCR_cr_cr_tire_par_camp_neutre );
	rcs_[ eRC_TirSurCivil ] = new MIL_RC_TirSurCivil( eRC_TirSurCivil, T_MsgCR_cr_cr_tir_sur_civil );
	rcs_[ eRC_TireParCivil ] = new MIL_RC_TireParCivil( eRC_TireParCivil, T_MsgCR_cr_cr_tire_par_civil );
	rcs_[ eRC_DestructionPC ] = new MIL_RC( eRC_DestructionPC, T_MsgCR_cr_cr_destruction_pc );
	rcs_[ eRC_DecesBlesse ] = new MIL_RC( eRC_DecesBlesse, T_MsgCR_cr_cr_deces_blesse );
	rcs_[ eRC_DecesBlessePendantTransport ] = new MIL_RC( eRC_DecesBlessePendantTransport, T_MsgCR_cr_cr_deces_blesse_pendant_transport );
	rcs_[ eRC_DecesBlessePendantHospitalisation ] = new MIL_RC( eRC_DecesBlessePendantHospitalisation, T_MsgCR_cr_cr_deces_blesse_pendant_hospitalisation );
	rcs_[ eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles ] = new MIL_RC( eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles, T_MsgCR_cr_cr_mission_impossible_unites_subordonnees_non_operationnelles );
	rcs_[ eRC_MissionImpossibleLimaInvalide ] = new MIL_RC( eRC_MissionImpossibleLimaInvalide, T_MsgCR_cr_cr_mission_impossible_lima_invalide );
	rcs_[ eRC_MissionImpossibleZoneHorsFuseau ] = new MIL_RC( eRC_MissionImpossibleZoneHorsFuseau, T_MsgCR_cr_cr_mission_impossible_zone_hors_fuseau );
	rcs_[ eRC_RapportDeForceDefavorable ] = new MIL_RC( eRC_RapportDeForceDefavorable, T_MsgCR_cr_cr_rapport_de_force_defavorable );
	rcs_[ eRC_TerrainDifficile ] = new MIL_RC( eRC_TerrainDifficile, T_MsgCR_cr_cr_terrain_difficile );
	rcs_[ eRC_PlusDeCarburant ] = new MIL_RC( eRC_PlusDeCarburant, T_MsgCR_cr_cr_plus_de_carburant );
	rcs_[ eRC_DispositifDisperse ] = new MIL_RC( eRC_DispositifDisperse, T_MsgCR_cr_cr_dispositif_disperse );
	rcs_[ eRC_DebutNeutralisation ] = new MIL_RC( eRC_DebutNeutralisation, T_MsgCR_cr_cr_debut_neutralisation );
	rcs_[ eRC_FinNeutralisation ] = new MIL_RC( eRC_FinNeutralisation, T_MsgCR_cr_cr_fin_neutralisation );
	rcs_[ eRC_DebutSilenceRadio ] = new MIL_RC( eRC_DebutSilenceRadio, T_MsgCR_cr_cr_debut_silence_radio );
	rcs_[ eRC_FinSilenceRadio ] = new MIL_RC( eRC_FinSilenceRadio, T_MsgCR_cr_cr_fin_silence_radio );
	rcs_[ eRC_DebutSilenceRadar ] = new MIL_RC( eRC_DebutSilenceRadar, T_MsgCR_cr_cr_debut_silence_radar );
	rcs_[ eRC_FinSilenceRadar ] = new MIL_RC( eRC_FinSilenceRadar, T_MsgCR_cr_cr_fin_silence_radar );
	rcs_[ eRC_TirRestreint ] = new MIL_RC( eRC_TirRestreint, T_MsgCR_cr_cr_tir_restreint );
	rcs_[ eRC_TirInterdit ] = new MIL_RC( eRC_TirInterdit, T_MsgCR_cr_cr_tir_interdit );
	rcs_[ eRC_TirLibre ] = new MIL_RC( eRC_TirLibre, T_MsgCR_cr_cr_tir_libre );
	rcs_[ eRC_UniteDetectee ] = new MIL_RC_UniteDetectee( eRC_UniteDetectee, T_MsgCR_cr_cr_unite_detectee );
	rcs_[ eRC_UniteAmieReconnue ] = new MIL_RC_UniteAmieReconnue( eRC_UniteAmieReconnue, T_MsgCR_cr_cr_unite_amie_reconnue );
	rcs_[ eRC_UniteEnnemieReconnue ] = new MIL_RC_UniteEnnemieReconnue( eRC_UniteEnnemieReconnue, T_MsgCR_cr_cr_unite_ennemie_reconnue );
	rcs_[ eRC_UniteNeutreReconnue ] = new MIL_RC_UniteNeutreReconnue( eRC_UniteNeutreReconnue, T_MsgCR_cr_cr_unite_neutre_reconnue );
	rcs_[ eRC_UniteAmieIdentifiee ] = new MIL_RC_UniteAmieIdentifiee( eRC_UniteAmieIdentifiee, T_MsgCR_cr_cr_unite_amie_identifiee );
	rcs_[ eRC_UniteEnnemieIdentifiee ] = new MIL_RC_UniteEnnemieIdentifiee( eRC_UniteEnnemieIdentifiee, T_MsgCR_cr_cr_unite_ennemie_identifiee );
	rcs_[ eRC_UniteNeutreIdentifiee ] = new MIL_RC_UniteNeutreIdentifiee( eRC_UniteNeutreIdentifiee, T_MsgCR_cr_cr_unite_neutre_identifiee );
	rcs_[ eRC_ObjetDetecte ] = new MIL_RC_ObjetDetecte( eRC_ObjetDetecte, T_MsgCR_cr_cr_objet_detecte );
	rcs_[ eRC_Trace ] = new MIL_RC_Trace( eRC_Trace, T_MsgCR_cr_cr_trace );
	rcs_[ eRC_TraceID ] = new MIL_RC_TraceID( eRC_TraceID, T_MsgCR_cr_cr_trace_id );
	rcs_[ eRC_TirInterditSurPopulation ] = new MIL_RC( eRC_TirInterditSurPopulation, T_MsgCR_cr_cr_tir_interdit_sur_population );
	rcs_[ eRC_TirSommationSurPopulation ] = new MIL_RC( eRC_TirSommationSurPopulation, T_MsgCR_cr_cr_tir_sommation_sur_population );
	rcs_[ eRC_TirDissuasionSurPopulation ] = new MIL_RC( eRC_TirDissuasionSurPopulation, T_MsgCR_cr_cr_tir_dissuasion_sur_population );
	rcs_[ eRC_TirNeutralisationSurPopulation ] = new MIL_RC( eRC_TirNeutralisationSurPopulation, T_MsgCR_cr_cr_tir_neutralisation_sur_population );
	rcs_[ eRC_PopulationDetectee ] = new MIL_RC_PopulationDetectee( eRC_PopulationDetectee, T_MsgCR_cr_cr_population_detectee );
	rcs_[ eRC_PriseAPartieParPopulation ] = new MIL_RC_PriseAPartieParPopulation( eRC_PriseAPartieParPopulation, T_MsgCR_cr_cr_prise_a_partie_par_population );
	rcs_[ eRC_RiposteSurPopulation ] = new MIL_RC_RiposteSurPopulation( eRC_RiposteSurPopulation, T_MsgCR_cr_cr_riposte_sur_population );
	rcs_[ eRC_PopulationDangereuseAProximite ] = new MIL_RC( eRC_PopulationDangereuseAProximite, T_MsgCR_cr_cr_population_dangereuse_a_proximite );
	rcs_[ eRC_EsquiveDeLaPopulation ] = new MIL_RC( eRC_EsquiveDeLaPopulation, T_MsgCR_cr_cr_esquive_de_la_population );
	rcs_[ eRC_DecrochageFaceAPopulation ] = new MIL_RC( eRC_DecrochageFaceAPopulation, T_MsgCR_cr_cr_decrochage_face_a_population );
	rcs_[ eRC_MiseEnPlaceControleDeFoule ] = new MIL_RC( eRC_MiseEnPlaceControleDeFoule, T_MsgCR_cr_cr_mise_en_place_controle_de_foule );
	rcs_[ eRC_MiseEnPlaceFiltrage ] = new MIL_RC( eRC_MiseEnPlaceFiltrage, T_MsgCR_cr_cr_mise_en_place_filtrage );
	rcs_[ eRC_AnimationBarrage ] = new MIL_RC( eRC_AnimationBarrage, T_MsgCR_cr_cr_animation_barrage );
	rcs_[ eRC_AnimationCanalisation ] = new MIL_RC( eRC_AnimationCanalisation, T_MsgCR_cr_cr_animation_canalisation );
	rcs_[ eRC_CollecteRenseignementsSurPopulations ] = new MIL_RC( eRC_CollecteRenseignementsSurPopulations, T_MsgCR_cr_cr_collecte_renseignements_sur_populations );
	rcs_[ eRC_PopulationTotalementReconnue ] = new MIL_RC( eRC_PopulationTotalementReconnue, T_MsgCR_cr_cr_population_totalement_reconnue );
	rcs_[ eRC_AttitudeCalme ] = new MIL_RC( eRC_AttitudeCalme, T_MsgCR_cr_cr_attitude_calme );
	rcs_[ eRC_AttitudeAgitee ] = new MIL_RC( eRC_AttitudeAgitee, T_MsgCR_cr_cr_attitude_agitee );
	rcs_[ eRC_AttitudeExcitee ] = new MIL_RC( eRC_AttitudeExcitee, T_MsgCR_cr_cr_attitude_excitee );
	rcs_[ eRC_AttitudeAgressive ] = new MIL_RC( eRC_AttitudeAgressive, T_MsgCR_cr_cr_attitude_agressive );
	rcs_[ eRC_AggravationAttitude ] = new MIL_RC( eRC_AggravationAttitude, T_MsgCR_cr_cr_aggravation_attitude );
	rcs_[ eRC_AffrontementAvecForcesEnnemies ] = new MIL_RC( eRC_AffrontementAvecForcesEnnemies, T_MsgCR_cr_cr_affrontement_avec_forces_ennemies );
	rcs_[ eRC_PopulationTemporairementControlee ] = new MIL_RC( eRC_PopulationTemporairementControlee, T_MsgCR_cr_cr_population_temporairement_controlee );
	rcs_[ eRC_PopulationTemporairementRepoussee ] = new MIL_RC( eRC_PopulationTemporairementRepoussee, T_MsgCR_cr_cr_population_temporairement_repoussee );
	rcs_[ eRC_RepriseDesAffrontements ] = new MIL_RC( eRC_RepriseDesAffrontements, T_MsgCR_cr_cr_reprise_des_affrontements );
	rcs_[ eRC_RecuperationProgressive ] = new MIL_RC_RecuperationProgressive( eRC_RecuperationProgressive, T_MsgCR_cr_cr_recuperation_progressive );
	rcs_[ eRC_AgressionDePopulation ] = new MIL_RC_AgressionDePopulation( eRC_AgressionDePopulation, T_MsgCR_cr_cr_agression_de_population );
	rcs_[ eRC_AgressionParPopulation ] = new MIL_RC_AgressionParPopulation( eRC_AgressionParPopulation, T_MsgCR_cr_cr_agression_par_population );
	rcs_[ eRC_RiposteContreAgressionParPopulation ] = new MIL_RC_RiposteContreAgressionParPopulation( eRC_RiposteContreAgressionParPopulation, T_MsgCR_cr_cr_riposte_contre_agression_par_population );
	rcs_[ eRC_AttaqueInstallation ] = new MIL_RC_AttaqueInstallation( eRC_AttaqueInstallation, T_MsgCR_cr_cr_attaque_installation );
	rcs_[ eRC_EnStationnement ] = new MIL_RC( eRC_EnStationnement, T_MsgCR_cr_cr_en_stationnement );
	rcs_[ eRC_ExecutionAttentat ] = new MIL_RC( eRC_ExecutionAttentat, T_MsgCR_cr_cr_execution_attentat );
	

    pRcRavitaillementDotationsEffectue_   =                                            rcs_[ eRC_RavitaillementDotationsEffectue   ];
    pRcRavitaillementStockEffectue_       =                                            rcs_[ eRC_RavitaillementStockEffectue       ];
    pRcHumainRetourDeSante_               =                                            rcs_[ eRC_HumainRetourDeSante               ];
    pRcANouveauDisponibleApresReparation_ =                                            rcs_[ eRC_ANouveauDisponibleApresReparation ];
    pRcMaterielRepareSurPlace_            =                                            rcs_[ eRC_MaterielRepareSurPlace            ];
    pRcMaterielRetourDeMaintenance_       =                                            rcs_[ eRC_MaterielRetourDeMaintenance       ];
    pRcEnCoursDeFranchissement_           =                                            rcs_[ eRC_EnCoursDeFranchissement           ];
    pRcObservationTirIndirect_            = (MIL_RC_ObservationTirIndirect*)           rcs_[ eRC_ObservationTirIndirect            ];
    pRcDemandeRavitaillement_             =                                            rcs_[ eRC_DemandeRavitaillement             ];
    pRcDemandeEvacuationSanitaire_        =                                            rcs_[ eRC_DemandeEvacuationSanitaire        ];
    pRcDemandeEvacuationMateriel_         =                                            rcs_[ eRC_DemandeEvacuationMateriel         ];
    pRcDecesBlesse_                       =                                            rcs_[ eRC_DecesBlesse                       ];
    pRcDecesBlessePendantTransport_       =                                            rcs_[ eRC_DecesBlessePendantTransport       ];
    pRcDecesBlessePendantHospitalisation_ =                                            rcs_[ eRC_DecesBlessePendantHospitalisation ];
    pRcTirDansZoneInterdite_              =                                            rcs_[ eRC_TirDansZoneInterdite              ];
    pRcTirSurCampAmi_                     = (MIL_RC_TirSurCampAmi*)                    rcs_[ eRC_TirSurCampAmi                     ];
    pRcTireParCampAmi_                    = (MIL_RC_TireParCampAmi*)                   rcs_[ eRC_TireParCampAmi                    ];
    pRcTirSurCampNeutre_                  = (MIL_RC_TirSurCampNeutre*)                 rcs_[ eRC_TirSurCampNeutre                  ];
    pRcTireParCampNeutre_                 = (MIL_RC_TireParCampNeutre*)                rcs_[ eRC_TireParCampNeutre                 ];
    pRcTirSurCivil_                       = (MIL_RC_TirSurCivil*)                      rcs_[ eRC_TirSurCivil                       ];
    pRcTireParCivil_                      = (MIL_RC_TireParCivil*)                     rcs_[ eRC_TireParCivil                      ];
    pRcAllocationConsentieBientotEpuisee_ = (MIL_RC_AllocationConsentieBientotEpuisee*)rcs_[ eRC_AllocationConsentieBientotEpuisee ];
    pRcMissionImpossible_                 =                                            rcs_[ eRC_MissionImpossible                 ];
    pRcUniteDecontaminee_                 = (MIL_RC_UniteDecontaminee*)                rcs_[ eRC_UniteDecontaminee                 ];
    pRcPlusDeCarburant_                   =                                            rcs_[ eRC_PlusDeCarburant                   ];
    pRcTerrainDifficile_                  =                                            rcs_[ eRC_TerrainDifficile                  ];
    pRcUniteDetectee_                     = (MIL_RC_UniteDetectee*)                    rcs_[ eRC_UniteDetectee                     ];
    pRcUniteAmieReconnue_                 = (MIL_RC_UniteAmieReconnue*)                rcs_[ eRC_UniteAmieReconnue                 ];
    pRcUniteEnnemieReconnue_              = (MIL_RC_UniteEnnemieReconnue*)             rcs_[ eRC_UniteEnnemieReconnue              ];
    pRcUniteNeutreReconnue_               = (MIL_RC_UniteNeutreReconnue*)              rcs_[ eRC_UniteNeutreReconnue               ];
    pRcUniteAmieIdentifiee_               = (MIL_RC_UniteAmieIdentifiee*)              rcs_[ eRC_UniteAmieIdentifiee               ];
    pRcUniteEnnemieIdentifiee_            = (MIL_RC_UniteEnnemieIdentifiee*)           rcs_[ eRC_UniteEnnemieIdentifiee            ];
    pRcUniteNeutreIdentifiee_             = (MIL_RC_UniteNeutreIdentifiee*)            rcs_[ eRC_UniteNeutreIdentifiee             ];
    pRcObjetDetecte_                      = (MIL_RC_ObjetDetecte*)                     rcs_[ eRC_ObjetDetecte                      ];
    pRcRendu_                             =                                            rcs_[ eRC_Rendu                             ];
}

// -----------------------------------------------------------------------------
// Name: MIL_RC::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC::Terminate()
{
    //$$ Voir pb RC statiques et non statiques pour destruction
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC::MIL_RC( E_RC nRC, int nAsnID )
    : nRC_   ( nRC )
    , nAsnID_( nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC constructor
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
MIL_RC::MIL_RC( E_RC nRC )
    : nRC_   ( nRC )
    , nAsnID_( (int)-1 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC::~MIL_RC()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC::Send( const MIL_AgentPion& sender, E_RcType nRcType, DIA_Parameters& /*parameters*/ ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nRcType );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC::Send( const MIL_Automate& sender, E_RcType nRcType, DIA_Parameters& /*parameters*/ ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nRcType );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC::Send
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_RC::Send( const MIL_Population& sender, E_RcType nRcType, DIA_Parameters& /*parameters*/ ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nRcType );
    asnMsg.Send();
}
