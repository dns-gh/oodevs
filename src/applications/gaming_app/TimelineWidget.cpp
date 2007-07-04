// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineWidget.h"
#include "TimelineEditor.h"
#include <qcanvas.h>

// -----------------------------------------------------------------------------
// Name: TimelineWidget constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineWidget::TimelineWidget( QWidget* parent, kernel::Controllers& controllers, ActionsModel& actions )
    : QHBox( parent )
{
    QCanvas* canvas = new QCanvas( 10000, 200 );
    canvas->setUpdatePeriod( 1 );
    canvas->setDoubleBuffering( true );
    new TimelineEditor( parent, canvas, controllers, actions );
}

// -----------------------------------------------------------------------------
// Name: TimelineWidget destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineWidget::~TimelineWidget()
{
    // NOTHING
}
