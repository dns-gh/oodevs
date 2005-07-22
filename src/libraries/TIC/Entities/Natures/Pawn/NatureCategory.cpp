// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureCategory.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:02 $
// $Revision: 2 $
// $Workfile: NatureCategory.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"

#include "NatureCategory.h"

using namespace TIC;

NatureCategory::T_NatureCategoryMap NatureCategory::natureCategory_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureCategory::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureCategory::Initialize()
{
    natureCategory_[ "Combat"             ] = new NatureCategory( "Combat"             , eCombat           , EnumUnitNatureCategory::combat            , "COMBAT" );
    natureCategory_[ "Soutien logistique" ] = new NatureCategory( "Soutien logistique" , eSoutienLogistique, EnumUnitNatureCategory::soutien_logistique, "COMSER" );
    natureCategory_[ "Combat support"     ] = new NatureCategory( "Combat support"     , eCombatSupport    , EnumUnitNatureCategory::combat_support    , "COMSPT" );
    natureCategory_[ "Inconnu"            ] = new NatureCategory( "Inconnu"            , eInconnu          , EnumUnitNatureCategory::inconnu           , "NKN"    );
    natureCategory_[ "None"               ] = new NatureCategory( "None"               , eNone             , EnumUnitNatureCategory::none              , "NKN"    );
}

// -----------------------------------------------------------------------------
// Name: NatureCategory::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureCategory::Terminate()
{
    for( CIT_NatureCategoryMap it = natureCategory_.begin(); it != natureCategory_.end(); ++it )
        delete it->second;
    natureCategory_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureCategory constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureCategory::NatureCategory( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureCategory nAsnID, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: NatureCategory destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureCategory::~NatureCategory()
{
}