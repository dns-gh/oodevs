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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBCampListViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_ODBCampListViewItem.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::GetFirstArme
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
QListViewItem* MOS_ODBCampListViewItem::GetFirstArme() const
{
    if ( armePtrMap_.size() )
        return armePtrMap_.begin()->second;
    return 0;
}


//-----------------------------------------------------------------------------
// Name: std::MOS_ODBCampListViewItem::GetName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
const std::string MOS_ODBCampListViewItem::GetName() const
{
    return text(0);
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::GetType
// Created: FBD 03-07-25
//-----------------------------------------------------------------------------
inline
E_TypeCamp MOS_ODBCampListViewItem::GetTypeCamp() const
{
    return nTypeCamp_;
}


