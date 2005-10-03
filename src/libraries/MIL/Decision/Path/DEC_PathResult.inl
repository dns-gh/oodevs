//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathResult.inl $
// $Author: Age $
// $Modtime: 13/04/05 18:34 $
// $Revision: 3 $
// $Workfile: DEC_PathResult.inl $
//
//*****************************************************************************

 //-----------------------------------------------------------------------------
// Name: DEC_PathResult::GetResult
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
inline
const DEC_PathResult::T_PathPointList& DEC_PathResult::GetResult() const
{
    assert( GetState() != eComputing );
    return resultList_;
}


