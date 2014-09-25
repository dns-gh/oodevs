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
InhabitantPanel::InhabitantPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "InhabitantPanel" )
{
    SubObjectName subObject( "InhabitantPanel" );
    RichGroupBox* densityGroup = new RichGroupBox( "densityGroup", tr( "Density Gradient map" ) );
    QHBoxLayout* densityGroupLayout = new QHBoxLayout( densityGroup );
    DensityWidget* densityWidget = new DensityWidget( "densityGroup", densityGroup, controllers, "Density" );
    densityGroupLayout->addWidget( densityWidget );

    RichGroupBox* occupationGroup = new RichGroupBox( "occupationGroup", tr( "Occupation Gradient map" ) );
    QHBoxLayout* occupationGroupLayout = new QHBoxLayout( occupationGroup );
    DensityWidget* occupationWidget = new DensityWidget( "occupationWidget", occupationGroup, controllers, "Accommodation" );
    occupationGroupLayout->addWidget( occupationWidget );

    RichGroupBox* box = new RichGroupBox( "colorsBox", tr( "Colors" ), this );
    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->addWidget( densityGroup );
    boxLayout->addWidget( occupationGroup );
    boxLayout->addStretch( 1 );
    setWidget( box );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel destructor
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
InhabitantPanel::~InhabitantPanel()
{
    // NOTHING
}
