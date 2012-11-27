// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_NbcSuit.h"
#include "MT_Tools/MT_Logger.h"

PHY_NbcSuit::T_NbcSuitMap PHY_NbcSuit::nbcSuits_;

const PHY_NbcSuit PHY_NbcSuit::none_  ( "none"  , eAgentNone );
const PHY_NbcSuit PHY_NbcSuit::level1_( "level1", eAgentNbcSuitLevel1 );
const PHY_NbcSuit PHY_NbcSuit::level2_( "level2", eAgentNbcSuitLevel2 );
const PHY_NbcSuit PHY_NbcSuit::level3_( "level3", eAgentNbcSuitLevel3 );
const PHY_NbcSuit PHY_NbcSuit::level4_( "level4", eAgentNbcSuitLevel4 );
const PHY_NbcSuit PHY_NbcSuit::level5_( "level5", eAgentNbcSuitLevel5 );

// -----------------------------------------------------------------------------
// Name: PHY_NbcSuit::Initialize
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
void PHY_NbcSuit::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing nbc suits" );
    nbcSuits_[ none_   .GetName() ] = &none_;
    nbcSuits_[ level1_ .GetName() ] = &level1_;
    nbcSuits_[ level2_ .GetName() ] = &level2_;
    nbcSuits_[ level3_ .GetName() ] = &level3_;
    nbcSuits_[ level4_ .GetName() ] = &level4_;
    nbcSuits_[ level5_ .GetName() ] = &level5_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NbcSuit::Find
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
const PHY_NbcSuit* PHY_NbcSuit::Find( const std::string& strName )
{
    CIT_NbcSuitMap it = nbcSuits_.find( strName );
    return it == nbcSuits_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_NbcSuit constructor
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
PHY_NbcSuit::PHY_NbcSuit( const std::string& strName, E_AgentNbcSuit suit )
    : strName_( strName )
    , suit_   ( suit )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_NbcSuit destructor
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
PHY_NbcSuit::~PHY_NbcSuit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_NbcSuit::GetName
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
const std::string& PHY_NbcSuit::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NbcSuit::GetType
// Created: LGY 2012-11-27
// -----------------------------------------------------------------------------
PHY_NbcSuit::E_AgentNbcSuit PHY_NbcSuit::GetType() const
{
    return suit_;
}
