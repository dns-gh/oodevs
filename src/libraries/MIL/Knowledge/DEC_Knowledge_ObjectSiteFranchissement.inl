// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectSiteFranchissement.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 11:25 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectSiteFranchissement.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement::GetWidth
// Created: NLD 2004-05-13
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_ObjectSiteFranchissement::GetWidth() const
{
    return rWidth_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement::IsBanksToFitOut
// Created: PBT 2005-03-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_ObjectSiteFranchissement::IsBanksToFitOut() const
{
    return bBanksToFitOut_;    
}
