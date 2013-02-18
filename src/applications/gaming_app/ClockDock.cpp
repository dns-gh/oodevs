// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
ClockDock::ClockDock( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation, ActionsScheduler& scheduler )
    : gui::RichDockWidget( controllers, parent, "clock" )
{
    setObjectName( "clock" );
    setWidget( new ClockWidget( this, controllers, simulation, scheduler ) );
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
