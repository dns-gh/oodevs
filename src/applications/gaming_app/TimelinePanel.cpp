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
#include "ActionsToolbar.h"
#include "TimelineWidget.h"
#include "gaming/Tools.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: TimelinePanel constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelinePanel::TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, ActionsModel& model, ActionsScheduler& scheduler, const tools::ExerciseConfig& config, gui::ItemFactory_ABC& factory )
    : QDockWindow( parent, "timeline" )
{
    setResizeEnabled( true );
    setNewLine( true );
    setCaption( tools::translate( "TimelinePanel", "Actions timeline" ) );
    setCloseMode( QDockWindow::Always );
    QVBox* box = new QVBox( this );
    new ActionsToolbar( box, model, scheduler, config, controllers );
    new TimelineWidget( box, controllers, model, scheduler, factory );
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
