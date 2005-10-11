//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Path/DEC_PathSection_ABC.inl $
// $Author: Nld $
// $Modtime: 22/02/05 14:32 $
// $Revision: 1 $
// $Workfile: DEC_PathSection_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::GetPosStart
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_PathSection_ABC::GetPosStart() const
{
    return vStartPoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::GetPosEnd
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_PathSection_ABC::GetPosEnd() const
{
    return vEndPoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::IsImpossible
// Created: AGE 2005-03-04
// -----------------------------------------------------------------------------
inline
bool DEC_PathSection_ABC::IsImpossible() const
{
    return nAddedPoints_ < 2;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::Cancel
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
inline
void DEC_PathSection_ABC::Cancel()
{
    bCanceled_ = true;
}
