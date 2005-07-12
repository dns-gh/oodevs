//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MagicPos.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 5 $
// $Workfile: MOS_MagicPos.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::IsModifiedAgent
// Created: FBD 03-01-16
//-----------------------------------------------------------------------------
inline
bool MOS_MagicPos::IsModifiedAgent() const
{
    return bModifiedAgent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::SetMission
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
inline
void MOS_MagicPos::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::SetAttrEditor
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
inline
void MOS_MagicPos::SetAttrEditor( MOS_AttrEditor* pAttrEditor )
{
    pAttrEditor_ = pAttrEditor;
}

//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
inline
void MOS_MagicPos::SetWidgetTab( QWidget* pWidget )
{
    pWidgetTab_ = pWidget;
}
