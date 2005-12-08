//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LineManager.inl $
// $Author: Ape $
// $Modtime: 14/04/04 18:52 $
// $Revision: 2 $
// $Workfile: MOS_LineManager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::GetLineList
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MOS_LineManager::CT_TacticalLineSet& MOS_LineManager::GetLineList()
{
    return lineSet_;
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::DeleteAll
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
inline
void MOS_LineManager::DeleteAll()
{
    for( IT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end() ; ++it )
        delete *it;
    lineSet_.clear();
}

// -----------------------------------------------------------------------------
// Name: MOS_LineManager::NeedSaving
// Created: SBO 2005-12-08
// -----------------------------------------------------------------------------
inline
bool MOS_LineManager::NeedSaving()
{
    for( CIT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end(); ++it )
        if( (*it)->IsCreatedByMos() )
            return true;
    return !bUseSimTacticalLines_;
}
