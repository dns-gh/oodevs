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

#include "clients_kernel/Controllers.h"
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
                                                kernel::Controllers& controllers,
                                                kernel::CoordinateConverter_ABC& coordConverter )
    : PreferencePanel_ABC( parent, "CoordinateSystemsPanel" )
    , controllers_( controllers )
    , coordConverter_( coordConverter )
    , options_( controllers.options_ )
    , previousCoordinateSystem_( coordConverter.GetDefaultCoordinateSystem() )
{
    QLabel* coordinateLabel = new QLabel( tr( "Select current coordinate system:" ) );
    coordSysComboBox_ = new RichWidget< QComboBox >( "listCoordSys" );
    coordSysComboBox_->setEditable( false );

    RichGroupBox* box = new RichGroupBox( "coordinateSystem", tr( "Coordinate System" ), this );
    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->addWidget( coordinateLabel );
    boxLayout->addWidget( coordSysComboBox_ );
    boxLayout->addStretch( 1 );

    for( int i = 0; i < eNbrCoordinateSystem; ++i )
        coordSysComboBox_->insertItem( QString::fromStdString( ENT_Tr::ConvertFromCoordinateSystem( static_cast< E_CoordinateSystem >( i ) ) ), i );

    gui::connect( coordSysComboBox_, SIGNAL( currentIndexChanged( int ) ), [&]{
        coordConverter_.SetDefaultCoordinateSystem( static_cast< E_CoordinateSystem >( coordSysComboBox_->currentItem() ) );
        options_.Change( "CoordSystem", static_cast< int >( coordConverter_.GetDefaultCoordinateSystem() ) );
    } );

    setWidget( box );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel destructor
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
CoordinateSystemsPanel::~CoordinateSystemsPanel()
{
     controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::Commit
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::Commit()
{
    previousCoordinateSystem_ = coordConverter_.GetDefaultCoordinateSystem();
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::Reset
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::Reset()
{
    coordSysComboBox_->setCurrentIndex( previousCoordinateSystem_ );
    coordConverter_.SetDefaultCoordinateSystem( previousCoordinateSystem_ );
}

// -----------------------------------------------------------------------------
// Name: CoordinateSystemsPanel::OptionChanged
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
void CoordinateSystemsPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "CoordSystem" )
        coordSysComboBox_->setCurrentIndex( value.To< int >() );
}
