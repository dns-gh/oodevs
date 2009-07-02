// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_Breakdown.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:17 $
// $Revision: 4 $
// $Workfile: PHY_Breakdown.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Breakdown.h"

BOOST_CLASS_EXPORT_GUID( PHY_Breakdown, "PHY_Breakdown" )

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown constructor
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
PHY_Breakdown::PHY_Breakdown( const PHY_BreakdownType& type )
    : pType_      ( &type )
    , nRepairTime_( type.ChooseARepairTime() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_Breakdown::PHY_Breakdown()
    : pType_      ( 0 )
    , nRepairTime_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown destructor
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
PHY_Breakdown::~PHY_Breakdown()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::load
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_Breakdown::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nID;
    
    file >> nID;
    pType_ = PHY_BreakdownType::Find( nID );
    assert( pType_ );
    
    file >> const_cast< uint& >( nRepairTime_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::save
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void PHY_Breakdown::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );
    unsigned type = pType_->GetID();
    file << type;
    file << nRepairTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetRepairTime
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
uint PHY_Breakdown::GetRepairTime() const
{
    return nRepairTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetTheoricRepairTime
// Created: NLD 2006-06-01
// -----------------------------------------------------------------------------
uint PHY_Breakdown::GetTheoricRepairTime() const
{
    assert( pType_ );
    return pType_->GetTheoricRepairTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::AffectMobility
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
bool PHY_Breakdown::AffectMobility() const
{
    assert( pType_ );
    return pType_->AffectMobility();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetNeededParts
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
const PHY_BreakdownType::T_PartMap& PHY_Breakdown::GetNeededParts() const
{
    assert( pType_ );
    return pType_->GetParts();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetMosID
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
uint PHY_Breakdown::GetID() const
{
    assert( pType_ );
    return pType_->GetID();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::AffectElectronic
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
bool PHY_Breakdown::AffectElectronic() const
{
    assert( pType_ ); 
    return pType_->AffectElectronic();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetMaintenanceLevel
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
const PHY_MaintenanceLevel& PHY_Breakdown::GetMaintenanceLevel() const
{
    assert( pType_ );
    return pType_->GetMaintenanceLevel();
}
