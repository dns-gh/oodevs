// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationOptionChooser.h"
#include "moc_PopulationOptionChooser.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser constructor
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
PopulationOptionChooser::PopulationOptionChooser( QMainWindow* parent, kernel::Controllers& controllers )
    : QDockWindow( parent, "population-option-chooser" )
    , controllers_( controllers )
{
    setCaption( tools::translate( "gui::PopulationOptionChooser", "Population Display Options" ) );
    {
        QVBox* box = new QVBox( this );
        QButtonGroup* optionGroup = new QButtonGroup( box );
        QRadioButton* off = new QRadioButton( optionGroup );
        off->setText( tools::translate( "gui::PopulationOptionChooser", "Off" );
        QRadioButton* density = new QRadioButton( optionGroup );
        density->setText( tools::translate( "gui::PopulationOptionChooser", "Density" );
        QRadioButton* occupation = new QRadioButton( optionGroup );
        occupation->setText( tools::translate( "gui::PopulationOptionChooser", "Occupation" );
        connect( density, SIGNAL( toggled( bool ) ), this, OnDensityToggled( bool ) );
        connect( occupation, SIGNAL( toggled( bool ) ), this, OnOccupationToggled( bool ) );
    }
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Always );
    undock();
    hide();
    parent->setAppropriate( this, false );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser destructor
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
PopulationOptionChooser::~PopulationOptionChooser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OnOccupationToggled
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnOccupationToggled( bool mode )
{
    controllers_.options_.Change( "UrbanDensityColor", mode );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OnDensityToggled
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnDensityToggled( bool mode )
{
    if( mode )
    {
        // show widget
        // display pop on map
    }
    else
    {
        // disable widget
        // hide pop on map
    }
}
