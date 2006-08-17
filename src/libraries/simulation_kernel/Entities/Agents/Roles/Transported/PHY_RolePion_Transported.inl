// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Transported/PHY_RolePion_Transported.inl $
// $Author: Nld $
// $Modtime: 24/11/04 18:42 $
// $Revision: 1 $
// $Workfile: PHY_RolePion_Transported.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::HasChanged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Transported::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::IsTransported
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Transported::IsTransported() const
{
    return pTransporter_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Transported::HasHumanTransportersToRecover
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Transported::HasHumanTransportersToRecover() const
{
    return !vHumanTransporterPosition_.IsZero();
}
