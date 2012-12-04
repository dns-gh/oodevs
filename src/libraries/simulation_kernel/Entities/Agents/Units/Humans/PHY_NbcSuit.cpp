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
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/assign/list_of.hpp>

PHY_NbcSuit::T_NbcSuitMap PHY_NbcSuit::nbcSuits_;

// -----------------------------------------------------------------------------
// Name: PHY_NbcSuit::Initialize
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
void PHY_NbcSuit::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing nbc suits" );
    nbcSuits_ = boost::assign::map_list_of( "none"  , boost::make_shared< PHY_NbcSuit >( "none"  , eAgentNone ) )
                                          ( "level1", boost::make_shared< PHY_NbcSuit >( "level1", eAgentNbcSuitLevel1 ) )
                                          ( "level2", boost::make_shared< PHY_NbcSuit >( "level2", eAgentNbcSuitLevel2 ) )
                                          ( "level3", boost::make_shared< PHY_NbcSuit >( "level3", eAgentNbcSuitLevel3 ) )
                                          ( "level4", boost::make_shared< PHY_NbcSuit >( "level4", eAgentNbcSuitLevel4 ) )
                                          ( "level5", boost::make_shared< PHY_NbcSuit >( "level5", eAgentNbcSuitLevel5 ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_NbcSuit::Find
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
boost::shared_ptr< const PHY_NbcSuit > PHY_NbcSuit::Find( const std::string& strName )
{
    CIT_NbcSuitMap it = nbcSuits_.find( strName );
    return it == nbcSuits_.end() ? boost::shared_ptr< const PHY_NbcSuit >() : it->second;
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
unsigned int PHY_NbcSuit::GetType() const
{
    return static_cast< unsigned int>( suit_ );
}
