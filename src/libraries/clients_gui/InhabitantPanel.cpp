// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InhabitantPanel.h"
#include "moc_InhabitantPanel.cpp"
#include "DensityWidget.h"
#include "RichGroupBox.h"
#include "SubObjectName.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: InhabitantPanel constructor
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
InhabitantPanel::InhabitantPanel( QWidget* parent, kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "InhabitantPanel" )
{
    SubObjectName subObject( "InhabitantPanel" );
    RichGroupBox* densityGroup = new RichGroupBox( "densityGroup", tr( "Density Gradient map" ) );
    QHBoxLayout* densityLayout = new QHBoxLayout( densityGroup );
    densityLayout->addWidget( new DensityWidget( options, "densityGroup", "Density" ) );

    RichGroupBox* occupationGroup = new RichGroupBox( "occupationGroup", tr( "Occupation Gradient map" ) );
    QHBoxLayout* occupationLayout = new QHBoxLayout( occupationGroup );
    occupationLayout->addWidget( new DensityWidget( options, "occupationWidget", "Accommodation" ) );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( densityGroup );
    layout->addWidget( occupationGroup );
    layout->addStretch( 1 );
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel destructor
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
InhabitantPanel::~InhabitantPanel()
{
    // NOTHING
}
