//*****************************************************************************
//
// $Created: NLD 2003-07-25 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Polyline.inl $
// $Author: Nld $
// $Modtime: 1/09/04 16:46 $
// $Revision: 3 $
// $Workfile: MT_Polyline.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: MT_Polyline::GetPoints
// Created: NLD 2003-07-31
// -----------------------------------------------------------------------------
inline
const T_PointVector& MT_Polyline::GetPoints() const
{
    return points_;
}
