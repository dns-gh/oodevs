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
// $Archive: /MVW_v10/Build/SDK/Light2/src/LineManager.inl $
// $Author: Ape $
// $Modtime: 14/04/04 18:52 $
// $Revision: 2 $
// $Workfile: LineManager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: LineManager::GetLineList
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
LineManager::CT_TacticalLineSet& LineManager::GetLineList()
{
    return lineSet_;
}


//-----------------------------------------------------------------------------
// Name: LineManager::DeleteAll
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
inline
void LineManager::DeleteAll()
{
    for( IT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end() ; ++it )
        delete *it;
    lineSet_.clear();
}

// -----------------------------------------------------------------------------
// Name: LineManager::NeedSaving
// Created: SBO 2005-12-08
// -----------------------------------------------------------------------------
inline
bool LineManager::NeedSaving()
{
    for( CIT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end(); ++it )
        if( (*it)->IsCreatedByMos() )
            return true;
    return !bUseSimTacticalLines_;
}
