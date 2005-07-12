// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FrontAndBackLinesComputer.inl $
// $Author: Nld $
// $Modtime: 19/10/04 10:56 $
// $Revision: 1 $
// $Workfile: DEC_FrontAndBackLinesComputer.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer::GetDistanceFromFrontLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
inline
MT_Float DEC_FrontAndBackLinesComputer::ComputeDistanceFromFrontLine( const MT_Vector2D& vPoint )
{
    Compute();
    return frontLineDroite_.GetDistanceToPoint( vPoint );
}


// -----------------------------------------------------------------------------
// Name: DEC_FrontAndBackLinesComputer::GetDistanceFromBackLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
inline
MT_Float DEC_FrontAndBackLinesComputer::ComputeDistanceFromBackLine( const MT_Vector2D& vPoint )
{
    Compute();
    return backLineDroite_.GetDistanceToPoint( vPoint );
}

