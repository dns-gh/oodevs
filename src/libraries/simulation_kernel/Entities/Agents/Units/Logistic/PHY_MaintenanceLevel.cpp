// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_MaintenanceLevel.cpp $
// $Author: Jvt $
// $Modtime: 26/04/05 15:05 $
// $Revision: 3 $
// $Workfile: PHY_MaintenanceLevel.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MaintenanceLevel.h"
#include "PHY_BreakdownType.h"

PHY_MaintenanceLevel::T_MaintenanceLevelMap PHY_MaintenanceLevel::maintenanceLevels_;

PHY_MaintenanceLevel PHY_MaintenanceLevel::nti1_( "NTI1" );
PHY_MaintenanceLevel PHY_MaintenanceLevel::nti2_( "NTI2" );
PHY_MaintenanceLevel PHY_MaintenanceLevel::nti3_( "NTI3" );   

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::Initialize
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_MaintenanceLevel::Initialize()
{
    maintenanceLevels_[ nti1_.GetName() ] = &nti1_;
    maintenanceLevels_[ nti2_.GetName() ] = &nti2_;
    maintenanceLevels_[ nti3_.GetName() ] = &nti3_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::Terminate
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_MaintenanceLevel::Terminate()
{
    maintenanceLevels_.clear();
}
  
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel constructor
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
PHY_MaintenanceLevel::PHY_MaintenanceLevel( const std::string& strName )
    : strName_                                ( strName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel destructor
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
PHY_MaintenanceLevel::~PHY_MaintenanceLevel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::GetMaintenanceLevels
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
const PHY_MaintenanceLevel::T_MaintenanceLevelMap& PHY_MaintenanceLevel::GetMaintenanceLevels()
{
    return maintenanceLevels_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_MaintenanceLevel::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::operator==
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
bool PHY_MaintenanceLevel::operator==( const PHY_MaintenanceLevel& rhs ) const
{
    return this == &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::operator!=
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
bool PHY_MaintenanceLevel::operator!=( const PHY_MaintenanceLevel& rhs ) const
{
    return this != &rhs;
}

