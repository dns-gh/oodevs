// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventWidget_ABC.h"

// -----------------------------------------------------------------------------
// Name: EventWidget_ABC constructor
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
EventWidget_ABC::EventWidget_ABC( QWidget* parent /*= 0*/ )
    : QWidget( parent )
    , mainLayout_( 0 )
{
    mainLayout_ = new QVBoxLayout( this );
    mainLayout_->setMargin( 0 );
    mainLayout_->setSpacing( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventWidget_ABC destructor
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
EventWidget_ABC::~EventWidget_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventWidget_ABC::IsValid
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
bool EventWidget_ABC::IsValid() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: EventWidget_ABC::Trigger
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventWidget_ABC::Trigger() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventWidget_ABC::Purge
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventWidget_ABC::Purge()
{
    // NOTHING
}
