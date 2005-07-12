// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Surrender/PHY_RoleHLA_Surrender.inl $
// $Author: Nld $
// $Modtime: 10/03/05 14:58 $
// $Revision: 1 $
// $Workfile: PHY_RoleHLA_Surrender.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool PHY_RoleHLA_Surrender::IsSurrendered() const
{
    return bSurrendered_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Surrender::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool PHY_RoleHLA_Surrender::IsPrisoner() const
{
    return bPrisoner_;
}
