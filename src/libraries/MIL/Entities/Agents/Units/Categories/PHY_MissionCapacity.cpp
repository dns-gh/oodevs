// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_MissionCapacity.cpp $
// $Author: Jvt $
// $Modtime: 9/12/04 15:09 $
// $Revision: 1 $
// $Workfile: PHY_MissionCapacity.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_MissionCapacity.h"

PHY_MissionCapacity::T_MissionCapacityMap PHY_MissionCapacity::missionCapacity_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MissionCapacity::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_MissionCapacity::Initialize()
{
    missionCapacity_[ "Civilian law enforcement"                    ] = new PHY_MissionCapacity( "Civilian law enforcement"                    , eCivilianLawEnforcement                 , EnumUnitCapaciteMission::civilian_law_enforcement                  );
    missionCapacity_[ "Command operations"                          ] = new PHY_MissionCapacity( "Command operations"                          , eCommandOperations                      , EnumUnitCapaciteMission::command_operations                        );
    missionCapacity_[ "Couverture aerienne"                         ] = new PHY_MissionCapacity( "Couverture aerienne"                         , eCouvertureAerienne                     , EnumUnitCapaciteMission::couverture_aerienne                       );
    missionCapacity_[ "Joint intelligence"                          ] = new PHY_MissionCapacity( "Joint intelligence"                          , eJointIntelligence                      , EnumUnitCapaciteMission::joint_intelligence                        );
    missionCapacity_[ "Morale, welfare recreation"                  ] = new PHY_MissionCapacity( "Morale, welfare recreation"                  , eMoraleWelfareRecreation                , EnumUnitCapaciteMission::morale_welfare_recreation                 );
    missionCapacity_[ "Reglage electronique"                        ] = new PHY_MissionCapacity( "Reglage electronique"                        , eReglageElectronique                    , EnumUnitCapaciteMission::reglage_electronique                      );
    missionCapacity_[ "Remplacement"                                ] = new PHY_MissionCapacity( "Remplacement"                                , eRemplacement                           , EnumUnitCapaciteMission::remplacement                              );
    missionCapacity_[ "Service mortuaire"                           ] = new PHY_MissionCapacity( "Service mortuaire"                           , eServiceMortuaire                       , EnumUnitCapaciteMission::service_mortuaire                         );
    missionCapacity_[ "Signals intelligence (SIGINT)"               ] = new PHY_MissionCapacity( "Signals intelligence (SIGINT)"               , eSignalsIntelligence                    , EnumUnitCapaciteMission::signals_intelligence                      );
    missionCapacity_[ "Theatre missile defence"                     ] = new PHY_MissionCapacity( "Theatre missile defence"                     , eTheatreMissileDefence                  , EnumUnitCapaciteMission::theatre_missile_defence                   );
    missionCapacity_[ "Utilitaire"                                  ] = new PHY_MissionCapacity( "Utilitaire"                                  , eUtilitaire                             , EnumUnitCapaciteMission::utilitaire                                );
    missionCapacity_[ "Aeroporte"                                   ] = new PHY_MissionCapacity( "Aeroporte"                                   , eAéroporte                              , EnumUnitCapaciteMission::aeroporte                                 );
    missionCapacity_[ "Affaires publiques"                          ] = new PHY_MissionCapacity( "Affaires publiques"                          , eAffairesPubliques                      , EnumUnitCapaciteMission::affaires_publiques                        );
    missionCapacity_[ "Affaires publiques, communication"           ] = new PHY_MissionCapacity( "Affaires publiques, communication"           , eAffairesPubliques_Communication        , EnumUnitCapaciteMission::affaires_publiques_communication          );
    missionCapacity_[ "Affaires publiques, joint information"       ] = new PHY_MissionCapacity( "Affaires publiques, joint information"       , eAffairesPubliques_JointInformation     , EnumUnitCapaciteMission::affaires_publiques_joint_information      );
    missionCapacity_[ "Alpin"                                       ] = new PHY_MissionCapacity( "Alpin"                                       , eAlpin                                  , EnumUnitCapaciteMission::alpin                                     );
    missionCapacity_[ "Amphibie"                                    ] = new PHY_MissionCapacity( "Amphibie"                                    , eAmphibie                               , EnumUnitCapaciteMission::amphibie                                  );
    missionCapacity_[ "Arctique"                                    ] = new PHY_MissionCapacity( "Arctique"                                    , eArctique                               , EnumUnitCapaciteMission::arctique                                  );
    missionCapacity_[ "Artillerie - reconnaissance"                 ] = new PHY_MissionCapacity( "Artillerie - reconnaissance"                 , eArtillerieReconnaissance               , EnumUnitCapaciteMission::artillerie_reconnaissance                 );
    missionCapacity_[ "Attaque"                                     ] = new PHY_MissionCapacity( "Attaque"                                     , eAttaque                                , EnumUnitCapaciteMission::attaque                                   );
    missionCapacity_[ "Attaque aerienne"                            ] = new PHY_MissionCapacity( "Attaque aerienne"                            , eAttaqueAerienne                        , EnumUnitCapaciteMission::attaque_aerienne                          );
    missionCapacity_[ "C2"                                          ] = new PHY_MissionCapacity( "C2"                                          , eC2                                     , EnumUnitCapaciteMission::c2                                        );
    missionCapacity_[ "Chemin de fer"                               ] = new PHY_MissionCapacity( "Chemin de fer"                               , eCheminDeFer                            , EnumUnitCapaciteMission::chemin_de_fer                             );
    missionCapacity_[ "Ciblage"                                     ] = new PHY_MissionCapacity( "Ciblage"                                     , eCiblage                                , EnumUnitCapaciteMission::ciblage                                   );
    missionCapacity_[ "Contre espionnage"                           ] = new PHY_MissionCapacity( "Contre espionnage"                           , eContreEspionnage                       , EnumUnitCapaciteMission::contre_espionnage                         );
    missionCapacity_[ "Eclairage"                                   ] = new PHY_MissionCapacity( "Eclairage"                                   , eEclairage                              , EnumUnitCapaciteMission::eclairage                                 );
    missionCapacity_[ "Entretien"                                   ] = new PHY_MissionCapacity( "Entretien"                                   , eEntretien                              , EnumUnitCapaciteMission::entretien                                 );
    missionCapacity_[ "Entretien, materiel"                         ] = new PHY_MissionCapacity( "Entretien, materiel"                         , eEntretienMateriel                      , EnumUnitCapaciteMission::entretien_materiel                        );
    missionCapacity_[ "Entretien, missile"                          ] = new PHY_MissionCapacity( "Entretien, missile"                          , eEntretienMissile                       , EnumUnitCapaciteMission::entretien_missile                         );
    missionCapacity_[ "Entretien, opto-electronique"                ] = new PHY_MissionCapacity( "Entretien, opto-electronique"                , eEntretienOptoElectronique              , EnumUnitCapaciteMission::entretien_opto_electronique               );
    missionCapacity_[ "Espionnage"                                  ] = new PHY_MissionCapacity( "Espionnage"                                  , eEspionnage                             , EnumUnitCapaciteMission::espionnage                                );
    missionCapacity_[ "Espionnage militaire, aerial exploitation"   ] = new PHY_MissionCapacity( "Espionnage militaire, aerial exploitation"   , eEspionnageMilitaireAerialExploitation  , EnumUnitCapaciteMission::espionnage_militaire_aerial_exploitation  );
    missionCapacity_[ "Espionnage militaire, operation"             ] = new PHY_MissionCapacity( "Espionnage militaire, operation"             , eEspionnageMilitaireOperation           , EnumUnitCapaciteMission::espionnage_militaire_operation            );
    missionCapacity_[ "Espionnage militaire, tactical exploitation" ] = new PHY_MissionCapacity( "Espionnage militaire, tactical exploitation" , eEspionnageMilitaireTacticalExploitation, EnumUnitCapaciteMission::espionnage_militaire_tactical_exploitation);
    missionCapacity_[ "Evacuation sanitaire"                        ] = new PHY_MissionCapacity( "Evacuation sanitaire"                        , eEvacuationSanitaire                    , EnumUnitCapaciteMission::evacuation_sanitaire                      );
    missionCapacity_[ "Finance"                                     ] = new PHY_MissionCapacity( "Finance"                                     , eFinance                                , EnumUnitCapaciteMission::finance                                   );
    missionCapacity_[ "Genie, combat"                               ] = new PHY_MissionCapacity( "Genie, combat"                               , eGenieCombat                            , EnumUnitCapaciteMission::genie_combat                              );
    missionCapacity_[ "Genie, construction"                         ] = new PHY_MissionCapacity( "Genie, construction"                         , eGenieConstruction                      , EnumUnitCapaciteMission::genie_construction                        );
    missionCapacity_[ "Genie, construction navale"                  ] = new PHY_MissionCapacity( "Genie, construction navale"                  , eGenieConstructionNavale                , EnumUnitCapaciteMission::genie_construction_navale                 );
    missionCapacity_[ "Guerre electronique"                         ] = new PHY_MissionCapacity( "Guerre electronique"                         , eGuerreElectronique                     , EnumUnitCapaciteMission::guerre_electronique                       );
    missionCapacity_[ "Guerre electronique, interception"           ] = new PHY_MissionCapacity( "Guerre electronique, interception"           , eGuerreElectroniqueInterception         , EnumUnitCapaciteMission::guerre_electronique_interception          );
    missionCapacity_[ "Guerre electronique, brouillage"             ] = new PHY_MissionCapacity( "Guerre electronique, brouillage"             , eGuerreElectroniqueBrouillage           , EnumUnitCapaciteMission::guerre_electronique_brouillage            );
    missionCapacity_[ "Guerre electronique, detection"              ] = new PHY_MissionCapacity( "Guerre electronique, detection"              , eGuerreElectroniqueDétection            , EnumUnitCapaciteMission::guerre_electronique_detection             );
    missionCapacity_[ "Interrogation"                               ] = new PHY_MissionCapacity( "Interrogation"                               , eInterrogation                          , EnumUnitCapaciteMission::interrogation                             );
    missionCapacity_[ "Main d'oeuvre"                               ] = new PHY_MissionCapacity( "Main d'oeuvre"                               , eMainDOeuvre                            , EnumUnitCapaciteMission::main_d_oeuvre                             );
    missionCapacity_[ "Maintien de la paix"                         ] = new PHY_MissionCapacity( "Maintien de la paix"                         , eMaintienDeLaPaix                       , EnumUnitCapaciteMission::maintien_de_la_paix                       );
    missionCapacity_[ "Marine"                                      ] = new PHY_MissionCapacity( "Marine"                                      , eMarine                                 , EnumUnitCapaciteMission::marine                                    );
    missionCapacity_[ "Mine de contre-mesure"                       ] = new PHY_MissionCapacity( "Mine de contre-mesure"                       , eMineDeContreMesure                     , EnumUnitCapaciteMission::mine_de_contre_mesure                     );
    missionCapacity_[ "NBC, biologique"                             ] = new PHY_MissionCapacity( "NBC, biologique"                             , eNBCBiologique                          , EnumUnitCapaciteMission::nbc_biologique                            );
    missionCapacity_[ "NBC, chimique"                               ] = new PHY_MissionCapacity( "NBC, chimique"                               , eNBCChimique                            , EnumUnitCapaciteMission::nbc_chimique                              );
    missionCapacity_[ "NBC, chimique, fumee"                        ] = new PHY_MissionCapacity( "NBC, chimique, fumee"                        , eNBCChimiqueFumee                       , EnumUnitCapaciteMission::nbc_chimique_fumee                        );
    missionCapacity_[ "NBC, decontamination"                        ] = new PHY_MissionCapacity( "NBC, decontamination"                        , eNBCDecontamination                     , EnumUnitCapaciteMission::nbc_decontamination                       );
    missionCapacity_[ "NBC, decontamination chimique"               ] = new PHY_MissionCapacity( "NBC, decontamination chimique"               , eNBCDecontaminationChimique             , EnumUnitCapaciteMission::nbc_decontamination_chimique              );
    missionCapacity_[ "NBC, nucleaire"                              ] = new PHY_MissionCapacity( "NBC, nucleaire"                              , eNBCNucleaire                           , EnumUnitCapaciteMission::nbc_nucleaire                             );
    missionCapacity_[ "Non renseigne"                               ] = new PHY_MissionCapacity( "Non renseigne"                               , eNonRenseigne                           , EnumUnitCapaciteMission::non_renseigne                             );
    missionCapacity_[ "Observation"                                 ] = new PHY_MissionCapacity( "Observation"                                 , eObservation                            , EnumUnitCapaciteMission::observation                               );
    missionCapacity_[ "Observation, capteur"                        ] = new PHY_MissionCapacity( "Observation, capteur"                        , eObservationCapteur                     , EnumUnitCapaciteMission::observation_capteur                       );
    missionCapacity_[ "Observation, longue portee"                  ] = new PHY_MissionCapacity( "Observation, longue portee"                  , eObservationLonguePortee                , EnumUnitCapaciteMission::observation_longue_portee                 );
    missionCapacity_[ "Observation, meteo"                          ] = new PHY_MissionCapacity( "Observation, meteo"                          , eObservationMeteo                       , EnumUnitCapaciteMission::observation_meteo                         );
    missionCapacity_[ "Observation, module terrestre"               ] = new PHY_MissionCapacity( "Observation, module terrestre"               , eObservationModuleTerrestre             , EnumUnitCapaciteMission::observation_module_terrestre              );
    missionCapacity_[ "Purification de l'eau"                       ] = new PHY_MissionCapacity( "Purification de l'eau"                       , ePurificationDeLEau                     , EnumUnitCapaciteMission::purification_de_l_eau                     );
    missionCapacity_[ "Ravitaillement (class I)"                    ] = new PHY_MissionCapacity( "Ravitaillement (class I)"                    , eRavitaillement_ClasseI                 , EnumUnitCapaciteMission::ravitaillement_class_i                    );
    missionCapacity_[ "Ravitaillement (class II)"                   ] = new PHY_MissionCapacity( "Ravitaillement (class II)"                   , eRavitaillement_ClasseII                , EnumUnitCapaciteMission::ravitaillement_class_ii                   );
    missionCapacity_[ "Ravitaillement (class III aviation)"         ] = new PHY_MissionCapacity( "Ravitaillement (class III aviation)"         , eRavitaillement_ClasseIIIAviation       , EnumUnitCapaciteMission::ravitaillement_class_iii_aviation         );
    missionCapacity_[ "Ravitaillement (class III)"                  ] = new PHY_MissionCapacity( "Ravitaillement (class III)"                  , eRavitaillement_ClasseIII               , EnumUnitCapaciteMission::ravitaillement_class_iii                  );
    missionCapacity_[ "Ravitaillement (class IV)"                   ] = new PHY_MissionCapacity( "Ravitaillement (class IV)"                   , eRavitaillement_ClasseIV                , EnumUnitCapaciteMission::ravitaillement_class_iv                   );
    missionCapacity_[ "Ravitaillement (class V)"                    ] = new PHY_MissionCapacity( "Ravitaillement (class V)"                    , eRavitaillement_ClasseV                 , EnumUnitCapaciteMission::ravitaillement_class_v                    );
    missionCapacity_[ "Ravitaillement (eau)"                        ] = new PHY_MissionCapacity( "Ravitaillement (eau)"                        , eRavitaillement_Eau                     , EnumUnitCapaciteMission::ravitaillement_eau                        );
    missionCapacity_[ "Ravitaillement, blanchisserie/bain"          ] = new PHY_MissionCapacity( "Ravitaillement, blanchisserie/bain"          , eRavitaillementBlanchisserieBain        , EnumUnitCapaciteMission::ravitaillement_blanchisserie_bain         );
    missionCapacity_[ "Recherche & sauvetage"                       ] = new PHY_MissionCapacity( "Recherche & sauvetage"                       , eRechercheEtSauvetage                   , EnumUnitCapaciteMission::recherche_et_sauvetage                    );
    missionCapacity_[ "Reconnaissance"                              ] = new PHY_MissionCapacity( "Reconnaissance"                              , eReconnaissance                         , EnumUnitCapaciteMission::reconnaissance                            );
    missionCapacity_[ "Reconnaissance aerienne tactique"            ] = new PHY_MissionCapacity( "Reconnaissance aerienne tactique"            , eReconnaissanceAerienneTactique         , EnumUnitCapaciteMission::reconnaissance_aerienne_tactique          );
    missionCapacity_[ "Recuperation"                                ] = new PHY_MissionCapacity( "Recuperation"                                , eRecuperation                           , EnumUnitCapaciteMission::recuperation                              );
    missionCapacity_[ "Service du personnel"                        ] = new PHY_MissionCapacity( "Service du personnel"                        , eServiceDuPersonnel                     , EnumUnitCapaciteMission::service_du_personnel                      );
    missionCapacity_[ "Service juridique"                           ] = new PHY_MissionCapacity( "Service juridique"                           , eServiceJuridique                       , EnumUnitCapaciteMission::service_juridique                         );
    missionCapacity_[ "Support aerien rapproche"                    ] = new PHY_MissionCapacity( "Support aerien rapproche"                    , eSupportAerienRapproche                 , EnumUnitCapaciteMission::support_aerien_rapproche                  );
    missionCapacity_[ "Transmissions, forward communications"       ] = new PHY_MissionCapacity( "Transmissions, forward communications"       , eTransmissions_ForwardCommunications    , EnumUnitCapaciteMission::transmissions_forward_communications      );
    missionCapacity_[ "Transmissions, node centre"                  ] = new PHY_MissionCapacity( "Transmissions, node centre"                  , eTransmissions_NodeCentre               , EnumUnitCapaciteMission::transmissions_node_centre                 );
    missionCapacity_[ "Transmissions, node, large extension"        ] = new PHY_MissionCapacity( "Transmissions, node, large extension"        , eTransmissions_NodeLargeExtension       , EnumUnitCapaciteMission::transmissions_node_large_extension        );
    missionCapacity_[ "Transmissions, node, small extension"        ] = new PHY_MissionCapacity( "Transmissions, node, small extension"        , eTransmissions_NodeSmallExtension       , EnumUnitCapaciteMission::transmissions_node_small_extension        );
    missionCapacity_[ "Transmissions, radio relay"                  ] = new PHY_MissionCapacity( "Transmissions, radio relay"                  , eTransmissions_RadioRelay               , EnumUnitCapaciteMission::transmissions_radio_relay                 );
    missionCapacity_[ "Transmissions, radio tactical satellite"     ] = new PHY_MissionCapacity( "Transmissions, radio tactical satellite"     , eTransmissions_RadioTacticalSatellite   , EnumUnitCapaciteMission::transmissions_radio_tactical_satellite    );
    missionCapacity_[ "Transmissions, radio teletype"               ] = new PHY_MissionCapacity( "Transmissions, radio teletype"               , eTransmissions_RadioTeletype            , EnumUnitCapaciteMission::transmissions_radio_teletype              );
    missionCapacity_[ "Transmissions, support"                      ] = new PHY_MissionCapacity( "Transmissions, support"                      , eTransmissions_Support                  , EnumUnitCapaciteMission::transmissions_support                     );
    missionCapacity_[ "Transmissions, radio"                        ] = new PHY_MissionCapacity( "Transmissions, radio"                        , eTransmissions_Radio                    , EnumUnitCapaciteMission::transmissions_radio                       );
    missionCapacity_[ "Transport, allocation des mouvements"        ] = new PHY_MissionCapacity( "Transport, allocation des mouvements"        , eTransport_AllocationDesMouvements      , EnumUnitCapaciteMission::transport_allocation_des_mouvements       );
    missionCapacity_[ "Transport, APOD/APOE"                        ] = new PHY_MissionCapacity( "Transport, APOD/APOE"                        , eTransport_APOD_APOE                    , EnumUnitCapaciteMission::transport_apod_apoe                       );
    missionCapacity_[ "Transport, SPOD/SPOE"                        ] = new PHY_MissionCapacity( "Transport, SPOD/SPOE"                        , eTransport_SPOD_SPOE                    , EnumUnitCapaciteMission::transport_spod_spoe                       );
    missionCapacity_[ "Transport, missile"                          ] = new PHY_MissionCapacity( "Transport, missile"                          , eTransportMissile                       , EnumUnitCapaciteMission::transport_missile                         );
    missionCapacity_[ "Verrouillage de cible"                       ] = new PHY_MissionCapacity( "Verrouillage de cible"                       , eVerrouillageDeCible                    , EnumUnitCapaciteMission::verrouillage_de_cible                     );
    missionCapacity_[ "Verrouillage de cible, flash"                ] = new PHY_MissionCapacity( "Verrouillage de cible, flash"                , eVerrouillageDeCible_Flash              , EnumUnitCapaciteMission::verrouillage_de_cible_flash               );
    missionCapacity_[ "Verrouillage de cible, radar"                ] = new PHY_MissionCapacity( "Verrouillage de cible, radar"                , eVerrouillageDeCible_Radar              , EnumUnitCapaciteMission::verrouillage_de_cible_radar               );
    missionCapacity_[ "Verrouillage de cible, sonore"               ] = new PHY_MissionCapacity( "Verrouillage de cible, sonore"               , eVerrouillageDeCible_Sonore             , EnumUnitCapaciteMission::verrouillage_de_cible_sonore              );
}

// -----------------------------------------------------------------------------
// Name: PHY_MissionCapacity::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_MissionCapacity::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MissionCapacity constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_MissionCapacity::PHY_MissionCapacity( const std::string& strName, E_Type nType, ASN1T_EnumUnitCapaciteMission nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MissionCapacity destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_MissionCapacity::~PHY_MissionCapacity()
{
}