// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureSpecialization.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:04 $
// $Revision: 2 $
// $Workfile: NatureSpecialization.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"

#include "NatureSpecialization.h"

using namespace TIC;

NatureSpecialization::T_NatureSpecializationMap NatureSpecialization::natureSpecialization_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureSpecialization::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureSpecialization::Initialize()
{
    natureSpecialization_[ "Jag"                                  ] = new NatureSpecialization( "Jag"                                  , eNatureSpec_Jag                               , EnumUnitNatureSpecialization::jag                                 , "JAG"    );
    natureSpecialization_[ "Riverine"                             ] = new NatureSpecialization( "Riverine"                             , eNatureSpec_Riverine                          , EnumUnitNatureSpecialization::riverine                            , "RIVERN" );
    natureSpecialization_[ "Security police"                      ] = new NatureSpecialization( "Security police"                      , eNatureSpec_SecurityPolice                    , EnumUnitNatureSpecialization::security_police                     , "SECPOL" );
    natureSpecialization_[ "Aerien"                               ] = new NatureSpecialization( "Aerien"                               , eNatureSpec_Aerien                            , EnumUnitNatureSpecialization::aerien                              , "AIR"    );
    natureSpecialization_[ "Aeroporte"                            ] = new NatureSpecialization( "Aeroporte"                            , eNatureSpec_Aeroporte                         , EnumUnitNatureSpecialization::aeroporte                           , "AIRBRN" );
    natureSpecialization_[ "Attaque aerienne"                     ] = new NatureSpecialization( "Attaque aerienne"                     , eNatureSpec_AttaqueAerienne                   , EnumUnitNatureSpecialization::attaque_aerienne                    , "AIRASL" );
    natureSpecialization_[ "Blinde"                               ] = new NatureSpecialization( "Blinde"                               , eNatureSpec_Blinde                            , EnumUnitNatureSpecialization::blinde                              , "ARM"    );
    natureSpecialization_[ "Chemin de fer"                        ] = new NatureSpecialization( "Chemin de fer"                        , eNatureSpec_CheminDeFer                       , EnumUnitNatureSpecialization::chemin_de_fer                       , "RAILWY" );
    natureSpecialization_[ "Chirurgie"                            ] = new NatureSpecialization( "Chirurgie"                            , eNatureSpec_Chirurgie                         , EnumUnitNatureSpecialization::chirurgie                           , "NKN"    );
    natureSpecialization_[ "Compose"                              ] = new NatureSpecialization( "Compose"                              , eNatureSpec_Compose                           , EnumUnitNatureSpecialization::compose                             , "COMPST" );
    natureSpecialization_[ "Construction"                         ] = new NatureSpecialization( "Construction"                         , eNatureSpec_Construction                      , EnumUnitNatureSpecialization::construction                        , "CONST"  );
    natureSpecialization_[ "Courrier"                             ] = new NatureSpecialization( "Courrier"                             , eNatureSpec_Courrier                          , EnumUnitNatureSpecialization::courrier                            , "POSTAL" );
    natureSpecialization_[ "Decollage atterissage vertical court" ] = new NatureSpecialization( "Decollage atterissage vertical court" , eNatureSpec_DecollageAtterissageVerticalCourt , EnumUnitNatureSpecialization::decollage_atterissage_vertical_court, "VSTOL"  );
    natureSpecialization_[ "Dentiste"                             ] = new NatureSpecialization( "Dentiste"                             , eNatureSpec_Dentiste                          , EnumUnitNatureSpecialization::dentiste                            , "DENTAL" );
    natureSpecialization_[ "Finance"                              ] = new NatureSpecialization( "Finance"                              , eNatureSpec_Finance                           , EnumUnitNatureSpecialization::finance                             , "FINANC" );
    natureSpecialization_[ "Guerre electronique"                  ] = new NatureSpecialization( "Guerre electronique"                  , eNatureSpec_GuerreElectronique                , EnumUnitNatureSpecialization::guerre_electronique                 , "EW"     );
    natureSpecialization_[ "Helicoptere"                          ] = new NatureSpecialization( "Helicoptere"                          , eNatureSpec_Helicoptere                       , EnumUnitNatureSpecialization::helicoptere                         , "HELCTR" );
    natureSpecialization_[ "Mecanise"                             ] = new NatureSpecialization( "Mecanise"                             , eNatureSpec_Mecanise                          , EnumUnitNatureSpecialization::mecanise                            , "MECH"   );
    natureSpecialization_[ "Motorise"                             ] = new NatureSpecialization( "Motorise"                             , eNatureSpec_Motorise                          , EnumUnitNatureSpecialization::motorise                            , "MTRSD"  );
    natureSpecialization_[ "Naval"                                ] = new NatureSpecialization( "Naval"                                , eNatureSpec_Naval                             , EnumUnitNatureSpecialization::naval                               , "NAVAL"  );
    natureSpecialization_[ "Patrouille cotiere"                   ] = new NatureSpecialization( "Patrouille cotiere"                   , eNatureSpec_PatrouilleCotiere                 , EnumUnitNatureSpecialization::patrouille_cotiere                  , "SHRPAT" );
    natureSpecialization_[ "Police militaire"                     ] = new NatureSpecialization( "Police militaire"                     , eNatureSpec_PoliceMilitaire                   , EnumUnitNatureSpecialization::police_militaire                    , "MILPOL" );
    natureSpecialization_[ "Psychologique"                        ] = new NatureSpecialization( "Psychologique"                        , eNatureSpec_Psychologique                     , EnumUnitNatureSpecialization::psychologique                       , "PSYCH"  );
    natureSpecialization_[ "Religieux"                            ] = new NatureSpecialization( "Religieux"                            , eNatureSpec_Religieux                         , EnumUnitNatureSpecialization::religieux                           , "RELCHP" );
    natureSpecialization_[ "Service du personnel"                 ] = new NatureSpecialization( "Service du personnel"                 , eNatureSpec_ServiceDuPersonnel                , EnumUnitNatureSpecialization::service_du_personnel                , "PERSVC" );
    natureSpecialization_[ "Terrestre"                            ] = new NatureSpecialization( "Terrestre"                            , eNatureSpec_Terrestre                         , EnumUnitNatureSpecialization::terrestre                           , "GROUND" );
    natureSpecialization_[ "Veterinaire"                          ] = new NatureSpecialization( "Veterinaire"                          , eNatureSpec_Veterinaire                       , EnumUnitNatureSpecialization::veterinaire                         , "VET"    );
    natureSpecialization_[ "Voilure fixe"                         ] = new NatureSpecialization( "Voilure fixe"                         , eNatureSpec_VoilureFixe                       , EnumUnitNatureSpecialization::voilure_fixe                        , "FIXWNG" );
    natureSpecialization_[ "None"                                 ] = new NatureSpecialization( "None"                                 , eNatureSpec_None                              , EnumUnitNatureSpecialization::none                                , "NKN"    );
}

// -----------------------------------------------------------------------------
// Name: NatureSpecialization::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureSpecialization::Terminate()
{
    for( CIT_NatureSpecializationMap it = natureSpecialization_.begin(); it != natureSpecialization_.end(); ++it )
        delete it->second;
    natureSpecialization_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureSpecialization constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureSpecialization::NatureSpecialization( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureSpecialization nAsnID, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: NatureSpecialization destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureSpecialization::~NatureSpecialization()
{
}