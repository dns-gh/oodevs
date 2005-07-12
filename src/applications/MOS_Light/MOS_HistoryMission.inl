//*****************************************************************************
//
// $Created: FBD 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_HistoryMission.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_HistoryMission.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: std::string
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
inline
const std::string& MOS_HistoryMission::GetName() const
{
    return sName_;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
inline
const std::string& MOS_HistoryMission::GetParam() const
{
    return sParam_;
}

//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::GetTimeStart
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
inline
uint MOS_HistoryMission::GetTimeStart() const
{
    return nTimeStart_;
}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::GetTimeEnd
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
inline
uint MOS_HistoryMission::GetTimeEnd() const
{
    return nTimeEnd_;
}



//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::SetTimeEnd
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
inline
void MOS_HistoryMission::SetTimeEnd( uint nTime )
{
    nTimeEnd_ = nTime;
}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::GetColor
// Created: FBD 03-08-26
//-----------------------------------------------------------------------------
inline
uint MOS_HistoryMission::GetColor() const
{
    return nColor_;
}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::GetRect
// Created: FBD 03-08-27
//-----------------------------------------------------------------------------
inline
QRect& MOS_HistoryMission::GetRect()
{
    return rect_;
}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::SetRect
// Created: FBD 03-08-27
//-----------------------------------------------------------------------------
inline
void MOS_HistoryMission::SetRect( QRect& rect )
{
    rect_ = rect;
}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::SetLastOrderState
// Created: FBD 03-09-02
//-----------------------------------------------------------------------------
inline
void MOS_HistoryMission::SetOrderState( E_StateMissionHistory nState, uint nTime )
{
    nState_ = nState;
    if ( nState_ == eMissionCanceled || nState == eMissionStoped || nState == eMissionError )
        nTimeEnd_ = nTime;
    else
    if ( nState_ == eMissionStarted )
        nTimeStart_ = nTime;
}




//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::GetStateOrder
// Created: FBD 03-09-02
//-----------------------------------------------------------------------------
inline
E_StateMissionHistory MOS_HistoryMission::GetStateOrder() const
{
    return nState_;
}



//-----------------------------------------------------------------------------
// Name: MOS_HistoryMission::GetColorState
// Created: FBD 03-09-02
//-----------------------------------------------------------------------------
inline
uint MOS_HistoryMission::GetColorState() const
{
    if ( nState_ == eMissionError )
        return 0;
    else
    if ( nState_ == eMissionSended )
        return 64;
    else 
        return 128;
}

