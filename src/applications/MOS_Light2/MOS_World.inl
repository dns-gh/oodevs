//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_World.inl $
// $Author: Ape $
// $Modtime: 21/07/04 9:58 $
// $Revision: 2 $
// $Workfile: MOS_World.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_World::GetPatchFromIdx
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_Patch* MOS_World::GetPatchFromIdx( int nIdxPatchX, int nIdxPatchY ) const
{
    if( nIdxPatchX < 0 || nIdxPatchY < 0 )
        return 0;

    uint nIdx = nIdxPatchX + ( nIdxPatchY *  nNbrPatchsX_ );
    if( nIdx >= patchVector_.size() )
        return 0;
    return patchVector_[ nIdx ];
}


//-----------------------------------------------------------------------------
// Name: MOS_World::GetRect
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_INLINE
const MT_Rect& MOS_World::GetRect() const
{
    return rect_;
}


//-----------------------------------------------------------------------------
// Name: MOS_World::GetExtent
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
MOS_INLINE
const MOS_Extent& MOS_World::GetExtent() const
{
    return extent_;
}
