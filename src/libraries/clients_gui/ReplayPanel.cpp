// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ReplayPanel.h"
#include "moc_ReplayPanel.cpp"
#include "OptionWidgets.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ReplayPanel constructor
// Created: SLI 2014-06-06
// -----------------------------------------------------------------------------
ReplayPanel::ReplayPanel( QWidget* parent, kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "ReplayPanel" )
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( new OptionCheckBox( options, "ReplayPanelCheckBox", "Replay/SmoothPositions", tr( "Set smooth movement" ) ) );
    layout->addStretch( 1 );
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: ReplayPanel destructor
// Created: SLI 2014-06-06
// -----------------------------------------------------------------------------
ReplayPanel::~ReplayPanel()
{
    // NOTHING
}
