// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "OrbatPanel.h"
#include "moc_OrbatPanel.cpp"
#include "OptionWidgets.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: OrbatPanel constructor
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
OrbatPanel::OrbatPanel( QWidget* parent, kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "OrbatPanel" )
{
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    {
        QGroupBox* box = new QGroupBox( tr( "Operational state color" ) );
        QVBoxLayout* layout = new QVBoxLayout( box );
        layout->addWidget( tools::AddLabeledWidget( tr( "Neutralized:" ),
                           new OptionColorButton( options, "neutralized_color_button", "Color/Neutralized" ) ) );
        layout->addWidget( tools::AddLabeledWidget( tr( "Tactically destroyed:" ),
                           new OptionColorButton( options, "tactically_destroyed_color_button", "Color/TacticallyDestroyed" ) ) );
        layout->addWidget( tools::AddLabeledWidget( tr( "Totally destroyed:" ),
                           new OptionColorButton( options, "totally_destroyed_color_button", "Color/TotallyDestroyed" ) ) );
        mainLayout->addWidget( box );
    }
    {
        QGroupBox* box = new QGroupBox( tr( "Crowds health state color" ) );
        QVBoxLayout* layout = new QVBoxLayout( box );
        layout->addWidget( tools::AddLabeledWidget( tr( "Healthy:" ),
                           new OptionColorButton( options, "healthy_color_button", "Color/Healthy" ) ) );
        layout->addWidget( tools::AddLabeledWidget( tr( "Contaminated:" ),
                           new OptionColorButton( options, "contaminated_color_button", "Color/Contaminated" ) ) );
        layout->addWidget( tools::AddLabeledWidget( tr( "Wounded:" ),
                           new OptionColorButton( options, "wounded_color_button", "Color/Wounded" ) ) );
        layout->addWidget( tools::AddLabeledWidget( tr( "Dead:" ),
                           new OptionColorButton( options, "dead_color_button", "Color/Dead" ) ) );
        layout->addWidget( tools::AddLabeledWidget( tr( "Mostly healthy:" ),
                           new OptionColorButton( options, "mostly_healthy_color_button", "Color/MostlyHealthy" ) ) );
        mainLayout->addWidget( box );
    }
    mainLayout->addStretch( 1 );
    setLayout( mainLayout );
}

// -----------------------------------------------------------------------------
// Name: OrbatPanel destructor
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
OrbatPanel::~OrbatPanel()
{
    // NOTHING
}
