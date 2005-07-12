//*****************************************************************************
// 
// $Created: FBD 03-01-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ValueList.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 6 $
// $Workfile: MOS_ValueList.inl $
// 
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::GetAgent
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
inline
MOS_Agent* MOS_ValueList::GetAgent() const
{
    return pAgent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::SetAgent
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
inline
void MOS_ValueList::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;

    for( IT_ValuePtrVector itValue = valueVector_.begin() ; itValue != valueVector_.end() ; ++itValue )
    {
        (*itValue)->SetAgent( pAgent );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::SetWidgetTab
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
inline
void MOS_ValueList::SetWidgetTab( QWidget* /*pWidget*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::SetMission
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
inline
void MOS_ValueList::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::NotifyAgentHasMoved
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
inline
void MOS_ValueList::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{

}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::SetName
// Created: FBD 03-02-06
//-----------------------------------------------------------------------------
inline
void MOS_ValueList::SetName( const std::string& sName )
{
    sName_ = sName;
}

