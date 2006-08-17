// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Humans/PHY_HumanRank.cpp $
// $Author: Nld $
// $Modtime: 29/12/04 14:01 $
// $Revision: 2 $
// $Workfile: PHY_HumanRank.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_HumanRank.h"

PHY_HumanRank::T_HumanRankMap PHY_HumanRank::humanRanks_;
const PHY_HumanRank           PHY_HumanRank::officier_       ( "Officier"       , eOfficier       , EnumHumanRank::officier    , true  );
const PHY_HumanRank           PHY_HumanRank::sousOfficier_   ( "SousOfficier"   , eSousOfficier   , EnumHumanRank::sous_officer, true  );
const PHY_HumanRank           PHY_HumanRank::militaireDuRang_( "MilitaireDuRang", eMilitaireDuRang, EnumHumanRank::mdr         , false );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_HumanRank::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing human ranks" );
    humanRanks_[ officier_.GetName()        ] = &officier_;
    humanRanks_[ sousOfficier_.GetName()    ] = &sousOfficier_;
    humanRanks_[ militaireDuRang_.GetName() ] = &militaireDuRang_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void PHY_HumanRank::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_HumanRank::PHY_HumanRank( const std::string& strName, E_Rank nRank, const ASN1T_EnumHumanRank nAsnID, bool bIsCommander )
    : strName_     ( strName )
    , nRank_       ( nRank )
    , nAsnID_      ( nAsnID )
    , bIsCommander_( bIsCommander )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_HumanRank destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_HumanRank::~PHY_HumanRank()
{

}
