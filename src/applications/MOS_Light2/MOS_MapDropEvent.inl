// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-02 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapDropEvent.inl $
// $Author: Ape $
// $Modtime: 2/09/04 18:51 $
// $Revision: 1 $
// $Workfile: MOS_MapDropEvent.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_MapDropEvent::GetMapPos
/** @return 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_MapDropEvent::GetMapPos() const
{
    return vMapPos_;
}
