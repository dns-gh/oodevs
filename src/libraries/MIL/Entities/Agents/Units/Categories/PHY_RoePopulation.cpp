// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_RoePopulation.cpp $
// $Author: Nld $
// $Modtime: 18/03/05 12:13 $
// $Revision: 3 $
// $Workfile: PHY_RoePopulation.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RoePopulation.h"

PHY_RoePopulation::T_RoePopulationMap PHY_RoePopulation::roePopulations_;
const PHY_RoePopulation               PHY_RoePopulation::none_( "None", PHY_RoePopulation::eNone, EnumRoePopulation::none );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_RoePopulation::Initialize()
{
    roePopulations_[ none_.GetName()  ] = &none_;
    roePopulations_[ "Interdit"       ] = new PHY_RoePopulation( "Interdit"      , eInterdit      , EnumRoePopulation::tir_interdit       );
    roePopulations_[ "Sommation"      ] = new PHY_RoePopulation( "Sommation"     , eSommation     , EnumRoePopulation::tir_sommation      ); 
    roePopulations_[ "Dissuasion"     ] = new PHY_RoePopulation( "Dissuasion"    , eDissuasion    , EnumRoePopulation::tir_dissuasion     );
    roePopulations_[ "Neutralisation" ] = new PHY_RoePopulation( "Neutralisation", eNeutralisation, EnumRoePopulation::tir_neutralisation );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_RoePopulation::Terminate()
{
    roePopulations_.erase( none_.GetName() );
    for( CIT_RoePopulationMap it = roePopulations_.begin(); it != roePopulations_.end(); ++it )
        delete it->second;
    roePopulations_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_RoePopulation::PHY_RoePopulation( const std::string& strName, E_Type nType, ASN1T_EnumRoePopulation nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_RoePopulation::~PHY_RoePopulation()
{
}