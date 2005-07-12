// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Morale.cpp $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: MOS_Morale.cpp $
//
// *****************************************************************************

#include "MOS_Light2_Pch.h"

#include "MOS_Morale.h"

MOS_Morale::T_MoraleMap MOS_Morale::morales_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const MOS_Morale MOS_Morale::mauvais_  ( "Mauvais"  , eMauvais  , EnumUnitMoral::mauvais   );
const MOS_Morale MOS_Morale::moyen_    ( "Moyen"    , eMoyen    , EnumUnitMoral::moyen     );
const MOS_Morale MOS_Morale::bon_      ( "Bon"      , eBon      , EnumUnitMoral::bon       );
const MOS_Morale MOS_Morale::fanatique_( "Fanatique", eFanatique, EnumUnitMoral::fanatique );

// -----------------------------------------------------------------------------
// Name: MOS_Morale::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MOS_Morale::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing morales" );
    morales_[ mauvais_  .GetName() ] = &mauvais_;
    morales_[ moyen_    .GetName() ] = &moyen_;
    morales_[ bon_      .GetName() ] = &bon_;
    morales_[ fanatique_.GetName() ] = &fanatique_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Morale::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MOS_Morale::Terminate()
{
    morales_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Morale constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MOS_Morale::MOS_Morale( const std::string& strName, E_MoraleType nType, ASN1T_EnumUnitMoral nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_Morale destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MOS_Morale::~MOS_Morale()
{
}
