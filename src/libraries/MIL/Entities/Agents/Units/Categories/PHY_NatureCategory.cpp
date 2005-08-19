// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureCategory.cpp $
// $Author: Jvt $
// $Modtime: 9/12/04 15:53 $
// $Revision: 2 $
// $Workfile: PHY_NatureCategory.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_NatureCategory.h"

PHY_NatureCategory::T_NatureCategoryMap PHY_NatureCategory::natureCategory_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureCategory::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureCategory::Initialize()
{
    natureCategory_[ "Combat"             ] = new PHY_NatureCategory( "Combat"             , eCombat           , EnumUnitNatureCategory::combat             );
    natureCategory_[ "Soutien logistique" ] = new PHY_NatureCategory( "Soutien logistique" , eSoutienLogistique, EnumUnitNatureCategory::soutien_logistique );
    natureCategory_[ "Combat support"     ] = new PHY_NatureCategory( "Combat support"     , eCombatSupport    , EnumUnitNatureCategory::combat_support     );
    natureCategory_[ "Inconnu"            ] = new PHY_NatureCategory( "Inconnu"            , eInconnu          , EnumUnitNatureCategory::inconnu            );
    natureCategory_[ "None"               ] = new PHY_NatureCategory( "None"               , eNone             , EnumUnitNatureCategory::none               );
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureCategory::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureCategory::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureCategory constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureCategory::PHY_NatureCategory( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureCategory nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureCategory destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureCategory::~PHY_NatureCategory()
{
}