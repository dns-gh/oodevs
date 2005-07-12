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
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBArmeListViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_ODBArmeListViewItem.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::GetFirstAutomate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
QListViewItem* MOS_ODBArmeListViewItem::GetFirstAutomate() const
{
    if ( ODBAutomatePtrVector_.size() )
        return *ODBAutomatePtrVector_.begin();
    return 0;
}


//-----------------------------------------------------------------------------
// Name: std::MOS_ODBArmeListViewItem::GetName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
const std::string MOS_ODBArmeListViewItem::GetName() const
{
    return text(0);
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::GetType
// Created: FBD 03-07-25
//-----------------------------------------------------------------------------
inline
E_TypeArme MOS_ODBArmeListViewItem::GetTypeArme() const
{
    return nTypeArme_;
}


