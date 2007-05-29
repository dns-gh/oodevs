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
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_Breakdown::PHY_Breakdown()
    : pType_      ( 0 )
    , nRepairTime_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown destructor
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
PHY_Breakdown::~PHY_Breakdown()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

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
