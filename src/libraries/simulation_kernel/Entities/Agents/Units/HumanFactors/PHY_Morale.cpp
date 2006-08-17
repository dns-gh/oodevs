// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/HumanFactors/PHY_Morale.cpp $
// $Author: Nld $
// $Modtime: 1/12/04 10:36 $
// $Revision: 2 $
// $Workfile: PHY_Morale.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_Morale.h"

PHY_Morale::T_MoraleMap PHY_Morale::morales_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const PHY_Morale PHY_Morale::mauvais_  ( "Mauvais"  , eMauvais  , EnumUnitMoral::mauvais  , 0.5  );
const PHY_Morale PHY_Morale::moyen_    ( "Moyen"    , eMoyen    , EnumUnitMoral::moyen    , 0.75 );
const PHY_Morale PHY_Morale::bon_      ( "Bon"      , eBon      , EnumUnitMoral::bon      , 1    );
const PHY_Morale PHY_Morale::fanatique_( "Fanatique", eFanatique, EnumUnitMoral::fanatique, 1.25 );

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Morale::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing morales" );
    morales_[ mauvais_  .GetName() ] = &mauvais_;
    morales_[ moyen_    .GetName() ] = &moyen_;
    morales_[ bon_      .GetName() ] = &bon_;
    morales_[ fanatique_.GetName() ] = &fanatique_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Morale::Terminate()
{
    morales_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Morale constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Morale::PHY_Morale( const std::string& strName, E_MoraleType nType, ASN1T_EnumUnitMoral nAsnID, MT_Float rDIAWeight )
    : strName_   ( strName )
    , nType_     ( nType   )
    , nAsnID_    ( nAsnID  )
    , rDIAWeight_( rDIAWeight )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Morale destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Morale::~PHY_Morale()
{
}
