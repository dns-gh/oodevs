// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOG_Supply.inl $
// $Author: Nld $
// $Modtime: 13/04/05 14:16 $
// $Revision: 5 $
// $Workfile: PHY_RolePionLOG_Supply.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Supply::EnableSystem()
{
    bSystemEnabled_ = true;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Supply::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetPion
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
inline
const MIL_AgentPionLOG_ABC& PHY_RolePionLOG_Supply::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}
