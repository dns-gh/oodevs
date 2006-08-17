// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureWeapon.cpp $
// $Author: Nld $
// $Modtime: 6/04/05 10:35 $
// $Revision: 6 $
// $Workfile: PHY_NatureWeapon.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_NatureWeapon.h"

PHY_NatureWeapon::T_NatureWeaponMap PHY_NatureWeapon::natureWeapons_;
const PHY_NatureWeapon              PHY_NatureWeapon::notDefined_   ( "None", eNone, EnumUnitNatureWeapon::none );


// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureWeapon::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_NatureWeapon::Initialize()
{
    natureWeapons_[ "Antichar"                             ] = new PHY_NatureWeapon( "Antichar"                            , eAntichar                        , EnumUnitNatureWeapon::antichar                            );                            
    natureWeapons_[ "Administration"                       ] = new PHY_NatureWeapon( "Administration"                      , eAdministration                  , EnumUnitNatureWeapon::administration                      );                      
    natureWeapons_[ "Defense aerienne"                     ] = new PHY_NatureWeapon( "Defense aerienne"                    , eDefenseAerienne                 , EnumUnitNatureWeapon::defense_aerienne                    );                     
    natureWeapons_[ "Centre soutien operations aeriennes"  ] = new PHY_NatureWeapon( "Centre soutien operations aeriennes" , eCentreSoutienOperationsAeriennes, EnumUnitNatureWeapon::centre_soutien_operations_aeriennes );    
    natureWeapons_[ "Blinde"                               ] = new PHY_NatureWeapon( "Blinde"                              , eBlinde                          , EnumUnitNatureWeapon::blinde                              );                              
    natureWeapons_[ "Aviation"                             ] = new PHY_NatureWeapon( "Aviation"                            , eAviation                        , EnumUnitNatureWeapon::aviation                            );                            
    natureWeapons_[ "Cavalerie"                            ] = new PHY_NatureWeapon( "Cavalerie"                           , eCavalerie                       , EnumUnitNatureWeapon::cavalerie                           );                           
    natureWeapons_[ "Genie"                                ] = new PHY_NatureWeapon( "Genie"                               , eGenie                           , EnumUnitNatureWeapon::genie                               );                               
    natureWeapons_[ "Deminage"                             ] = new PHY_NatureWeapon( "Deminage"                            , eDeminage                        , EnumUnitNatureWeapon::deminage                            );                            
    natureWeapons_[ "Soutien artillerie"                   ] = new PHY_NatureWeapon( "Soutien artillerie"                  , eSoutienArtillerie               , EnumUnitNatureWeapon::soutien_artillerie                  );                   
    natureWeapons_[ "Artillerie"                           ] = new PHY_NatureWeapon( "Artillerie"                          , eArtillerie                      , EnumUnitNatureWeapon::artillerie                          );                          
    natureWeapons_[ "Infanterie"                           ] = new PHY_NatureWeapon( "Infanterie"                          , eInfanterie                      , EnumUnitNatureWeapon::infanterie                          );                          
    natureWeapons_[ "Guerre information"                   ] = new PHY_NatureWeapon( "Guerre information"                  , eGuerreInformation               , EnumUnitNatureWeapon::guerre_information                  );                   
    natureWeapons_[ "Forces securite interne"              ] = new PHY_NatureWeapon( "Forces securite interne"             , eForcesSecuriteInterne           , EnumUnitNatureWeapon::forces_securite_interne             );               
    natureWeapons_[ "Execution loi"                        ] = new PHY_NatureWeapon( "Execution loi"                       , eExecutionLoi                    , EnumUnitNatureWeapon::execution_loi                       );                        
    natureWeapons_[ "Soutien atterrissage"                 ] = new PHY_NatureWeapon( "Soutien atterrissage"                , eSoutienAtterrissage             , EnumUnitNatureWeapon::soutien_atterrissage                );                 
    natureWeapons_[ "Logistique"                           ] = new PHY_NatureWeapon( "Logistique"                          , eLogistique                      , EnumUnitNatureWeapon::logistique                          );                          
    natureWeapons_[ "Reparation"                           ] = new PHY_NatureWeapon( "Reparation"                          , eReparation                      , EnumUnitNatureWeapon::reparation                          );                          
    natureWeapons_[ "Manoeuvre"                            ] = new PHY_NatureWeapon( "Manoeuvre"                           , eManoeuvre                       , EnumUnitNatureWeapon::manoeuvre                           );                           
    natureWeapons_[ "Medical"                              ] = new PHY_NatureWeapon( "Medical"                             , eMedical                         , EnumUnitNatureWeapon::medical                             );                             
    natureWeapons_[ "Renseignement militaire"              ] = new PHY_NatureWeapon( "Renseignement militaire"             , eRenseignementMilitaire          , EnumUnitNatureWeapon::renseignement_militaire             );              
    natureWeapons_[ "Missile"                              ] = new PHY_NatureWeapon( "Missile"                             , eMissile                         , EnumUnitNatureWeapon::missile                             );                             
    natureWeapons_[ "NBC"                                  ] = new PHY_NatureWeapon( "NBC"                                 , eNbc                             , EnumUnitNatureWeapon::nbc                                 );                                 
    natureWeapons_[ "Inconnue"                             ] = new PHY_NatureWeapon( "Inconnue"                            , eInconnue                        , EnumUnitNatureWeapon::inconnue                            );                            
    natureWeapons_[ "Non specifiee"                        ] = new PHY_NatureWeapon( "Non specifiee"                       , eNonSpecifiee                    , EnumUnitNatureWeapon::non_specifiee                       );                           
    natureWeapons_[ "Reconnaissance"                       ] = new PHY_NatureWeapon( "Reconnaissance"                      , eReconnaissance                  , EnumUnitNatureWeapon::reconnaissance                      );                      
    natureWeapons_[ "Transmissions"                        ] = new PHY_NatureWeapon( "Transmissions"                       , eTransmissions                   , EnumUnitNatureWeapon::transmissions                       );                       
    natureWeapons_[ "Ravitaillement"                       ] = new PHY_NatureWeapon( "Ravitaillement"                      , eRavitaillement                  , EnumUnitNatureWeapon::ravitaillement                      );                      
    natureWeapons_[ "Tactical air control party"           ] = new PHY_NatureWeapon( "Tactical air control party"          , eTacticalAirControlParty         , EnumUnitNatureWeapon::tactical_air_control_party          );             
    natureWeapons_[ "Transport"                            ] = new PHY_NatureWeapon( "Transport"                           , eTransport                       , EnumUnitNatureWeapon::transport                           );
    
    natureWeapons_[ notDefined_.GetName()                  ] = &notDefined_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureWeapon::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_NatureWeapon::Terminate()
{
    natureWeapons_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureWeapon constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_NatureWeapon::PHY_NatureWeapon( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureWeapon nAsnID )
    : strName_ ( strName )
    , nType_   ( nType )
    , nAsnID_  ( nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureWeapon destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_NatureWeapon::~PHY_NatureWeapon()
{
}