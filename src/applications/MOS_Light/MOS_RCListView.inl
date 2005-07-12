//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCListView.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_RCListView.inl $
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// Name: MOS_RCListView::GetType
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
inline
E_ListViewType MOS_RCListView::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::SetOldPos
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
void MOS_RCListView::SetOldPos( const MT_Vector2D& vPos )
{
    vOldPos_ = vPos;
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::GetOldPos
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MOS_RCListView::GetOldPos()
{
    return vOldPos_;
}

