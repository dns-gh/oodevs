// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ClockDock.h"
#include "ClockWidget.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: ClockDock constructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockDock::ClockDock( QWidget* parent, kernel::Controllers& controllers, const kernel::Time_ABC& simulation, ActionsScheduler& scheduler )
    : gui::RichDockWidget( controllers, parent, "clock" )
{
    setObjectName( "clock" );
    clockWidget_ = new ClockWidget( this, controllers, simulation, scheduler );
    setWidget( clockWidget_ );
    setCaption( tools::translate( "ClockDock","Clock" ) );
}

// -----------------------------------------------------------------------------
// Name: ClockDock destructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockDock::~ClockDock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClockDock::NotifyModeChanged
// Created: NPT 2013-02-28
// -----------------------------------------------------------------------------
void ClockDock::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    RichDockWidget::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
    clockWidget_->SetAlarmVisible( newMode != eModes_Replay );
}
