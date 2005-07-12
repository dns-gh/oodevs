// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_MaintenanceLevel.inl $
// $Author: Jvt $
// $Modtime: 26/04/05 12:04 $
// $Revision: 3 $
// $Workfile: PHY_MaintenanceLevel.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::GetMaintenanceLevels
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
const PHY_MaintenanceLevel::T_MaintenanceLevelMap& PHY_MaintenanceLevel::GetMaintenanceLevels()
{
    return maintenanceLevels_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_MaintenanceLevel::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::operator==
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
bool PHY_MaintenanceLevel::operator==( const PHY_MaintenanceLevel& rhs ) const
{
    return this == &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceLevel::operator!=
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
bool PHY_MaintenanceLevel::operator!=( const PHY_MaintenanceLevel& rhs ) const
{
    return this != &rhs;
}


