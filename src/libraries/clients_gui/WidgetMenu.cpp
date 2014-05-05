// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "WidgetMenu.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: WidgetMenu constructor
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
WidgetMenu::WidgetMenu( QWidget* parent )
    : QMenu( parent )
{
    QWidget* widget = new QWidget();
    layout_ = new QVBoxLayout( widget );

    QWidgetAction* action = new QWidgetAction( this );
    action->setDefaultWidget( widget );
    addAction( action );
}

// -----------------------------------------------------------------------------
// Name: WidgetMenu destructor
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
WidgetMenu::~WidgetMenu()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WidgetMenu::AddWidget
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
void WidgetMenu::AddWidget( QWidget* widget )
{
    layout_->addWidget( widget );
}
