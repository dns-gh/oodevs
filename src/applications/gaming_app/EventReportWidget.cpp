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
#include "ENT/ENT_Tr.h"
#include <timeline/api.h>

// -----------------------------------------------------------------------------
// Name: EventReportWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventReportWidget::EventReportWidget()
    : EventWidget_ABC()
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

// -----------------------------------------------------------------------------
// Name: EventReportWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventReportWidget::Fill( const Event& /*event*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: EventReportWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventReportWidget::Commit( timeline::Event& event ) const
{
    event.action.target = "sword://client";
}
