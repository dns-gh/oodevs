// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GridPanel.h"
#include "moc_GridPanel.cpp"
#include "SubObjectName.h"
#include "OptionWidgets.h"
#include "Tools.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GridPanel constructor
// Created: JSR 2014-11-19
// -----------------------------------------------------------------------------
GridPanel::GridPanel( QWidget* parent, kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "GridPanel" )
    , options_( options )
{
    SubObjectName subObject( "GridPanel" );

    auto enabled = new OptionCheckBox( options, "enabled", "Grid/Enabled", tr( "Show grid" ) );
    auto color = new OptionColorButton( options, "color", "Grid/Color" );
    auto type = new OptionComboBox< int >( options, "gridType", "Grid/Type" );
    type->AddItem( QString::fromStdString( ENT_Tr::ConvertFromCoordinateSystem( eCoordinateSystem_Local ) ), eCoordinateSystem_Local );
    type->AddItem( QString::fromStdString( ENT_Tr::ConvertFromCoordinateSystem( eCoordinateSystem_Mgrs ) ), eCoordinateSystem_Mgrs );
    auto size = new OptionComboBox< float >( options, "gridSize", "Grid/Size" );
    size->AddItem( tr( "100m" ), 0.1f );
    size->AddItem( tr( "250m" ), 0.25f );
    size->AddItem( tr( "500m" ), 0.5f );
    size->AddItem( tr( "1km" ), 1.0f );
    size->AddItem( tr( "2.5km" ), 2.5f );
    size->AddItem( tr( "5km" ), 5.0f );
    size->AddItem( tr( "10km" ), 10.0f );
    size->AddItem( tr( "100km" ), 100.0f );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( enabled );
    layout->addWidget( tools::AddLabeledWidget( tr( "Color" ), color ) );
    layout->addWidget( tools::AddLabeledWidget( tr( "Type" ),  type  ) );
    layout->addWidget( tools::AddLabeledWidget( tr( "Size" ),  size  ) );
    layout->addStretch( 1 );
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: GridPanel destructor
// Created: JSR 2014-11-19
// -----------------------------------------------------------------------------
GridPanel::~GridPanel()
{
    // NOTHING
}
