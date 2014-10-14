// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RefreshRatePanel.h"
#include "moc_RefreshRatePanel.cpp"
#include "OptionWidgets.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RefreshRatePanel constructor
// Created: LDC 2014-02-21
// -----------------------------------------------------------------------------
RefreshRatePanel::RefreshRatePanel( QWidget* parent, kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "RefreshRatePanel" )
{
    auto spinbox = new OptionSpinBox( options, "RefreshRateSpinBox", "RefreshRate", 10, 1000 );
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( tools::AddLabeledWidget( tr( "Select refresh rate (in ms):" ), spinbox, 0, 0 ) );
    layout->addStretch( 1 );
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: RefreshRatePanel destructor
// Created: LDC 2014-02-21
// -----------------------------------------------------------------------------
RefreshRatePanel::~RefreshRatePanel()
{
    // NOTHING
}
