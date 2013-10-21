// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichBaseObject.h"
#include "moc_RichBaseObject.cpp"
#include "RichWarnWidget_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichBaseObject::RichBaseObject
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
RichBaseObject::RichBaseObject( RichWarnWidget_ABC& widget, const QString& objectName, QObject* parent )
    : QObject( parent )
    , widget_( widget )
{
    setObjectName( objectName );
}

// -----------------------------------------------------------------------------
// Name: RichBaseObject::~RichBaseObject
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
RichBaseObject::~RichBaseObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichBaseObject::OnBlink
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
void RichBaseObject::OnBlink()
{
    widget_.OnBlink();
}

// -----------------------------------------------------------------------------
// Name: RichBaseObject::OnDone
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
void RichBaseObject::OnDone()
{
    widget_.OnDone();
}
