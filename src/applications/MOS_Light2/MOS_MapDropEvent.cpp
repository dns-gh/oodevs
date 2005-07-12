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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapDropEvent.cpp $
// $Author: Ape $
// $Modtime: 3/09/04 11:24 $
// $Revision: 1 $
// $Workfile: MOS_MapDropEvent.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_MapDropEvent.h"


// -----------------------------------------------------------------------------
// Name: MOS_MapDropEvent constructor
/** @param  dropEvent 
    @param  vMapPos 
    */
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
MOS_MapDropEvent::MOS_MapDropEvent( const QDropEvent& dropEvent, const MT_Vector2D& vMapPos )
    : QDropEvent( dropEvent )
    , vMapPos_  ( vMapPos )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MapDropEvent destructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
MOS_MapDropEvent::~MOS_MapDropEvent()
{
}
