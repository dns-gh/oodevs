// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_ConsumptionType.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_ConsumptionType.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_ConsumptionType.h"

PHY_ConsumptionType::T_ConsumptionTypeMap PHY_ConsumptionType::consumptionTypes_;

const PHY_ConsumptionType PHY_ConsumptionType::engineStopped_( "ArretMoteurEteint", eEngineStopped );
const PHY_ConsumptionType PHY_ConsumptionType::engineStarted_( "ArretMoteurAllume", eEngineStarted );
const PHY_ConsumptionType PHY_ConsumptionType::moving_       ( "Deplacement"      , eMoving        );
const PHY_ConsumptionType PHY_ConsumptionType::working_      ( "EnTravaux"        , eWorking       );

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_ConsumptionType::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing consumption types" );
    consumptionTypes_[ engineStopped_.GetName() ] = &engineStopped_;
    consumptionTypes_[ engineStarted_.GetName() ] = &engineStarted_;
    consumptionTypes_[ moving_       .GetName() ] = &moving_;
    consumptionTypes_[ working_      .GetName() ] = &working_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_ConsumptionType::Terminate()
{    
    consumptionTypes_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_ConsumptionType::PHY_ConsumptionType( const std::string& strName, E_ConsumptionType nType )
    : strName_( strName )
    , nType_  ( nType )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_ConsumptionType::~PHY_ConsumptionType()
{
}
