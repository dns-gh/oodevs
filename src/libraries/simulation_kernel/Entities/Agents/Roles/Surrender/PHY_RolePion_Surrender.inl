// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.inl $
// $Author: Nld $
// $Modtime: 24/02/05 17:31 $
// $Revision: 1 $
// $Workfile: PHY_RolePion_Surrender.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Surrender::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Surrender::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Surrender::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Surrender::IsPrisoner() const
{
    return bPrisoner_;
}