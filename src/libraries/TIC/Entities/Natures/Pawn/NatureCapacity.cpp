// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureCapacity.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:02 $
// $Revision: 2 $
// $Workfile: NatureCapacity.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "NatureCapacity.h"

using namespace TIC;

NatureCapacity::T_NatureCapacityMap NatureCapacity::natureCapacity_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureCapacity::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureCapacity::Initialize()
{
    natureCapacity_[ "Civilian law enforcement"                    ] = new NatureCapacity( "Civilian law enforcement"                    , eCivilianLawEnforcement                 , EnumUnitCapaciteMission::civilian_law_enforcement                  , "CIVLWE" );
    natureCapacity_[ "Command operations"                          ] = new NatureCapacity( "Command operations"                          , eCommandOperations                      , EnumUnitCapaciteMission::command_operations                        , "NKN" );
    natureCapacity_[ "Couverture aerienne"                         ] = new NatureCapacity( "Couverture aerienne"                         , eCouvertureAerienne                     , EnumUnitCapaciteMission::couverture_aerienne                       , "NKN" );
    natureCapacity_[ "Joint intelligence"                          ] = new NatureCapacity( "Joint intelligence"                          , eJointIntelligence                      , EnumUnitCapaciteMission::joint_intelligence                        , "JNTINT" );
    natureCapacity_[ "Morale, welfare recreation"                  ] = new NatureCapacity( "Morale, welfare recreation"                  , eMoraleWelfareRecreation                , EnumUnitCapaciteMission::morale_welfare_recreation                 , "MWR" );
    natureCapacity_[ "Reglage electronique"                        ] = new NatureCapacity( "Reglage electronique"                        , eReglageElectronique                    , EnumUnitCapaciteMission::reglage_electronique                      , "ELCRNG" );
    natureCapacity_[ "Remplacement"                                ] = new NatureCapacity( "Remplacement"                                , eRemplacement                           , EnumUnitCapaciteMission::remplacement                              , "REPLHO" );
    natureCapacity_[ "Service mortuaire"                           ] = new NatureCapacity( "Service mortuaire"                           , eServiceMortuaire                       , EnumUnitCapaciteMission::service_mortuaire                         , "MRTGRR" );
    natureCapacity_[ "Signals intelligence (SIGINT)"               ] = new NatureCapacity( "Signals intelligence (SIGINT)"               , eSignalsIntelligence                    , EnumUnitCapaciteMission::signals_intelligence                      , "SIGINT" );
    natureCapacity_[ "Theatre missile defence"                     ] = new NatureCapacity( "Theatre missile defence"                     , eTheatreMissileDefence                  , EnumUnitCapaciteMission::theatre_missile_defence                   , "THTMSD" );
    natureCapacity_[ "Utilitaire"                                  ] = new NatureCapacity( "Utilitaire"                                  , eUtilitaire                             , EnumUnitCapaciteMission::utilitaire                                , "UTILTY" );
    natureCapacity_[ "Aeroporte"                                   ] = new NatureCapacity( "Aeroporte"                                   , eAéroporte                              , EnumUnitCapaciteMission::aeroporte                                 , "AIRBRN" );
    natureCapacity_[ "Affaires publiques"                          ] = new NatureCapacity( "Affaires publiques"                          , eAffairesPubliques                      , EnumUnitCapaciteMission::affaires_publiques                        , "PUBAF" );
    natureCapacity_[ "Affaires publiques, communication"           ] = new NatureCapacity( "Affaires publiques, communication"           , eAffairesPubliques_Communication        , EnumUnitCapaciteMission::affaires_publiques_communication          , "PUBAFB" );
    natureCapacity_[ "Affaires publiques, joint information"       ] = new NatureCapacity( "Affaires publiques, joint information"       , eAffairesPubliques_JointInformation     , EnumUnitCapaciteMission::affaires_publiques_joint_information      , "PUBAFJ" );
    natureCapacity_[ "Alpin"                                       ] = new NatureCapacity( "Alpin"                                       , eAlpin                                  , EnumUnitCapaciteMission::alpin                                     , "MOUNTN" );
    natureCapacity_[ "Amphibie"                                    ] = new NatureCapacity( "Amphibie"                                    , eAmphibie                               , EnumUnitCapaciteMission::amphibie                                  , "AMPH" );
    natureCapacity_[ "Arctique"                                    ] = new NatureCapacity( "Arctique"                                    , eArctique                               , EnumUnitCapaciteMission::arctique                                  , "ARC" );
    natureCapacity_[ "Artillerie - reconnaissance"                 ] = new NatureCapacity( "Artillerie - reconnaissance"                 , eArtillerieReconnaissance               , EnumUnitCapaciteMission::artillerie_reconnaissance                 , "ARTYSV" );
    natureCapacity_[ "Attaque"                                     ] = new NatureCapacity( "Attaque"                                     , eAttaque                                , EnumUnitCapaciteMission::attaque                                   , "ATTACK" );
    natureCapacity_[ "Attaque aerienne"                            ] = new NatureCapacity( "Attaque aerienne"                            , eAttaqueAerienne                        , EnumUnitCapaciteMission::attaque_aerienne                          , "ARASLT" );
    natureCapacity_[ "C2"                                          ] = new NatureCapacity( "C2"                                          , eC2                                     , EnumUnitCapaciteMission::c2                                        , "C2" );
    natureCapacity_[ "Chemin de fer"                               ] = new NatureCapacity( "Chemin de fer"                               , eCheminDeFer                            , EnumUnitCapaciteMission::chemin_de_fer                             , "RAIL" );
    natureCapacity_[ "Ciblage"                                     ] = new NatureCapacity( "Ciblage"                                     , eCiblage                                , EnumUnitCapaciteMission::ciblage                                   , "TGTNG" );
    natureCapacity_[ "Contre espionnage"                           ] = new NatureCapacity( "Contre espionnage"                           , eContreEspionnage                       , EnumUnitCapaciteMission::contre_espionnage                         , "CNTRIN" );
    natureCapacity_[ "Eclairage"                                   ] = new NatureCapacity( "Eclairage"                                   , eEclairage                              , EnumUnitCapaciteMission::eclairage                                 , "SCOUT" );
    natureCapacity_[ "Entretien"                                   ] = new NatureCapacity( "Entretien"                                   , eEntretien                              , EnumUnitCapaciteMission::entretien                                 , "MNT" );
    natureCapacity_[ "Entretien, materiel"                         ] = new NatureCapacity( "Entretien, materiel"                         , eEntretienMateriel                      , EnumUnitCapaciteMission::entretien_materiel                        , "MNTOD" );
    natureCapacity_[ "Entretien, missile"                          ] = new NatureCapacity( "Entretien, missile"                          , eEntretienMissile                       , EnumUnitCapaciteMission::entretien_missile                         , "MNTODM" );
    natureCapacity_[ "Entretien, opto-electronique"                ] = new NatureCapacity( "Entretien, opto-electronique"                , eEntretienOptoElectronique              , EnumUnitCapaciteMission::entretien_opto_electronique               , "MNTELO" );
    natureCapacity_[ "Espionnage"                                  ] = new NatureCapacity( "Espionnage"                                  , eEspionnage                             , EnumUnitCapaciteMission::espionnage                                , "CTRINT" );
    natureCapacity_[ "Espionnage militaire, aerial exploitation"   ] = new NatureCapacity( "Espionnage militaire, aerial exploitation"   , eEspionnageMilitaireAerialExploitation  , EnumUnitCapaciteMission::espionnage_militaire_aerial_exploitation  , "MIAREX" );
    natureCapacity_[ "Espionnage militaire, operation"             ] = new NatureCapacity( "Espionnage militaire, operation"             , eEspionnageMilitaireOperation           , EnumUnitCapaciteMission::espionnage_militaire_operation            , "MIOPS" );
    natureCapacity_[ "Espionnage militaire, tactical exploitation" ] = new NatureCapacity( "Espionnage militaire, tactical exploitation" , eEspionnageMilitaireTacticalExploitation, EnumUnitCapaciteMission::espionnage_militaire_tactical_exploitation, "MITCEX" );
    natureCapacity_[ "Evacuation sanitaire"                        ] = new NatureCapacity( "Evacuation sanitaire"                        , eEvacuationSanitaire                    , EnumUnitCapaciteMission::evacuation_sanitaire                      , "MEDEVC" );
    natureCapacity_[ "Finance"                                     ] = new NatureCapacity( "Finance"                                     , eFinance                                , EnumUnitCapaciteMission::finance                                   , "FINANC" );
    natureCapacity_[ "Genie, combat"                               ] = new NatureCapacity( "Genie, combat"                               , eGenieCombat                            , EnumUnitCapaciteMission::genie_combat                              , "ENGCBT" );
    natureCapacity_[ "Genie, construction"                         ] = new NatureCapacity( "Genie, construction"                         , eGenieConstruction                      , EnumUnitCapaciteMission::genie_construction                        , "ENGCN" );
    natureCapacity_[ "Genie, construction navale"                  ] = new NatureCapacity( "Genie, construction navale"                  , eGenieConstructionNavale                , EnumUnitCapaciteMission::genie_construction_navale                 , "ENGCNN" );
    natureCapacity_[ "Guerre electronique"                         ] = new NatureCapacity( "Guerre electronique"                         , eGuerreElectronique                     , EnumUnitCapaciteMission::guerre_electronique                       , "EW" );
    natureCapacity_[ "Guerre electronique, interception"           ] = new NatureCapacity( "Guerre electronique, interception"           , eGuerreElectroniqueInterception         , EnumUnitCapaciteMission::guerre_electronique_interception          , "EWINTC" );
    natureCapacity_[ "Guerre electronique, brouillage"             ] = new NatureCapacity( "Guerre electronique, brouillage"             , eGuerreElectroniqueBrouillage           , EnumUnitCapaciteMission::guerre_electronique_brouillage            , "EWJAM" );
    natureCapacity_[ "Guerre electronique, detection"              ] = new NatureCapacity( "Guerre electronique, detection"              , eGuerreElectroniqueDétection            , EnumUnitCapaciteMission::guerre_electronique_detection             , "EWDF" );
    natureCapacity_[ "Interrogation"                               ] = new NatureCapacity( "Interrogation"                               , eInterrogation                          , EnumUnitCapaciteMission::interrogation                             , "INTERO" );
    natureCapacity_[ "Main d'oeuvre"                               ] = new NatureCapacity( "Main d'oeuvre"                               , eMainDOeuvre                            , EnumUnitCapaciteMission::main_d_oeuvre                             , "LABOUR" );
    natureCapacity_[ "Maintien de la paix"                         ] = new NatureCapacity( "Maintien de la paix"                         , eMaintienDeLaPaix                       , EnumUnitCapaciteMission::maintien_de_la_paix                       , "NKN" );
    natureCapacity_[ "Marine"                                      ] = new NatureCapacity( "Marine"                                      , eMarine                                 , EnumUnitCapaciteMission::marine                                    , "MARINE" );
    natureCapacity_[ "Mine de contre-mesure"                       ] = new NatureCapacity( "Mine de contre-mesure"                       , eMineDeContreMesure                     , EnumUnitCapaciteMission::mine_de_contre_mesure                     , "MCM" );
    natureCapacity_[ "NBC, biologique"                             ] = new NatureCapacity( "NBC, biologique"                             , eNBCBiologique                          , EnumUnitCapaciteMission::nbc_biologique                            , "NBCBIO" );
    natureCapacity_[ "NBC, chimique"                               ] = new NatureCapacity( "NBC, chimique"                               , eNBCChimique                            , EnumUnitCapaciteMission::nbc_chimique                              , "NBCCHM" );
    natureCapacity_[ "NBC, chimique, fumee"                        ] = new NatureCapacity( "NBC, chimique, fumee"                        , eNBCChimiqueFumee                       , EnumUnitCapaciteMission::nbc_chimique_fumee                        , "NBCCHS" );
    natureCapacity_[ "NBC, decontamination"                        ] = new NatureCapacity( "NBC, decontamination"                        , eNBCDecontamination                     , EnumUnitCapaciteMission::nbc_decontamination                       , "NBCDEC" );
    natureCapacity_[ "NBC, decontamination chimique"               ] = new NatureCapacity( "NBC, decontamination chimique"               , eNBCDecontaminationChimique             , EnumUnitCapaciteMission::nbc_decontamination_chimique              , "NBCDEC" );
    natureCapacity_[ "NBC, nucleaire"                              ] = new NatureCapacity( "NBC, nucleaire"                              , eNBCNucleaire                           , EnumUnitCapaciteMission::nbc_nucleaire                             , "NBCNUC" );
    natureCapacity_[ "Non renseigne"                               ] = new NatureCapacity( "Non renseigne"                               , eNonRenseigne                           , EnumUnitCapaciteMission::non_renseigne                             , "NOS" );
    natureCapacity_[ "Observation"                                 ] = new NatureCapacity( "Observation"                                 , eObservation                            , EnumUnitCapaciteMission::observation                               , "SRV" );
    natureCapacity_[ "Observation, capteur"                        ] = new NatureCapacity( "Observation, capteur"                        , eObservationCapteur                     , EnumUnitCapaciteMission::observation_capteur                       , "SRVSEN" );
    natureCapacity_[ "Observation, longue portee"                  ] = new NatureCapacity( "Observation, longue portee"                  , eObservationLonguePortee                , EnumUnitCapaciteMission::observation_longue_portee                 , "SRVLR" );
    natureCapacity_[ "Observation, meteo"                          ] = new NatureCapacity( "Observation, meteo"                          , eObservationMeteo                       , EnumUnitCapaciteMission::observation_meteo                         , "SRVMET" );
    natureCapacity_[ "Observation, module terrestre"               ] = new NatureCapacity( "Observation, module terrestre"               , eObservationModuleTerrestre             , EnumUnitCapaciteMission::observation_module_terrestre              , "SRVGM" );
    natureCapacity_[ "Purification de l'eau"                       ] = new NatureCapacity( "Purification de l'eau"                       , ePurificationDeLEau                     , EnumUnitCapaciteMission::purification_de_l_eau                     , "NKN" );
    natureCapacity_[ "Ravitaillement (class I)"                    ] = new NatureCapacity( "Ravitaillement (class I)"                    , eRavitaillement_ClasseI                 , EnumUnitCapaciteMission::ravitaillement_class_i                    , "SPLC1" );
    natureCapacity_[ "Ravitaillement (class II)"                   ] = new NatureCapacity( "Ravitaillement (class II)"                   , eRavitaillement_ClasseII                , EnumUnitCapaciteMission::ravitaillement_class_ii                   , "SPLC2" );
    natureCapacity_[ "Ravitaillement (class III aviation)"         ] = new NatureCapacity( "Ravitaillement (class III aviation)"         , eRavitaillement_ClasseIIIAviation       , EnumUnitCapaciteMission::ravitaillement_class_iii_aviation         , "SPLC3A" );
    natureCapacity_[ "Ravitaillement (class III)"                  ] = new NatureCapacity( "Ravitaillement (class III)"                  , eRavitaillement_ClasseIII               , EnumUnitCapaciteMission::ravitaillement_class_iii                  , "SPLC3" );
    natureCapacity_[ "Ravitaillement (class IV)"                   ] = new NatureCapacity( "Ravitaillement (class IV)"                   , eRavitaillement_ClasseIV                , EnumUnitCapaciteMission::ravitaillement_class_iv                   , "SPLC4" );
    natureCapacity_[ "Ravitaillement (class V)"                    ] = new NatureCapacity( "Ravitaillement (class V)"                    , eRavitaillement_ClasseV                 , EnumUnitCapaciteMission::ravitaillement_class_v                    , "SPLC5" );
    natureCapacity_[ "Ravitaillement (eau)"                        ] = new NatureCapacity( "Ravitaillement (eau)"                        , eRavitaillement_Eau                     , EnumUnitCapaciteMission::ravitaillement_eau                        , "SPLWAT" );
    natureCapacity_[ "Ravitaillement, blanchisserie/bain"          ] = new NatureCapacity( "Ravitaillement, blanchisserie/bain"          , eRavitaillementBlanchisserieBain        , EnumUnitCapaciteMission::ravitaillement_blanchisserie_bain         , "SPLLDB" );
    natureCapacity_[ "Recherche & sauvetage"                       ] = new NatureCapacity( "Recherche & sauvetage"                       , eRechercheEtSauvetage                   , EnumUnitCapaciteMission::recherche_et_sauvetage                    , "NKN" );
    natureCapacity_[ "Reconnaissance"                              ] = new NatureCapacity( "Reconnaissance"                              , eReconnaissance                         , EnumUnitCapaciteMission::reconnaissance                            , "RECCE" );
    natureCapacity_[ "Reconnaissance aerienne tactique"            ] = new NatureCapacity( "Reconnaissance aerienne tactique"            , eReconnaissanceAerienneTactique         , EnumUnitCapaciteMission::reconnaissance_aerienne_tactique          , "RECCE" );
    natureCapacity_[ "Recuperation"                                ] = new NatureCapacity( "Recuperation"                                , eRecuperation                           , EnumUnitCapaciteMission::recuperation                              , "RECVRY" );
    natureCapacity_[ "Service du personnel"                        ] = new NatureCapacity( "Service du personnel"                        , eServiceDuPersonnel                     , EnumUnitCapaciteMission::service_du_personnel                      , "PSC" );
    natureCapacity_[ "Service juridique"                           ] = new NatureCapacity( "Service juridique"                           , eServiceJuridique                       , EnumUnitCapaciteMission::service_juridique                         , "LEGAL" );
    natureCapacity_[ "Support aerien rapproche"                    ] = new NatureCapacity( "Support aerien rapproche"                    , eSupportAerienRapproche                 , EnumUnitCapaciteMission::support_aerien_rapproche                  , "NKN" );
    natureCapacity_[ "Transmissions, forward communications"       ] = new NatureCapacity( "Transmissions, forward communications"       , eTransmissions_ForwardCommunications    , EnumUnitCapaciteMission::transmissions_forward_communications      , "SGFC" );
    natureCapacity_[ "Transmissions, node centre"                  ] = new NatureCapacity( "Transmissions, node centre"                  , eTransmissions_NodeCentre               , EnumUnitCapaciteMission::transmissions_node_centre                 , "SGNC" );
    natureCapacity_[ "Transmissions, node, large extension"        ] = new NatureCapacity( "Transmissions, node, large extension"        , eTransmissions_NodeLargeExtension       , EnumUnitCapaciteMission::transmissions_node_large_extension        , "SGNLE" );
    natureCapacity_[ "Transmissions, node, small extension"        ] = new NatureCapacity( "Transmissions, node, small extension"        , eTransmissions_NodeSmallExtension       , EnumUnitCapaciteMission::transmissions_node_small_extension        , "SGNSE" );
    natureCapacity_[ "Transmissions, radio relay"                  ] = new NatureCapacity( "Transmissions, radio relay"                  , eTransmissions_RadioRelay               , EnumUnitCapaciteMission::transmissions_radio_relay                 , "SGRDRL" );
    natureCapacity_[ "Transmissions, radio tactical satellite"     ] = new NatureCapacity( "Transmissions, radio tactical satellite"     , eTransmissions_RadioTacticalSatellite   , EnumUnitCapaciteMission::transmissions_radio_tactical_satellite    , "SGRDTA" );
    natureCapacity_[ "Transmissions, radio teletype"               ] = new NatureCapacity( "Transmissions, radio teletype"               , eTransmissions_RadioTeletype            , EnumUnitCapaciteMission::transmissions_radio_teletype              , "SGRDTE" );
    natureCapacity_[ "Transmissions, support"                      ] = new NatureCapacity( "Transmissions, support"                      , eTransmissions_Support                  , EnumUnitCapaciteMission::transmissions_support                     , "SGSPT" );
    natureCapacity_[ "Transmissions, radio"                        ] = new NatureCapacity( "Transmissions, radio"                        , eTransmissions_Radio                    , EnumUnitCapaciteMission::transmissions_radio                       , "SIGRAD" );
    natureCapacity_[ "Transport, allocation des mouvements"        ] = new NatureCapacity( "Transport, allocation des mouvements"        , eTransport_AllocationDesMouvements      , EnumUnitCapaciteMission::transport_allocation_des_mouvements       , "TRNMVC" );
    natureCapacity_[ "Transport, APOD/APOE"                        ] = new NatureCapacity( "Transport, APOD/APOE"                        , eTransport_APOD_APOE                    , EnumUnitCapaciteMission::transport_apod_apoe                       , "TRNAPD" );
    natureCapacity_[ "Transport, SPOD/SPOE"                        ] = new NatureCapacity( "Transport, SPOD/SPOE"                        , eTransport_SPOD_SPOE                    , EnumUnitCapaciteMission::transport_spod_spoe                       , "TRNSPD" );
    natureCapacity_[ "Transport, missile"                          ] = new NatureCapacity( "Transport, missile"                          , eTransportMissile                       , EnumUnitCapaciteMission::transport_missile                         , "TRNMSL" );
    natureCapacity_[ "Verrouillage de cible"                       ] = new NatureCapacity( "Verrouillage de cible"                       , eVerrouillageDeCible                    , EnumUnitCapaciteMission::verrouillage_de_cible                     , "TGT" );
    natureCapacity_[ "Verrouillage de cible, flash"                ] = new NatureCapacity( "Verrouillage de cible, flash"                , eVerrouillageDeCible_Flash              , EnumUnitCapaciteMission::verrouillage_de_cible_flash               , "TGTAFL" );
    natureCapacity_[ "Verrouillage de cible, radar"                ] = new NatureCapacity( "Verrouillage de cible, radar"                , eVerrouillageDeCible_Radar              , EnumUnitCapaciteMission::verrouillage_de_cible_radar               , "TGTARD" );
    natureCapacity_[ "Verrouillage de cible, sonore"               ] = new NatureCapacity( "Verrouillage de cible, sonore"               , eVerrouillageDeCible_Sonore             , EnumUnitCapaciteMission::verrouillage_de_cible_sonore              , "TGTASD" );
}

// -----------------------------------------------------------------------------
// Name: NatureCapacity::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureCapacity::Terminate()
{
    for( CIT_NatureCapacityMap it = natureCapacity_.begin(); it != natureCapacity_.end(); ++it )
        delete it->second;
    natureCapacity_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureCapacity constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureCapacity::NatureCapacity( const std::string& strName, E_Type nType, uint nAsnID, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: NatureCapacity destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureCapacity::~NatureCapacity()
{
}