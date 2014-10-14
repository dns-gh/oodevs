// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "OrbatPanel.h"
#include "moc_OrbatPanel.cpp"
#include "clients_gui/OptionWidgets.h"
#include "clients_gui/Tools.h"

using namespace preparation;

// -----------------------------------------------------------------------------
// Name: OrbatPanel constructor
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
OrbatPanel::OrbatPanel( QWidget* parent, kernel::OptionsController& options )
    : gui::PreferencePanel_ABC( parent, "OrbatPanel" )
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( tools::AddLabeledWidget( tr( "Ghost:" ),
                       new gui::OptionColorButton( options, "ghost_color_button", "Color/Phantom" ) ) );
    layout->addStretch( 1 );
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: OrbatPanel destructor
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
OrbatPanel::~OrbatPanel()
{
    // NOTHING
}
