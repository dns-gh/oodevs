//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitMission.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 5 $
// $Workfile: MOS_LimitMission.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::GetLimitListView
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MOS_LimitListView& MOS_LimitMission::GetLimitListView() const
{
    return *pLimitListView_;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::SetWidgetTab
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
inline
void MOS_LimitMission::SetWidgetTab( QWidget* /*pWidget*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::NotifyAgentHasMoved
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
inline
void MOS_LimitMission::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{
}

