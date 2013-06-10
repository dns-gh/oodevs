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
#include "ENT/ENT_Tr.h"
#include "gaming/Event.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventSupervisorActionWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventSupervisorActionWidget::EventSupervisorActionWidget()
    : EventWidget_ABC()
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

// -----------------------------------------------------------------------------
// Name: EventSupervisorActionWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventSupervisorActionWidget::Fill( const Event& /*event*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: EventSupervisorActionWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventSupervisorActionWidget::Commit( timeline::Event& event ) const
{
    event.action.target = "sword://sim";
}
