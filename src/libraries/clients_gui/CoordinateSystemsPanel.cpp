// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CoordinateSystemsPanel.h"
#include "moc_CoordinateSystemsPanel.cpp"

#include "RichGroupBox.h"
#include "RichWidget.h"
#include "SignalAdapter.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel constructor
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
CoordinateSystemsPanel::CoordinateSystemsPanel( QWidget* parent,
                                                kernel::OptionsController& options,
                                                kernel::CoordinateConverter_ABC& coordConverter )
    : PreferencePanel_ABC( parent, "CoordinateSystemsPanel" )
    , coordConverter_( coordConverter )
{
    QLabel* coordinateLabel = new QLabel( tr( "Select current coordinate system:" ) );
    coordSysComboBox_ = new RichWidget< QComboBox >( "listCoordSys" );
    coordSysComboBox_->setEditable( false );

    for( int i = 0; i < eNbrCoordinateSystem; ++i )
        coordSysComboBox_->insertItem( QString::fromStdString( ENT_Tr::ConvertFromCoordinateSystem( static_cast< E_CoordinateSystem >( i ) ) ), i );

    gui::connect( coordSysComboBox_, SIGNAL( currentIndexChanged( int ) ), [&]{
        coordConverter_.SetDefaultCoordinateSystem( static_cast< E_CoordinateSystem >( coordSysComboBox_->currentItem() ) );
        options.Change( "CoordSystem", static_cast< int >( coordConverter_.GetDefaultCoordinateSystem() ) );
    } );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( coordinateLabel );
    layout->addWidget( coordSysComboBox_ );
    layout->addStretch( 1 );

    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel destructor
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
CoordinateSystemsPanel::~CoordinateSystemsPanel()
{
     // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::Load
// Created: ABR 2014-10-01
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::Load( const GlProxy& )
{
    coordSysComboBox_->setCurrentIndex( static_cast< int >( coordConverter_.GetDefaultCoordinateSystem() ) );
}
