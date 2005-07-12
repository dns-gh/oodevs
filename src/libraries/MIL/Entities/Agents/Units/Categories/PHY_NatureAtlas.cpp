// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_NatureAtlas.cpp $
// $Author: Nld $
// $Modtime: 18/03/05 12:13 $
// $Revision: 3 $
// $Workfile: PHY_NatureAtlas.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_NatureAtlas.h"

PHY_NatureAtlas::T_NatureAtlasMap PHY_NatureAtlas::natureAtlas_;
const PHY_NatureAtlas             PHY_NatureAtlas::notDefined_ ( "None", PHY_NatureAtlas::eNatureAtlas_None, 0x20 );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureAtlas::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureAtlas::Initialize()
{
    natureAtlas_[ "Blinde"              ] = new PHY_NatureAtlas( "Blinde"           , eNatureAtlas_Blinde        , 0x8000 );
    natureAtlas_[ "ASS"                 ] = new PHY_NatureAtlas( "ASS"              , eNatureAtlas_ASS           , 0x4000 );
    natureAtlas_[ "PC"                  ] = new PHY_NatureAtlas( "PC"               , eNatureAtlas_PC            , 0x2000 );
    natureAtlas_[ "Log"                 ] = new PHY_NatureAtlas( "Log"              , eNatureAtlas_Log           , 0x1000 );
    natureAtlas_[ "Mortier"             ] = new PHY_NatureAtlas( "Mortier"          , eNatureAtlas_Mortier       , 0x800  );
    natureAtlas_[ "Lance roquette"      ] = new PHY_NatureAtlas( "Lance roquette"   , eNatureAtlas_LanceRoquette , 0x400  );
    natureAtlas_[ "Vehicule"            ] = new PHY_NatureAtlas( "Vehicule"         , eNatureAtlas_Vehicule      , 0x200  );
    natureAtlas_[ "Personnel a pied"    ] = new PHY_NatureAtlas( "Personnel a pied" , eNatureAtlas_PersonnelAPied, 0x100  );
    natureAtlas_[ "Helicoptere"         ] = new PHY_NatureAtlas( "Helicoptere"      , eNatureAtlas_Helicoptere   , 0x80   );
    natureAtlas_[ "Indefini"            ] = new PHY_NatureAtlas( "Indefini"         , eNatureAtlas_Indefini      , 0x40   );
    natureAtlas_[ "ASA"                 ] = new PHY_NatureAtlas( "ASA"              , eNatureAtlas_ASA           , 0x20   );
    natureAtlas_[ notDefined_.GetName() ] = &notDefined_; 
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureAtlas::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureAtlas::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureAtlas constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureAtlas::PHY_NatureAtlas( const std::string& strName, E_Type nType, uint nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureAtlas destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureAtlas::~PHY_NatureAtlas()
{
}