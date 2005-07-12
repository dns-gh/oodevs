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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LineManager.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_LineManager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::GetLimaList
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MOS_LineManager::CT_LimaSet& MOS_LineManager::GetLimaList()
{
    return limaSet_;
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::GetLimitList
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MOS_LineManager::CT_LimitSet& MOS_LineManager::GetLimitList()
{
    return limitSet_;
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RemoveAll
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
inline
void MOS_LineManager::RemoveAll()
{
    RemoveAllLimas ();
    RemoveAllLimits();
}

