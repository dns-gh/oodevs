// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventMagicWidget.h"
#include "moc_EventMagicWidget.cpp"

// -----------------------------------------------------------------------------
// Name: EventMagicWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventMagicWidget::EventMagicWidget( gui::EventPresenter& presenter )
    : EventDefaultWidget_ABC( presenter )
{
    mainLayout_->addWidget( new QLabel( "Supervisor Widget" ) );
}

// -----------------------------------------------------------------------------
// Name: EventMagicWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventMagicWidget::~EventMagicWidget()
{
    // NOTHING
}
