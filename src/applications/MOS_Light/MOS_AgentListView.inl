//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentListView.inl $
// $Author: Nld $
// $Modtime: 14/02/05 14:27 $
// $Revision: 21 $
// $Workfile: MOS_AgentListView.inl $
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GetType
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
inline
E_ListViewType MOS_AgentListView::GetType() const
{
    return nType_;
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawItineraire
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawItineraire()  const
{
    return bItineraire_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawAltitude
// Created: FBD 03-04-25
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawAltitude() const
{
    return bDrawAltitude_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawAllItineraire
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawAllItineraire()  const
{
    return bAllItineraire_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawAllLima
// Created: FBD 03-04-28
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawAllLima()  const
{
    return bAllLima_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawAllLimit
// Created: FBD 03-04-28
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawAllLimit()  const
{
    return bAllLimit_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawAllVision
// Created: FBD 03-02-12
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawAllVision()  const
{
    return bAllVision_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawAllVision
// Created: FBD 03-02-12
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsOptiDraw()  const
{
    return bOptiDraw_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawAllCone
// Created: JVT 03-04-08
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawAllCone() const
{
    return bAllCone_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawOldPath
// Created: APE 04-02-02
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawOldPath() const
{
    return bDrawOldPath_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsDrawText
// Created: APE 04-02-02
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsDrawText() const
{
    return bDrawText_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetDrawLimit
// Created: FBD 03-04-28
//-----------------------------------------------------------------------------
inline
void MOS_AgentListView::SlotSetDrawLimit( bool bAllLimit )
{
    bAllLimit_ = bAllLimit;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetAllLima
// Created: FBD 03-04-28
//-----------------------------------------------------------------------------
inline
void MOS_AgentListView::SlotSetDrawLima( bool bAllLima )
{
    bAllLima_ = bAllLima;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetDrawText
// Created: APE 04-02-02
//-----------------------------------------------------------------------------
inline
void MOS_AgentListView::SlotSetDrawText( bool bDrawText )
{
    bDrawText_ = bDrawText;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetScrollTrace
// Created: APE 04-02-02
//-----------------------------------------------------------------------------
inline
void MOS_AgentListView::SlotSetScrollTrace( bool bScrollTrace )
{
    bScrollTrace_ = bScrollTrace;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::MustSaveCoordinates
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::MustSaveCoordinates()  const
{
    return bPointEdition_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsEmpty
// Created: AGN 03-07-22
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsEmpty() const
{
    return agentVector_.empty();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsScrollTrace
// Created: APE
//-----------------------------------------------------------------------------
inline
bool MOS_AgentListView::IsScrollTrace() const
{
    return bScrollTrace_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GetListAgent
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
inline
MOS_AgentListView::T_AgentPtrVector& MOS_AgentListView::GetListAgent()
{
    return agentVector_;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::SetDrawVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
inline
void MOS_AgentListView::SetDrawVisionCones( bool bEnable )
{
    bAllCone_ = bEnable;
}
