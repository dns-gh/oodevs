//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Lima/MIL_Lima.inl $
// $Author: Nld $
// $Modtime: 21/10/04 15:49 $
// $Revision: 2 $
// $Workfile: MIL_Lima.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MIL_Lima::GetID
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
uint MIL_Lima::GetID() const
{
    return nID_;
}

//-----------------------------------------------------------------------------
// Name: MIL_Lima::GetPointVector
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
inline
const T_PointVector& MIL_Lima::GetPointVector() const
{
    return pointVector_;
}

//-----------------------------------------------------------------------------
// Name: MIL_Lima::GetPointVector
// Created: NLD 2003-03-02
//-----------------------------------------------------------------------------
inline
MIL_Lima::E_LimaFunctions MIL_Lima::GetLimaFunction() const
{
    return  nLimaFunction_;
}
