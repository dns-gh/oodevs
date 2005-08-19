// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePion_Maintenance.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_Maintenance.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RolePion_Maintenance.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Maintenance, "PHY_RolePion_Maintenance" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Maintenance::PHY_RolePion_Maintenance( MT_RoleContainer& role )
    : PHY_RoleInterface_Maintenance( role )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_Maintenance::PHY_RolePion_Maintenance()
    : PHY_RoleInterface_Maintenance()
{
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Maintenance::~PHY_RolePion_Maintenance()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::serailize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Maintenance::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Maintenance >( *this );
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::Clean()
{
    // NOTHING
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::SendChangedState() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::SendFullState() const
{
    // NOTHING
}
