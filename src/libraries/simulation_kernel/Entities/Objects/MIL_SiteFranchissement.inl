//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_SiteFranchissement.inl $
// $Author: Nld $
// $Modtime: 2/11/04 11:43 $
// $Revision: 3 $
// $Workfile: MIL_SiteFranchissement.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_SiteFranchissement::Create()
{
    return *new MIL_SiteFranchissement();
}


// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::GetWidth
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
inline
MT_Float MIL_SiteFranchissement::GetWidth() const
{
    return rWidth_;

}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::GetDepth
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
inline
MT_Float MIL_SiteFranchissement::GetDepth() const
{
    return rDepth_;
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::GetCurrentSpeed
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
inline
MT_Float MIL_SiteFranchissement::GetCurrentSpeed() const
{
    return rCurrentSpeed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::GetBanksToFitOut
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
inline
bool MIL_SiteFranchissement::GetBanksToFitOut() const
{
    return bBanksToFitOut_;
}
