// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureSpecialization.cpp $
// $Author: Nld $
// $Modtime: 6/04/05 10:24 $
// $Revision: 3 $
// $Workfile: PHY_NatureSpecialization.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_NatureSpecialization.h"

PHY_NatureSpecialization::T_NatureSpecializationMap PHY_NatureSpecialization::natureSpecialization_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureSpecialization::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureSpecialization::Initialize()
{
    natureSpecialization_[ "Jag"                                  ] = new PHY_NatureSpecialization( "Jag"                                  , eNatureSpec_Jag                               , EnumUnitNatureSpecialization::jag                                 );
    natureSpecialization_[ "Riverine"                             ] = new PHY_NatureSpecialization( "Riverine"                             , eNatureSpec_Riverine                          , EnumUnitNatureSpecialization::riverine                            );
    natureSpecialization_[ "Security police"                      ] = new PHY_NatureSpecialization( "Security police"                      , eNatureSpec_SecurityPolice                    , EnumUnitNatureSpecialization::security_police                     );
    natureSpecialization_[ "Aerien"                               ] = new PHY_NatureSpecialization( "Aerien"                               , eNatureSpec_Aerien                            , EnumUnitNatureSpecialization::aerien                              );
    natureSpecialization_[ "Aeroporte"                            ] = new PHY_NatureSpecialization( "Aeroporte"                            , eNatureSpec_Aeroporte                         , EnumUnitNatureSpecialization::aeroporte                           );
    natureSpecialization_[ "Attaque aerienne"                     ] = new PHY_NatureSpecialization( "Attaque aerienne"                     , eNatureSpec_AttaqueAerienne                   , EnumUnitNatureSpecialization::attaque_aerienne                    );
    natureSpecialization_[ "Blinde"                               ] = new PHY_NatureSpecialization( "Blinde"                               , eNatureSpec_Blinde                            , EnumUnitNatureSpecialization::blinde                              );
    natureSpecialization_[ "Chemin de fer"                        ] = new PHY_NatureSpecialization( "Chemin de fer"                        , eNatureSpec_CheminDeFer                       , EnumUnitNatureSpecialization::chemin_de_fer                       );
    natureSpecialization_[ "Chirurgie"                            ] = new PHY_NatureSpecialization( "Chirurgie"                            , eNatureSpec_Chirurgie                         , EnumUnitNatureSpecialization::chirurgie                           );
    natureSpecialization_[ "Compose"                              ] = new PHY_NatureSpecialization( "Compose"                              , eNatureSpec_Compose                           , EnumUnitNatureSpecialization::compose                             );
    natureSpecialization_[ "Construction"                         ] = new PHY_NatureSpecialization( "Construction"                         , eNatureSpec_Construction                      , EnumUnitNatureSpecialization::construction                        );
    natureSpecialization_[ "Courrier"                             ] = new PHY_NatureSpecialization( "Courrier"                             , eNatureSpec_Courrier                          , EnumUnitNatureSpecialization::courrier                            );
    natureSpecialization_[ "Decollage atterissage vertical court" ] = new PHY_NatureSpecialization( "Decollage atterissage vertical court" , eNatureSpec_DecollageAtterissageVerticalCourt , EnumUnitNatureSpecialization::decollage_atterissage_vertical_court);
    natureSpecialization_[ "Dentiste"                             ] = new PHY_NatureSpecialization( "Dentiste"                             , eNatureSpec_Dentiste                          , EnumUnitNatureSpecialization::dentiste                            );
    natureSpecialization_[ "Finance"                              ] = new PHY_NatureSpecialization( "Finance"                              , eNatureSpec_Finance                           , EnumUnitNatureSpecialization::finance                             );
    natureSpecialization_[ "Guerre electronique"                  ] = new PHY_NatureSpecialization( "Guerre electronique"                  , eNatureSpec_GuerreElectronique                , EnumUnitNatureSpecialization::guerre_electronique                 );
    natureSpecialization_[ "Helicoptere"                          ] = new PHY_NatureSpecialization( "Helicoptere"                          , eNatureSpec_Helicoptere                       , EnumUnitNatureSpecialization::helicoptere                         );
    natureSpecialization_[ "Mecanise"                             ] = new PHY_NatureSpecialization( "Mecanise"                             , eNatureSpec_Mecanise                          , EnumUnitNatureSpecialization::mecanise                            );
    natureSpecialization_[ "Motorise"                             ] = new PHY_NatureSpecialization( "Motorise"                             , eNatureSpec_Motorise                          , EnumUnitNatureSpecialization::motorise                            );
    natureSpecialization_[ "Naval"                                ] = new PHY_NatureSpecialization( "Naval"                                , eNatureSpec_Naval                             , EnumUnitNatureSpecialization::naval                               );
    natureSpecialization_[ "Patrouille cotiere"                   ] = new PHY_NatureSpecialization( "Patrouille cotiere"                   , eNatureSpec_PatrouilleCotiere                 , EnumUnitNatureSpecialization::patrouille_cotiere                  );
    natureSpecialization_[ "Police militaire"                     ] = new PHY_NatureSpecialization( "Police militaire"                     , eNatureSpec_PoliceMilitaire                   , EnumUnitNatureSpecialization::police_militaire                    );
    natureSpecialization_[ "Psychologique"                        ] = new PHY_NatureSpecialization( "Psychologique"                        , eNatureSpec_Psychologique                     , EnumUnitNatureSpecialization::psychologique                       );
    natureSpecialization_[ "Religieux"                            ] = new PHY_NatureSpecialization( "Religieux"                            , eNatureSpec_Religieux                         , EnumUnitNatureSpecialization::religieux                           );
    natureSpecialization_[ "Service du personnel"                 ] = new PHY_NatureSpecialization( "Service du personnel"                 , eNatureSpec_ServiceDuPersonnel                , EnumUnitNatureSpecialization::service_du_personnel                );
    natureSpecialization_[ "Terrestre"                            ] = new PHY_NatureSpecialization( "Terrestre"                            , eNatureSpec_Terrestre                         , EnumUnitNatureSpecialization::terrestre                           );
    natureSpecialization_[ "Veterinaire"                          ] = new PHY_NatureSpecialization( "Veterinaire"                          , eNatureSpec_Veterinaire                       , EnumUnitNatureSpecialization::veterinaire                         );
    natureSpecialization_[ "Voilure fixe"                         ] = new PHY_NatureSpecialization( "Voilure fixe"                         , eNatureSpec_VoilureFixe                       , EnumUnitNatureSpecialization::voilure_fixe                        );
    natureSpecialization_[ "None"                                 ] = new PHY_NatureSpecialization( "None"                                 , eNatureSpec_None                              , EnumUnitNatureSpecialization::none                                );
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureSpecialization::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureSpecialization::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureSpecialization constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureSpecialization::PHY_NatureSpecialization( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureSpecialization nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureSpecialization destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureSpecialization::~PHY_NatureSpecialization()
{
}