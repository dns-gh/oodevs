// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Morale.cpp $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Morale.cpp $
//
// *****************************************************************************

#include "astec_pch.h"

#include "Morale.h"

Morale::T_MoraleMap Morale::morales_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const Morale Morale::mauvais_  ( "Mauvais"  , eMauvais  , EnumUnitMoral::mauvais   );
const Morale Morale::moyen_    ( "Moyen"    , eMoyen    , EnumUnitMoral::moyen     );
const Morale Morale::bon_      ( "Bon"      , eBon      , EnumUnitMoral::bon       );
const Morale Morale::fanatique_( "Fanatique", eFanatique, EnumUnitMoral::fanatique );

// -----------------------------------------------------------------------------
// Name: Morale::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void Morale::Initialize()
{
    morales_[ mauvais_  .GetName() ] = &mauvais_;
    morales_[ moyen_    .GetName() ] = &moyen_;
    morales_[ bon_      .GetName() ] = &bon_;
    morales_[ fanatique_.GetName() ] = &fanatique_;
}

// -----------------------------------------------------------------------------
// Name: Morale::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void Morale::Terminate()
{
    morales_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Morale constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
Morale::Morale( const std::string& strName, E_MoraleType nType, ASN1T_EnumUnitMoral nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: Morale destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
Morale::~Morale()
{
}
