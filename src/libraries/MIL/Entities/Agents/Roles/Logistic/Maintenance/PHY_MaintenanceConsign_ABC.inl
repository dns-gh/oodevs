// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceConsign_ABC.inl $
// $Author: Jvt $
// $Modtime: 4/05/05 10:43 $
// $Revision: 4 $
// $Workfile: PHY_MaintenanceConsign_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::Clean
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
void PHY_MaintenanceConsign_ABC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::SetTimer
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
void PHY_MaintenanceConsign_ABC::SetState( E_State nNewState )
{
    nState_ = nNewState;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
PHY_MaintenanceConsign_ABC::E_State PHY_MaintenanceConsign_ABC::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::HasChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
bool PHY_MaintenanceConsign_ABC::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::IsFinished
// Created: NLD 2005-03-08
// -----------------------------------------------------------------------------
inline
bool PHY_MaintenanceConsign_ABC::IsFinished() const
{
    return nState_ == eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetComposanteState
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
inline
const PHY_MaintenanceComposanteState& PHY_MaintenanceConsign_ABC::GetComposanteState() const
{
    assert( pComposanteState_ );
    return *pComposanteState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetPionMaintenance
// Created: NLD 2006-08-11
// -----------------------------------------------------------------------------
inline
PHY_RolePionLOG_Maintenance& PHY_MaintenanceConsign_ABC::GetPionMaintenance() const
{
    assert( pMaintenance_ );
    return *pMaintenance_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceConsign_ABC::serialize( Archive& file, const uint )
{
    file & pMaintenance_
         & pComposanteState_
         & nState_
         & nTimer_;
}
