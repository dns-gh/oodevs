// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-01 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapMouseEvent.inl $
// $Author: Ape $
// $Modtime: 21/09/04 18:35 $
// $Revision: 2 $
// $Workfile: MOS_MapMouseEvent.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_MapMouseEvent::GetMapPos
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_MapMouseEvent::GetMapPos() const
{
    return vMapPos_;
}


// -----------------------------------------------------------------------------
// Name: MOS_MapMouseEvent::GetDistancePerPixel
/** @return 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
inline
const float MOS_MapMouseEvent::GetDistancePerPixel() const
{
    return rDistancePerPixel_;
}


