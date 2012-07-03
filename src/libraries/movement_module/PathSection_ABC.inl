//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Path/PathSection_ABC.inl $
// $Author: Nld $
// $Modtime: 22/02/05 14:32 $
// $Revision: 1 $
// $Workfile: PathSection_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::GetPosStart
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PathSection_ABC::GetPosStart() const
{
    return startPoint_;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::GetPosEnd
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PathSection_ABC::GetPosEnd() const
{
    return endPoint_;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::IsImpossible
// Created: AGE 2005-03-04
// -----------------------------------------------------------------------------
inline
bool PathSection_ABC::IsImpossible() const
{
    return nAddedPoints_ < 2;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::Cancel
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
inline
void PathSection_ABC::Cancel()
{
    bCanceled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
inline
double PathSection_ABC::GetLength() const
{
    return startPoint_.Distance( endPoint_ );
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::GetPath
// Created: CMA 2011-04-18
// -----------------------------------------------------------------------------
inline
const Path_ABC& PathSection_ABC::GetPath() const
{
    return path_;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::SetPosStart
// Created: CMA 2011-04-18
// -----------------------------------------------------------------------------
inline
void PathSection_ABC::SetPosStart( const MT_Vector2D& point )
{
    startPoint_ = point;
}
