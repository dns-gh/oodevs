// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_BreakdownType.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:15 $
// $Revision: 2 $
// $Workfile: PHY_BreakdownType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetDiagnosticTime
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
uint PHY_BreakdownType::GetDiagnosticTime()
{
    return nDiagnosticTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_BreakdownType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetMosID
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
uint PHY_BreakdownType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::Find
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
const PHY_BreakdownType* PHY_BreakdownType::Find( const std::string& strName )
{
    CIT_BreakdownMap it = breakdowns_.find( strName );
    
    return it == breakdowns_.end() ? 0 :  it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_BreakdownType* PHY_BreakdownType::Find( uint nID )
{
    CIT_BreakdownMap it = std::find_if( breakdowns_.begin(), breakdowns_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_BreakdownType::GetID ), std::select2nd< T_BreakdownMap::value_type >() ) ) );

    return it == breakdowns_.end() ? 0 :  it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::AffectMobility
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
bool PHY_BreakdownType::AffectMobility() const
{
    return nType_ == eMobility;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::AffectElectronic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
bool PHY_BreakdownType::AffectElectronic() const
{
    return nType_ == eElectronic;   
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetMaintenanceLevel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
const PHY_MaintenanceLevel& PHY_BreakdownType::GetMaintenanceLevel() const
{
    return maintenanceLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetParts
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
const PHY_BreakdownType::T_PartMap& PHY_BreakdownType::GetParts() const
{
    return parts_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ChooseARepairTime
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
uint PHY_BreakdownType::ChooseARepairTime() const
{
    return (uint)repairTime_.rand();
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetTheoricRepairTime
// Created: NLD 2006-06-01
// -----------------------------------------------------------------------------
inline
uint PHY_BreakdownType::GetTheoricRepairTime() const
{
    return nTheoricRepairTime_;
}
