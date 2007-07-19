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
TimelineItem_ABC::TimelineItem_ABC( QCanvas* canvas )
    : QCanvasRectangle( canvas )
    , overlayed_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC constructor
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
TimelineItem_ABC::TimelineItem_ABC( QCanvas* canvas, const QRect& area )
    : QCanvasRectangle( area, canvas )
    , overlayed_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC destructor
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
TimelineItem_ABC::~TimelineItem_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC::Shift
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineItem_ABC::Shift( long )
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
// Name: TimelineItem_ABC::SetOverlayed
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineItem_ABC::SetOverlayed( bool overlayed )
{
    overlayed_ = overlayed;
}

// -----------------------------------------------------------------------------
// Name: TimelineItem_ABC::IsOverlayed
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
bool TimelineItem_ABC::IsOverlayed() const
{
    return overlayed_;
}
