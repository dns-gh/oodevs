//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceListView.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_TraceListView.inl $
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::GetType
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
inline
E_ListViewType MOS_TraceListView::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::SetOldPos
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
void MOS_TraceListView::SetOldPos( const MT_Vector2D& vPos )
{
    vOldPos_ = vPos;
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::GetOldPos
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MOS_TraceListView::GetOldPos()
{
    return vOldPos_;
}

