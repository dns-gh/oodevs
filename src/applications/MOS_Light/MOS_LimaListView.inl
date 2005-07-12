//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaListView.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 6 $
// $Workfile: MOS_LimaListView.inl $
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::GetType
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
inline
E_ListViewType MOS_LimaListView::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SetOldPos
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
void MOS_LimaListView::SetOldPos( const MT_Vector2D& vPos )
{
    vOldPos_ = vPos;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::GetOldPos
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MOS_LimaListView::GetOldPos()
{
    return vOldPos_;
}


