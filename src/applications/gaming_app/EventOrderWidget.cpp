// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventOrderWidget.h"
#include "ENT/ENT_Tr.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventOrderWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::EventOrderWidget()
    : EventWidget_ABC()
{
    mainLayout_->addWidget( new QLabel( "Order Widget" ) );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::~EventOrderWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventOrderWidget::Fill( const Event& /*event*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventOrderWidget::Commit( timeline::Event& event ) const
{
    event.action.target = "sword://sim";
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::IsValid
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
bool EventOrderWidget::IsValid() const
{
    // Check if missions parameters are all filled
    return true;
}
