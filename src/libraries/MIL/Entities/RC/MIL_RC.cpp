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

#include "MIL_Pch.h"

#include "MIL_RC.h"

#include "MIL_RC_EchecRenforcement.h"
#include "MIL_RC_EnEsquive.h"
#include "MIL_RC_EnPause.h"
#include "MIL_RC_EnPosteFaceAObstacle.h"
#include "MIL_RC_EniSurObjectif.h"
#include "MIL_RC_EnnemiDetruit.h"
#include "MIL_RC_MissionTerminee.h"
#include "MIL_RC_PerteDeContactVisuel.h"
#include "MIL_RC_PionPriseContact.h"
#include "MIL_RC_PionRuptureContact.h"
#include "MIL_RC_PointTenuParEni.h"
#include "MIL_RC_PosteFaceEni.h"
#include "MIL_RC_PrecisionContact.h"
#include "MIL_RC_PreparationMission.h"
#include "MIL_RC_PriseAPartie.h"
#include "MIL_RC_ProgressionVersEni.h"
#include "MIL_RC_Releve.h"
#include "MIL_RC_RenforcementBientotFini.h"
#include "MIL_RC_ReussiteRenforcement.h"
#include "MIL_RC_SituationLogDefavorable.h"
#include "MIL_RC_SurLima.h"
#include "MIL_RC_SurPIA.h"
#include "MIL_RC_SurTypeTerrainParticulier.h"
#include "MIL_RC_UniteRecueillie.h"
#include "MIL_RC_ObservationTirIndirect.h"
#include "MIL_RC_ObstacleEnAttenteActivation.h"
#include "MIL_RC_AllocationConsentieBientotEpuisee.h"
#include "MIL_RC_PionDecontamine.h"
#include "MIL_RC_TirSurCampAmi.h"    
#include "MIL_RC_TireParCampAmi.h"   
#include "MIL_RC_TirSurCampNeutre.h" 
#include "MIL_RC_TireParCampNeutre.h"
#include "MIL_RC_TirSurCivil.h"      
#include "MIL_RC_TireParCivil.h"
#include "MIL_RC_TransportUnitePasPrete.h"
#include "MIL_RC_Trace.h"
#include "MIL_RC_TraceID.h"

#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/MIL_AgentPion.h"

MIL_RC::T_RCMap MIL_RC::rcs_;

// Statics
const MIL_RC*                                   MIL_RC::pRcMissionImpossible_                 = 0;
const MIL_RC*                                   MIL_RC::pRcEnCoursDeFranchissement_           = 0;
const MIL_RC*                                   MIL_RC::pRcMaterielRetourDeMaintenance_       = 0;
const MIL_RC_ObservationTirIndirect*            MIL_RC::pRcObservationTirIndirect_            = 0;
const MIL_RC*                                   MIL_RC::pRcDemandeRavitaillement_             = 0;
const MIL_RC*                                   MIL_RC::pRcDemandeEvacuationSanitaire_        = 0;
const MIL_RC*                                   MIL_RC::pRcDemandeEvacuationMateriel_         = 0;
const MIL_RC*                                   MIL_RC::pRcDecesBlesse_                       = 0;
const MIL_RC*                                   MIL_RC::pRcDecesBlessePendantTransport_       = 0;
const MIL_RC*                                   MIL_RC::pRcDecesBlessePendantHospitalisation_ = 0;
const MIL_RC*                                   MIL_RC::pRcTirDansZoneInterdite_              = 0;
const MIL_RC_TirSurCampAmi*                     MIL_RC::pRcTirSurCampAmi_                     = 0;
const MIL_RC_TireParCampAmi*                    MIL_RC::pRcTireParCampAmi_                    = 0;
const MIL_RC_TirSurCampNeutre*                  MIL_RC::pRcTirSurCampNeutre_                  = 0;
const MIL_RC_TireParCampNeutre*                 MIL_RC::pRcTireParCampNeutre_                 = 0;
const MIL_RC_TirSurCivil*                       MIL_RC::pRcTirSurCivil_                       = 0;
const MIL_RC_TireParCivil*                      MIL_RC::pRcTireParCivil_                      = 0;
const MIL_RC*                                   MIL_RC::pRcDestructionPC_                     = 0;
const MIL_RC_PionDecontamine*                   MIL_RC::pRcPionDecontamine_                   = 0;
const MIL_RC_AllocationConsentieBientotEpuisee* MIL_RC::pRcAllocationConsentieBientotEpuisee_ = 0;
const MIL_RC*                                   MIL_RC::pRcPlusDeCarburant_                   = 0;
const MIL_RC*                                   MIL_RC::pRcTerrainDifficile_                  = 0;

