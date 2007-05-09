// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Frames.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Frame::Frame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
Frame::Frame()
    : offset_( 0 ), count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Frames::Reset
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Frame::Reset( unsigned offset /*= 0*/ )
{
    offset_ = offset;
    count_ = 0;
}

// -----------------------------------------------------------------------------
// Name: KeyFrame::KeyFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
KeyFrame::KeyFrame()
    : offset_     ( 0 )
    , size_       ( 0 )
    , frameNumber_( 0 )
{
    // NOTHING
}
