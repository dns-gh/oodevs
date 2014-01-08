// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventSupervisorActionWidget.h"
#include "moc_EventSupervisorActionWidget.cpp"

// -----------------------------------------------------------------------------
// Name: EventSupervisorActionWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventSupervisorActionWidget::EventSupervisorActionWidget( gui::EventPresenter& presenter )
    : EventDefaultWidget_ABC( presenter )
{
    mainLayout_->addWidget( new QLabel( "Supervisor Widget" ) );
}

// -----------------------------------------------------------------------------
// Name: EventSupervisorActionWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventSupervisorActionWidget::~EventSupervisorActionWidget()
{
    // NOTHING
}
