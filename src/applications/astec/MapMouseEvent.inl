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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapMouseEvent.inl $
// $Author: Ape $
// $Modtime: 21/09/04 18:35 $
// $Revision: 2 $
// $Workfile: MapMouseEvent.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MapMouseEvent::GetMapPos
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MapMouseEvent::GetMapPos() const
{
    return vMapPos_;
}


// -----------------------------------------------------------------------------
// Name: MapMouseEvent::GetDistancePerPixel
/** @return 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
inline
const float MapMouseEvent::GetDistancePerPixel() const
{
    return rDistancePerPixel_;
}


