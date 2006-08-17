// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Humans/PHY_RolePion_Humans.inl $
// $Author: Nld $
// $Modtime: 10/01/05 11:08 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Humans.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNbrUsableHumans
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
uint PHY_RolePion_Humans::GetNbrUsableHumans() const
{
    return nNbrUsableHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Humans::HasChanged() const
{
    return nNbrHumansDataChanged_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::ChangeEvacuationMode
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Humans::ChangeEvacuationMode( E_EvacuationMode nMode )
{
    nEvacuationMode_ = nMode;
}
