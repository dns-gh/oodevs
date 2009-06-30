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

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Maintenance.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Maintenance, "PHY_RolePion_Maintenance" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Maintenance::PHY_RolePion_Maintenance( MT_RoleContainer& role )
    : PHY_RoleInterface_Maintenance( role )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_Maintenance::PHY_RolePion_Maintenance()
    : PHY_RoleInterface_Maintenance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Maintenance::~PHY_RolePion_Maintenance()
{
    // NOTHING
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
// Name: PHY_RolePion_Maintenance::UpdateLogistic
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::UpdateLogistic( bool /*bIsDead*/ )
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* PHY_RolePion_Maintenance::HandleComposanteForTransport( MIL_AgentPion& /*pion*/, PHY_ComposantePion& /*composante*/ )
{
    return 0;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePion_Maintenance::HandleComposanteForTransport( PHY_MaintenanceComposanteState& /*composanteState*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::GetAvailabilityScoreForTransport
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePion_Maintenance::GetAvailabilityScoreForTransport( const PHY_ComposantePion& /*composante*/ )
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::HandleComposanteForRepair
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_RolePion_Maintenance::HandleComposanteForRepair( PHY_MaintenanceComposanteState& /*composanteState*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::GetAvailabilityScoreForRepair
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
int PHY_RolePion_Maintenance::GetAvailabilityScoreForRepair( PHY_MaintenanceComposanteState& /*composanteState*/ )
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::EnableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::DisableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::ChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::ChangePriorities( const T_MaintenancePriorityVector& /*priorities*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::ChangePriorities
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::ChangePriorities( const T_AutomateVector& /*priorities*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::ChangeWorkRate
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Maintenance::ChangeWorkRate( const PHY_MaintenanceWorkRate& /*workRate*/ )
{
    // NOTHING
}
