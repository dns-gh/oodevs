// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.inl $
// $Author: Nld $
// $Modtime: 10/03/05 15:42 $
// $Revision: 1 $
// $Workfile: PHY_RolePion_Refugee.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Refugee::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Refugee::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Refugee::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::IsManaged
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Refugee::IsManaged() const
{
    return bManaged_;
}
    