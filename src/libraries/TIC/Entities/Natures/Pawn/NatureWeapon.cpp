// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureWeapon.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:05 $
// $Revision: 3 $
// $Workfile: NatureWeapon.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "NatureWeapon.h"

using namespace TIC;

NatureWeapon::T_NatureWeaponMap NatureWeapon::natureWeapons_;


// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureWeapon::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void NatureWeapon::Initialize()
{
    natureWeapons_[ "Antichar"                             ] = new NatureWeapon( "Antichar"                            , eAntichar                        , EnumUnitNatureWeapon::antichar                           , "AARMOR" );                            
    natureWeapons_[ "Administration"                       ] = new NatureWeapon( "Administration"                      , eAdministration                  , EnumUnitNatureWeapon::administration                     , "ADMIN"  );
    natureWeapons_[ "Defense aerienne"                     ] = new NatureWeapon( "Defense aerienne"                    , eDefenseAerienne                 , EnumUnitNatureWeapon::defense_aerienne                   , "AIRDEF" );
    natureWeapons_[ "Centre soutien operations aeriennes"  ] = new NatureWeapon( "Centre soutien operations aeriennes" , eCentreSoutienOperationsAeriennes, EnumUnitNatureWeapon::centre_soutien_operations_aeriennes, "NKN"    );
    natureWeapons_[ "Blinde"                               ] = new NatureWeapon( "Blinde"                              , eBlinde                          , EnumUnitNatureWeapon::blinde                             , "ARMOR"  );
    natureWeapons_[ "Aviation"                             ] = new NatureWeapon( "Aviation"                            , eAviation                        , EnumUnitNatureWeapon::aviation                           , "AV"     );
    natureWeapons_[ "Cavalerie"                            ] = new NatureWeapon( "Cavalerie"                           , eCavalerie                       , EnumUnitNatureWeapon::cavalerie                          , "CAV"    );
    natureWeapons_[ "Genie"                                ] = new NatureWeapon( "Genie"                               , eGenie                           , EnumUnitNatureWeapon::genie                              , "ENG"    );
    natureWeapons_[ "Deminage"                             ] = new NatureWeapon( "Deminage"                            , eDeminage                        , EnumUnitNatureWeapon::deminage                           , "EOD"    );
    natureWeapons_[ "Soutien artillerie"                   ] = new NatureWeapon( "Soutien artillerie"                  , eSoutienArtillerie               , EnumUnitNatureWeapon::soutien_artillerie                 , "NKN"    );
    natureWeapons_[ "Artillerie"                           ] = new NatureWeapon( "Artillerie"                          , eArtillerie                      , EnumUnitNatureWeapon::artillerie                         , "FLDART" );
    natureWeapons_[ "Infanterie"                           ] = new NatureWeapon( "Infanterie"                          , eInfanterie                      , EnumUnitNatureWeapon::infanterie                         , "INF"    );
    natureWeapons_[ "Guerre information"                   ] = new NatureWeapon( "Guerre information"                  , eGuerreInformation               , EnumUnitNatureWeapon::guerre_information                 , "INFWAR" );
    natureWeapons_[ "Forces securite interne"              ] = new NatureWeapon( "Forces securite interne"             , eForcesSecuriteInterne           , EnumUnitNatureWeapon::forces_securite_interne            , "ISFRCE" );
    natureWeapons_[ "Execution loi"                        ] = new NatureWeapon( "Execution loi"                       , eExecutionLoi                    , EnumUnitNatureWeapon::execution_loi                      , "LAWENF" );
    natureWeapons_[ "Soutien atterrissage"                 ] = new NatureWeapon( "Soutien atterrissage"                , eSoutienAtterrissage             , EnumUnitNatureWeapon::soutien_atterrissage               , "LNDSPT" );
    natureWeapons_[ "Logistique"                           ] = new NatureWeapon( "Logistique"                          , eLogistique                      , EnumUnitNatureWeapon::logistique                         , "LOG"    );
    natureWeapons_[ "Reparation"                           ] = new NatureWeapon( "Reparation"                          , eReparation                      , EnumUnitNatureWeapon::reparation                         , "MAINT"  );
    natureWeapons_[ "Manoeuvre"                            ] = new NatureWeapon( "Manoeuvre"                           , eManoeuvre                       , EnumUnitNatureWeapon::manoeuvre                          , "MANOV"  );
    natureWeapons_[ "Medical"                              ] = new NatureWeapon( "Medical"                             , eMedical                         , EnumUnitNatureWeapon::medical                            , "MEDCL"  );
    natureWeapons_[ "Renseignement militaire"              ] = new NatureWeapon( "Renseignement militaire"             , eRenseignementMilitaire          , EnumUnitNatureWeapon::renseignement_militaire            , "MILINT" );
    natureWeapons_[ "Missile"                              ] = new NatureWeapon( "Missile"                             , eMissile                         , EnumUnitNatureWeapon::missile                            , "MSL"    );
    natureWeapons_[ "NBC"                                  ] = new NatureWeapon( "NBC"                                 , eNbc                             , EnumUnitNatureWeapon::nbc                                , "NBC"    );
    natureWeapons_[ "Inconnue"                             ] = new NatureWeapon( "Inconnue"                            , eInconnue                        , EnumUnitNatureWeapon::inconnue                           , "NKN"    );
    natureWeapons_[ "Non specifiee"                        ] = new NatureWeapon( "Non specifiee"                       , eNonSpecifiee                    , EnumUnitNatureWeapon::non_specifiee                      , "NOS"    );
    natureWeapons_[ "Reconnaissance"                       ] = new NatureWeapon( "Reconnaissance"                      , eReconnaissance                  , EnumUnitNatureWeapon::reconnaissance                     , "RECCE"  );
    natureWeapons_[ "Transmissions"                        ] = new NatureWeapon( "Transmissions"                       , eTransmissions                   , EnumUnitNatureWeapon::transmissions                      , "SIG"    );
    natureWeapons_[ "Ravitaillement"                       ] = new NatureWeapon( "Ravitaillement"                      , eRavitaillement                  , EnumUnitNatureWeapon::ravitaillement                     , "SUPPLY" );
    natureWeapons_[ "Tactical air control party"           ] = new NatureWeapon( "Tactical air control party"          , eTacticalAirControlParty         , EnumUnitNatureWeapon::tactical_air_control_party         , "NKN"    );
    natureWeapons_[ "Transport"                            ] = new NatureWeapon( "Transport"                           , eTransport                       , EnumUnitNatureWeapon::transport                          , "TRANS"  );
    natureWeapons_[ "None"                                 ] = new NatureWeapon( "None"                                , eNone                            , EnumUnitNatureWeapon::none                               , "NKN"    );
}

// -----------------------------------------------------------------------------
// Name: NatureWeapon::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void NatureWeapon::Terminate()
{
    for( CIT_NatureWeaponMap it = natureWeapons_.begin(); it != natureWeapons_.end(); ++it )
        delete it->second;
    natureWeapons_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureWeapon constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
NatureWeapon::NatureWeapon( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureWeapon nAsnID, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: NatureWeapon destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
NatureWeapon::~NatureWeapon()
{
}