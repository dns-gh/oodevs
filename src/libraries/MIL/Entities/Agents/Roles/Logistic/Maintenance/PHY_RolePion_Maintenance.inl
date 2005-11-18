// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePion_Maintenance.inl $
// $Author: Nld $
// $Modtime: 27/04/05 17:15 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_Maintenance.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
PHY_MaintenanceComposanteState* PHY_RolePion_Maintenance::HandleComposanteForTransport( MIL_AgentPion& /*pion*/, PHY_ComposantePion& /*composante*/ )
{
    return 0;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::HandleComposanteForTransport
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Maintenance::HandleComposanteForTransport( PHY_MaintenanceComposanteState& /*composanteState*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::HandleComposanteForRepair
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Maintenance::HandleComposanteForRepair( PHY_MaintenanceComposanteState& /*composanteState*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::GetAvailabilityScoreForRepair
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
int PHY_RolePion_Maintenance::GetAvailabilityScoreForRepair( PHY_MaintenanceComposanteState& /*composanteState*/ )
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Maintenance::EnableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Maintenance::DisableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::ChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Maintenance::ChangePriorities( const T_MaintenancePriorityVector& /*priorities*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::ChangePriorities
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Maintenance::ChangePriorities( const T_AutomateVector& /*priorities*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Maintenance::ChangeWorkTime
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Maintenance::ChangeWorkTime( const PHY_LogWorkTime& /*workTime*/ )
{
    // NOTHING
}