// -----------------------------------------------------------------------------
// Name: MIL_RC::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing RCs" );
    rcs_[ eRC_RAS                                               ] = new MIL_RC                                     ( eRC_RAS                                                , T_MsgCR_cr_cr_ras                                                     );  
    rcs_[ eRC_EnPosteFaceAObstacle                              ] = new MIL_RC_EnPosteFaceAObstacle                ( eRC_EnPosteFaceAObstacle                               , T_MsgCR_cr_cr_en_poste_face_a_obstacle                                );  
    rcs_[ eRC_EnContournementObstacle                           ] = new MIL_RC                                     ( eRC_EnContournementObstacle                            , T_MsgCR_cr_cr_en_contournement_obstacle                               );  
    rcs_[ eRC_MissionImpossible                                 ] = new MIL_RC                                     ( eRC_MissionImpossible                                  , T_MsgCR_cr_cr_mission_impossible                                      );  
    rcs_[ eRC_PretPourNouvelleMission                           ] = new MIL_RC                                     ( eRC_PretPourNouvelleMission                            , T_MsgCR_cr_cr_pret_pour_nouvelle_mission                              );  
    rcs_[ eRC_RejointFuseau                                     ] = new MIL_RC                                     ( eRC_RejointFuseau                                      , T_MsgCR_cr_cr_rejoint_fuseau                                          );  
    rcs_[ eRC_ProgressionSurAxe                                 ] = new MIL_RC                                     ( eRC_ProgressionSurAxe                                  , T_MsgCR_cr_cr_progression_sur_axe                                     );  
    rcs_[ eRC_ProgressionVersLocalisation                       ] = new MIL_RC                                     ( eRC_ProgressionVersLocalisation                        , T_MsgCR_cr_cr_progression_vers_localisation                           );  
    rcs_[ eRC_ArriveSurLocalisation                             ] = new MIL_RC                                     ( eRC_ArriveSurLocalisation                              , T_MsgCR_cr_cr_arrive_sur_localisation                                 );  
    rcs_[ eRC_ReconnaissanceLocalisation                        ] = new MIL_RC                                     ( eRC_ReconnaissanceLocalisation                         , T_MsgCR_cr_cr_reconnaissance_localisation                             );  
    rcs_[ eRC_EnPosteFaceADirectionDangereuse                   ] = new MIL_RC                                     ( eRC_EnPosteFaceADirectionDangereuse                    , T_MsgCR_cr_cr_en_poste_face_a_direction_dangereuse                    );  
    rcs_[ eRC_EnPosteSurBond                                    ] = new MIL_RC                                     ( eRC_EnPosteSurBond                                     , T_MsgCR_cr_cr_en_poste_sur_bond                                       );  
    rcs_[ eRC_EnJalonnement                                     ] = new MIL_RC                                     ( eRC_EnJalonnement                                      , T_MsgCR_cr_cr_en_jalonnement                                          );  
    rcs_[ eRC_PrecisionContact                                  ] = new MIL_RC_PrecisionContact                    ( eRC_PrecisionContact                                   , T_MsgCR_cr_cr_precision_contact                                       );  
    rcs_[ eRC_PionPriseContact                                  ] = new MIL_RC_PionPriseContact                    ( eRC_PionPriseContact                                   , T_MsgCR_cr_cr_pion_prise_contact                                      );  
    rcs_[ eRC_PionRuptureContact                                ] = new MIL_RC_PionRuptureContact                  ( eRC_PionRuptureContact                                 , T_MsgCR_cr_cr_pion_rupture_contact                                    );  
    rcs_[ eRC_EnnemiDetruit                                     ] = new MIL_RC_EnnemiDetruit                       ( eRC_EnnemiDetruit                                      , T_MsgCR_cr_cr_ennemi_detruit                                          );             
    rcs_[ eRC_PerteDeContactVisuel                              ] = new MIL_RC_PerteDeContactVisuel                ( eRC_PerteDeContactVisuel                               , T_MsgCR_cr_cr_perte_de_contact_visuel                                 );         
    rcs_[ eRC_PosteFaceEni                                      ] = new MIL_RC_PosteFaceEni                        ( eRC_PosteFaceEni                                       , T_MsgCR_cr_cr_poste_face_eni                                          );  
    rcs_[ eRC_PriseAPartie                                      ] = new MIL_RC_PriseAPartie                        ( eRC_PriseAPartie                                       , T_MsgCR_cr_cr_prise_a_partie                                          );  
    rcs_[ eRC_PointTenuParEni                                   ] = new MIL_RC_PointTenuParEni                     ( eRC_PointTenuParEni                                    , T_MsgCR_cr_cr_point_tenu_par_eni                                      );           
    rcs_[ eRC_ProgressionVersEni                                ] = new MIL_RC_ProgressionVersEni                  ( eRC_ProgressionVersEni                                 , T_MsgCR_cr_cr_progression_vers_eni                                    );  
    rcs_[ eRC_PresenceEniProbable                               ] = new MIL_RC                                     ( eRC_PresenceEniProbable                                , T_MsgCR_cr_cr_presence_eni_probable                                   );  
    rcs_[ eRC_Decroche                                          ] = new MIL_RC                                     ( eRC_Decroche                                           , T_MsgCR_cr_cr_decroche                                                );  
    rcs_[ eRC_EniSurObjectif                                    ] = new MIL_RC_EniSurObjectif                      ( eRC_EniSurObjectif                                     , T_MsgCR_cr_cr_eni_sur_objectif                                        );  
    rcs_[ eRC_Riposte                                           ] = new MIL_RC                                     ( eRC_Riposte                                            , T_MsgCR_cr_cr_riposte                                                 );  
    rcs_[ eRC_SuitUnite                                         ] = new MIL_RC                                     ( eRC_SuitUnite                                          , T_MsgCR_cr_cr_suit_unite                                              );  
    rcs_[ eRC_PasseEnAppui                                      ] = new MIL_RC                                     ( eRC_PasseEnAppui                                       , T_MsgCR_cr_cr_passe_en_appui                                          );  
    rcs_[ eRC_EnAppui                                           ] = new MIL_RC                                     ( eRC_EnAppui                                            , T_MsgCR_cr_cr_en_appui                                                );  
    rcs_[ eRC_SituationLogFavorable                             ] = new MIL_RC                                     ( eRC_SituationLogFavorable                              , T_MsgCR_cr_cr_situation_log_favorable                                 );        
    rcs_[ eRC_SituationLogDefavorable                           ] = new MIL_RC_SituationLogDefavorable             ( eRC_SituationLogDefavorable                            , T_MsgCR_cr_cr_situation_log_defavorable                               ); 
    rcs_[ eRC_DetruitTactiquement                               ] = new MIL_RC                                     ( eRC_DetruitTactiquement                                , T_MsgCR_cr_cr_detruit_tactiquement                                    );  
    rcs_[ eRC_ObservationTirIndirect                            ] = new MIL_RC_ObservationTirIndirect              ( eRC_ObservationTirIndirect                             , T_MsgCR_cr_cr_observation_tir_indirect                                );
    rcs_[ eRC_AmbianceVitesse                                   ] = new MIL_RC                                     ( eRC_AmbianceVitesse                                    , T_MsgCR_cr_cr_ambiance_vitesse                                        );  
    rcs_[ eRC_AmbianceSurete                                    ] = new MIL_RC                                     ( eRC_AmbianceSurete                                     , T_MsgCR_cr_cr_ambiance_surete                                         );  
    rcs_[ eRC_SurLima                                           ] = new MIL_RC_SurLima                             ( eRC_SurLima                                            , T_MsgCR_cr_cr_sur_lima                                                );  
    rcs_[ eRC_SurTypeTerrainParticulier                         ] = new MIL_RC_SurTypeTerrainParticulier           ( eRC_SurTypeTerrainParticulier                          , T_MsgCR_cr_cr_sur_type_terrain_particulier                            );    
    rcs_[ eRC_TravauxRealises                                   ] = new MIL_RC                                     ( eRC_TravauxRealises                                    , T_MsgCR_cr_cr_travaux_realises                                        );  
    rcs_[ eRC_ObstacleDegage                                    ] = new MIL_RC                                     ( eRC_ObstacleDegage                                     , T_MsgCR_cr_cr_obstacle_degage                                         );  
    rcs_[ eRC_OrganisationChantier                              ] = new MIL_RC                                     ( eRC_OrganisationChantier                               , T_MsgCR_cr_cr_organisation_chantier                                   );  
    rcs_[ eRC_DesorganisationChantier                           ] = new MIL_RC                                     ( eRC_DesorganisationChantier                            , T_MsgCR_cr_cr_desorganisation_chantier                                );  
    rcs_[ eRC_ReconnaissancePositionObstacle                    ] = new MIL_RC                                     ( eRC_ReconnaissancePositionObstacle                     , T_MsgCR_cr_cr_reconnaissance_position_obstacle                        );  
    rcs_[ eRC_ContournementRealise                              ] = new MIL_RC                                     ( eRC_ContournementRealise                               , T_MsgCR_cr_cr_contournement_realise                                   );  
    rcs_[ eRC_AttenteRenforcement                               ] = new MIL_RC                                     ( eRC_AttenteRenforcement                                , T_MsgCR_cr_cr_attente_renforcement                                    );  
    rcs_[ eRC_DebutTravaux                                      ] = new MIL_RC                                     ( eRC_DebutTravaux                                       , T_MsgCR_cr_cr_debut_travaux                                           );  
    rcs_[ eRC_DebutFranchissement                               ] = new MIL_RC                                     ( eRC_DebutFranchissement                                , T_MsgCR_cr_cr_debut_franchissement                                    );  
    rcs_[ eRC_FinFranchissement                                 ] = new MIL_RC                                     ( eRC_FinFranchissement                                  , T_MsgCR_cr_cr_fin_franchissement                                      );     
    rcs_[ eRC_SectionDeployee                                   ] = new MIL_RC                                     ( eRC_SectionDeployee                                    , T_MsgCR_cr_cr_section_deployee                                        );  
    rcs_[ eRC_ReconnaissanceTerminee                            ] = new MIL_RC                                     ( eRC_ReconnaissanceTerminee                             , T_MsgCR_cr_cr_reconnaissance_terminee                                 ); 
    rcs_[ eRC_ReconnaissanceInitialeTerminee                    ] = new MIL_RC                                     ( eRC_ReconnaissanceInitialeTerminee                     , T_MsgCR_cr_cr_reconnaissance_initiale_terminee                        ); 
    rcs_[ eRC_PremierCoupParti                                  ] = new MIL_RC                                     ( eRC_PremierCoupParti                                   , T_MsgCR_cr_cr_premier_coup_parti                                      ); 
    rcs_[ eRC_TirExecute                                        ] = new MIL_RC                                     ( eRC_TirExecute                                         , T_MsgCR_cr_cr_tir_execute                                             ); 
    rcs_[ eRC_RejointPlotRavitaillement                         ] = new MIL_RC                                     ( eRC_RejointPlotRavitaillement                          , T_MsgCR_cr_cr_rejoint_plot_ravitaillement                             ); 
    rcs_[ eRC_ChangementDePosition                              ] = new MIL_RC                                     ( eRC_ChangementDePosition                               , T_MsgCR_cr_cr_changement_de_position                                  ); 
    rcs_[ eRC_DebutSurveillance                                 ] = new MIL_RC                                     ( eRC_DebutSurveillance                                  , T_MsgCR_cr_cr_debut_surveillance                                      ); 
    rcs_[ eRC_DebutInstallation                                 ] = new MIL_RC                                     ( eRC_DebutInstallation                                  , T_MsgCR_cr_cr_debut_installation                                      ); 
    rcs_[ eRC_PretPourConduiteTir                               ] = new MIL_RC                                     ( eRC_PretPourConduiteTir                                , T_MsgCR_cr_cr_pret_pour_conduite_tir                                  ); 
    rcs_[ eRC_ContamineNBC                                      ] = new MIL_RC                                     ( eRC_ContamineNBC                                       , T_MsgCR_cr_cr_contamine_nbc                                           ); 
    rcs_[ eRC_DecontamineNBC                                    ] = new MIL_RC                                     ( eRC_DecontamineNBC                                     , T_MsgCR_cr_cr_decontamine_nbc                                         ); 
    rcs_[ eRC_TenueProtectionNBCMise                            ] = new MIL_RC                                     ( eRC_TenueProtectionNBCMise                             , T_MsgCR_cr_cr_tenue_protection_nbc_mise                               ); 
    rcs_[ eRC_TenueProtectionNBCEnlevee                         ] = new MIL_RC                                     ( eRC_TenueProtectionNBCEnlevee                          , T_MsgCR_cr_cr_tenue_protection_nbc_enlevee                            ); 
    rcs_[ eRC_ReussiteRenforcement                              ] = new MIL_RC_ReussiteRenforcement                ( eRC_ReussiteRenforcement                               , T_MsgCR_cr_cr_reussite_renforcement                                   ); 
    rcs_[ eRC_EchecRenforcement                                 ] = new MIL_RC_EchecRenforcement                   ( eRC_EchecRenforcement                                  , T_MsgCR_cr_cr_echec_renforcement                                      ); 
    rcs_[ eRC_PreparationMission                                ] = new MIL_RC_PreparationMission                  ( eRC_PreparationMission                                 , T_MsgCR_cr_cr_preparation_mission                                     ); 
    rcs_[ eRC_DebutMission                                      ] = new MIL_RC                                     ( eRC_DebutMission                                       , T_MsgCR_cr_cr_debut_mission                                           ); 
    rcs_[ eRC_FinMission                                        ] = new MIL_RC                                     ( eRC_FinMission                                         , T_MsgCR_cr_cr_fin_mission                                             ); 
    rcs_[ eRC_Automate_PriseContact                             ] = new MIL_RC                                     ( eRC_Automate_PriseContact                              , T_MsgCR_cr_cr_automate_prise_contact                                  ); 
    rcs_[ eRC_Automate_RuptureContact                           ] = new MIL_RC                                     ( eRC_Automate_RuptureContact                            , T_MsgCR_cr_cr_automate_rupture_contact                                ); 
    rcs_[ eRC_DispositifInstalle                                ] = new MIL_RC                                     ( eRC_DispositifInstalle                                 , T_MsgCR_cr_cr_dispositif_installe                                     ); 
    rcs_[ eRC_EnPosition                                        ] = new MIL_RC                                     ( eRC_EnPosition                                         , T_MsgCR_cr_cr_en_position                                             ); 
    rcs_[ eRC_ObjectifAtteint                                   ] = new MIL_RC                                     ( eRC_ObjectifAtteint                                    , T_MsgCR_cr_cr_objectif_atteint                                        ); 
    rcs_[ eRC_FuseauRejoint                                     ] = new MIL_RC                                     ( eRC_FuseauRejoint                                      , T_MsgCR_cr_cr_fuseau_rejoint                                          ); 
    rcs_[ eRC_Debouche                                          ] = new MIL_RC                                     ( eRC_Debouche                                           , T_MsgCR_cr_cr_debouche                                                ); 
    rcs_[ eRC_AlerteNBC                                         ] = new MIL_RC                                     ( eRC_AlerteNBC                                          , T_MsgCR_cr_cr_alerte_nbc                                              ); 
    rcs_[ eRC_SiteDecontaminationAnime                          ] = new MIL_RC                                     ( eRC_SiteDecontaminationAnime                           , T_MsgCR_cr_cr_site_decontamination_anime                              ); 
    rcs_[ eRC_SiteDecontaminationDesactive                      ] = new MIL_RC                                     ( eRC_SiteDecontaminationDesactive                       , T_MsgCR_cr_cr_site_decontamination_desactive                          ); 
    rcs_[ eRC_DemandeDecontamination                            ] = new MIL_RC                                     ( eRC_DemandeDecontamination                             , T_MsgCR_cr_cr_demande_decontamination                                 ); 
    rcs_[ eRC_Releve                                            ] = new MIL_RC_Releve                              ( eRC_Releve                                             , T_MsgCR_cr_cr_releve                                                  ); 
    rcs_[ eRC_SurPIA                                            ] = new MIL_RC_SurPIA                              ( eRC_SurPIA                                             , T_MsgCR_cr_cr_sur_pia                                                 ); 
    rcs_[ eRC_PassageSurPIA                                     ] = new MIL_RC                                     ( eRC_PassageSurPIA                                      , T_MsgCR_cr_cr_passage_sur_pia                                         ); 
    rcs_[ eRC_UniteRecueillie                                   ] = new MIL_RC_UniteRecueillie                     ( eRC_UniteRecueillie                                    , T_MsgCR_cr_cr_unite_recueillie                                        ); 
    rcs_[ eRC_EnControleZone                                    ] = new MIL_RC                                     ( eRC_EnControleZone                                     , T_MsgCR_cr_cr_en_controle_zone                                        ); 
    rcs_[ eRC_EnSurveillance                                    ] = new MIL_RC                                     ( eRC_EnSurveillance                                     , T_MsgCR_cr_cr_en_surveillance                                         ); 
    rcs_[ eRC_EnInfiltration                                    ] = new MIL_RC                                     ( eRC_EnInfiltration                                     , T_MsgCR_cr_cr_en_infiltration                                         ); 
    rcs_[ eRC_EnExfiltration                                    ] = new MIL_RC                                     ( eRC_EnExfiltration                                     , T_MsgCR_cr_cr_en_exfiltration                                         ); 
    rcs_[ eRC_AttenteConditionFavorablePourInfiltration         ] = new MIL_RC                                     ( eRC_AttenteConditionFavorablePourInfiltration          , T_MsgCR_cr_cr_attente_condition_favorable_pour_infiltration           ); 
    rcs_[ eRC_ReceptionOrdreDeFeu                               ] = new MIL_RC                                     ( eRC_ReceptionOrdreDeFeu                                , T_MsgCR_cr_cr_reception_ordre_de_feu                                  ); 
    rcs_[ eRC_PretAuTir                                         ] = new MIL_RC                                     ( eRC_PretAuTir                                          , T_MsgCR_cr_cr_pret_au_tir                                             ); 
    rcs_[ eRC_ItineraireRetabli                                 ] = new MIL_RC                                     ( eRC_ItineraireRetabli                                  , T_MsgCR_cr_cr_itineraire_retabli                                      ); 
    rcs_[ eRC_RenforcementBientotFini                           ] = new MIL_RC_RenforcementBientotFini             ( eRC_RenforcementBientotFini                            , T_MsgCR_cr_cr_renforcement_bientot_fini                               ); 
    rcs_[ eRC_EnCoursDeFranchissement                           ] = new MIL_RC                                     ( eRC_EnCoursDeFranchissement                            , T_MsgCR_cr_cr_en_cours_de_franchissement                              );
    rcs_[ eRC_MaterielRetourDeMaintenance                       ] = new MIL_RC                                     ( eRC_MaterielRetourDeMaintenance                        , T_MsgCR_cr_cr_materiel_retour_de_maintenance                          );
    rcs_[ eRC_AttenteOrdrePoursuivre                            ] = new MIL_RC                                     ( eRC_AttenteOrdrePoursuivre                             , T_MsgCR_cr_cr_attente_ordre_poursuivre                                );
    rcs_[ eRC_AttenteOrdreDeboucher                             ] = new MIL_RC                                     ( eRC_AttenteOrdreDeboucher                              , T_MsgCR_cr_cr_attente_ordre_deboucher                                 );
    rcs_[ eRC_DroneLance                                        ] = new MIL_RC                                     ( eRC_DroneLance                                         , T_MsgCR_cr_cr_drone_lance                                             );
    rcs_[ eRC_DroneEnExploitation                               ] = new MIL_RC                                     ( eRC_DroneEnExploitation                                , T_MsgCR_cr_cr_drone_en_exploitation                                   );
    rcs_[ eRC_DroneExploite                                     ] = new MIL_RC                                     ( eRC_DroneExploite                                      , T_MsgCR_cr_cr_drone_exploite                                          );
    rcs_[ eRC_Brouille                                          ] = new MIL_RC                                     ( eRC_Brouille                                           , T_MsgCR_cr_cr_brouille                                                );
    rcs_[ eRC_EnEcoute                                          ] = new MIL_RC                                     ( eRC_EnEcoute                                           , T_MsgCR_cr_cr_en_ecoute                                               );
    rcs_[ eRC_EnEcouteRadar                                     ] = new MIL_RC                                     ( eRC_EnEcouteRadar                                      , T_MsgCR_cr_cr_en_ecoute_radar                                         );
    rcs_[ eRC_DemandeRavitaillement                             ] = new MIL_RC                                     ( eRC_DemandeRavitaillement                              , T_MsgCR_cr_cr_demande_ravitaillement                                  );
    rcs_[ eRC_DemandeEvacuationSanitaire                        ] = new MIL_RC                                     ( eRC_DemandeEvacuationSanitaire                         , T_MsgCR_cr_cr_demande_evacuation_sanitaire                            );
    rcs_[ eRC_DemandeEvacuationMateriel                         ] = new MIL_RC                                     ( eRC_DemandeEvacuationMateriel                          , T_MsgCR_cr_cr_demande_evacuation_materiel                             ); 
    rcs_[ eRC_EnCoursRavitaillement                             ] = new MIL_RC                                     ( eRC_EnCoursRavitaillement                              , T_MsgCR_cr_cr_en_cours_ravitaillement                                 ); 
    rcs_[ eRC_RavitaillementTermine                             ] = new MIL_RC                                     ( eRC_RavitaillementTermine                              , T_MsgCR_cr_cr_ravitaillement_termine                                  );
    rcs_[ eRC_HelicoptereEnObservation                          ] = new MIL_RC                                     ( eRC_HelicoptereEnObservation                           , T_MsgCR_cr_cr_helicoptere_en_observation                              );
    rcs_[ eRC_PerteInfoGuidage                                  ] = new MIL_RC                                     ( eRC_PerteInfoGuidage                                   , T_MsgCR_cr_cr_perte_info_guidage                                      );
    rcs_[ eRC_ZoneReconnue                                      ] = new MIL_RC                                     ( eRC_ZoneReconnue                                       , T_MsgCR_cr_cr_zone_reconnue                                           );
    rcs_[ eRC_ZoneDecontaminee                                  ] = new MIL_RC                                     ( eRC_ZoneDecontaminee                                   , T_MsgCR_cr_cr_zone_decontaminee                                       );
    rcs_[ eRC_SiteROTAReconnu                                   ] = new MIL_RC                                     ( eRC_SiteROTAReconnu                                    , T_MsgCR_cr_cr_site_rota_reconnu                                       );
    rcs_[ eRC_IncidentNBCTermine                                ] = new MIL_RC                                     ( eRC_IncidentNBCTermine                                 , T_MsgCR_cr_cr_incident_nbc_termine                                    );
    rcs_[ eRC_ObstacleEnAttenteActivation                       ] = new MIL_RC_ObstacleEnAttenteActivation         ( eRC_ObstacleEnAttenteActivation                        , T_MsgCR_cr_cr_obstacle_en_attente_activation                          ); 
    rcs_[ eRC_AllocationConsentieBientotEpuisee                 ] = new MIL_RC_AllocationConsentieBientotEpuisee   ( eRC_AllocationConsentieBientotEpuisee                  , T_MsgCR_cr_cr_allocation_consentie_bientot_epuisee                    ); 
    rcs_[ eRC_PionDecontamine                                   ] = new MIL_RC_PionDecontamine                     ( eRC_PionDecontamine                                    , T_MsgCR_cr_cr_pion_decontamine                                        ); 
    rcs_[ eRC_TirDansZoneInterdite                              ] = new MIL_RC                                     ( eRC_TirDansZoneInterdite                               , T_MsgCR_cr_cr_tir_dans_zone_interdite                                 );
    rcs_[ eRC_TirSurCampAmi                                     ] = new MIL_RC_TirSurCampAmi                       ( eRC_TirSurCampAmi                                      , T_MsgCR_cr_cr_tir_sur_camp_ami                                        );
    rcs_[ eRC_TireParCampAmi                                    ] = new MIL_RC_TireParCampAmi                      ( eRC_TireParCampAmi                                     , T_MsgCR_cr_cr_tire_par_camp_ami                                       );
    rcs_[ eRC_TirSurCampNeutre                                  ] = new MIL_RC_TirSurCampNeutre                    ( eRC_TirSurCampNeutre                                   , T_MsgCR_cr_cr_tir_sur_camp_neutre                                     );
    rcs_[ eRC_TireParCampNeutre                                 ] = new MIL_RC_TireParCampNeutre                   ( eRC_TireParCampNeutre                                  , T_MsgCR_cr_cr_tire_par_camp_neutre                                    );
    rcs_[ eRC_TirSurCivil                                       ] = new MIL_RC_TirSurCivil                         ( eRC_TirSurCivil                                        , T_MsgCR_cr_cr_tir_sur_civil                                           );
    rcs_[ eRC_TireParCivil                                      ] = new MIL_RC_TireParCivil                        ( eRC_TireParCivil                                       , T_MsgCR_cr_cr_tire_par_civil                                          );
    rcs_[ eRC_DestructionPC                                     ] = new MIL_RC                                     ( eRC_DestructionPC                                      , T_MsgCR_cr_cr_destruction_pc                                          );
    rcs_[ eRC_DecesBlesse                                       ] = new MIL_RC                                     ( eRC_DecesBlesse                                        , T_MsgCR_cr_cr_deces_blesse                                            );
    rcs_[ eRC_DecesBlessePendantTransport                       ] = new MIL_RC                                     ( eRC_DecesBlessePendantTransport                        , T_MsgCR_cr_cr_deces_blesse_pendant_transport                          );
    rcs_[ eRC_DecesBlessePendantHospitalisation                 ] = new MIL_RC                                     ( eRC_DecesBlessePendantHospitalisation                  , T_MsgCR_cr_cr_deces_blesse_pendant_hospitalisation                    );
    rcs_[ eRC_PresencePopulation                                ] = new MIL_RC                                     ( eRC_PresencePopulation                                 , T_MsgCR_cr_cr_presence_population                                     );
    rcs_[ eRC_OrientationPopulationVersCampRefugies             ] = new MIL_RC                                     ( eRC_OrientationPopulationVersCampRefugies              , T_MsgCR_cr_cr_orientation_population_vers_camp_refugies               );
    rcs_[ eRC_PasDeCampRefugiesDisponible                       ] = new MIL_RC                                     ( eRC_PasDeCampRefugiesDisponible                        , T_MsgCR_cr_cr_pas_de_camp_refugies_disponible                         );
    rcs_[ eRC_PresenceEnnemiRendu                               ] = new MIL_RC                                     ( eRC_PresenceEnnemiRendu                                , T_MsgCR_cr_cr_presence_ennemi_rendu                                   );
    rcs_[ eRC_OrientationEnnemiRenduVersCampPrisonniers         ] = new MIL_RC                                     ( eRC_OrientationEnnemiRenduVersCampPrisonniers          , T_MsgCR_cr_cr_orientation_ennemi_rendu_vers_camp_prisonniers          );
    rcs_[ eRC_PasDeCampPrisonniersDisponible                    ] = new MIL_RC                                     ( eRC_PasDeCampPrisonniersDisponible                     , T_MsgCR_cr_cr_pas_de_camp_prisonniers_disponible                      );
    rcs_[ eRC_TransportUnitePasPrete                            ] = new MIL_RC_TransportUnitePasPrete              ( eRC_TransportUnitePasPrete                             , T_MsgCR_cr_cr_transport_unite_pas_prete                               );
    rcs_[ eRC_Trace                                             ] = new MIL_RC_Trace                               ( eRC_Trace                                              , T_MsgCR_cr_cr_trace                                                   );
    rcs_[ eRC_TraceID                                           ] = new MIL_RC_TraceID                             ( eRC_TraceID                                            , T_MsgCR_cr_cr_trace_id                                                );
    rcs_[ eRC_TransportEnCours                                  ] = new MIL_RC                                     ( eRC_TransportEnCours                                   , T_MsgCR_cr_cr_transport_en_cours                                      );
    rcs_[ eRC_TransportEmbarquement                             ] = new MIL_RC                                     ( eRC_TransportEmbarquement                              , T_MsgCR_cr_cr_transport_embarquement                                  );
    rcs_[ eRC_TransportDebarquement                             ] = new MIL_RC                                     ( eRC_TransportDebarquement                              , T_MsgCR_cr_cr_transport_debarquement                                  );
    rcs_[ eRC_ZoneImplantationOccupee                           ] = new MIL_RC                                     ( eRC_ZoneImplantationOccupee                            , T_MsgCR_cr_cr_zone_implantation_occupee                               );
    rcs_[ eRC_InstallationSurZoneImplantation                   ] = new MIL_RC                                     ( eRC_InstallationSurZoneImplantation                    , T_MsgCR_cr_cr_installation_sur_zone_implantation                      );
    rcs_[ eRC_TirHorsDePortee                                   ] = new MIL_RC                                     ( eRC_TirHorsDePortee                                    , T_MsgCR_cr_cr_tir_hors_de_portee                                      );
    rcs_[ eRC_TirAnnule                                         ] = new MIL_RC                                     ( eRC_TirAnnule                                          , T_MsgCR_cr_cr_tir_annule                                              );
    rcs_[ eRC_DebutMiseEnOeuvreDrone                            ] = new MIL_RC                                     ( eRC_DebutMiseEnOeuvreDrone                             , T_MsgCR_cr_cr_debut_mise_en_oeuvre_drone                              );
    rcs_[ eRC_FinMiseEnOeuvreDrone                              ] = new MIL_RC                                     ( eRC_FinMiseEnOeuvreDrone                               , T_MsgCR_cr_cr_fin_mise_en_oeuvre_drone                                );
    rcs_[ eRC_VehiculeLaisseSurPosition                         ] = new MIL_RC                                     ( eRC_VehiculeLaisseSurPosition                          , T_MsgCR_cr_cr_vehicule_laisse_sur_position                            );
    rcs_[ eRC_RejointPositionEmbarquement                       ] = new MIL_RC                                     ( eRC_RejointPositionEmbarquement                        , T_MsgCR_cr_cr_rejoint_position_embarquement                           );
    rcs_[ eRC_EnAttenteTransporteurs                            ] = new MIL_RC                                     ( eRC_EnAttenteTransporteurs                             , T_MsgCR_cr_cr_en_attente_transporteurs                                );
    rcs_[ eRC_MissionImpossiblePionsSubordonnesNonOperationnels ] = new MIL_RC                                     ( eRC_MissionImpossiblePionsSubordonnesNonOperationnels  , T_MsgCR_cr_cr_mission_impossible_pions_subordonnes_non_operationnels  );
    rcs_[ eRC_MissionImpossibleLimaInvalide                     ] = new MIL_RC                                     ( eRC_MissionImpossibleLimaInvalide                      , T_MsgCR_cr_cr_mission_impossible_lima_invalide                        );
    rcs_[ eRC_MissionImpossibleZoneHorsFuseau                   ] = new MIL_RC                                     ( eRC_MissionImpossibleZoneHorsFuseau                    , T_MsgCR_cr_cr_mission_impossible_zone_hors_fuseau                     );
    rcs_[ eRC_RapportDeForceDefavorable                         ] = new MIL_RC                                     ( eRC_RapportDeForceDefavorable                          , T_MsgCR_cr_cr_rapport_de_force_defavorable                            );
    rcs_[ eRC_TerrainDifficile                                  ] = new MIL_RC                                     ( eRC_TerrainDifficile                                   , T_MsgCR_cr_cr_terrain_difficile                                       );
    rcs_[ eRC_PlusDeCarburant                                   ] = new MIL_RC                                     ( eRC_PlusDeCarburant                                    , T_MsgCR_cr_cr_plus_de_carburant                                       );
    rcs_[ eRC_DispositifDisperse                                ] = new MIL_RC                                     ( eRC_DispositifDisperse                                 , T_MsgCR_cr_cr_dispositif_disperse                                     );
    rcs_[ eRC_PionNeutralise                                    ] = new MIL_RC                                     ( eRC_PionNeutralise                                     , T_MsgCR_cr_cr_pion_neutralise                                         );
    rcs_[ eRC_SilenceRadio                                      ] = new MIL_RC                                     ( eRC_SilenceRadio                                       , T_MsgCR_cr_cr_silence_radio                                           );
    rcs_[ eRC_TirRestreint                                      ] = new MIL_RC                                     ( eRC_TirRestreint                                       , T_MsgCR_cr_cr_tir_restreint                                           );
    rcs_[ eRC_TirInterdit                                       ] = new MIL_RC                                     ( eRC_TirInterdit                                        , T_MsgCR_cr_cr_tir_interdit                                            );
    rcs_[ eRC_TirLibre                                          ] = new MIL_RC                                     ( eRC_TirLibre                                           , T_MsgCR_cr_cr_tir_libre                                               );                                                                                                                     

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
    pRcPionDecontamine_                   = (MIL_RC_PionDecontamine*)                  rcs_[ eRC_PionDecontamine                   ];
    pRcPlusDeCarburant_                   =                                            rcs_[ eRC_PlusDeCarburant                   ];
    pRcTerrainDifficile_                  =                                            rcs_[ eRC_TerrainDifficile                  ];

    // Internes Sim                                       
    rcs_[ eRC_MissionTerminee                            ] = new MIL_RC_MissionTerminee                ( eRC_MissionTerminee );      
    rcs_[ eRC_EnPause                                    ] = new MIL_RC_EnPause                        ( eRC_EnPause         );
    rcs_[ eRC_EnEsquive                                  ] = new MIL_RC_EnEsquive                      ( eRC_EnEsquive       );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC::Terminate()
{
    // Voir pb RC statiques et non statiques pour destruction
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
