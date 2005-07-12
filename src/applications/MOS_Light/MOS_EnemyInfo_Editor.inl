//*****************************************************************************
//
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_Editor.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_EnemyInfo_Editor.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Editor::IsEditing
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
inline
bool MOS_EnemyInfo_Editor::IsEditing() const
{
    return pEditedInfo_ != 0;
}

