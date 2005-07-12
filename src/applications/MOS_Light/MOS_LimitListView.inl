//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitListView.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 5 $
// $Workfile: MOS_LimitListView.inl $
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::GetType
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
inline
E_ListViewType MOS_LimitListView::GetType() const
{
    return nType_;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SetOldPos
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
void MOS_LimitListView::SetOldPos( const MT_Vector2D& vPos )
{
    vOldPos_ = vPos;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::GetOldPos
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MOS_LimitListView::GetOldPos()
{
    return vOldPos_;
}

