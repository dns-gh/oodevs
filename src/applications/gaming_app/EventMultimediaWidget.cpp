// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventMultimediaWidget.h"
#include "moc_EventMultimediaWidget.cpp"

// -----------------------------------------------------------------------------
// Name: EventMultimediaWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventMultimediaWidget::EventMultimediaWidget( gui::EventPresenter& presenter )
    : EventWidget_ABC< gui::EventView_ABC >( presenter )
{
    mainLayout_->addWidget( new QLabel( "Multimedia Widget" ) );
}

// -----------------------------------------------------------------------------
// Name: EventMultimediaWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventMultimediaWidget::~EventMultimediaWidget()
{
    // NOTHING
}
