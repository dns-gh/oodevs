//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_SegmentPos.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_MTH_SegmentPos.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::IsModifiedAgent
// Created: FBD 03-01-16
//-----------------------------------------------------------------------------
inline
bool MOS_MTH_SegmentPos::IsModifiedAgent() const
{
    return bModifiedAgent_;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::SetFirstWidget
// Created: FBD 03-04-28
//-----------------------------------------------------------------------------
inline
void MOS_MTH_SegmentPos::SetFirstWidget( bool bFirstWidget )
{
    bFirstWidget_ = bFirstWidget;
}