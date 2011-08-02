// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineItem_ABC.h"

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC constructor
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
TimelineItem_ABC::TimelineItem_ABC( Q3Canvas* canvas )
    : Q3CanvasRectangle( canvas )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC destructor
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
TimelineItem_ABC::~TimelineItem_ABC()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC::Move
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineItem_ABC::Move( long )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC::Delete
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
void TimelineItem_ABC::Delete()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC::DisplayToolTip
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineItem_ABC::DisplayToolTip( QWidget* ) const
{
   // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC::DisplayContextMenu
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineItem_ABC::DisplayContextMenu( Q3PopupMenu* ) const
{
    // NOTHING
}
