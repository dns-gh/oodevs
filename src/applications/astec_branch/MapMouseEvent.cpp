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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapMouseEvent.cpp $
// $Author: Ape $
// $Modtime: 21/09/04 18:35 $
// $Revision: 2 $
// $Workfile: MapMouseEvent.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "MapMouseEvent.h"


// -----------------------------------------------------------------------------
// Name: MapMouseEvent constructor
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
MapMouseEvent::MapMouseEvent( const QMouseEvent& mouseEvent, const MT_Vector2D& vMapPos, float rDistancePerPixel )
    : QMouseEvent       ( mouseEvent.type(), mouseEvent.pos(), mouseEvent.button(), mouseEvent.state() )
    , vMapPos_          ( vMapPos )
    ,rDistancePerPixel_ ( rDistancePerPixel )
{
}


// -----------------------------------------------------------------------------
// Name: MapMouseEvent destructor
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
MapMouseEvent::~MapMouseEvent()
{
}
