// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureMobility.cpp $
// $Author: Nld $
// $Modtime: 6/04/05 10:29 $
// $Revision: 3 $
// $Workfile: PHY_NatureMobility.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_NatureMobility.h"

PHY_NatureMobility::T_NatureMobilityMap PHY_NatureMobility::natureMobility_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureMobility::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureMobility::Initialize()
{
    natureMobility_[ "Aeroporte"                 ] = new PHY_NatureMobility( "Aeroporte"                 , eAeroporte            , EnumUnitNatureMobility::aeroporte                 );
    natureMobility_[ "Aerien compose"            ] = new PHY_NatureMobility( "Aerien compose"            , eAerienCompose        , EnumUnitNatureMobility::aerien_compose            );
    natureMobility_[ "Aerien voilure fixe"       ] = new PHY_NatureMobility( "Aerien voilure fixe"       , eAerienVoilureFixe    , EnumUnitNatureMobility::aerien_voilure_fixe       );
    natureMobility_[ "Aerien plus leger que air" ] = new PHY_NatureMobility( "Aerien plus leger que air" , eAerienPlusLegerQueAir, EnumUnitNatureMobility::aerien_plus_leger_que_air );
    natureMobility_[ "Aerien ailes rotatives"    ] = new PHY_NatureMobility( "Aerien ailes rotatives"    , eAerienAilesRotatives , EnumUnitNatureMobility::aerien_ailes_rotatives    );
    natureMobility_[ "Aerien amphibie"           ] = new PHY_NatureMobility( "Aerien amphibie"           , eAerienAmphibie       , EnumUnitNatureMobility::aerien_amphibie           );
    natureMobility_[ "Animal monte"              ] = new PHY_NatureMobility( "Animal monte"              , eAnimalMonte          , EnumUnitNatureMobility::animal_monte              );
    natureMobility_[ "Demonte"                   ] = new PHY_NatureMobility( "Demonte"                   , eDemonte              , EnumUnitNatureMobility::demonte                   );
    natureMobility_[ "Terrestre chemin de fer"   ] = new PHY_NatureMobility( "Terrestre chemin de fer"   , eTerrestreCheminDeFer , EnumUnitNatureMobility::terrestre_chemin_de_fer   );
    natureMobility_[ "Terrestre auto propulse"   ] = new PHY_NatureMobility( "Terrestre auto propulse"   , eTerrestreAutoPropulse, EnumUnitNatureMobility::terrestre_auto_propulse   );
    natureMobility_[ "Terrestre chenille"        ] = new PHY_NatureMobility( "Terrestre chenille"        , eTerrestreChenille    , EnumUnitNatureMobility::terrestre_chenille        );
    natureMobility_[ "Terrestre remorque"        ] = new PHY_NatureMobility( "Terrestre remorque"        , eTerrestreRemorque    , EnumUnitNatureMobility::terrestre_remorque        );
    natureMobility_[ "Terrestre roues"           ] = new PHY_NatureMobility( "Terrestre roues"           , eTerrestreRoues       , EnumUnitNatureMobility::terrestre_roues           );
    natureMobility_[ "Maritime sous marin"       ] = new PHY_NatureMobility( "Maritime sous marin"       , eMaritimeSousMarin    , EnumUnitNatureMobility::maritime_sous_marin       );
    natureMobility_[ "Maritime surface"          ] = new PHY_NatureMobility( "Maritime surface"          , eMaritimeSurface      , EnumUnitNatureMobility::maritime_surface          );
    natureMobility_[ "Inconnu"                   ] = new PHY_NatureMobility( "Inconnu"                   , eInconnu              , EnumUnitNatureMobility::inconnu                   );
    natureMobility_[ "None"                      ] = new PHY_NatureMobility( "None"                      , eNone                 , EnumUnitNatureMobility::none                      );
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureMobility::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureMobility::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureMobility constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureMobility::PHY_NatureMobility( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureMobility nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureMobility destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureMobility::~PHY_NatureMobility()
{
}