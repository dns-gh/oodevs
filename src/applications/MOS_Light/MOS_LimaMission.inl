//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaMission.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_LimaMission.inl $
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::GetLimaListView
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MOS_LimaListView& MOS_LimaMission::GetLimaListView() const
{
    return *pLimaListView_;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::SetWidgetTab
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
inline
void MOS_LimaMission::SetWidgetTab( QWidget* /*pWidget*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::NotifyAgentHasMoved
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
inline
void MOS_LimaMission::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{
}

