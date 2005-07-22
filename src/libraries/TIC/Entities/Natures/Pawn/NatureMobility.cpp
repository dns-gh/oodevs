// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureMobility.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:03 $
// $Revision: 2 $
// $Workfile: NatureMobility.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"

#include "NatureMobility.h"

using namespace TIC;

NatureMobility::T_NatureMobilityMap NatureMobility::natureMobility_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureMobility::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureMobility::Initialize()
{
    natureMobility_[ "Aeroporte"                 ] = new NatureMobility( "Aeroporte"                 , eAeroporte            , EnumUnitNatureMobility::aeroporte                , "LNDTRC" );
    natureMobility_[ "Aerien compose"            ] = new NatureMobility( "Aerien compose"            , eAerienCompose        , EnumUnitNatureMobility::aerien_compose           , "NKN" );
    natureMobility_[ "Aerien voilure fixe"       ] = new NatureMobility( "Aerien voilure fixe"       , eAerienVoilureFixe    , EnumUnitNatureMobility::aerien_voilure_fixe      , "NKN" );
    natureMobility_[ "Aerien plus leger que air" ] = new NatureMobility( "Aerien plus leger que air" , eAerienPlusLegerQueAir, EnumUnitNatureMobility::aerien_plus_leger_que_air, "NKN" );
    natureMobility_[ "Aerien ailes rotatives"    ] = new NatureMobility( "Aerien ailes rotatives"    , eAerienAilesRotatives , EnumUnitNatureMobility::aerien_ailes_rotatives   , "NKN" );
    natureMobility_[ "Aerien amphibie"           ] = new NatureMobility( "Aerien amphibie"           , eAerienAmphibie       , EnumUnitNatureMobility::aerien_amphibie          , "LNDTRC" );
    natureMobility_[ "Animal monte"              ] = new NatureMobility( "Animal monte"              , eAnimalMonte          , EnumUnitNatureMobility::animal_monte             , "ANIMNT" );
    natureMobility_[ "Demonte"                   ] = new NatureMobility( "Demonte"                   , eDemonte              , EnumUnitNatureMobility::demonte                  , "DSMNTD" );
    natureMobility_[ "Terrestre chemin de fer"   ] = new NatureMobility( "Terrestre chemin de fer"   , eTerrestreCheminDeFer , EnumUnitNatureMobility::terrestre_chemin_de_fer  , "NKN" );
    natureMobility_[ "Terrestre auto propulse"   ] = new NatureMobility( "Terrestre auto propulse"   , eTerrestreAutoPropulse, EnumUnitNatureMobility::terrestre_auto_propulse  , "LNDSPP" );
    natureMobility_[ "Terrestre chenille"        ] = new NatureMobility( "Terrestre chenille"        , eTerrestreChenille    , EnumUnitNatureMobility::terrestre_chenille       , "NKN" );
    natureMobility_[ "Terrestre remorque"        ] = new NatureMobility( "Terrestre remorque"        , eTerrestreRemorque    , EnumUnitNatureMobility::terrestre_remorque       , "NKN" );
    natureMobility_[ "Terrestre roues"           ] = new NatureMobility( "Terrestre roues"           , eTerrestreRoues       , EnumUnitNatureMobility::terrestre_roues          , "LNDWHL" );
    natureMobility_[ "Maritime sous marin"       ] = new NatureMobility( "Maritime sous marin"       , eMaritimeSousMarin    , EnumUnitNatureMobility::maritime_sous_marin      , "NKN" );
    natureMobility_[ "Maritime surface"          ] = new NatureMobility( "Maritime surface"          , eMaritimeSurface      , EnumUnitNatureMobility::maritime_surface         , "NKN" );
    natureMobility_[ "Inconnu"                   ] = new NatureMobility( "Inconnu"                   , eInconnu              , EnumUnitNatureMobility::inconnu                  , "NKN" );
    natureMobility_[ "None"                      ] = new NatureMobility( "None"                      , eNone                 , EnumUnitNatureMobility::none                     , "NONE" );
}

// -----------------------------------------------------------------------------
// Name: NatureMobility::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureMobility::Terminate()
{
    for( CIT_NatureMobilityMap it = natureMobility_.begin(); it != natureMobility_.end(); ++it )
        delete it->second;
    natureMobility_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureMobility constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureMobility::NatureMobility( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureMobility nAsnID, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: NatureMobility destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureMobility::~NatureMobility()
{
}