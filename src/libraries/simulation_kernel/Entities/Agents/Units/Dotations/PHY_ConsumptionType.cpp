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

#include "simulation_kernel_pch.h"
#include "PHY_ConsumptionType.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Stl.h"

PHY_ConsumptionType::T_ConsumptionTypeMap PHY_ConsumptionType::consumptionTypes_;

const PHY_ConsumptionType PHY_ConsumptionType::engineStopped_( "ArretMoteurEteint", eEngineStopped );
const PHY_ConsumptionType PHY_ConsumptionType::engineStarted_( "ArretMoteurAllume", eEngineStarted );
const PHY_ConsumptionType PHY_ConsumptionType::moving_       ( "Deplacement"      , eMoving        );
const PHY_ConsumptionType PHY_ConsumptionType::working_      ( "EnTravaux"        , eWorking       );

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

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_ConsumptionType::PHY_ConsumptionType( const std::string& strName, E_ConsumptionType nType )
    : strName_( strName )
    , nType_  ( nType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_ConsumptionType::~PHY_ConsumptionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::GetConsumptionTypes
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_ConsumptionType::T_ConsumptionTypeMap& PHY_ConsumptionType::GetConsumptionTypes()
{
    return consumptionTypes_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::FindConsumptionType
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_ConsumptionType* PHY_ConsumptionType::FindConsumptionType( const std::string& strName )
{
    CIT_ConsumptionTypeMap it = consumptionTypes_.find( strName );
    return it == consumptionTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::FindConsumptionType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_ConsumptionType* PHY_ConsumptionType::FindConsumptionType( unsigned int nID )
{
    // $$$$ JVT : Recherche linéaire, mais n'est utilisé que lors de la reprise de la sim depuis un checkpoint
    CIT_ConsumptionTypeMap it = std::find_if( consumptionTypes_.begin(), consumptionTypes_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &PHY_ConsumptionType::GetID ), std::select2nd< T_ConsumptionTypeMap::value_type >() ) ) );

    return it == consumptionTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_ConsumptionType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
unsigned int PHY_ConsumptionType::GetID() const
{
    return static_cast< unsigned int >( nType_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::operator <
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
bool PHY_ConsumptionType::operator <( const PHY_ConsumptionType& rhs ) const
{
    return nType_ < rhs.nType_;
}
