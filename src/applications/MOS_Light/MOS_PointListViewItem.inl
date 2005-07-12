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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PointListViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_PointListViewItem.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::GetPoint
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MOS_PointListViewItem::GetPoint()
{
    return point_;
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::GetPos
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PointListViewItem::GetPos() const
{
    return point_;
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::GetSize
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
const MT_Vector2D MOS_PointListViewItem::GetSize() const
{
    return MT_Vector2D( rect_.GetWidth(), rect_.GetHeight() );
}
//-----------------------------------------------------------------------------
// Name: std::string
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
inline
const std::string& MOS_PointListViewItem::GetName() const
{
    return sMGRS_;
}

