// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelinePanel.h"
#include "TimelineWidget.h"

// -----------------------------------------------------------------------------
// Name: TimelinePanel constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelinePanel::TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, ActionsModel& actions )
    : QDockWindow( parent )
{
    setResizeEnabled( true );
    setCaption( tr( "Actions timeline" ) );
    setCloseMode( QDockWindow::Always );
    QVBox* box = new QVBox( this );
    new TimelineWidget( box, controllers, actions );
    setWidget( box );
}

// -----------------------------------------------------------------------------
// Name: TimelinePanel destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelinePanel::~TimelinePanel()
{
    // NOTHING
}
