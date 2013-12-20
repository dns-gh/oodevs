// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventReportWidget.h"
#include "moc_EventReportWidget.cpp"

// -----------------------------------------------------------------------------
// Name: EventReportWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventReportWidget::EventReportWidget( gui::EventPresenter& presenter )
    : EventDefaultWidget_ABC( presenter )
{
    mainLayout_->addWidget( new QLabel( "Report Widget" ) );
}

// -----------------------------------------------------------------------------
// Name: EventReportWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventReportWidget::~EventReportWidget()
{
    // NOTHING
}
