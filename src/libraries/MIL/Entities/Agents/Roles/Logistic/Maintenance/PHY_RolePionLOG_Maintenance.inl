// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.inl $
// $Author: Jvt $
// $Modtime: 30/03/05 17:29 $
// $Revision: 3 $
// $Workfile: PHY_RolePionLOG_Maintenance.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::GetPion
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
const MIL_AgentPionLOG_ABC& PHY_RolePionLOG_Maintenance::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Maintenance::EnableSystem()
{
    bSystemEnabled_ = true;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Maintenance::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Maintenance::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_    = true;
}
